/*
  dvdetect DVD detection, analysis & DVDETECT lookup library

  Copyright (C) 2013 Norbert Schlia <nschlia@dvdetect.de>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*! \file dvdparse.cpp
 *
 *  \brief dvdparse class implementation
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>

#include "localutils.h"
#include "vts_ifo.h"
#include "vmg_ifo.h"

#include <sstream>
//#include <iostream>
#include <fstream>
#include <iomanip>
//#include <limits.h>
//#include <stdio.h>
#include <assert.h>

dvdparse::dvdparse() :
    m_bFullScan(true)
{
    clear();
}

dvdparse::~dvdparse()
{

}

void dvdparse::clear()
{
    memset(&m_DVDVMGM, 0, sizeof(DVDVMGM));

    m_dvdTitleLst.clear();
    m_dvdPttVmgLst.clear();
    m_dvdFileLst.clear();
    m_strPath.clear();
    m_strErrorString.clear();
    m_nVirtTitleCount = 0;
}

bool dvdparse::locateDVD()
{
    struct stat buf;

#ifdef _WIN32
    if (*m_strPath.rbegin() == ':')
    {
        ::addSeparator(m_strPath);
    }
#endif

    if (stat(getWin32ShortFilePath(m_strPath).c_str(), &buf) == -1)
    {
        setError(((std::string)"Error reading directory '" + m_strPath + "': ") + strerror(errno), DVDERRORCODE_DIROPEN);
        return false;
    }

    if (!S_ISDIR(buf.st_mode))
    {
        ::removeFileSpec(m_strPath);
    }

    ::addSeparator(m_strPath);

    std::string strFileName = getDvdFileName(DVDFILETYPE_VMG_IFO);

    if (stat(getWin32ShortFilePath(m_strPath + strFileName).c_str(), &buf) == -1)
    {
        // If not DVD files found, autoadd subdirectory
        m_strPath += "VIDEO_TS/";
    }

    return true;
}

int dvdparse::parse(const std::string & strPath)
{
    int res = 0;

    clear();

    m_strPath = strPath;

    try
    {
        if (m_strPath.empty())
        {
            // Path cannot be empty
            setError("Path cannot be empty.", DVDERRORCODE_EMPTY_PATH);
            throw (int)-1;
        }

        if (!locateDVD())
        {
            throw (int)-1;
        }

        if (!parseVideoManager())
        {
            throw (int)-1;
        }

        m_dvdTitleLst.resize(m_DVDVMGM.m_wNumberOfTitleSets);

        for (uint16_t wTitleSetNo = 1; wTitleSetNo <= m_DVDVMGM.m_wNumberOfTitleSets; wTitleSetNo++)
        {
            if (!parseTitleSet(wTitleSetNo))
            {
                throw (int)-1;
            }
        }
    }
    catch (int _res)
    {
        res = _res;
    }
    return res;
}

#ifndef NDEBUG
void dvdparse::checkFile(const dvdfile & dvdFile)
{
    struct stat buf;
    assert(stat(getWin32ShortFilePath(m_strPath + dvdFile.m_DVDFILE.m_szFileName).c_str(), &buf) != -1);
    assert(dvdFile.m_DVDFILE.m_dwSize == (uint32_t)buf.st_size);
}
#endif

void dvdparse::getVmgMain(const uint8_t* pData)
{
    LPCVMG_IFO vmg_ifo = (LPCVMG_IFO)pData;

    m_DVDVMGM.m_wVersionNumberMajor    = ((vmg_ifo->m_wVersionNumber & 0xF000) >> 12);
    m_DVDVMGM.m_wVersionNumberMinor    = ((vmg_ifo->m_wVersionNumber & 0x0F00) >> 8);
    for (int i = 0; i < 8; i++)
    {
        m_DVDVMGM.m_bRegionProhibited[i] = ((vmg_ifo->m_byRegionMask >> i) & 0x01) ? true : false;
    }
    m_DVDVMGM.m_wNumberOfVolumes       = be2native(vmg_ifo->m_wNumberOfVolumes);
    m_DVDVMGM.m_wVolumeNumber          = be2native(vmg_ifo->m_wVolumeNumber);
    m_DVDVMGM.m_wSideID                = vmg_ifo->m_bySideID;
    getString(m_DVDVMGM.m_szProviderID, vmg_ifo->m_ProviderID);
    m_DVDVMGM.m_wNumberOfTitleSets     = be2native(vmg_ifo->m_wNumberOfTitleSets);
}

void dvdparse::getVmgAttributes(const uint8_t* pData)
{
    LPCVMG_IFO vmg_ifo = (LPCVMG_IFO)pData;

    // video attributes of VMGM_VOBS
    getVideoAttributes(m_DVDVMGM.m_VideoAttributesOfVMGM_VOBS, vmg_ifo->m_byVideoAttributesOfVMGM_VOBS);

    // audio attributes of VMGM_VOBS
    m_DVDVMGM.m_wNumberOfAudioStreamsInVMGM_VOBS        = be2native(vmg_ifo->m_wNumberOfAudioStreamsInVMGM_VOBS);
    if (m_DVDVMGM.m_wNumberOfAudioStreamsInVMGM_VOBS)
    {
        getAudioAttributes(m_DVDVMGM.m_AudioAttributesOfVMGM_VOBS, vmg_ifo->m_byAudioAttributesOfVMGM_VOBS, 0);
    }

    // subpicture attributes of VMGM_VOBS
    m_DVDVMGM.m_wNumberOfSubpictureStreamsInVMGM_VOBS   = be2native(vmg_ifo->m_wNumberOfSubpictureStreamsInVMGM_VOBS);
    if (m_DVDVMGM.m_wNumberOfSubpictureStreamsInVMGM_VOBS)
    {
        getSubpictureAttributes(m_DVDVMGM.m_SubpictureAttributesOfVMGM_VOBS, vmg_ifo->m_bySubpictureAttributesOfVMGM_VOBS, 0);
    }
}

void dvdparse::getVmgIfo()
{
    struct stat buf;
    dvdfile dvdFile;
    std::string strFileName = getDvdFileName(DVDFILETYPE_VMG_IFO);

    if (stat(getWin32ShortFilePath(m_strPath + strFileName).c_str(), &buf) != -1)
    {
        dvdFile.m_DVDFILE.m_eDvdFileType    = DVDFILETYPE_VMG_IFO;
        //dvdFile.m_DVDFILE.m_wTitleSetNo   = 0;
        dvdFile.m_DVDFILE.m_dwSize          = buf.st_size;
        dvdFile.m_DVDFILE.m_Date            = buf.st_ctime;

        safecpy(dvdFile.m_DVDFILE.m_szFileName, strFileName.c_str());

        m_dvdFileLst.push_back(dvdFile);
    }

    strFileName = getDvdFileName(DVDFILETYPE_VMG_BUP);

    if (stat(getWin32ShortFilePath(m_strPath + strFileName).c_str(), &buf) != -1)
    {
        dvdFile.m_DVDFILE.m_eDvdFileType    = DVDFILETYPE_VMG_BUP;
        //dvdFile.m_DVDFILE.m_wTitleSetNo   = 0;
        //dvdFile.m_DVDFILE.m_wVobNo        = 0;
        dvdFile.m_DVDFILE.m_dwSize          = buf.st_size;
        dvdFile.m_DVDFILE.m_Date            = buf.st_ctime;

        safecpy(dvdFile.m_DVDFILE.m_szFileName, strFileName.c_str());

        m_dvdFileLst.push_back(dvdFile);
    }
}

void dvdparse::getVmgMenuVob()
{
    struct stat buf;
    dvdfile dvdFile;
    std::string strFileName = getDvdFileName(DVDFILETYPE_VMG_VOB);

    if (stat(getWin32ShortFilePath(m_strPath + strFileName).c_str(), &buf) != -1)
    {
        dvdFile.m_DVDFILE.m_eDvdFileType    = DVDFILETYPE_VMG_VOB;
        //dvdFile.m_DVDFILE.m_wTitleSetNo   = 0;
        //dvdFile.m_DVDFILE.m_wVobNo        = 0;
        dvdFile.m_DVDFILE.m_dwSize          = buf.st_size;
        dvdFile.m_DVDFILE.m_Date            = buf.st_ctime;

        safecpy(dvdFile.m_DVDFILE.m_szFileName, strFileName.c_str());

        m_dvdFileLst.push_back(dvdFile);
    }
}

void dvdparse::getVmgIfo(time_t ftime, uint64_t qwSizeOfVMGI)
{
    dvdfile dvdFile;
    std::string strFileName = getDvdFileName(DVDFILETYPE_VMG_IFO);

    dvdFile.m_DVDFILE.m_eDvdFileType        = DVDFILETYPE_VMG_IFO;
    //dvdFile.m_DVDFILE.m_wTitleSetNo       = 0;
    //dvdFile.m_DVDFILE.m_wVobNo            = 0;
    dvdFile.m_DVDFILE.m_dwSize              = qwSizeOfVMGI;         // Path cannot be empty
    dvdFile.m_DVDFILE.m_Date                = ftime;                // simply use date of title IFO

    safecpy(dvdFile.m_DVDFILE.m_szFileName, strFileName.c_str());

    m_dvdFileLst.push_back(dvdFile);
#ifndef NDEBUG
    checkFile(dvdFile);
#endif

    strFileName = getDvdFileName(DVDFILETYPE_VMG_BUP);

    dvdFile.m_DVDFILE.m_eDvdFileType        = DVDFILETYPE_VMG_BUP;
    //dvdFile.m_DVDFILE.m_wTitleSetNo       = 0;
    //dvdFile.m_DVDFILE.m_wVobNo            = 0;
    //dvdFile.m_DVDFILE.m_dwSize            = qwSizeOfVMGI;
    //dvdFile.m_DVDFILE.m_Date              = buf.st_ctime;

    safecpy(dvdFile.m_DVDFILE.m_szFileName, strFileName.c_str());

    m_dvdFileLst.push_back(dvdFile);
#ifndef NDEBUG
    checkFile(dvdFile);
#endif
}

void dvdparse::getVmgMenuVob(time_t ftime, uint32_t dwMenuVobSize)
{
    dvdfile dvdFile;
    std::string strFileName = getDvdFileName(DVDFILETYPE_VMG_VOB);

    dvdFile.m_DVDFILE.m_eDvdFileType        = DVDFILETYPE_VMG_VOB;
    //dvdFile.m_DVDFILE.m_wTitleSetNo       = 0;
    //dvdFile.m_DVDFILE.m_wVobNo            = 0;
    dvdFile.m_DVDFILE.m_dwSize              = dwMenuVobSize;
    dvdFile.m_DVDFILE.m_Date                = ftime;            // simply use date of title IFO

    safecpy(dvdFile.m_DVDFILE.m_szFileName, strFileName.c_str());

    m_dvdFileLst.push_back(dvdFile);
#ifndef NDEBUG
    checkFile(dvdFile);
#endif
}

void dvdparse::getVmgPtt(const uint8_t* pData)
{
    LPCVMG_IFO vmg_ifo = (LPCVMG_IFO)pData;
    uint32_t dwStartAddressVMG_PTT_SRPT                 = dvdSector2bytes(be2native(vmg_ifo->m_dwSectorPointerVMG_PTT_SRPT));    // 0C4 	sector pointer to VMG_PTT_SRPT (table of titles)
    LPCVMG_PTT_SRPT_HEADER pVMG_PTT_SRPT_HEADER         = (LPCVMG_PTT_SRPT_HEADER)(pData + dwStartAddressVMG_PTT_SRPT);

    m_DVDVMGM.m_wPTTNumberOfTitles                      = be2native(pVMG_PTT_SRPT_HEADER->m_wNumberOfTitles);
    for (uint16_t wTitle = 1; wTitle <= m_DVDVMGM.m_wPTTNumberOfTitles; wTitle++)
    {
        LPCVMG_PTT_SRPT pVMG_PTT_SRPT = (LPCVMG_PTT_SRPT)(pData + dwStartAddressVMG_PTT_SRPT +  sizeof(VMG_PTT_SRPT_HEADER) + (wTitle - 1) * sizeof(VMG_PTT_SRPT));
        dvdpttvmg dvdPttVmg(this);

        dvdPttVmg.m_DVDPTTVMG.m_byPlaybackType          = getbyte(pVMG_PTT_SRPT->m_byPlaybackType);
        dvdPttVmg.m_DVDPTTVMG.m_byNumberOfVideoAngles   = getbyte(pVMG_PTT_SRPT->m_byNumberOfVideoAngles);
        dvdPttVmg.m_DVDPTTVMG.m_wNumberOfChapters       = be2native(pVMG_PTT_SRPT->m_wNumberOfChapters);
        dvdPttVmg.m_DVDPTTVMG.m_byParentalMgmMaskVMG    = getbyte(pVMG_PTT_SRPT->m_byParentalMgmMaskVMG);
        dvdPttVmg.m_DVDPTTVMG.m_byParentalMgmMaskVTS    = getbyte(pVMG_PTT_SRPT->m_byParentalMgmMaskVTS);
        dvdPttVmg.m_DVDPTTVMG.m_byVideoTitleSet         = getbyte(pVMG_PTT_SRPT->m_byVideoTitleSet);
        dvdPttVmg.m_DVDPTTVMG.m_byTitleNumber           = getbyte(pVMG_PTT_SRPT->m_byTitleNumber);
        dvdPttVmg.m_DVDPTTVMG.m_dwVtsAddress            = dvdSector2bytes(be2native(pVMG_PTT_SRPT->m_dwVtsSectorAddress));

        m_dvdPttVmgLst.push_back(dvdPttVmg);
    }
}

bool dvdparse::parseVideoManager()
{
    string strFileName = getDvdFileName(DVDFILETYPE_VMG_IFO);
    string strFilePath = m_strPath + strFileName;
    const uint8_t* pData = NULL;
    time_t ftime = 0;
    bool bSuccess = false;

    pData = readIFO(strFilePath, ftime);
    if (pData != NULL)
    {
        LPCVMG_IFO vmg_ifo = (LPCVMG_IFO)pData;

        if (getString(vmg_ifo->m_header) == "DVDVIDEO-VMG")
        {
            getVmgPtt(pData);

            // TODO...
            // uint32_t dwStartAddressVMGM_PGCI_UT     = dvdSector2bytes(be2native(vmg_ifo->m_dwSectorPointerVMGM_PGCI_UT));   // 0C8 	sector pointer to VMGM_PGCI_UT (Menu Program Chain table)
            // uint32_t dwStartAddressVMG_PTL_MAIT     = dvdSector2bytes(be2native(vmg_ifo->m_dwSectorPointerVMG_PTL_MAIT));   // 0CC 	sector pointer to VMG_PTL_MAIT (Parental Management masks)
            // uint32_t dwStartAddressVMG_VTS_ATRT     = dvdSector2bytes(be2native(vmg_ifo->m_dwSectorPointerVMG_VTS_ATRT));   // 0D0 	sector pointer to VMG_VTS_ATRT (copies of VTS audio/sub-picture attributes)
            // TODO: VMG_TXTDT_MG nicht dokumentiert... Keine DVD mit Daten gefunden...
            // uint32_t dwStartAddressVMG_TXTDT_MG     = dvdSector2bytes(be2native(vmg_ifo->m_dwSectorPointerVMG_TXTDT_MG));   // 0D4 	sector pointer to VMG_TXTDT_MG (text data)
            // assert(dwStartAddressVMG_TXTDT_MG == 0);
            // uint32_t dwStartAddressVMGM_C_ADT       = dvdSector2bytes(be2native(vmg_ifo->m_dwSectorPointerVMGM_C_ADT));     // 0D8 	sector pointer to VMGM_C_ADT (menu cell address table)
            // uint32_t dwStartAddressVMGM_VOBU_ADMAP  = dvdSector2bytes(be2native(vmg_ifo->m_dwSectorPointerVMGM_VOBU_ADMAP));// 0DC 	sector pointer to VMGM_VOBU_ADMAP (menu VOBU address map)

            getVmgMain(pData);

            getVmgAttributes(pData);

            if (m_bFullScan)
            {
                // Menu IFO
                getVmgIfo();
                // Menu VOB
                getVmgMenuVob();
            }
            else
            {
                uint64_t qwSizeOfVMG    = dvdSector2bytes(be2native(vmg_ifo->m_dwLastSectorVMG) + 1);           // 00C 	last sector of VMG set (last sector of BUP)
                uint64_t qwSizeOfVMGI   = dvdSector2bytes(be2native(vmg_ifo->m_dwLastSectorVMGI) + 1);          // 01C 	last sector of IFO
                //uint64_t qwSizeOfVMGM_VOBS = dvdSector2bytes(be2native(vmg_ifo->m_dwSectorPointerVMGM_VOBS)); // 0C0 	sector pointer to VMGM_VOBS (Menu VOB)

                // Calculate size of VIDEO_TS.VOB (if present)
                uint32_t dwMenuVobSize = qwSizeOfVMG - 2 * qwSizeOfVMGI;

                // Menu IFO
                getVmgIfo(ftime, qwSizeOfVMGI);

                if (dwMenuVobSize)
                {
                    // Menu VOB
                    getVmgMenuVob(ftime, dwMenuVobSize);
                }
            }

            bSuccess = true;
        }
        else
        {
            setError("Error reading file '" + strFilePath + "': Not a Video Manager IFO file header.", DVDERRORCODE_VMG_IFO);
        }
        delete pData;
    }

    return bSuccess;
}

void dvdparse::addUnit(const uint8_t* pData, dvdcell & dvdCell)
{
    LPCVTS_IFO pVTS_IFO                          = (LPCVTS_IFO)pData;

    uint32_t dwStartAddressVTS_C_ADT             = dvdSector2bytes(be2native(pVTS_IFO->m_dwSectorPointerVTS_C_ADT));

    assert(dwStartAddressVTS_C_ADT);            // Should not be 0 (required field by specs)
    if (dwStartAddressVTS_C_ADT == 0)
    {
        return;
    }

    LPCVTS_C_ADT_HEADER pC_ADT_HEADER = (LPCVTS_C_ADT_HEADER)(pData + dwStartAddressVTS_C_ADT);
    uint16_t wOldVOBidn = 0;
    uint8_t byOldCELLidn = 0;
    uint32_t dwCounter = (be2native(pC_ADT_HEADER->m_dwEndAdress) - 7) / sizeof(VTS_C_ADT);
    dvdCell.m_DVDCELL.m_wCellPosInfoCount       = 0;
    dvdCell.m_DVDCELL.m_wNumberOfVOBIds         = be2native(pC_ADT_HEADER->m_wNumberOfVOBIds);

    uint16_t wUnit = 1;

    for (uint32_t u = 0; u < dwCounter; u++)
    {
        LPCVTS_C_ADT pC_ADT = (LPCVTS_C_ADT)(pData + (dwStartAddressVTS_C_ADT + sizeof(VTS_C_ADT_HEADER) + (sizeof(VTS_C_ADT) * u)));
        uint16_t wVOBidn                        = be2native(pC_ADT->m_wVOBidn);
        uint8_t byCELLidn                       = pC_ADT->m_byCELLidn;

        if (wVOBidn == 0)
        {
            wVOBidn     = wOldVOBidn + 1;   // automatic calculation
        }

        if (byCELLidn == 0)
        {
            byCELLidn   = byOldCELLidn + 1;
        }

        wOldVOBidn      = wVOBidn;
        byOldCELLidn    = byCELLidn;

        if (dvdCell.m_DVDCELL.m_wVOBidn == wVOBidn &&
                dvdCell.m_DVDCELL.m_wCELLidn == byCELLidn)
        {
            dvdunit dvdUnit;

            dvdUnit.m_DVDUNIT.m_wTitleSetNo        = dvdCell.m_DVDCELL.m_wTitleSetNo;
            dvdUnit.m_DVDUNIT.m_wProgramChainNo = dvdCell.m_DVDCELL.m_wProgramChainNo;
            dvdUnit.m_DVDUNIT.m_wProgramNo      = dvdCell.m_DVDCELL.m_wProgramNo;
            dvdUnit.m_DVDUNIT.m_wCell           = dvdCell.m_DVDCELL.m_wCell;
            dvdUnit.m_DVDUNIT.m_wUnit           = wUnit++;

            dvdUnit.m_DVDUNIT.m_dwStartSector   = be2native(pC_ADT->m_dwStartSector);
            dvdUnit.m_DVDUNIT.m_dwEndSector     = be2native(pC_ADT->m_dwEndSector);
            dvdCell.m_DVDCELL.m_wCellPosInfoCount++;

            dvdCell.m_dvdUnitLst.push_back(dvdUnit);
        }
    }
}

void dvdparse::addCell(const uint8_t* pData, dvdprogram& dvdProgram, uint16_t wCell, uint16_t wPGCCellPlaybackOffset, uint32_t dwOffsetVTS_PGC, uint16_t wCellPositionOffset)
{
    dvdcell dvdCell;
    LPCVTS_IFO pVTS_IFO                             = (LPCVTS_IFO)pData;

    uint32_t dwStartAddressVTS_PGCI_UT              = dvdSector2bytes(be2native(pVTS_IFO->m_dwSectorPointerVTS_PGCI_UT));

    LPCVTS_PGC pVTS_PGC = (LPCVTS_PGC)(pData + (dwStartAddressVTS_PGCI_UT + dwOffsetVTS_PGC + wPGCCellPlaybackOffset + (sizeof(VTS_PGC) * (wCell - 1))));
    dvdCell.m_DVDCELL.m_wTitleSetNo                 = dvdProgram.m_DVDPROGRAM.m_wTitleSetNo;
    dvdCell.m_DVDCELL.m_wProgramChainNo             = dvdProgram.m_DVDPROGRAM.m_wProgramChainNo;
    dvdCell.m_DVDCELL.m_wProgramNo                  = dvdProgram.m_DVDPROGRAM.m_wProgramNo;
    dvdCell.m_DVDCELL.m_wCell                       = wCell;

    dvdCell.m_DVDCELL.m_eCellType                   = (CELLTYPE)((((pVTS_PGC->m_byCellCategory1 & 0xC0) >> 6) & 0x03) + 1);
    dvdCell.m_DVDCELL.m_eBlockType                  = (BLOCKTYPE)((((pVTS_PGC->m_byCellCategory1 & 0x30) >> 4) & 0x03) + 1);
    dvdCell.m_DVDCELL.m_bSeamlessMultiplex          = (pVTS_PGC->m_byCellCategory1 & 0x08) ? true : false;
    dvdCell.m_DVDCELL.m_bInterleaved                = (pVTS_PGC->m_byCellCategory1 & 0x04) ? true : false;
    dvdCell.m_DVDCELL.m_bSCRdiscontinuity           = (pVTS_PGC->m_byCellCategory1 & 0x02) ? true : false;
    dvdCell.m_DVDCELL.m_bSeamlessAngleLinkedInDSI   = (pVTS_PGC->m_byCellCategory1 & 0x01) ? true : false;
    dvdCell.m_DVDCELL.m_bVOBStillMode               = (pVTS_PGC->m_byCellCategory2 & 0x40) ? true : false;;
    dvdCell.m_DVDCELL.m_bStopsTrickPlay             = (pVTS_PGC->m_byCellCategory2 & 0x20) ? true : false;;
    dvdCell.m_DVDCELL.m_wCellStillTime              = pVTS_PGC->m_byCellStillTime;
    dvdCell.m_DVDCELL.m_wCellCommand                = pVTS_PGC->m_byCellCommand;
    dvdCell.m_DVDCELL.m_qwPlayTime                  = BCDtime(pVTS_PGC->m_byPlaytime);
    dvdCell.m_DVDCELL.m_wFrameRate                  = frameRate(pVTS_PGC->m_byPlaytime);
    dvdCell.m_DVDCELL.m_dwFirstVOBUStartSector      = be2native(pVTS_PGC->m_dwFirstVOBUStartSector);
    dvdCell.m_DVDCELL.m_dwFirstILVUEndSector        = be2native(pVTS_PGC->m_dwFirstILVUEndSector);
    dvdCell.m_DVDCELL.m_dwLastVOBUStartSector       = be2native(pVTS_PGC->m_dwLastVOBUStartSector);
    dvdCell.m_DVDCELL.m_dwLastVOBUEndSector         = be2native(pVTS_PGC->m_dwLastVOBUEndSector);

    assert(wCellPositionOffset);                // Can this be 0??? (not required)
    if (wCellPositionOffset != 0)
    {
        LPCVTS_CELLPOS pVTS_CELLPOS = (LPCVTS_CELLPOS)(pData + (dwStartAddressVTS_PGCI_UT + dwOffsetVTS_PGC + wCellPositionOffset + (sizeof(VTS_CELLPOS) * (wCell - 1))));
        dvdCell.m_DVDCELL.m_wVOBidn                 = be2native(pVTS_CELLPOS->m_wVOBidn);
        dvdCell.m_DVDCELL.m_wCELLidn                = pVTS_CELLPOS->m_byCELLidn;
    }

    addUnit(pData, dvdCell);

    dvdProgram.m_DVDPROGRAM.m_wCells++;

    dvdProgram.m_dvdCellLst.push_back(dvdCell);
}

void dvdparse::addPrograms(const uint8_t* pData, dvdpgc & dvdPgc, uint16_t wTitleSetNo, uint16_t wProgramChainNo)
{
    LPCVTS_IFO pVTS_IFO                     = (LPCVTS_IFO)pData;

    dvdPgc.m_DVDPGC.m_wTitleSetNo           = wTitleSetNo;

    uint32_t dwStartAddressVTS_PGCI_UT      = dvdSector2bytes(be2native(pVTS_IFO->m_dwSectorPointerVTS_PGCI_UT));

    LPCVTS_PGCI_UT pVTS_PGCI_UT = (LPCVTS_PGCI_UT)(pData + (dwStartAddressVTS_PGCI_UT + sizeof(VTS_PGCI_UT_HEADER) + (wProgramChainNo - 1) * sizeof(VTS_PGCI_UT)));
    dvdPgc.m_DVDPGC.m_bEntryPGC             = (getbyte(pVTS_PGCI_UT->m_byTitleNo) & 0x80) ? true : false;
    dvdPgc.m_DVDPGC.m_wProgramChainNo       = getbyte(pVTS_PGCI_UT->m_byTitleNo) & 0x7f;
    uint32_t dwOffsetVTS_PGC                = be2native(pVTS_PGCI_UT->m_dwOffsetVTS_PGC);

    LPCVTS_PGCHEADER pVTS_PGCHEADER = (LPCVTS_PGCHEADER)(pData + (dwStartAddressVTS_PGCI_UT + dwOffsetVTS_PGC));
    dvdPgc.m_DVDPGC.m_wNumberOfPrograms     = getbyte(pVTS_PGCHEADER->m_byNumberOfPrograms);
    dvdPgc.m_DVDPGC.m_wNumberOfCells        = getbyte(pVTS_PGCHEADER->m_byNumberOfCells);
    dvdPgc.m_DVDPGC.m_qwPlayTime            = BCDtime(pVTS_PGCHEADER->m_byPlaytime);
    dvdPgc.m_DVDPGC.m_wFrameRate            = frameRate(pVTS_PGCHEADER->m_byPlaytime);
    uint16_t wPGCProgramMapOffset           = be2native(pVTS_PGCHEADER->m_wPGCProgramMapOffset);    // offset within PGC to program map
    uint16_t wPGCCellPlaybackOffset         = be2native(pVTS_PGCHEADER->m_wPGCCellPlaybackOffset);  // offset within PGC to cell playback information table
    uint16_t wCellPositionOffset            = be2native(pVTS_PGCHEADER->m_wCellPositionOffset);     // offset within PGC to cell position information table

    dvdPgc.m_dvdProgramLst.resize(dvdPgc.m_DVDPGC.m_wNumberOfPrograms);

    for (uint16_t wProgramNo = 1; wProgramNo <= dvdPgc.m_DVDPGC.m_wNumberOfPrograms; wProgramNo++)
    {
        dvdprogram & dvdProgram = dvdPgc.m_dvdProgramLst[wProgramNo - 1];

        LPCVTS_PROGRAMMAP pVTS_PROGRAMMAP = (LPCVTS_PROGRAMMAP)(pData + (dwStartAddressVTS_PGCI_UT + dwOffsetVTS_PGC + wPGCProgramMapOffset + (wProgramNo - 1)));
        uint16_t wEntryCell = getbyte(pVTS_PROGRAMMAP->m_byEntryCell);
        uint16_t wLastCell  = 0;

        if (wProgramNo < dvdPgc.m_DVDPGC.m_wNumberOfPrograms)
        {
            wLastCell = getbyte((pVTS_PROGRAMMAP + 1)->m_byEntryCell) - 1;
        }
        else
        {
            wLastCell = dvdPgc.m_DVDPGC.m_wNumberOfCells;
        }

        dvdProgram.m_DVDPROGRAM.m_wTitleSetNo      = wTitleSetNo;
        dvdProgram.m_DVDPROGRAM.m_wProgramChainNo  = wProgramChainNo;
        dvdProgram.m_DVDPROGRAM.m_wProgramNo       = wProgramNo;

        dvdProgram.m_DVDPROGRAM.m_wCells           = 0;

        for (uint16_t wCell = wEntryCell; wCell <= wLastCell; wCell++)
        {
            addCell(pData, dvdProgram, wCell, wPGCCellPlaybackOffset, dwOffsetVTS_PGC, wCellPositionOffset);
        }
    }
}

void dvdparse::getVtsIfo(dvdtitle& dvdTitle, uint16_t wTitleSetNo)
{
    struct stat buf;
    dvdfile dvdFile;
    std::string strFileName = getDvdFileName(DVDFILETYPE_VTS_IFO, wTitleSetNo, 0);

    if (stat(getWin32ShortFilePath(m_strPath + strFileName).c_str(), &buf) != -1)
    {
        dvdFile.m_DVDFILE.m_eDvdFileType            = DVDFILETYPE_VTS_IFO;
        dvdFile.m_DVDFILE.m_wTitleSetNo             = wTitleSetNo;
        dvdFile.m_DVDFILE.m_wVobNo                  = 0;
        dvdFile.m_DVDFILE.m_dwSize                  = buf.st_size;
        dvdFile.m_DVDFILE.m_Date                    = buf.st_ctime;

        safecpy(dvdFile.m_DVDFILE.m_szFileName, strFileName.c_str());

        dvdTitle.m_dvdFileLst.push_back(dvdFile);
    }

    strFileName = getDvdFileName(DVDFILETYPE_VTS_BUP, wTitleSetNo, 0);

    if (stat(getWin32ShortFilePath(m_strPath + strFileName).c_str(), &buf) != -1)
    {
        dvdFile.m_DVDFILE.m_eDvdFileType            = DVDFILETYPE_VTS_BUP;
        //dvdFile.m_DVDFILE.m_wTitleSetNo           = wTitleSetNo;
        //dvdFile.m_DVDFILE.m_wVobNo                = 0;
        dvdFile.m_DVDFILE.m_dwSize                  = buf.st_size;
        dvdFile.m_DVDFILE.m_Date                    = buf.st_ctime;

        safecpy(dvdFile.m_DVDFILE.m_szFileName, strFileName.c_str());

        dvdTitle.m_dvdFileLst.push_back(dvdFile);
    }
}

void dvdparse::getVtsMenuVob(dvdtitle& dvdTitle, uint16_t wTitleSetNo)
{
    struct stat buf;
    dvdfile dvdFile;
    std::string strFileName = getDvdFileName(DVDFILETYPE_MENU_VOB, wTitleSetNo, 0);

    if (stat(getWin32ShortFilePath(m_strPath + strFileName).c_str(), &buf) != -1)
    {
        dvdFile.m_DVDFILE.m_eDvdFileType            = DVDFILETYPE_MENU_VOB;
        dvdFile.m_DVDFILE.m_wTitleSetNo             = wTitleSetNo;
        dvdFile.m_DVDFILE.m_wVobNo                  = 0;
        dvdFile.m_DVDFILE.m_dwSize                  = buf.st_size;
        dvdFile.m_DVDFILE.m_Date                    = buf.st_ctime;

        safecpy(dvdFile.m_DVDFILE.m_szFileName, strFileName.c_str());

        dvdTitle.m_dvdFileLst.push_back(dvdFile);
    }
}

void dvdparse::getVtsVob(dvdtitle& dvdTitle, uint16_t wVobNo, uint16_t wTitleSetNo)
{
    struct stat buf;
    dvdfile dvdFile;
    std::string strFileName = getDvdFileName(DVDFILETYPE_VTS_VOB, wTitleSetNo, wVobNo);

    if (stat(getWin32ShortFilePath(m_strPath + strFileName).c_str(), &buf) != -1)
    {
        dvdFile.m_DVDFILE.m_eDvdFileType            = DVDFILETYPE_VTS_VOB;
        dvdFile.m_DVDFILE.m_wTitleSetNo             = wTitleSetNo;
        dvdFile.m_DVDFILE.m_wVobNo                  = wVobNo;
        dvdFile.m_DVDFILE.m_dwSize                  = buf.st_size;
        dvdFile.m_DVDFILE.m_Date                    = buf.st_ctime;

        safecpy(dvdFile.m_DVDFILE.m_szFileName, strFileName.c_str());

        dvdTitle.m_dvdFileLst.push_back(dvdFile);
    }
}

void dvdparse::getVtsIfo(dvdtitle& dvdTitle, time_t ftime, uint64_t qwSizeOfVMGI, uint16_t wTitleSetNo)
{
    dvdfile dvdFile;
    std::string strFileName = getDvdFileName(DVDFILETYPE_VTS_IFO, wTitleSetNo, 0);

    dvdFile.m_DVDFILE.m_eDvdFileType                = DVDFILETYPE_VTS_IFO;
    dvdFile.m_DVDFILE.m_wTitleSetNo                 = wTitleSetNo;
    dvdFile.m_DVDFILE.m_wVobNo                      = 0;
    dvdFile.m_DVDFILE.m_dwSize                      = qwSizeOfVMGI;
    dvdFile.m_DVDFILE.m_Date                        = ftime;           // simply use date of title IFO

    safecpy(dvdFile.m_DVDFILE.m_szFileName, strFileName.c_str());

    dvdTitle.m_dvdFileLst.push_back(dvdFile);
#ifndef NDEBUG
    checkFile(dvdFile);
#endif

    strFileName = getDvdFileName(DVDFILETYPE_VTS_BUP, wTitleSetNo, 0);

    dvdFile.m_DVDFILE.m_eDvdFileType                = DVDFILETYPE_VTS_BUP;
    //dvdFile.m_DVDFILE.m_wTitleSetNo               = wTitleSetNo;
    //dvdFile.m_DVDFILE.m_wVobNo                    = 0;
    //dvdFile.m_DVDFILE.m_dwSize                    = qwSizeOfVMGI;
    //dvdFile.m_DVDFILE.m_Date                      = buf.st_ctime;

    safecpy(dvdFile.m_DVDFILE.m_szFileName, strFileName.c_str());

    dvdTitle.m_dvdFileLst.push_back(dvdFile);
#ifndef NDEBUG
    checkFile(dvdFile);
#endif
}

void dvdparse::getVtsMenuVob(dvdtitle& dvdTitle, time_t ftime, uint32_t dwMenuVobSize, uint16_t wTitleSetNo)
{
    dvdfile dvdFile;
    std::string strFileName = getDvdFileName(DVDFILETYPE_MENU_VOB, wTitleSetNo, 0);

    dvdFile.m_DVDFILE.m_eDvdFileType                = DVDFILETYPE_MENU_VOB;
    dvdFile.m_DVDFILE.m_wTitleSetNo                 = wTitleSetNo;
    dvdFile.m_DVDFILE.m_wVobNo                      = 0;
    dvdFile.m_DVDFILE.m_dwSize                      = dwMenuVobSize;
    dvdFile.m_DVDFILE.m_Date                        = ftime;           // simply use date of title IFO

    safecpy(dvdFile.m_DVDFILE.m_szFileName, strFileName.c_str());

    dvdTitle.m_dvdFileLst.push_back(dvdFile);
#ifndef NDEBUG
    checkFile(dvdFile);
#endif
}

void dvdparse::getVtsVob(dvdtitle& dvdTitle, time_t ftime, uint32_t dwSize, uint16_t wVobNo, uint16_t wTitleSetNo)
{
    dvdfile dvdFile;
    std::string strFileName = getDvdFileName(DVDFILETYPE_VTS_VOB, wTitleSetNo, wVobNo);

    dvdFile.m_DVDFILE.m_eDvdFileType                = DVDFILETYPE_VTS_VOB;
    dvdFile.m_DVDFILE.m_wTitleSetNo                 = wTitleSetNo;
    dvdFile.m_DVDFILE.m_wVobNo                      = wVobNo;
    dvdFile.m_DVDFILE.m_dwSize                      = dwSize;
    dvdFile.m_DVDFILE.m_Date                        = ftime;           // simply use date of title IFO

    safecpy(dvdFile.m_DVDFILE.m_szFileName, strFileName.c_str());

    dvdTitle.m_dvdFileLst.push_back(dvdFile);

#ifndef NDEBUG
    checkFile(dvdFile);
#endif
}

void dvdparse::getVtsAttributes(const uint8_t *pData, dvdtitle& dvdTitle)
{
    LPCVTS_IFO pVTS_IFO = (LPCVTS_IFO)pData;

    // video attributes of VTSM_VOBS
    getVideoAttributes(dvdTitle.m_DVDVTS.m_VideoAttributesOfVTSM_VOBS, pVTS_IFO->m_byVideoAttributesOfVTS_VOBS);

    // audio attributes of VTSM_VOBS
    dvdTitle.m_DVDVTS.m_wNumberOfAudioStreamsInVTSM_VOBS        = be2native(pVTS_IFO->m_wNumberOfAudioStreamsInVTSM_VOBS);
    if (dvdTitle.m_DVDVTS.m_wNumberOfAudioStreamsInVTSM_VOBS)
    {
        getAudioAttributes(dvdTitle.m_DVDVTS.m_AudioAttributesOfVTSM_VOBS, pVTS_IFO->m_byAudioAttributesOfVTSM_VOBS, 0);
    }

    // subpicture attributes of VTSM_VOBS
    dvdTitle.m_DVDVTS.m_wNumberOfSubpictureStreamsInVTSM_VOBS   = be2native(pVTS_IFO->m_wNumberOfSubpictureStreamsInVTSM_VOBS);
    if (dvdTitle.m_DVDVTS.m_wNumberOfSubpictureStreamsInVTSM_VOBS)
    {
        getSubpictureAttributes(dvdTitle.m_DVDVTS.m_SubpictureAttributesOfVTSM_VOBS, pVTS_IFO->m_bySubpictureAttributesOfVTSM_VOBS, 0);
    }

    // video attributes of VTS_VOBS
    getVideoAttributes(dvdTitle.m_DVDVTS.m_VideoAttributesOfVTS_VOBS, pVTS_IFO->m_byVideoAttributesOfVTS_VOBS);
    // audio attributes of VTS_VOBS
    dvdTitle.m_DVDVTS.m_wNumberOfAudioStreamsInVTS_VOBS         = be2native(pVTS_IFO->m_wNumberOfAudioStreamsInVTS_VOBS);
    for (uint16_t wAudioStream = 1; wAudioStream <= dvdTitle.m_DVDVTS.m_wNumberOfAudioStreamsInVTS_VOBS; wAudioStream++)
    {
        getAudioAttributes(dvdTitle.m_DVDVTS.m_AudioAttributesOfVTS_VOBS[wAudioStream - 1], pVTS_IFO->m_byAudioAttributesOfVTS_VOBS[wAudioStream - 1], wAudioStream);
    }

    // subpicture attributes of VTS_VOBS
    dvdTitle.m_DVDVTS.m_wNumberOfSubpictureStreamsInVTS_VOBS    = be2native(pVTS_IFO->m_wNumberOfSubpictureStreamsInVTS_VOBS);
    for (uint16_t wSubpictureStream = 1; wSubpictureStream <= dvdTitle.m_DVDVTS.m_wNumberOfSubpictureStreamsInVTS_VOBS; wSubpictureStream++)
    {
        getSubpictureAttributes(dvdTitle.m_DVDVTS.m_SubpictureAttributesOfVTS_VOBS[wSubpictureStream - 1], pVTS_IFO->m_bySubpictureAttributesOfVTS_VOBS[wSubpictureStream - 1], wSubpictureStream);
    }
}

bool dvdparse::parseTitleSet(uint16_t wTitleSetNo)
{
    string strFilePath;
    const uint8_t* pData = NULL;
    time_t ftime = 0;
    bool bSuccess = true;

    strFilePath = m_strPath + getDvdFileName(DVDFILETYPE_VTS_IFO, wTitleSetNo, 0);

    pData = readIFO(strFilePath, ftime);
    if (pData != NULL)
    {
        LPCVTS_IFO pVTS_IFO = (LPCVTS_IFO)pData;
        if (getString(pVTS_IFO->m_header) == "DVDVIDEO-VTS")
        {
            dvdtitle & dvdTitle = m_dvdTitleLst[wTitleSetNo - 1];

            dvdTitle.m_DVDVTS.m_wTitleSetNo                 = wTitleSetNo;
            dvdTitle.m_DVDVTS.m_wVersionNumberMajor         = ((pVTS_IFO->m_wVersionNumber & 0xF000) >> 12);
            dvdTitle.m_DVDVTS.m_wVersionNumberMinor         = ((pVTS_IFO->m_wVersionNumber & 0x0F00) >> 8);

            uint32_t dwStartAddressVTS_PTT_SRPT             = dvdSector2bytes(be2native(pVTS_IFO->m_dwSectorPointerVTS_PTT_SRPT));

            getVtsAttributes(pData, dvdTitle);

            LPCVTS_PTT_SRPT_HEADER pVTS_PTT_SRPT_HEADER     = (LPCVTS_PTT_SRPT_HEADER)(pData + dwStartAddressVTS_PTT_SRPT);
            uint16_t wNumberOfTitles                        = be2native(pVTS_PTT_SRPT_HEADER->m_wNumberOfTitles);
            uint32_t dwEndAdress                            = be2native(pVTS_PTT_SRPT_HEADER->m_dwEndAdress);

            if (wTitleSetNo == 1)
            {
                m_nVirtTitleCount = 0;
            }

            for (uint16_t wPttVmgNo = 1; wPttVmgNo <= wNumberOfTitles; wPttVmgNo++)
            {
                LPCVTS_PTT_SRPT pVTS_PTT_SRPT = (LPCVTS_PTT_SRPT)(pData + dwStartAddressVTS_PTT_SRPT + sizeof(VTS_PTT_SRPT_HEADER) + (wPttVmgNo - 1) * sizeof(VTS_PTT_SRPT));
                int16_t dwOffsetToPTT                       = be2native(pVTS_PTT_SRPT->m_dwOffsetToPTT);
                uint32_t dwBlockSize = 0;
                uint16_t wVirtualPttNo = wPttVmgNo - 1 + m_nVirtTitleCount;

                if (wVirtualPttNo >= m_dvdPttVmgLst.size())
                {
                    // Badly authored DVD: Contains more virtual titles than defined in video manager.
                    // This means maybe there's at least one hidden title set.
                    wNumberOfTitles = 0;
                    break;
                }

                if (wPttVmgNo < wNumberOfTitles)
                {
                    LPCVTS_PTT_SRPT pVTS_PTT_SRPT2          = (LPCVTS_PTT_SRPT)(pData + dwStartAddressVTS_PTT_SRPT + sizeof(VTS_PTT_SRPT_HEADER) + wPttVmgNo * sizeof(VTS_PTT_SRPT));
                    dwBlockSize = be2native(pVTS_PTT_SRPT2->m_dwOffsetToPTT) - dwOffsetToPTT;
                }
                else
                {
                    dwBlockSize = dwEndAdress - dwOffsetToPTT + 1;
                }

                uint32_t dwCounter = dwBlockSize / sizeof(VTS_PTT);

                if (!dwCounter)
                {
                    continue;
                }

                dvdpttvmg & dvdPttVmg = m_dvdPttVmgLst[wVirtualPttNo];

                for (uint16_t wPttVmgVts = 1; wPttVmgVts <= dwCounter; wPttVmgVts++)
                {
                    dvdpttvts dvdPttVts(this);

                    LPCVTS_PTT pVTS_PTT = (LPCVTS_PTT)(pData + dwStartAddressVTS_PTT_SRPT + dwOffsetToPTT + (wPttVmgVts - 1) * sizeof(VTS_PTT));

                    dvdPttVts.m_DVDPTTVTS.m_wTitleSetNo     = wTitleSetNo;
                    dvdPttVts.m_DVDPTTVTS.m_wPttTitleSetNo  = wVirtualPttNo + 1;
                    dvdPttVts.m_DVDPTTVTS.m_wPttNo          = wPttVmgVts;

                    dvdPttVts.m_DVDPTTVTS.m_wProgramChainNo = be2native(pVTS_PTT->m_wProgramChain);
                    dvdPttVts.m_DVDPTTVTS.m_wProgram        = be2native(pVTS_PTT->m_wProgram);

                    dvdPttVmg.m_dvdPttVtsLst.push_back(dvdPttVts);
                }
            }

            m_nVirtTitleCount += wNumberOfTitles;

            uint32_t dwStartAddressVTS_PGCI_UT              = dvdSector2bytes(be2native(pVTS_IFO->m_dwSectorPointerVTS_PGCI_UT));
            LPCVTS_PGCI_UT_HEADER pVTS_PGCI_UT_HEADER       = (LPCVTS_PGCI_UT_HEADER)(pData + dwStartAddressVTS_PGCI_UT);
            dvdTitle.m_DVDVTS.m_wNumberOfProgramChains      = be2native(pVTS_PGCI_UT_HEADER->m_wNumberOfProgramChains);

            dvdTitle.m_dvdPgcLst.resize(dvdTitle.m_DVDVTS.m_wNumberOfProgramChains);

            for (uint16_t wProgramChainNo = 1; wProgramChainNo <= dvdTitle.m_DVDVTS.m_wNumberOfProgramChains; wProgramChainNo++)
            {
                dvdpgc & dvdPgc                             = dvdTitle.m_dvdPgcLst[wProgramChainNo - 1];

                addPrograms(pData, dvdPgc, wTitleSetNo, wProgramChainNo);
            }

            if (m_bFullScan)
            {
                // Menu IFO
                getVtsIfo(dvdTitle, wTitleSetNo);
                // Menu VOB
                getVtsMenuVob(dvdTitle, wTitleSetNo);

                for (uint16_t wVobNo = 1; wVobNo <= DVD_MAX_VOB; wVobNo++)
                {
                    getVtsVob(dvdTitle, wVobNo, wTitleSetNo);
                }
            }
            else
            {
                uint64_t qwSizeOfVTS = dvdSector2bytes(be2native(pVTS_IFO->m_dwLastSectorVTS) + 1);             // 00C 	4		last sector of VMG set (last sector of BUP)
                uint64_t qwSizeOfVMGI = dvdSector2bytes(be2native(pVTS_IFO->m_dwLastSectorVMGI) + 1);           // 01C 	4		last sector of IFO
                uint64_t qwSizeOfVTSM_VOBS = dvdSector2bytes(be2native(pVTS_IFO->m_dwSectorPointerVTSM_VOBS));  // 0C0 	4		sector pointer to VTSM_VOBS (Menu VOB)
                uint64_t qwSizeOfVTST_VOBS = dvdSector2bytes(be2native(pVTS_IFO->m_dwSectorPointerVTST_VOBS));  // 0C4 	4		sector pointer to VTST_VOBS (Title VOB)

                // Calculate size of VTS_##_0.VOB
                uint32_t dwMenuVobSize = qwSizeOfVTSM_VOBS ? (qwSizeOfVTST_VOBS - qwSizeOfVTSM_VOBS) : 0;
                // Calculate size of VTS_##_1.VOB to VTS_##_9.VOB
                uint64_t qwTitleVobSize = qwSizeOfVTS - qwSizeOfVTST_VOBS - qwSizeOfVMGI;
                // Number of VOBS
                uint16_t wVobCount = qwTitleVobSize / DVD_MAX_VOB_SIZE;
                // Size of last VOB
                uint32_t dwLastVobSize = qwTitleVobSize % DVD_MAX_VOB_SIZE;

                // Menu IFO
                getVtsIfo(dvdTitle, ftime, qwSizeOfVMGI, wTitleSetNo);

                if (dwLastVobSize)
                {
                    wVobCount++;
                }

                if (dwMenuVobSize)
                {
                    // Menu VOB
                    getVtsMenuVob(dvdTitle, ftime, dwMenuVobSize, wTitleSetNo);
                }

                for (uint16_t wVobNo = 1; wVobNo < wVobCount; wVobNo++)
                {
                    getVtsVob(dvdTitle, ftime, DVD_MAX_VOB_SIZE, wVobNo, wTitleSetNo);
                }

                if (dwLastVobSize)
                {
                    getVtsVob(dvdTitle, ftime, dwLastVobSize, wVobCount, wTitleSetNo);
                }
            }
        }
        else
        {
            setError("Error reading file '" + strFilePath + "': Not a Video Title Set IFO file header.", DVDERRORCODE_VTS_IFO);
            bSuccess = false;
        }

        delete pData;
    }

    return bSuccess;
}

LPCDVDVMGM dvdparse::getDVDVMGM() const
{
    return &m_DVDVMGM;
}

const dvdtitle *dvdparse::getDvdTitle(uint16_t wTitleSetNo) const
{
    if (!wTitleSetNo || wTitleSetNo > m_dvdTitleLst.size())
    {
        return NULL;
    }

    return &m_dvdTitleLst[wTitleSetNo - 1];
}

uint16_t dvdparse::getTitleCount() const
{
    return m_DVDVMGM.m_wNumberOfTitleSets;
}

const dvdfile * dvdparse::getDvdFile(uint16_t wFileNo) const
{
    if (wFileNo >= m_dvdFileLst.size())
    {
        return NULL;
    }

    return &m_dvdFileLst[wFileNo];
}

uint16_t dvdparse::getFileCount() const
{
    return (uint16_t)m_dvdFileLst.size();
}

const dvdpgc * dvdparse::getDvdPgc(uint16_t wTitleSetNo, uint16_t wProgramChainNo) const
{
    const dvdtitle *pDvdTitle = getDvdTitle(wTitleSetNo);

    if (pDvdTitle == NULL)
    {
        return NULL;
    }

    return pDvdTitle->getDvdPgc(wProgramChainNo);
}

const dvdprogram * dvdparse::getDvdProgram(uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram) const
{
    const dvdpgc *pDvdPgc = getDvdPgc(wTitleSetNo, wProgramChainNo);

    if (pDvdPgc == NULL)
    {
        return NULL;
    }

    return pDvdPgc->getDvdProgram(wProgram);
}

const dvdprogram * dvdparse::getDvdProgram(LPCDVDPTTVTS pDVDPTTVTS) const
{
    /*const*/ dvdprogram *pDvdProgram = (dvdprogram *)getDvdProgram(pDVDPTTVTS->m_wTitleSetNo, pDVDPTTVTS->m_wProgramChainNo, pDVDPTTVTS->m_wProgram);

    pDvdProgram->m_DVDPROGRAM.m_wPTTTitleSetNo      = pDVDPTTVTS->m_wPttTitleSetNo;
    pDvdProgram->m_DVDPROGRAM.m_wPTTProgramNo       = pDVDPTTVTS->m_wPttNo;

    return pDvdProgram;
}

const dvdpttvmg * dvdparse::getDvdPttVmg(uint16_t wTitleSetNo) const
{
    if (!wTitleSetNo || wTitleSetNo > getDvdPttCount())
    {
        return NULL;
    }

    return &m_dvdPttVmgLst[wTitleSetNo - 1];
}

uint16_t dvdparse::getDvdPttCount() const
{
    return m_dvdPttVmgLst.size();
}

const dvdpttvts * dvdparse::getDvdPttVts(uint16_t wTitleSetNo, uint16_t wPtt) const
{
    const dvdpttvmg *pDvdPttVmg = getDvdPttVmg(wTitleSetNo);

    if (pDvdPttVmg == NULL)
    {
        return NULL;
    }

    return pDvdPttVmg->getDvdPttVts(wPtt);
}

const dvdcell *dvdparse::getDvdCell(uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram, uint16_t wCell) const
{
    const dvdprogram *pDvdProgram = getDvdProgram(wTitleSetNo, wProgramChainNo, wProgram);

    if (pDvdProgram == NULL)
    {
        return NULL;
    }

    return pDvdProgram->getDvdCell(wCell);
}

const dvdunit * dvdparse::getDvdUnit(uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram, uint16_t wCell, uint16_t wUnit) const
{
    const dvdcell *pDvdCell = getDvdCell(wTitleSetNo, wProgramChainNo, wProgram, wCell);

    if (pDvdCell == NULL)
    {
        return NULL;
    }

    return pDvdCell->getDvdUnit(wUnit);
}

const dvdpgc * dvdparse::getStartPgc() const
{
    // TODO: Should be derived from m_dwStartAddressFP_PGC
    const dvdtitle * pDvdTitle = getDvdTitle(1);
    if (pDvdTitle == NULL)
    {
        return NULL;
    }
    const dvdpgc * pDvdPgc = pDvdTitle->getDvdPgc(1);

    return pDvdPgc;
}

std::string dvdparse::getErrorString() const
{
    return m_strErrorString;
}

DVDERRORCODE dvdparse::getErrorCode() const
{
    return m_eErrorCode;
}

std::string dvdparse::getPath() const
{
    return m_strPath;
}

std::string dvdparse::getTitle() const
{
    return "Unknown DVD";       // TODO: richtig ermitteln
}

void dvdparse::setFullScan(bool bFullScan)
{
    m_bFullScan = bFullScan;
}

bool dvdparse::getFullScan() const
{
    return m_bFullScan;
}

const std::type_info & dvdparse::classtype() const
{
    return typeid(*this);
}

void dvdparse::getVideoAttributes(DVDVIDEOATTRIBUTES & videoAttributes, const uint8_t byVideoAttributes[2])
{
    int nCoding     = (byVideoAttributes[0] & 0xC0) >> 6;
    int nStandard   = (byVideoAttributes[0] & 0x30) >> 4;
    int nAspect     = (byVideoAttributes[0] & 0x0C) >> 2;
    int nResolution = (byVideoAttributes[0] & 0x08) >> 3;

    switch (nCoding)
    {
    case 0:
        videoAttributes.m_eDvdVideoCodingMode = DVDVIDEOCODINGMODE_MPEG1;
        break;
    case 1:
        videoAttributes.m_eDvdVideoCodingMode = DVDVIDEOCODINGMODE_MPEG2;
        break;
    default:
        videoAttributes.m_eDvdVideoCodingMode = DVDVIDEOCODINGMODE_INVALID;
        break;
    }

    switch (nStandard)
    {
    case 0:
        videoAttributes.m_eDvdVideoStandard = DVDVIDEOTVSTANDARD_NTSC;

        switch (nResolution)
        {
        case 0:
            videoAttributes.m_wResolutionX = 720;
            videoAttributes.m_wResolutionY = 480;
            break;
        case 1:
            videoAttributes.m_wResolutionX = 704;
            videoAttributes.m_wResolutionY = 480;
            break;
        case 2:
            videoAttributes.m_wResolutionX = 352;
            videoAttributes.m_wResolutionY = 480;
            break;
        case 3:
            videoAttributes.m_wResolutionX = 352;
            videoAttributes.m_wResolutionY = 240;
            break;
        }
        break;
    case 1:
        videoAttributes.m_eDvdVideoStandard = DVDVIDEOTVSTANDARD_PAL;

        switch (nResolution)
        {
        case 0:
            videoAttributes.m_wResolutionX = 720;
            videoAttributes.m_wResolutionY = 576;
            break;
        case 1:
            videoAttributes.m_wResolutionX = 704;
            videoAttributes.m_wResolutionY = 576;
            break;
        case 2:
            videoAttributes.m_wResolutionX = 352;
            videoAttributes.m_wResolutionY = 576;
            break;
        case 3:
            videoAttributes.m_wResolutionX = 352;
            videoAttributes.m_wResolutionY = 288;
            break;
        }

        break;
    default:
        videoAttributes.m_eDvdVideoStandard = DVDVIDEOTVSTANDARD_INVALID;
        break;
    }

    switch (nAspect)
    {
    case 0:
        videoAttributes.m_eDvdVideoAspect = DVDVIDEOASPECT_4_3;
        break;
    case 3:
        videoAttributes.m_eDvdVideoAspect = DVDVIDEOASPECT_16_9;
        break;
    default:
        videoAttributes.m_eDvdVideoAspect = DVDVIDEOASPECT_INVALID;
        break;
    }

    videoAttributes.m_bAutomaticPanScanDisallowed   = (byVideoAttributes[0] & 0x02) ? true : false;
    videoAttributes.m_bAutomaticLetterboxDisallowed = (byVideoAttributes[0] & 0x01) ? true : false;
    videoAttributes.m_bCCForLine21Field1InGOP       = (byVideoAttributes[1] & 0x80) ? true : false;
    videoAttributes.m_bCCForLine21Field2InGOP       = (byVideoAttributes[1] & 0x40) ? true : false;
    videoAttributes.m_bCBR                          = (byVideoAttributes[1] & 0x10) ? true : false;
    videoAttributes.m_bLetterBoxed                  = (byVideoAttributes[1] & 0x04) ? true : false;
    videoAttributes.m_bSourceFilm                   = (byVideoAttributes[1] & 0x01) ? true : false;

    videoAttributes.m_byID                          = 0xE0;  // Fixed
}

void dvdparse::getAudioAttributes(DVDAUDIOATTRIBUTESMENU & audioAttributes, const uint8_t byAudioAttributes[8], uint16_t wAudioStream)
{
    int nCoding                 = (byAudioAttributes[0] & 0xE0) >> 5;
    int nSoundApplicationMode   = (byAudioAttributes[0] & 0x03);
    int nQuantisation           = (byAudioAttributes[1] & 0xC0) >> 6;
    int nSampleRate             = (byAudioAttributes[1] & 0x30) >> 4;

    switch (nCoding)
    {
    case 0:	// 0 = AC3
    {
        audioAttributes.m_eAudioCodingMode = DVDAUDIOCODINGMODE_AC3;
        audioAttributes.m_byID             = 0x80 + (wAudioStream - 1);
        break;
    }
    case 1:	// 1 = Undefined
    {
        audioAttributes.m_eAudioCodingMode = DVDAUDIOCODINGMODE_INVALID;
        audioAttributes.m_byID             = 0x80 + (wAudioStream - 1);  // ???
        break;
    }
    case 2:	// 2 = MPEG-1
    {
        audioAttributes.m_eAudioCodingMode = DVDAUDIOCODINGMODE_MPEG1;
        audioAttributes.m_byID             = 0xC0 + (wAudioStream - 1);
        break;
    }
    case 3:	// 3 = MPEG-2
    {
        audioAttributes.m_eAudioCodingMode = DVDAUDIOCODINGMODE_MPEG2_EXT;
        audioAttributes.m_byID             = 0xC8 + (wAudioStream - 1);
        break;
    }
    case 4:	// 4 = LPCM
    {
        audioAttributes.m_eAudioCodingMode = DVDAUDIOCODINGMODE_LPCM;
        audioAttributes.m_byID             = 0xA0 + (wAudioStream - 1);
        break;
    }
    case 5:	// 5 = Undefined
    {
        audioAttributes.m_eAudioCodingMode = DVDAUDIOCODINGMODE_INVALID;
        audioAttributes.m_byID             = 0x88 + (wAudioStream - 1);
        break;
    }
    case 6:	// 6 = DTS
    {
        audioAttributes.m_eAudioCodingMode = DVDAUDIOCODINGMODE_DTS;
        audioAttributes.m_byID             = 0x88 + (wAudioStream - 1);
        break;
    }
    case 7:	// 7 = Undefined
    {
        audioAttributes.m_eAudioCodingMode = DVDAUDIOCODINGMODE_INVALID;
        audioAttributes.m_byID             = 0x88 + (wAudioStream - 1);    // ???
        break;
    }
    }

    audioAttributes.m_bMultichannelExtensionPresent = (byAudioAttributes[0] & 0x10) ? true : false;
    //audioAttributes.m_bLanguageTypePresent          = ((((byAudioAttributes[0] & 0x0C) >> 2) == 1) && byAudioAttributes[2]) ? true : false;

    switch (nSoundApplicationMode)
    {
    case 0:
        audioAttributes.m_eAudioApplicationMode = DVDAUDIOAPPLICATIONMODE_UNSPECIFIED;
        break;
    case 1:
        audioAttributes.m_eAudioApplicationMode = DVDAUDIOAPPLICATIONMODE_KARAOKE;
        break;
    case 2:
        audioAttributes.m_eAudioApplicationMode = DVDAUDIOAPPLICATIONMODE_SURROUND;
        break;
    default:
        audioAttributes.m_eAudioApplicationMode = DVDAUDIOAPPLICATIONMODE_INVALID;
        break;
    }

    switch (nQuantisation)
    {
    case 0:
        audioAttributes.m_eAudioQuantisation = DVDAUDIOQUANTISATION_16BPS;                     // 0 = 16bps
        break;
    case 1:
        audioAttributes.m_eAudioQuantisation = DVDAUDIOQUANTISATION_20BPS;                     // 1 = 20bps
        break;
    case 2:
        audioAttributes.m_eAudioQuantisation = DVDAUDIOQUANTISATION_24BPS;                     // 2 = 24bps
        break;
    case 3:
        audioAttributes.m_eAudioQuantisation = DVDAUDIOQUANTISATION_DRC;                       // 3 = DRC
        break;
    }

    switch (nSampleRate)
    {
    case 0:
        audioAttributes.m_dwSampleRate = 48000;
        break;
    case 2:
        audioAttributes.m_dwSampleRate = 96000;
        break;
    default:
        audioAttributes.m_dwSampleRate = 0;
        break;
    }

    audioAttributes.m_wChannels = (byAudioAttributes[1] & 0x07) + 1;
}

void dvdparse::getAudioAttributes(DVDAUDIOATTRIBUTESTITLE & audioAttributes, const uint8_t byAudioAttributes[8], uint16_t wAudioStream)
{
    int nCoding                 = (byAudioAttributes[0] & 0xE0) >> 5;
    int nSoundApplicationMode   = (byAudioAttributes[0] & 0x03);
    int nQuantisation           = (byAudioAttributes[1] & 0xC0) >> 6;
    int nSampleRate             = (byAudioAttributes[1] & 0x30) >> 4;
    int nCodeExtension          = byAudioAttributes[5];

    switch (nCoding)
    {
    case 0:	// 0 = AC3
    {
        audioAttributes.m_eAudioCodingMode = DVDAUDIOCODINGMODE_AC3;
        audioAttributes.m_byID             = 0x80 + (wAudioStream - 1);
        break;
    }
    case 1:	// 1 = Undefined
    {
        audioAttributes.m_eAudioCodingMode = DVDAUDIOCODINGMODE_INVALID;
        audioAttributes.m_byID             = 0x80 + (wAudioStream - 1);  // ???
        break;
    }
    case 2:	// 2 = MPEG-1
    {
        audioAttributes.m_eAudioCodingMode = DVDAUDIOCODINGMODE_MPEG1;
        audioAttributes.m_byID             = 0xC0 + (wAudioStream - 1);
        break;
    }
    case 3:	// 3 = MPEG-2
    {
        audioAttributes.m_eAudioCodingMode = DVDAUDIOCODINGMODE_MPEG2_EXT;
        audioAttributes.m_byID             = 0xC8 + (wAudioStream - 1);
        break;
    }
    case 4:	// 4 = LPCM
    {
        audioAttributes.m_eAudioCodingMode = DVDAUDIOCODINGMODE_LPCM;
        audioAttributes.m_byID             = 0xA0 + (wAudioStream - 1);
        break;
    }
    case 5:	// 5 = Undefined
    {
        audioAttributes.m_eAudioCodingMode = DVDAUDIOCODINGMODE_INVALID;
        audioAttributes.m_byID             = 0x88 + (wAudioStream - 1);
        break;
    }
    case 6:	// 6 = DTS
    {
        audioAttributes.m_eAudioCodingMode = DVDAUDIOCODINGMODE_DTS;
        audioAttributes.m_byID             = 0x88 + (wAudioStream - 1);
        break;
    }
    case 7:	// 7 = Undefined
    {
        audioAttributes.m_eAudioCodingMode = DVDAUDIOCODINGMODE_INVALID;
        audioAttributes.m_byID             = 0x88 + (wAudioStream - 1);    // ???
        break;
    }
    }

    audioAttributes.m_bMultichannelExtensionPresent = (byAudioAttributes[0] & 0x10) ? true : false;
    audioAttributes.m_bLanguageTypePresent          = ((((byAudioAttributes[0] & 0x0C) >> 2) == 1) && byAudioAttributes[2]) ? true : false;

    switch (nSoundApplicationMode)
    {
    case 0:
        audioAttributes.m_eAudioApplicationMode = DVDAUDIOAPPLICATIONMODE_UNSPECIFIED;
        break;
    case 1:
        audioAttributes.m_eAudioApplicationMode = DVDAUDIOAPPLICATIONMODE_KARAOKE;
        break;
    case 2:
        audioAttributes.m_eAudioApplicationMode = DVDAUDIOAPPLICATIONMODE_SURROUND;
        break;
    default:
        audioAttributes.m_eAudioApplicationMode = DVDAUDIOAPPLICATIONMODE_INVALID;
        break;
    }

    switch (nQuantisation)
    {
    case 0:
        audioAttributes.m_eAudioQuantisation = DVDAUDIOQUANTISATION_16BPS;                     // 0 = 16bps
        break;
    case 1:
        audioAttributes.m_eAudioQuantisation = DVDAUDIOQUANTISATION_20BPS;                     // 1 = 20bps
        break;
    case 2:
        audioAttributes.m_eAudioQuantisation = DVDAUDIOQUANTISATION_24BPS;                     // 2 = 24bps
        break;
    case 3:
        audioAttributes.m_eAudioQuantisation = DVDAUDIOQUANTISATION_DRC;                       // 3 = DRC
        break;
    }

    switch (nSampleRate)
    {
    case 0:
        audioAttributes.m_dwSampleRate = 48000;
        break;
    case 2:
        audioAttributes.m_dwSampleRate = 96000;
        break;
    default:
        audioAttributes.m_dwSampleRate = 0;
        break;
    }

    audioAttributes.m_wChannels = (byAudioAttributes[1] & 0x07) + 1;

    if (audioAttributes.m_bLanguageTypePresent)
    {
        getString2(audioAttributes.m_szLanguageCode, &byAudioAttributes[2]);
    }

    switch (nCodeExtension)
    {
    case 0:
        audioAttributes.m_eAudioCodeExtension = DVDAUDIOCODEEXTENSION_UNSPECIFIED;              // 0 = unspecified
        break;
    case 1:
        audioAttributes.m_eAudioCodeExtension = DVDAUDIOCODEEXTENSION_NORMAL;                   // 1 = normal,
        break;
    case 2:
        audioAttributes.m_eAudioCodeExtension = DVDAUDIOCODEEXTENSION_VISUALLY_IMPAIRED;        // 2 = for visually impaired
        break;
    case 3:
        audioAttributes.m_eAudioCodeExtension = DVDAUDIOCODEEXTENSION_DIRECTORS_COMMENTS;       // 3 = director's comments
        break;
    case 4:
        audioAttributes.m_eAudioCodeExtension = DVDAUDIOCODEEXTENSION_ALT_DIRECTORS_COMMENTS;    // 4 = alternate director's comments
        break;
    default:
        audioAttributes.m_eAudioCodeExtension = DVDAUDIOCODEEXTENSION_INVALID;
        break;
    }

    switch (audioAttributes.m_eAudioApplicationMode)
    {
    case DVDAUDIOAPPLICATIONMODE_KARAOKE:
    {
        int nChannelAssignments     = (byAudioAttributes[7] & 0x70) >> 4;

        switch (nChannelAssignments)
        {
        case 2:
            audioAttributes.m_eAudioChannelAssignment = DVDAUDIOCHANNELASSIGNMENT_2_0;  //  2 = 2/0 L,R
            break;
        case 3:
            audioAttributes.m_eAudioChannelAssignment = DVDAUDIOCHANNELASSIGNMENT_3_0;  //  3 = 3/0 L,M,R
            break;
        case 4:
            audioAttributes.m_eAudioChannelAssignment = DVDAUDIOCHANNELASSIGNMENT_2_1;  //  4 = 2/1 L,R,V1
            break;
        case 5:
            audioAttributes.m_eAudioChannelAssignment = DVDAUDIOCHANNELASSIGNMENT_3_1;  //  5 = 3/1 L,M,R,V1
            break;
        case 6:
            audioAttributes.m_eAudioChannelAssignment = DVDAUDIOCHANNELASSIGNMENT_2_2;  //  6 = 2/2 L,R,V1,V2
            break;
        case 7:
            audioAttributes.m_eAudioChannelAssignment = DVDAUDIOCHANNELASSIGNMENT_3_2;  //  7 = 3/2 L,M,R,V1,V2
            break;
        default:
            audioAttributes.m_eAudioChannelAssignment = DVDAUDIOCHANNELASSIGNMENT_INVALID;
            break;
        }

        audioAttributes.m_wKaraokeVersion       = (byAudioAttributes[7] & 0x0C) >> 2;

        audioAttributes.m_bMCIntroPresent       = (byAudioAttributes[7] & 0x02) ? true : false;
        audioAttributes.m_eKaraokeMode          = (byAudioAttributes[7] & 0x01) ? DVDKARAOKEMODE_DUET : DVDKARAOKEMODE_SOLO;

        break;
    }
    case DVDAUDIOAPPLICATIONMODE_SURROUND:
    {
        audioAttributes.m_bSuitableForDolbySurroundDecoding = (byAudioAttributes[7] & 0x08) ? true : false;
        break;
    }
    default:
        break;
    }
}

void dvdparse::getSubpictureAttributes(DVDSUBPICTUREATTRIBUTES & subpictureAttributes, const uint8_t bySubpictureAttributes[6], uint16_t wSubpictureStream)
{
    int nSoundApplicationMode                           = bySubpictureAttributes[5];

    subpictureAttributes.m_wCodingMode                  = (bySubpictureAttributes[0] & 0xE0) >> 5;
    subpictureAttributes.m_bLanguageTypePresent         = (((bySubpictureAttributes[0] & 0x03) == 1) && bySubpictureAttributes[2]) ? true : false;

    if (subpictureAttributes.m_bLanguageTypePresent)
    {
        getString2(subpictureAttributes.m_szLanguageCode, &bySubpictureAttributes[2]);
    }

    switch (nSoundApplicationMode)
    {
    case 0:
        subpictureAttributes.m_eSubpictureCodeExtension = DVDSUBPICTURECODEEXT_NOT_SPECIFIED;                       //  0 = not specified
        break;
    case 1:
        subpictureAttributes.m_eSubpictureCodeExtension = DVDSUBPICTURECODEEXT_NORMAL;                              //  1 = normal
        break;
    case 2:
        subpictureAttributes.m_eSubpictureCodeExtension = DVDSUBPICTURECODEEXT_LARGE;                               //  2 = large
        break;
    case 3:
        subpictureAttributes.m_eSubpictureCodeExtension = DVDSUBPICTURECODEEXT_CHILDREN;                            //  3 = children
        break;
    case 5:
        subpictureAttributes.m_eSubpictureCodeExtension = DVDSUBPICTURECODEEXT_NORMAL_CAPTIONS;                     //  5 = normal captions
        break;
    case 6:
        subpictureAttributes.m_eSubpictureCodeExtension = DVDSUBPICTURECODEEXT_LARGE_CAPTIONS;                      //  6 = large captions
        break;
    case 7:
        subpictureAttributes.m_eSubpictureCodeExtension = DVDSUBPICTURECODEEXT_CHILDRENS_CAPTION;                   //  7 = children's captions
        break;
    case 9:
        subpictureAttributes.m_eSubpictureCodeExtension = DVDSUBPICTURECODEEXT_FORCED;                              //  9 = forced
        break;
    case 13:
        subpictureAttributes.m_eSubpictureCodeExtension = DVDSUBPICTURECODEEXT_DIRECTORS_COMMENTS;                  // 13 = director comments
        break;
    case 14:
        subpictureAttributes.m_eSubpictureCodeExtension = DVDSUBPICTURECODEEXT_LARGE_DIRECTORS_COMMENTS;            // 14 = large director comments
        break;
    case 15:
        subpictureAttributes.m_eSubpictureCodeExtension = DVDSUBPICTURECODEEXT_DIRECTORS_COMMENTS_FOR_CHILDREN;     // 15 = director comments for children
        break;
    default:
        subpictureAttributes.m_eSubpictureCodeExtension = DVDSUBPICTURECODEEXT_INVALID;
        break;
    }

    subpictureAttributes.m_byID                         = 0x20 + wSubpictureStream - 1;
}

uint64_t dvdparse::getSize() const
{
    uint64_t size = 0;

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= m_DVDVMGM.m_wNumberOfTitleSets; wTitleSetNo++)
    {
        size += m_dvdTitleLst[wTitleSetNo - 1].getSize();
    }

    return size;
}

uint64_t dvdparse::getPlayTime() const
{
    uint64_t qwPlayTime = 0;

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= m_DVDVMGM.m_wNumberOfTitleSets; wTitleSetNo++)
    {
        qwPlayTime += m_dvdTitleLst[wTitleSetNo - 1].getPlayTime();
    }

    return qwPlayTime;
}

uint64_t dvdparse::getVirtSize() const
{
    uint64_t size = 0;

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= getDvdPttCount(); wTitleSetNo++)
    {
        size += getDvdPttVmg(wTitleSetNo)->getSize();
    }

    return size;
}

uint64_t dvdparse::getVirtPlayTime() const
{
    uint64_t qwPlayTime = 0;

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= getDvdPttCount(); wTitleSetNo++)
    {
        qwPlayTime += getDvdPttVmg(wTitleSetNo)->getPlayTime();
    }

    return qwPlayTime;
}

const uint8_t* dvdparse::readIFO(const string & strFilePath, time_t & ftime)
{
    string strError;
    std::FILE *fpi = NULL;
    struct stat buf;
    uint8_t* pData = NULL;

    m_strErrorString.clear();

    fpi = std::fopen(getWin32ShortFilePath(strFilePath).c_str(), "rb");
    if (fpi == NULL)
    {
        strError = "Error opening file: " + strFilePath + "\n";
        strError += strerror(errno);
        setError(strError, DVDERRORCODE_FILEOPEN);
        return NULL;
    }

    memset(&buf, 0, sizeof(buf));

    if (fstat(fileno(fpi), &buf) == -1)
    {
        strError = "Cannot stat file: " + strFilePath + "\n";
        strError += strerror(errno);
        setError(strError, DVDERRORCODE_FILEOPEN);
    }
    else
    {
        pData = new uint8_t[buf.st_size];
        if (pData != NULL)
        {
            memset(pData, 0, buf.st_size);
            if (std::fread(pData, 1, buf.st_size, fpi) != (size_t)buf.st_size)
            {
                strError = "Error reading file: " + strFilePath + "\n";
                strError += strerror(errno);
                setError(strError, DVDERRORCODE_FILEOPEN);
                delete pData;
                pData = NULL;
            }
        }

        ftime = buf.st_mtime;
    }

    std::fclose(fpi);

    return pData;
}

void dvdparse::setError(const std::string & strErrorString, DVDERRORCODE eErrorCode)
{
    m_strErrorString    = strErrorString;
    m_eErrorCode        = eErrorCode;
}
