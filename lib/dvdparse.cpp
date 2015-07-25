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
#ifdef HAVE_OPENSSL_MD5_H
// Have native m5 support
#include <openssl/md5.h>
#define MD5_type                    uint8_t
#else
// Use our code
#include "md5/md5.h"
#define MD5_CTX                     md5_state_t
#define MD5_type                    md5_byte_t

#define MD5_Init(c)                 md5_init(c)
#define MD5_Update(c, data, len)    md5_append(c, (const md5_byte_t *)data, len)
#define MD5_Final(c, md)            md5_finish(md, c)
#endif

#include <sys/stat.h>

#include <fstream>

// #undef HAVE_ASSERT_H

dvdparse::dvdparse(
        void *  (*openFile)(const char *, const char *, const char *),
        size_t  (*readFile)(void*, size_t, size_t, void*),
        int     (*writeFile)(const void*, size_t, size_t, void*),
        int     (*closeFile)(void *),
        int     (*statFile)(const char *, LPDVDFILESTAT, const char *),
        int     (*fstatFile)(void*, LPDVDFILESTAT)) :
    m_eScanMode(DVDSCANMODE_AUTO),
    m_pOpenFile(openFile),
    m_pReadFile(readFile),
    m_pWriteFile(writeFile),
    m_pCloseFile(closeFile),
    m_pStatFile(statFile),
    m_pFstatFile(fstatFile)
{
    clear();
}

dvdparse::~dvdparse()
{
}

dvdparse& dvdparse::operator= (dvdparse const & source)
{
    if (this != &source)
    {
        m_strPath               = source.m_strPath;
        memcpy(&m_DVDVMGM, &source.m_DVDVMGM, sizeof(DVDVMGM));
        m_lstDvdTitle           = source.m_lstDvdTitle;
        m_lstDvdPttVmg          = source.m_lstDvdPttVmg;
        // Change owner parser as well
        for (dvdpttvmglst::iterator itDvdPttVmg = m_lstDvdPttVmg.begin(); itDvdPttVmg != m_lstDvdPttVmg.end(); itDvdPttVmg++)
        {
            (*itDvdPttVmg)->m_pDvdParse = this;
            for (dvdpttvtslst::iterator itDvdPttVts = (*itDvdPttVmg)->m_lstDvdPttVts.begin(); itDvdPttVts != (*itDvdPttVmg)->m_lstDvdPttVts.end(); itDvdPttVts++)
            {
                (*itDvdPttVts)->m_pDvdParse = this;
            }
        }
        m_lstDvdFile            = source.m_lstDvdFile;
        m_strErrorString        = source.m_strErrorString;
        m_eErrorCode            = source.m_eErrorCode;
        m_eScanMode             = source.m_eScanMode;
        m_nVirtTitleCount       = source.m_nVirtTitleCount;
        m_bIsLoaded             = source.m_bIsLoaded;
        m_strHash               = source.m_strHash;
        m_strAlbumArtist        = source.m_strAlbumArtist;
        m_strAlbum              = source.m_strAlbum;
        m_strOriginalAlbum      = source.m_strOriginalAlbum;
        m_strGenre              = source.m_strGenre;
        m_strCast               = source.m_strCast;
        m_strCrew               = source.m_strCrew;
        m_strDirector           = source.m_strDirector;
        m_strScreenplay         = source.m_strScreenplay;
        m_strProducer           = source.m_strProducer;
        m_strEditing            = source.m_strEditing;
        m_strCinematography     = source.m_strCinematography;
        m_strCountry            = source.m_strCountry;
        m_strOriginalLanguage   = source.m_strOriginalLanguage;
        m_strReleaseDate        = source.m_strReleaseDate;
        m_strSpecialFeatures    = source.m_strSpecialFeatures;
        m_strEAN_UPC            = source.m_strEAN_UPC;
        m_strStoryline          = source.m_strStoryline;
        m_strRemarks            = source.m_strRemarks;
        m_nRevision             = source.m_nRevision;
        m_strSubmitter          = source.m_strSubmitter;
        m_strSubmitterIP        = source.m_strSubmitterIP;
        m_strClient             = source.m_strClient;
        m_strDateCreated        = source.m_strDateCreated;
        m_strDateLastChanged    = source.m_strDateLastChanged;
        m_strKeywords           = source.m_strKeywords;
        m_nProtocolVersion      = source.m_nProtocolVersion;
        m_strLibraryVersion     = source.m_strLibraryVersion;
        m_strLibraryName        = source.m_strLibraryName;
        m_strClientName         = source.m_strClientName;
        m_strProxy              = source.m_strProxy;
    }
    return *this;
}

void dvdparse::clear()
{
    memset(&m_DVDVMGM, 0, sizeof(DVDVMGM));

    m_lstDvdTitle.clear();
    m_lstDvdPttVmg.clear();
    m_lstDvdFile.clear();
    m_strPath.clear();
    m_strErrorString.clear();
    m_eErrorCode = DVDERRORCODE_SUCCESS;
    m_nVirtTitleCount = 0;
    m_bIsLoaded = false;
    m_strHash.clear();
    m_strAlbumArtist.clear();
    m_strAlbum.clear();
    m_strOriginalAlbum.clear();
    m_strGenre.clear();
    m_strCast.clear();
    m_strCrew.clear();
    m_strDirector.clear();
    m_strScreenplay.clear();
    m_strProducer.clear();
    m_strEditing.clear();
    m_strCinematography.clear();
    m_strCountry.clear();
    m_strOriginalLanguage.clear();
    m_strReleaseDate.clear();
    m_strSpecialFeatures.clear();
    m_strEAN_UPC.clear();
    m_strStoryline.clear();
    m_strRemarks.clear();
    m_nRevision = 0;
    m_strSubmitter.clear();
    m_strSubmitterIP.clear();
    m_strClient.clear();
    m_strDateCreated.clear();
    m_strDateLastChanged.clear();
    m_strKeywords.clear();
    m_nProtocolVersion = 0;
    m_strLibraryVersion.clear();
    m_strLibraryName.clear();
    m_strClientName.clear();
}

bool dvdparse::locateDVD()
{
    if (!isUrl(m_strPath))
    {
        DVDFILESTAT filestat;

#ifdef _WIN32
        if (*m_strPath.rbegin() == ':')
        {
            ::addSeparator(m_strPath);
        }
#endif

        if (m_pStatFile(m_strPath.c_str(), &filestat, m_strProxy.c_str()) == -1)
        {
            setError(((std::string)"Error reading directory '" + m_strPath + "': ") + strerror(errno), DVDERRORCODE_DIROPEN);
            return false;
        }

        if (!filestat.m_bIsDirectory)
        {
            ::removeFileSpec(m_strPath);
        }

        ::addSeparator(m_strPath);

        std::string strFileName = getDvdFileName(DVDFILETYPE_VMG_IFO);

        if (m_pStatFile((m_strPath + strFileName).c_str(), &filestat, m_strProxy.c_str()) == -1)
        {
            // If not DVD files found, autoadd subdirectory
            m_strPath += "VIDEO_TS/";
        }
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

        uint16_t wTitleSetCount = parseVideoManager();
        if (!wTitleSetCount)
        {
            throw (int)-1;
        }

        for (uint16_t wTitleSetNo = 1; wTitleSetNo <= wTitleSetCount; wTitleSetNo++)
        {
            if (!parseTitleSet(wTitleSetNo))
            {
                throw (int)-1;
            }
        }
    }
    catch (std::bad_alloc& ba)
    {
        stringstream streamError;

        streamError << "Internal error: bad_alloc caught: " << ba.what();
        setError(streamError.str(), DVDERRORCODE_INTERNAL_ERROR);
        res = -1;
    }
    catch (int _res)
    {
        res = _res;
    }

    m_bIsLoaded = (res == 0) ? true : false;

    return res;
}

#if !defined(NDEBUG) && defined(HAVE_ASSERT_H)
void dvdparse::checkFile(const dvdfile * dvdFile)
{
    DVDFILESTAT filestat;
    assert(m_pStatFile((m_strPath + dvdFile->getFileName()).c_str(), &filestat, m_strProxy.c_str()) != -1);
    assert(dvdFile->m_DVDFILE.m_dwSize == (uint32_t)filestat.m_qwFileSize);
}
#endif

uint16_t dvdparse::getVmgMain(const uint8_t* pData)
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

    return be2native(vmg_ifo->m_wNumberOfTitleSets);
}

void dvdparse::getVmgAttributes(const uint8_t* pData)
{
    LPCVMG_IFO vmg_ifo = (LPCVMG_IFO)pData;

    // video attributes of VMGM_VOBS
    getVideoAttributes(m_DVDVMGM.m_VideoStream, vmg_ifo->m_byVideoStreamVMGM_VOBS);

    // audio attributes of VMGM_VOBS
    m_DVDVMGM.m_wAudioStreamCount        = be2native(vmg_ifo->m_wAudioStreamCount);
    if (m_DVDVMGM.m_wAudioStreamCount)
    {
        getAudioAttributes(m_DVDVMGM.m_AudioStream, vmg_ifo->m_byAudioStreamsVMGM_VOBS, 0);
    }

    // subpicture attributes of VMGM_VOBS
    m_DVDVMGM.m_wSubpicStreamCount   = be2native(vmg_ifo->m_wSubpicStreamCount);
    if (m_DVDVMGM.m_wSubpicStreamCount)
    {
        getSubpictureAttributes(m_DVDVMGM.m_SubpicStream, vmg_ifo->m_bySubpicStreamVMGM_VOBS, 0);
    }
}

int dvdparse::getVmgIfo()
{
    DVDFILESTAT filestat;
    std::string strFileName = getDvdFileName(DVDFILETYPE_VMG_IFO);
    int res = -1;

    if (m_pStatFile((m_strPath + strFileName).c_str(), &filestat, m_strProxy.c_str()) != -1)
    {
        dvdfile *dvdFile = new dvdfile;

        dvdFile->m_DVDFILE.m_eDvdFileType   = DVDFILETYPE_VMG_IFO;
        //dvdFile->m_DVDFILE.m_wTitleSetNo  = 0;
        //dvdFile->m_DVDFILE.m_wVobNo       = 0;
        dvdFile->m_DVDFILE.m_dwSize         = filestat.m_qwFileSize;
        dvdFile->m_DVDFILE.m_Date           = filestat.m_FileTime;

        m_lstDvdFile.push_back(dvdFile);
        res = 0;
    }

    strFileName = getDvdFileName(DVDFILETYPE_VMG_BUP);

    if (m_pStatFile((m_strPath + strFileName).c_str(), &filestat, m_strProxy.c_str()) != -1)
    {
        dvdfile *dvdFile = new dvdfile;

        dvdFile->m_DVDFILE.m_eDvdFileType   = DVDFILETYPE_VMG_BUP;
        //dvdFile->m_DVDFILE.m_wTitleSetNo  = 0;
        //dvdFile->m_DVDFILE.m_wVobNo       = 0;
        dvdFile->m_DVDFILE.m_dwSize         = filestat.m_qwFileSize;
        dvdFile->m_DVDFILE.m_Date           = filestat.m_FileTime;

        m_lstDvdFile.push_back(dvdFile);
        res = 0;
    }

    return res;
}

int dvdparse::getVmgMenuVob()
{
    DVDFILESTAT filestat;
    std::string strFileName = getDvdFileName(DVDFILETYPE_VMG_VOB);

    if (m_pStatFile((m_strPath + strFileName).c_str(), &filestat, m_strProxy.c_str()) != -1)
    {
        dvdfile *dvdFile = new dvdfile;

        dvdFile->m_DVDFILE.m_eDvdFileType   = DVDFILETYPE_VMG_VOB;
        //dvdFile->m_DVDFILE.m_wTitleSetNo  = 0;
        //dvdFile->m_DVDFILE.m_wVobNo       = 0;
        dvdFile->m_DVDFILE.m_dwSize         = filestat.m_qwFileSize;
        dvdFile->m_DVDFILE.m_Date           = filestat.m_FileTime;

        m_lstDvdFile.push_back(dvdFile);
        return 0;
    }
    return -1;
}

void dvdparse::getVmgIfo(time_t ftime, uint64_t qwSizeOfVMGI)
{
    {
        dvdfile *dvdFile = new dvdfile;

        dvdFile->m_DVDFILE.m_eDvdFileType       = DVDFILETYPE_VMG_IFO;
        //dvdFile->m_DVDFILE.m_wTitleSetNo      = 0;
        //dvdFile->m_DVDFILE.m_wVobNo           = 0;
        dvdFile->m_DVDFILE.m_dwSize             = qwSizeOfVMGI;         // Path cannot be empty
        dvdFile->m_DVDFILE.m_Date               = ftime;                // simply use date of title IFO

        m_lstDvdFile.push_back(dvdFile);

#if !defined(NDEBUG) && defined(HAVE_ASSERT_H)
        checkFile(dvdFile);
#endif
    }


    {
        dvdfile *dvdFile = new dvdfile;

        dvdFile->m_DVDFILE.m_eDvdFileType       = DVDFILETYPE_VMG_BUP;
        //dvdFile->m_DVDFILE.m_wTitleSetNo      = 0;
        //dvdFile->m_DVDFILE.m_wVobNo           = 0;
        dvdFile->m_DVDFILE.m_dwSize             = qwSizeOfVMGI;
        dvdFile->m_DVDFILE.m_Date               = ftime;

        m_lstDvdFile.push_back(dvdFile);
#if !defined(NDEBUG) && defined(HAVE_ASSERT_H)
        checkFile(dvdFile);
#endif
    }
}

void dvdparse::getVmgMenuVob(time_t ftime, uint32_t dwMenuVobSize)
{
    dvdfile *dvdFile = new dvdfile;

    dvdFile->m_DVDFILE.m_eDvdFileType        = DVDFILETYPE_VMG_VOB;
    //dvdFile->m_DVDFILE.m_wTitleSetNo       = 0;
    //dvdFile->m_DVDFILE.m_wVobNo            = 0;
    dvdFile->m_DVDFILE.m_dwSize              = dwMenuVobSize;
    dvdFile->m_DVDFILE.m_Date                = ftime;            // simply use date of title IFO

    m_lstDvdFile.push_back(dvdFile);
#if !defined(NDEBUG) && defined(HAVE_ASSERT_H)
    checkFile(dvdFile);
#endif
}

void dvdparse::getVmgPtt(const uint8_t* pData)
{
    LPCVMG_IFO vmg_ifo = (LPCVMG_IFO)pData;
    uint32_t dwStartAddressVMG_PTT_SRPT                 = dvdSector2bytes(be2native(vmg_ifo->m_dwSectorPointerVMG_PTT_SRPT));    // 0C4 	sector pointer to VMG_PTT_SRPT (table of titles)
    LPCVMG_PTT_SRPT_HEADER pVMG_PTT_SRPT_HEADER         = (LPCVMG_PTT_SRPT_HEADER)(pData + dwStartAddressVMG_PTT_SRPT);

    uint16_t wPTTNumberOfTitles                         = be2native(pVMG_PTT_SRPT_HEADER->m_wNumberOfTitles);
    for (uint16_t wTitle = 1; wTitle <= wPTTNumberOfTitles; wTitle++)
    {
        LPCVMG_PTT_SRPT pVMG_PTT_SRPT = (LPCVMG_PTT_SRPT)(pData + dwStartAddressVMG_PTT_SRPT +  sizeof(VMG_PTT_SRPT_HEADER) + (wTitle - 1) * sizeof(VMG_PTT_SRPT));
        dvdpttvmg *dvdPttVmg = new dvdpttvmg(this);

        dvdPttVmg->m_DVDPTTVMG.m_wPlaybackType          = getbyte(pVMG_PTT_SRPT->m_byPlaybackType);
        dvdPttVmg->m_DVDPTTVMG.m_wNumberOfVideoAngles   = getbyte(pVMG_PTT_SRPT->m_byNumberOfVideoAngles);
        dvdPttVmg->m_DVDPTTVMG.m_byParentalMgmMaskVMG   = getbyte(pVMG_PTT_SRPT->m_byParentalMgmMaskVMG);
        dvdPttVmg->m_DVDPTTVMG.m_byParentalMgmMaskVTS   = getbyte(pVMG_PTT_SRPT->m_byParentalMgmMaskVTS);
        dvdPttVmg->m_DVDPTTVMG.m_wVideoTitleSetNo       = getbyte(pVMG_PTT_SRPT->m_byVideoTitleSet);
        dvdPttVmg->m_DVDPTTVMG.m_wTitleNo               = getbyte(pVMG_PTT_SRPT->m_byTitleNumber);

        m_lstDvdPttVmg.push_back(dvdPttVmg);
    }
}

uint16_t dvdparse::parseVideoManager()
{
    string strFileName = getDvdFileName(DVDFILETYPE_VMG_IFO);
    string strFilePath = m_strPath + strFileName;
    const uint8_t* pData = NULL;
    uint16_t wTitleSetCount = 0;
    time_t ftime = 0;

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
            // TODO: VMG_TXTDT_MG not documented... No DVD containing data found yet...
            // uint32_t dwStartAddressVMG_TXTDT_MG     = dvdSector2bytes(be2native(vmg_ifo->m_dwSectorPointerVMG_TXTDT_MG));   // 0D4 	sector pointer to VMG_TXTDT_MG (text data)
            // assert(dwStartAddressVMG_TXTDT_MG == 0);
            // uint32_t dwStartAddressVMGM_C_ADT       = dvdSector2bytes(be2native(vmg_ifo->m_dwSectorPointerVMGM_C_ADT));     // 0D8 	sector pointer to VMGM_C_ADT (menu cell address table)
            // uint32_t dwStartAddressVMGM_VOBU_ADMAP  = dvdSector2bytes(be2native(vmg_ifo->m_dwSectorPointerVMGM_VOBU_ADMAP));// 0DC 	sector pointer to VMGM_VOBU_ADMAP (menu VOBU address map)

            wTitleSetCount = getVmgMain(pData);

            getVmgAttributes(pData);

            if (getFullScan())
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
        }
        else
        {
            setError("Error reading file '" + strFilePath + "': Not a Video Manager IFO file header.", DVDERRORCODE_VMG_IFO);
            wTitleSetCount = 0;
        }
        delete [] pData;
    }

    return wTitleSetCount;
}

void dvdparse::addUnit(const uint8_t* pData, dvdcell * dvdCell)
{
    LPCVTS_IFO pVTS_IFO                          = (LPCVTS_IFO)pData;

    uint32_t dwStartAddressVTS_C_ADT             = dvdSector2bytes(be2native(pVTS_IFO->m_dwSectorPointerVTS_C_ADT));

#ifdef ASSERT_H
    assert(dwStartAddressVTS_C_ADT);            // Should not be 0 (required field by specs)
#endif
    if (dwStartAddressVTS_C_ADT == 0)
    {
        return;
    }

    LPCVTS_C_ADT_HEADER pC_ADT_HEADER = (LPCVTS_C_ADT_HEADER)(pData + dwStartAddressVTS_C_ADT);
    uint16_t wOldVOBidn = 0;
    uint8_t byOldCELLidn = 0;
    uint32_t dwCounter = (be2native(pC_ADT_HEADER->m_dwEndAdress) - 7) / sizeof(VTS_C_ADT);
    dvdCell->m_DVDCELL.m_wNumberOfVOBIds       	= be2native(pC_ADT_HEADER->m_wNumberOfVOBIds);

    uint16_t wUnitNo = 1;

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

        if (dvdCell->m_DVDCELL.m_wVOBidn == wVOBidn &&
                dvdCell->m_DVDCELL.m_wCELLidn == byCELLidn)
        {
            dvdunit *dvdUnit = new dvdunit;

            dvdUnit->m_DVDUNIT.m_wTitleSetNo     = dvdCell->m_DVDCELL.m_wTitleSetNo;
            dvdUnit->m_DVDUNIT.m_wProgramChainNo = dvdCell->m_DVDCELL.m_wProgramChainNo;
            dvdUnit->m_DVDUNIT.m_wProgramNo      = dvdCell->m_DVDCELL.m_wProgramNo;
            dvdUnit->m_DVDUNIT.m_wCellNo         = dvdCell->m_DVDCELL.m_wCellNo;
            dvdUnit->m_DVDUNIT.m_wUnitNo         = wUnitNo++;

            dvdUnit->m_DVDUNIT.m_dwStartSector   = be2native(pC_ADT->m_dwStartSector);
            dvdUnit->m_DVDUNIT.m_dwEndSector     = be2native(pC_ADT->m_dwEndSector);

            dvdCell->m_lstDvdUnit.push_back(dvdUnit);
        }
    }
}

void dvdparse::addCell(const uint8_t* pData, dvdprogram * dvdProgram, uint16_t wCellNo, uint16_t wPGCCellPlaybackOffset, uint32_t dwOffsetVTS_PGC, uint16_t wCellPositionOffset)
{
    dvdcell * dvdCell = new dvdcell;
    LPCVTS_IFO pVTS_IFO                             = (LPCVTS_IFO)pData;

    uint32_t dwStartAddressVTS_PGCI_UT              = dvdSector2bytes(be2native(pVTS_IFO->m_dwSectorPointerVTS_PGCI_UT));

    LPCVTS_PGC pVTS_PGC = (LPCVTS_PGC)(pData + (dwStartAddressVTS_PGCI_UT + dwOffsetVTS_PGC + wPGCCellPlaybackOffset + (sizeof(VTS_PGC) * (wCellNo - 1))));
    dvdCell->m_DVDCELL.m_wTitleSetNo                 = dvdProgram->m_DVDPROGRAM.m_wTitleSetNo;
    dvdCell->m_DVDCELL.m_wProgramChainNo             = dvdProgram->m_DVDPROGRAM.m_wProgramChainNo;
    dvdCell->m_DVDCELL.m_wProgramNo                  = dvdProgram->m_DVDPROGRAM.m_wProgramNo;
    dvdCell->m_DVDCELL.m_wCellNo                     = wCellNo;

    dvdCell->m_DVDCELL.m_eCellType                   = (CELLTYPE)((((pVTS_PGC->m_byCellCategory1 & 0xC0) >> 6) & 0x03) + 1);
    dvdCell->m_DVDCELL.m_eBlockType                  = (BLOCKTYPE)((((pVTS_PGC->m_byCellCategory1 & 0x30) >> 4) & 0x03) + 1);
    dvdCell->m_DVDCELL.m_bSeamlessMultiplex          = (pVTS_PGC->m_byCellCategory1 & 0x08) ? true : false;
    dvdCell->m_DVDCELL.m_bInterleaved                = (pVTS_PGC->m_byCellCategory1 & 0x04) ? true : false;
    dvdCell->m_DVDCELL.m_bSCRdiscontinuity           = (pVTS_PGC->m_byCellCategory1 & 0x02) ? true : false;
    dvdCell->m_DVDCELL.m_bSeamlessAngleLinkedInDSI   = (pVTS_PGC->m_byCellCategory1 & 0x01) ? true : false;
    dvdCell->m_DVDCELL.m_bVOBStillMode               = (pVTS_PGC->m_byCellCategory2 & 0x40) ? true : false;;
    dvdCell->m_DVDCELL.m_bStopsTrickPlay             = (pVTS_PGC->m_byCellCategory2 & 0x20) ? true : false;;
    dvdCell->m_DVDCELL.m_wCellStillTime              = pVTS_PGC->m_byCellStillTime;
    dvdCell->m_DVDCELL.m_wCellCommand                = pVTS_PGC->m_byCellCommand;
    dvdCell->m_DVDCELL.m_qwPlayTime                  = BCDtime(pVTS_PGC->m_byPlaytime);
    dvdCell->m_DVDCELL.m_wFrameRate                  = frameRate(pVTS_PGC->m_byPlaytime);
    dvdCell->m_DVDCELL.m_dwFirstVOBUStartSector      = be2native(pVTS_PGC->m_dwFirstVOBUStartSector);
    dvdCell->m_DVDCELL.m_dwFirstILVUEndSector        = be2native(pVTS_PGC->m_dwFirstILVUEndSector);
    dvdCell->m_DVDCELL.m_dwLastVOBUStartSector       = be2native(pVTS_PGC->m_dwLastVOBUStartSector);
    dvdCell->m_DVDCELL.m_dwLastVOBUEndSector         = be2native(pVTS_PGC->m_dwLastVOBUEndSector);

    if (wCellPositionOffset != 0)
    {
        LPCVTS_CELLPOS pVTS_CELLPOS = (LPCVTS_CELLPOS)(pData + (dwStartAddressVTS_PGCI_UT + dwOffsetVTS_PGC + wCellPositionOffset + (sizeof(VTS_CELLPOS) * (wCellNo - 1))));
        dvdCell->m_DVDCELL.m_wVOBidn                 = be2native(pVTS_CELLPOS->m_wVOBidn);
        dvdCell->m_DVDCELL.m_wCELLidn                = pVTS_CELLPOS->m_byCELLidn;
    }

    addUnit(pData, dvdCell);

    dvdProgram->m_lstDvdCell.push_back(dvdCell);
}

void dvdparse::addPrograms(const uint8_t* pData, dvdpgc *dvdPgc, uint16_t wTitleSetNo, uint16_t wProgramChainNo)
{
    LPCVTS_IFO pVTS_IFO                     = (LPCVTS_IFO)pData;

    dvdPgc->m_DVDPGC.m_wTitleSetNo          = wTitleSetNo;

    uint32_t dwStartAddressVTS_PGCI_UT      = dvdSector2bytes(be2native(pVTS_IFO->m_dwSectorPointerVTS_PGCI_UT));

    LPCVTS_PGCI_UT pVTS_PGCI_UT = (LPCVTS_PGCI_UT)(pData + (dwStartAddressVTS_PGCI_UT + sizeof(VTS_PGCI_UT_HEADER) + (wProgramChainNo - 1) * sizeof(VTS_PGCI_UT)));
    dvdPgc->m_DVDPGC.m_bEntryPGC            = (getbyte(pVTS_PGCI_UT->m_byTitleNo) & 0x80) ? true : false;
    dvdPgc->m_DVDPGC.m_wProgramChainNo      = getbyte(pVTS_PGCI_UT->m_byTitleNo) & 0x7f;
    uint32_t dwOffsetVTS_PGC                = be2native(pVTS_PGCI_UT->m_dwOffsetVTS_PGC);

    LPCVTS_PGCHEADER pVTS_PGCHEADER = (LPCVTS_PGCHEADER)(pData + (dwStartAddressVTS_PGCI_UT + dwOffsetVTS_PGC));
    uint16_t wPGCProgramMapOffset           = be2native(pVTS_PGCHEADER->m_wPGCProgramMapOffset);    // offset within PGC to program map
    uint16_t wPGCCellPlaybackOffset         = be2native(pVTS_PGCHEADER->m_wPGCCellPlaybackOffset);  // offset within PGC to cell playback information table
    uint16_t wCellPositionOffset            = be2native(pVTS_PGCHEADER->m_wCellPositionOffset);     // offset within PGC to cell position information table

    uint16_t wNumberOfPrograms              = getbyte(pVTS_PGCHEADER->m_byNumberOfPrograms);
    uint16_t wNumberOfCells                 = getbyte(pVTS_PGCHEADER->m_byNumberOfCells);

    for (uint16_t wProgramNo = 1; wProgramNo <= wNumberOfPrograms; wProgramNo++)
    {
        //dvdprogram * dvdProgram = dvdPgc->m_lstDvdProgram[wProgramNo - 1];
        dvdprogram * dvdProgram = new dvdprogram;
        dvdPgc->m_lstDvdProgram.push_back(dvdProgram);

        LPCVTS_PROGRAMMAP pVTS_PROGRAMMAP = (LPCVTS_PROGRAMMAP)(pData + (dwStartAddressVTS_PGCI_UT + dwOffsetVTS_PGC + wPGCProgramMapOffset + (wProgramNo - 1)));
        uint16_t wEntryCell = getbyte(pVTS_PROGRAMMAP->m_byEntryCell);
        uint16_t wLastCell  = 0;

        if (wProgramNo < wNumberOfPrograms)
        {
            wLastCell = getbyte((pVTS_PROGRAMMAP + 1)->m_byEntryCell) - 1;
        }
        else
        {
            wLastCell = wNumberOfCells;
        }

        dvdProgram->m_DVDPROGRAM.m_wTitleSetNo      = wTitleSetNo;
        dvdProgram->m_DVDPROGRAM.m_wProgramChainNo  = wProgramChainNo;
        dvdProgram->m_DVDPROGRAM.m_wProgramNo       = wProgramNo;

        for (uint16_t wCellNo = wEntryCell; wCellNo <= wLastCell; wCellNo++)
        {
            addCell(pData, dvdProgram, wCellNo, wPGCCellPlaybackOffset, dwOffsetVTS_PGC, wCellPositionOffset);
        }
    }
}

int dvdparse::getVtsIfo(dvdtitle *dvdTitle, uint16_t wTitleSetNo)
{
    DVDFILESTAT filestat;
    std::string strFileName = getDvdFileName(DVDFILETYPE_VTS_IFO, wTitleSetNo, 0);
    int res = -1;

    if (m_pStatFile((m_strPath + strFileName).c_str(), &filestat, m_strProxy.c_str()) != -1)
    {
        dvdfile *dvdFile = new dvdfile;

        dvdFile->m_DVDFILE.m_eDvdFileType           = DVDFILETYPE_VTS_IFO;
        dvdFile->m_DVDFILE.m_wTitleSetNo            = wTitleSetNo;
        dvdFile->m_DVDFILE.m_wVobNo                 = 0;
        dvdFile->m_DVDFILE.m_dwSize                 = filestat.m_qwFileSize;
        dvdFile->m_DVDFILE.m_Date                   = filestat.m_FileTime;

        dvdTitle->m_lstDvdFile.push_back(dvdFile);
        res = 0;
    }

    strFileName = getDvdFileName(DVDFILETYPE_VTS_BUP, wTitleSetNo, 0);

    if (m_pStatFile((m_strPath + strFileName).c_str(), &filestat, m_strProxy.c_str()) != -1)
    {
        dvdfile *dvdFile = new dvdfile;

        dvdFile->m_DVDFILE.m_eDvdFileType           = DVDFILETYPE_VTS_BUP;
        dvdFile->m_DVDFILE.m_wTitleSetNo            = wTitleSetNo;
        dvdFile->m_DVDFILE.m_wVobNo                 = 0;
        dvdFile->m_DVDFILE.m_dwSize                 = filestat.m_qwFileSize;
        dvdFile->m_DVDFILE.m_Date                   = filestat.m_FileTime;

        dvdTitle->m_lstDvdFile.push_back(dvdFile);
        res = 0;
    }
    return res;
}

int dvdparse::getVtsMenuVob(dvdtitle *dvdTitle, uint16_t wTitleSetNo)
{
    DVDFILESTAT filestat;
    std::string strFileName = getDvdFileName(DVDFILETYPE_MENU_VOB, wTitleSetNo, 0);

    if (m_pStatFile((m_strPath + strFileName).c_str(), &filestat, m_strProxy.c_str()) != -1)
    {
        dvdfile *dvdFile = new dvdfile;

        dvdFile->m_DVDFILE.m_eDvdFileType            = DVDFILETYPE_MENU_VOB;
        dvdFile->m_DVDFILE.m_wTitleSetNo             = wTitleSetNo;
        dvdFile->m_DVDFILE.m_wVobNo                  = 0;
        dvdFile->m_DVDFILE.m_dwSize                  = filestat.m_qwFileSize;
        dvdFile->m_DVDFILE.m_Date                    = filestat.m_FileTime;

        dvdTitle->m_lstDvdFile.push_back(dvdFile);
        return 0;
    }
    return -1;
}

int dvdparse::getVtsVob(dvdtitle *dvdTitle, uint16_t wVobNo, uint16_t wTitleSetNo)
{
    DVDFILESTAT filestat;
    std::string strFileName = getDvdFileName(DVDFILETYPE_VTS_VOB, wTitleSetNo, wVobNo);

    if (m_pStatFile((m_strPath + strFileName).c_str(), &filestat, m_strProxy.c_str()) != -1)
    {
        dvdfile *dvdFile = new dvdfile;

        dvdFile->m_DVDFILE.m_eDvdFileType            = DVDFILETYPE_VTS_VOB;
        dvdFile->m_DVDFILE.m_wTitleSetNo             = wTitleSetNo;
        dvdFile->m_DVDFILE.m_wVobNo                  = wVobNo;
        dvdFile->m_DVDFILE.m_dwSize                  = filestat.m_qwFileSize;
        dvdFile->m_DVDFILE.m_Date                    = filestat.m_FileTime;

        dvdTitle->m_lstDvdFile.push_back(dvdFile);
        return 0;
    }
    return -1;
}

void dvdparse::getVtsIfo(dvdtitle *dvdTitle, time_t ftime, uint64_t qwSizeOfVMGI, uint16_t wTitleSetNo)
{
    {
        dvdfile *dvdFile = new dvdfile;

        dvdFile->m_DVDFILE.m_eDvdFileType               = DVDFILETYPE_VTS_IFO;
        dvdFile->m_DVDFILE.m_wTitleSetNo                = wTitleSetNo;
        dvdFile->m_DVDFILE.m_wVobNo                     = 0;
        dvdFile->m_DVDFILE.m_dwSize                     = qwSizeOfVMGI;
        dvdFile->m_DVDFILE.m_Date                       = ftime;           // simply use date of title IFO

        dvdTitle->m_lstDvdFile.push_back(dvdFile);
#if !defined(NDEBUG) && defined(HAVE_ASSERT_H)
        checkFile(dvdFile);
#endif
    }

    {
        dvdfile *dvdFile = new dvdfile;

        dvdFile->m_DVDFILE.m_eDvdFileType               = DVDFILETYPE_VTS_BUP;
        dvdFile->m_DVDFILE.m_wTitleSetNo                = wTitleSetNo;
        dvdFile->m_DVDFILE.m_wVobNo                     = 0;
        dvdFile->m_DVDFILE.m_dwSize                     = qwSizeOfVMGI;
        dvdFile->m_DVDFILE.m_Date                       = ftime;

        dvdTitle->m_lstDvdFile.push_back(dvdFile);
#if !defined(NDEBUG) && defined(HAVE_ASSERT_H)
        checkFile(dvdFile);
#endif
    }
}

void dvdparse::getVtsMenuVob(dvdtitle *dvdTitle, time_t ftime, uint32_t dwMenuVobSize, uint16_t wTitleSetNo)
{
    dvdfile *dvdFile = new dvdfile;

    dvdFile->m_DVDFILE.m_eDvdFileType                = DVDFILETYPE_MENU_VOB;
    dvdFile->m_DVDFILE.m_wTitleSetNo                 = wTitleSetNo;
    dvdFile->m_DVDFILE.m_wVobNo                      = 0;
    dvdFile->m_DVDFILE.m_dwSize                      = dwMenuVobSize;
    dvdFile->m_DVDFILE.m_Date                        = ftime;           // simply use date of title IFO

    dvdTitle->m_lstDvdFile.push_back(dvdFile);
#if !defined(NDEBUG) && defined(HAVE_ASSERT_H)
    checkFile(dvdFile);
#endif
}

void dvdparse::getVtsVob(dvdtitle *dvdTitle, time_t ftime, uint32_t dwSize, uint16_t wVobNo, uint16_t wTitleSetNo)
{
    dvdfile *dvdFile = new dvdfile;

    dvdFile->m_DVDFILE.m_eDvdFileType                = DVDFILETYPE_VTS_VOB;
    dvdFile->m_DVDFILE.m_wTitleSetNo                 = wTitleSetNo;
    dvdFile->m_DVDFILE.m_wVobNo                      = wVobNo;
    dvdFile->m_DVDFILE.m_dwSize                      = dwSize;
    dvdFile->m_DVDFILE.m_Date                        = ftime;           // simply use date of title IFO

    dvdTitle->m_lstDvdFile.push_back(dvdFile);

#if !defined(NDEBUG) && defined(HAVE_ASSERT_H)
    checkFile(dvdFile);
#endif
}

void dvdparse::getVtsAttributes(const uint8_t *pData, dvdtitle * dvdTitle)
{
    LPCVTS_IFO pVTS_IFO = (LPCVTS_IFO)pData;

    // video attributes of VTSM_VOBS
    getVideoAttributes(dvdTitle->m_DVDVTS.m_VideoStreamVTSM, pVTS_IFO->m_byVideoStreamVTS_VOBS);

    // audio attributes of VTSM_VOBS
    dvdTitle->m_DVDVTS.m_wAudioStreamCountVTSM        = be2native(pVTS_IFO->m_wAudioStreamCountVTSM);
    if (dvdTitle->m_DVDVTS.m_wAudioStreamCountVTSM)
    {
        getAudioAttributes(dvdTitle->m_DVDVTS.m_AudioStreamVTSM, pVTS_IFO->m_byAudioStreamVTSM, 0);
    }

    // subpicture attributes of VTSM_VOBS
    dvdTitle->m_DVDVTS.m_wSubpicStreamCountVTSM   = be2native(pVTS_IFO->m_wSubpicStreamCountVTSM);
    if (dvdTitle->m_DVDVTS.m_wSubpicStreamCountVTSM)
    {
        getSubpictureAttributes(dvdTitle->m_DVDVTS.m_SubpicStreamVTSM, pVTS_IFO->m_bySubpicStreamVTSM_VOBS, 0);
    }

    // video attributes of VTS_VOBS
    getVideoAttributes(dvdTitle->m_DVDVTS.m_VideoStreamVTS, pVTS_IFO->m_byVideoStreamVTS_VOBS);
    // audio attributes of VTS_VOBS
    dvdTitle->m_DVDVTS.m_wAudioStreamCountVTS         = be2native(pVTS_IFO->m_wAudioStreamCountVTS);
    for (uint16_t wAudioStreamNo = 1; wAudioStreamNo <= dvdTitle->m_DVDVTS.m_wAudioStreamCountVTS; wAudioStreamNo++)
    {
        getAudioAttributes(dvdTitle->m_DVDVTS.m_AudioStreamVTS[wAudioStreamNo - 1], pVTS_IFO->m_byAudioStreamsVTS_VOBS[wAudioStreamNo - 1], wAudioStreamNo);
        getAudioAttributesEx(dvdTitle->m_DVDVTS.m_AudioStreamExVTS[wAudioStreamNo - 1], pVTS_IFO->m_byAudioStreamsVTS_VOBS[wAudioStreamNo - 1]);
    }

    // subpicture attributes of VTS_VOBS
    dvdTitle->m_DVDVTS.m_wSubpicStreamCountVTS    = be2native(pVTS_IFO->m_wSubpicStreamCountVTS);
    for (uint16_t wSubpicStreamNo = 1; wSubpicStreamNo <= dvdTitle->m_DVDVTS.m_wSubpicStreamCountVTS; wSubpicStreamNo++)
    {
        getSubpictureAttributes(dvdTitle->m_DVDVTS.m_SubpicStreamVTS[wSubpicStreamNo - 1], pVTS_IFO->m_bySubpicStreamVTS_VOBS[wSubpicStreamNo - 1], wSubpicStreamNo);
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
            //dvdtitle * dvdTitle = m_lstDvdTitle[wTitleSetNo - 1];
            dvdtitle * dvdTitle = new dvdtitle;
            m_lstDvdTitle.push_back(dvdTitle);

            dvdTitle->m_DVDVTS.m_wTitleSetNo                = wTitleSetNo;
            dvdTitle->m_DVDVTS.m_wVersionNumberMajor        = ((pVTS_IFO->m_wVersionNumber & 0xF000) >> 12);
            dvdTitle->m_DVDVTS.m_wVersionNumberMinor        = ((pVTS_IFO->m_wVersionNumber & 0x0F00) >> 8);

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
                uint16_t wPttTitleSetNo = wPttVmgNo - 1 + m_nVirtTitleCount;

                if (wPttTitleSetNo >= getDvdPttVmgCount())
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

                dvdpttvmg * dvdPttVmg = m_lstDvdPttVmg[wPttTitleSetNo];

                for (uint16_t wPttChapterNo = 1; wPttChapterNo <= dwCounter; wPttChapterNo++)
                {
                    dvdpttvts *dvdPttVts = new dvdpttvts(this);

                    LPCVTS_PTT pVTS_PTT = (LPCVTS_PTT)(pData + dwStartAddressVTS_PTT_SRPT + dwOffsetToPTT + (wPttChapterNo - 1) * sizeof(VTS_PTT));

                    dvdPttVts->m_DVDPTTVTS.m_wTitleSetNo        = wTitleSetNo;
                    dvdPttVts->m_DVDPTTVTS.m_wPttTitleSetNo     = wPttTitleSetNo + 1;
                    dvdPttVts->m_DVDPTTVTS.m_wPttChapterNo      = wPttChapterNo;

                    dvdPttVts->m_DVDPTTVTS.m_wProgramChainNo    = be2native(pVTS_PTT->m_wProgramChainNo);
                    dvdPttVts->m_DVDPTTVTS.m_wProgramNo         = be2native(pVTS_PTT->m_wProgramNo);

                    dvdPttVmg->m_lstDvdPttVts.push_back(dvdPttVts);
                }
            }

            m_nVirtTitleCount += wNumberOfTitles;

            uint32_t dwStartAddressVTS_PGCI_UT                  = dvdSector2bytes(be2native(pVTS_IFO->m_dwSectorPointerVTS_PGCI_UT));
            LPCVTS_PGCI_UT_HEADER pVTS_PGCI_UT_HEADER           = (LPCVTS_PGCI_UT_HEADER)(pData + dwStartAddressVTS_PGCI_UT);
            uint16_t wNumberOfProgramChains                     = be2native(pVTS_PGCI_UT_HEADER->m_wNumberOfProgramChains);

            for (uint16_t wProgramChainNo = 1; wProgramChainNo <= wNumberOfProgramChains; wProgramChainNo++)
            {
                dvdpgc * dvdPgc = new dvdpgc;
                dvdTitle->m_lstDvdPgc.push_back(dvdPgc);

                addPrograms(pData, dvdPgc, wTitleSetNo, wProgramChainNo);
            }

            if (getFullScan())
            {
                // Menu IFO
                getVtsIfo(dvdTitle, wTitleSetNo);
                // Menu VOB
                getVtsMenuVob(dvdTitle, wTitleSetNo);

                for (uint16_t wVobNo = 1; wVobNo <= DVD_MAX_VOB; wVobNo++)
                {
                    if (getVtsVob(dvdTitle, wVobNo, wTitleSetNo) == -1)
                    {
                        break;
                    }
                }
            }
            else
            {
                uint64_t qwSizeOfVTS        = dvdSector2bytes(be2native(pVTS_IFO->m_dwLastSectorVTS) + 1);      // 00C 	4		last sector of VMG set (last sector of BUP)
                uint64_t qwSizeOfVMGI       = dvdSector2bytes(be2native(pVTS_IFO->m_dwLastSectorVMGI) + 1);     // 01C 	4		last sector of IFO
                uint64_t qwSizeOfVTSM_VOBS  = dvdSector2bytes(be2native(pVTS_IFO->m_dwSectorPointerVTSM_VOBS)); // 0C0 	4		sector pointer to VTSM_VOBS (Menu VOB)
                uint64_t qwSizeOfVTST_VOBS  = dvdSector2bytes(be2native(pVTS_IFO->m_dwSectorPointerVTST_VOBS)); // 0C4 	4		sector pointer to VTST_VOBS (Title VOB)

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

        delete [] pData;
    }

    return bSuccess;
}

LPCDVDVMGM dvdparse::getDVDVMGM() const
{
    return &m_DVDVMGM;
}

dvdtitle *dvdparse::getDvdTitle(uint16_t wTitleSetNo) const
{
    if (!wTitleSetNo || wTitleSetNo > getTitleSetCount())
    {
        return NULL;
    }

    return m_lstDvdTitle[wTitleSetNo - 1];
}

uint16_t dvdparse::getTitleSetCount() const
{
    return m_lstDvdTitle.size();
}

dvdfile * dvdparse::getDvdFile(uint16_t wFileNo) const
{
    if (wFileNo >= m_lstDvdFile.size())
    {
        return NULL;
    }

    return m_lstDvdFile[wFileNo];
}

uint16_t dvdparse::getFileCount() const
{
    return (uint16_t)m_lstDvdFile.size();
}

dvdpgc * dvdparse::getDvdPgc(uint16_t wTitleSetNo, uint16_t wProgramChainNo) const
{
    const dvdtitle *pDvdTitle = getDvdTitle(wTitleSetNo);

    if (pDvdTitle == NULL)
    {
        return NULL;
    }

    return pDvdTitle->getDvdPgc(wProgramChainNo);
}

dvdprogram * dvdparse::getDvdProgram(uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo) const
{
    const dvdpgc *pDvdPgc = getDvdPgc(wTitleSetNo, wProgramChainNo);

    if (pDvdPgc == NULL)
    {
        return NULL;
    }

    return pDvdPgc->getDvdProgram(wProgramNo);
}

dvdprogram * dvdparse::getDvdProgram(LPCDVDPTTVTS pDVDPTTVTS) const
{
    dvdprogram *pDvdProgram = getDvdProgram(pDVDPTTVTS->m_wTitleSetNo, pDVDPTTVTS->m_wProgramChainNo, pDVDPTTVTS->m_wProgramNo);

    if (pDvdProgram == NULL)
    {
        return NULL;
    }

    return pDvdProgram;
}

dvdpttvmg * dvdparse::getDvdPttVmg(uint16_t wTitleSetNo) const
{
    if (!wTitleSetNo || wTitleSetNo > getDvdPttVmgCount())
    {
        return NULL;
    }

    return m_lstDvdPttVmg[wTitleSetNo - 1];
}

uint16_t dvdparse::getDvdPttVmgCount() const
{
    return m_lstDvdPttVmg.size();
}

dvdpttvts * dvdparse::getDvdPttVts(uint16_t wTitleSetNo, uint16_t wPttNo) const
{
    const dvdpttvmg *pDvdPttVmg = getDvdPttVmg(wTitleSetNo);

    if (pDvdPttVmg == NULL)
    {
        return NULL;
    }

    return pDvdPttVmg->getDvdPttVts(wPttNo);
}

dvdpttvts* dvdparse::getDvdPttVts(const dvdprogram *pDvdProgram) const
{
    if (pDvdProgram == NULL)
    {
        return NULL;
    }

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= getDvdPttVmgCount(); wTitleSetNo++)
    {
        const dvdpttvmg *pDvdPttVmg = getDvdPttVmg(wTitleSetNo);

        for (uint16_t wPttNo = 1; wPttNo <= pDvdPttVmg->getPttVtsCount(); wPttNo++)
        {
            dvdpttvts * pDvdPttVts = pDvdPttVmg->getDvdPttVts(wPttNo);
            LPCDVDPTTVTS pDVDPTTVTS = pDvdPttVts->getDVDPTTVTS();
            const dvdprogram * pDvdProgram2 = getDvdProgram(pDVDPTTVTS);

            if (pDvdProgram == pDvdProgram2)
            {
                return pDvdPttVts;
            }
        }
    }

    return NULL;
}

dvdcell *dvdparse::getDvdCell(uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo, uint16_t wCellNo) const
{
    const dvdprogram *pDvdProgram = getDvdProgram(wTitleSetNo, wProgramChainNo, wProgramNo);

    if (pDvdProgram == NULL)
    {
        return NULL;
    }

    return pDvdProgram->getDvdCell(wCellNo);
}

dvdunit * dvdparse::getDvdUnit(uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo, uint16_t wCellNo, uint16_t wUnitNo) const
{
    const dvdcell *pDvdCell = getDvdCell(wTitleSetNo, wProgramChainNo, wProgramNo, wCellNo);

    if (pDvdCell == NULL)
    {
        return NULL;
    }

    return pDvdCell->getDvdUnit(wUnitNo);
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

const std::string & dvdparse::getErrorString() const
{
    return m_strErrorString;
}

DVDERRORCODE dvdparse::getErrorCode() const
{
    return m_eErrorCode;
}

const std::string & dvdparse::getPath() const
{
    return m_strPath;
}

void dvdparse::setScanMode(DVDSCANMODE eScanMode)
{
    m_eScanMode = eScanMode;
}

bool dvdparse::getScanMode() const
{
    return m_eScanMode;
}

bool dvdparse::getFullScan() const
{
    bool bFullScan = true;

    switch (m_eScanMode)
    {
    case DVDSCANMODE_FAST:
    {
        bFullScan = false;
        break;
    }
    case DVDSCANMODE_FULL:
    {
        bFullScan = true;
        break;
    }
    case DVDSCANMODE_AUTO:
    {
        bFullScan = !::isUrl(m_strPath);    // Auto: full scan only for local files
        break;
    }
    default:
        break;
    }
    return bFullScan;
}

void dvdparse::getVideoAttributes(DVDVIDEOSTREAM & videoAttributes, const uint8_t byVideoAttributes[2])
{
    int nCoding     = (byVideoAttributes[0] & 0xC0) >> 6;
    int nStandard   = (byVideoAttributes[0] & 0x30) >> 4;
    int nAspect     = (byVideoAttributes[0] & 0x0C) >> 2;
    int nResolution = (byVideoAttributes[0] & 0x08) >> 3;

    switch (nCoding)
    {
    case 0:
        videoAttributes.m_eCodingMode = DVDVIDEOCODINGMODE_MPEG1;
        break;
    case 1:
        videoAttributes.m_eCodingMode = DVDVIDEOCODINGMODE_MPEG2;
        break;
    default:
        videoAttributes.m_eCodingMode = DVDVIDEOCODINGMODE_INVALID;
        break;
    }

    switch (nStandard)
    {
    case 0:
        videoAttributes.m_eStandard = DVDVIDEOTVSTANDARD_NTSC;

        switch (nResolution)
        {
        case 0:
            videoAttributes.m_Resolution.m_wX = 720;
            videoAttributes.m_Resolution.m_wY = 480;
            break;
        case 1:
            videoAttributes.m_Resolution.m_wX = 704;
            videoAttributes.m_Resolution.m_wY = 480;
            break;
        case 2:
            videoAttributes.m_Resolution.m_wX = 352;
            videoAttributes.m_Resolution.m_wY = 480;
            break;
        case 3:
            videoAttributes.m_Resolution.m_wX = 352;
            videoAttributes.m_Resolution.m_wY = 240;
            break;
        }
        break;
    case 1:
        videoAttributes.m_eStandard = DVDVIDEOTVSTANDARD_PAL;

        switch (nResolution)
        {
        case 0:
            videoAttributes.m_Resolution.m_wX = 720;
            videoAttributes.m_Resolution.m_wY = 576;
            break;
        case 1:
            videoAttributes.m_Resolution.m_wX = 704;
            videoAttributes.m_Resolution.m_wY = 576;
            break;
        case 2:
            videoAttributes.m_Resolution.m_wX = 352;
            videoAttributes.m_Resolution.m_wY = 576;
            break;
        case 3:
            videoAttributes.m_Resolution.m_wX = 352;
            videoAttributes.m_Resolution.m_wY = 288;
            break;
        }

        break;
    default:
        videoAttributes.m_eStandard = DVDVIDEOTVSTANDARD_INVALID;
        break;
    }

    switch (nAspect)
    {
    case 0:
        videoAttributes.m_eAspect = DVDVIDEOASPECT_4_3;
        break;
    case 3:
        videoAttributes.m_eAspect = DVDVIDEOASPECT_16_9;
        break;
    default:
        videoAttributes.m_eAspect = DVDVIDEOASPECT_INVALID;
        break;
    }

    videoAttributes.m_bAutomaticPanScanDisallowed   = (byVideoAttributes[0] & 0x02) ? true : false;
    videoAttributes.m_bAutomaticLetterboxDisallowed = (byVideoAttributes[0] & 0x01) ? true : false;
    videoAttributes.m_bCCForLine21Field1InGOP       = (byVideoAttributes[1] & 0x80) ? true : false;
    videoAttributes.m_bCCForLine21Field2InGOP       = (byVideoAttributes[1] & 0x40) ? true : false;
    videoAttributes.m_bCBR                          = (byVideoAttributes[1] & 0x10) ? true : false;
    videoAttributes.m_bLetterBoxed                  = (byVideoAttributes[1] & 0x04) ? true : false;
    videoAttributes.m_bSourceFilm                   = (byVideoAttributes[1] & 0x01) ? true : false;

    videoAttributes.m_wID                           = 0xE0;  // Fixed
}

void dvdparse::getAudioAttributes(DVDAUDIOSTREAM & audioAttributes, const uint8_t byAudioAttributes[DVDVTS_MAXAUDIOSTREAM], uint16_t wAudioStreamNo)
{
    int nCoding                 = (byAudioAttributes[0] & 0xE0) >> 5;
    //    int nSoundApplicationMode   = (byAudioAttributes[0] & 0x03);
    int nQuantisation           = (byAudioAttributes[1] & 0xC0) >> 6;
    int nSampleRate             = (byAudioAttributes[1] & 0x30) >> 4;

    switch (nCoding)
    {
    case 0:	// 0 = AC3
    {
        audioAttributes.m_eCodingMode = DVDAUDIOCODINGMODE_AC3;
        audioAttributes.m_wID                   = 0x80 + (wAudioStreamNo - 1);
        break;
    }
    case 1:	// 1 = Undefined
    {
        audioAttributes.m_eCodingMode = DVDAUDIOCODINGMODE_INVALID;
        audioAttributes.m_wID                   = 0x80 + (wAudioStreamNo - 1);
        break;
    }
    case 2:	// 2 = MPEG-1
    {
        audioAttributes.m_eCodingMode = DVDAUDIOCODINGMODE_MPEG1;
        audioAttributes.m_wID                   = 0xC0 + (wAudioStreamNo - 1);
        break;
    }
    case 3:	// 3 = MPEG-2
    {
        audioAttributes.m_eCodingMode = DVDAUDIOCODINGMODE_MPEG2;
        audioAttributes.m_wID                   = 0xC8 + (wAudioStreamNo - 1);
        break;
    }
    case 4:	// 4 = LPCM
    {
        audioAttributes.m_eCodingMode = DVDAUDIOCODINGMODE_LPCM;
        audioAttributes.m_wID                   = 0xA0 + (wAudioStreamNo - 1);
        break;
    }
    case 5:	// 5 = Undefined
    {
        audioAttributes.m_eCodingMode = DVDAUDIOCODINGMODE_INVALID;
        audioAttributes.m_wID                   = 0x88 + (wAudioStreamNo - 1);
        break;
    }
    case 6:	// 6 = DTS
    {
        audioAttributes.m_eCodingMode = DVDAUDIOCODINGMODE_DTS;
        audioAttributes.m_wID                   = 0x88 + (wAudioStreamNo - 1);
        break;
    }
    case 7:	// 7 = Undefined
    {
        audioAttributes.m_eCodingMode = DVDAUDIOCODINGMODE_INVALID;
        audioAttributes.m_wID                   = 0x88 + (wAudioStreamNo - 1);
        break;
    }
    }

    audioAttributes.m_bMultichannelExtPresent = (byAudioAttributes[0] & 0x10) ? true : false;
    //audioAttributes.m_bLanguageCodePresent          = ((((byAudioAttributes[0] & 0x0C) >> 2) == 1) && byAudioAttributes[2]) ? true : false;

    //    switch (nSoundApplicationMode)
    //    {
    //    case 0:
    //        audioAttributes.m_eApplicationMode = DVDAUDIOAPPLICATIONMODE_UNSPECIFIED;
    //        break;
    //    case 1:
    //        audioAttributes.m_eApplicationMode = DVDAUDIOAPPLICATIONMODE_KARAOKE;
    //        break;
    //    case 2:
    //        audioAttributes.m_eApplicationMode = DVDAUDIOAPPLICATIONMODE_SURROUND;
    //        break;
    //    default:
    //        audioAttributes.m_eApplicationMode = DVDAUDIOAPPLICATIONMODE_INVALID;
    //        break;
    //    }

    switch (nQuantisation)
    {
    case 0:
        audioAttributes.m_eQuantisation = DVDAUDIOQUANTISATION_16BPS;                     // 0 = 16bps
        break;
    case 1:
        audioAttributes.m_eQuantisation = DVDAUDIOQUANTISATION_20BPS;                     // 1 = 20bps
        break;
    case 2:
        audioAttributes.m_eQuantisation = DVDAUDIOQUANTISATION_24BPS;                     // 2 = 24bps
        break;
    case 3:
        audioAttributes.m_eQuantisation = DVDAUDIOQUANTISATION_DRC;                       // 3 = DRC
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

void dvdparse::getAudioAttributesEx(DVDAUDIOSTREAMEX & audioAttributesEx, const uint8_t byAudioAttributes[DVDVTS_MAXAUDIOSTREAM])
{
    //    int nCoding                 = (byAudioAttributes[0] & 0xE0) >> 5;
    int nSoundApplicationMode   = (byAudioAttributes[0] & 0x03);
    //    int nQuantisation           = (byAudioAttributes[1] & 0xC0) >> 6;
    //    int nSampleRate             = (byAudioAttributes[1] & 0x30) >> 4;
    int nCodeExt                = byAudioAttributes[5];

    //    switch (nCoding)
    //    {
    //    case 0:	// 0 = AC3
    //    {
    //        audioAttributes.m_eCodingMode           = DVDAUDIOCODINGMODE_AC3;
    //        audioAttributes.m_wID                  = 0x80 + (wAudioStreamNo - 1);
    //        break;
    //    }
    //    case 1:	// 1 = Undefined
    //    {
    //        audioAttributes.m_eCodingMode           = DVDAUDIOCODINGMODE_INVALID;
    //        audioAttributes.m_wID                  = 0x80 + (wAudioStreamNo - 1);
    //        break;
    //    }
    //    case 2:	// 2 = MPEG-1
    //    {
    //        audioAttributes.m_eCodingMode           = DVDAUDIOCODINGMODE_MPEG1;
    //        audioAttributes.m_wID                  = 0xC0 + (wAudioStreamNo - 1);
    //        break;
    //    }
    //    case 3:	// 3 = MPEG-2
    //    {
    //        audioAttributes.m_eCodingMode           = DVDAUDIOCODINGMODE_MPEG2;
    //        audioAttributes.m_wID                  = 0xC8 + (wAudioStreamNo - 1);
    //        break;
    //    }
    //    case 4:	// 4 = LPCM
    //    {
    //        audioAttributes.m_eCodingMode           = DVDAUDIOCODINGMODE_LPCM;
    //        audioAttributes.m_wID                  = 0xA0 + (wAudioStreamNo - 1);
    //        break;
    //    }
    //    case 5:	// 5 = Undefined
    //    {
    //        audioAttributes.m_eCodingMode           = DVDAUDIOCODINGMODE_INVALID;
    //        audioAttributes.m_wID                  = 0x88 + (wAudioStreamNo - 1);
    //        break;
    //    }
    //    case 6:	// 6 = DTS
    //    {
    //        audioAttributes.m_eCodingMode           = DVDAUDIOCODINGMODE_DTS;
    //        audioAttributes.m_wID                  = 0x88 + (wAudioStreamNo - 1);
    //        break;
    //    }
    //    case 7:	// 7 = Undefined
    //    {
    //        audioAttributes.m_eCodingMode           = DVDAUDIOCODINGMODE_INVALID;
    //        audioAttributes.m_wID                  = 0x88 + (wAudioStreamNo - 1);
    //        break;
    //    }
    //    }

    //    audioAttributes.m_bMultichannelExtPresent   = (byAudioAttributes[0] & 0x10) ? true : false;
    audioAttributesEx.m_bLanguageCodePresent      = ((((byAudioAttributes[0] & 0x0C) >> 2) == 1) && byAudioAttributes[2]) ? true : false;

    switch (nSoundApplicationMode)
    {
    case 0:
        audioAttributesEx.m_eApplicationMode      = DVDAUDIOAPPLICATIONMODE_UNSPECIFIED;
        break;
    case 1:
        audioAttributesEx.m_eApplicationMode      = DVDAUDIOAPPLICATIONMODE_KARAOKE;
        break;
    case 2:
        audioAttributesEx.m_eApplicationMode      = DVDAUDIOAPPLICATIONMODE_SURROUND;
        break;
    default:
        audioAttributesEx.m_eApplicationMode      = DVDAUDIOAPPLICATIONMODE_INVALID;
        break;
    }

    //    switch (nQuantisation)
    //    {
    //    case 0:
    //        audioAttributes.m_eQuantisation         = DVDAUDIOQUANTISATION_16BPS;                     // 0 = 16bps
    //        break;
    //    case 1:
    //        audioAttributes.m_eQuantisation         = DVDAUDIOQUANTISATION_20BPS;                     // 1 = 20bps
    //        break;
    //    case 2:
    //        audioAttributes.m_eQuantisation         = DVDAUDIOQUANTISATION_24BPS;                     // 2 = 24bps
    //        break;
    //    case 3:
    //        audioAttributes.m_eQuantisation         = DVDAUDIOQUANTISATION_DRC;                       // 3 = DRC
    //        break;
    //    }

    //    switch (nSampleRate)
    //    {
    //    case 0:
    //        audioAttributes.m_dwSampleRate          = 48000;
    //        break;
    //    case 2:
    //        audioAttributes.m_dwSampleRate          = 96000;
    //        break;
    //    default:
    //        audioAttributes.m_dwSampleRate          = 0;
    //        break;
    //    }

    //    audioAttributes.m_wChannels = (byAudioAttributes[1] & 0x07) + 1;

    if (audioAttributesEx.m_bLanguageCodePresent)
    {
        getString2(audioAttributesEx.m_szLanguageCode, &byAudioAttributes[2]);
    }

    switch (nCodeExt)
    {
    case 0:
        audioAttributesEx.m_eCodeExt              = DVDAUDIOCODEEXT_UNSPECIFIED;              // 0 = unspecified
        break;
    case 1:
        audioAttributesEx.m_eCodeExt              = DVDAUDIOCODEEXT_NORMAL;                   // 1 = normal,
        break;
    case 2:
        audioAttributesEx.m_eCodeExt              = DVDAUDIOCODEEXT_VISUALLY_IMPAIRED;        // 2 = for visually impaired
        break;
    case 3:
        audioAttributesEx.m_eCodeExt              = DVDAUDIOCODEEXT_DIRECTORS_COMMENTS;       // 3 = director's comments
        break;
    case 4:
        audioAttributesEx.m_eCodeExt              = DVDAUDIOCODEEXT_ALT_DIRECTORS_COMMENTS;    // 4 = alternate director's comments
        break;
    default:
        audioAttributesEx.m_eCodeExt              = DVDAUDIOCODEEXT_INVALID;
        break;
    }

    switch (audioAttributesEx.m_eApplicationMode)
    {
    case DVDAUDIOAPPLICATIONMODE_KARAOKE:
    {
        int nChannelAssignments     = (byAudioAttributes[7] & 0x70) >> 4;

        switch (nChannelAssignments)
        {
        case 2:
            audioAttributesEx.m_eChannelAssignment = DVDAUDIOCHANNELASSIGNMENT_2_0;  //  2 = 2/0 L,R
            break;
        case 3:
            audioAttributesEx.m_eChannelAssignment = DVDAUDIOCHANNELASSIGNMENT_3_0;  //  3 = 3/0 L,M,R
            break;
        case 4:
            audioAttributesEx.m_eChannelAssignment = DVDAUDIOCHANNELASSIGNMENT_2_1;  //  4 = 2/1 L,R,V1
            break;
        case 5:
            audioAttributesEx.m_eChannelAssignment = DVDAUDIOCHANNELASSIGNMENT_3_1;  //  5 = 3/1 L,M,R,V1
            break;
        case 6:
            audioAttributesEx.m_eChannelAssignment = DVDAUDIOCHANNELASSIGNMENT_2_2;  //  6 = 2/2 L,R,V1,V2
            break;
        case 7:
            audioAttributesEx.m_eChannelAssignment = DVDAUDIOCHANNELASSIGNMENT_3_2;  //  7 = 3/2 L,M,R,V1,V2
            break;
        default:
            audioAttributesEx.m_eChannelAssignment = DVDAUDIOCHANNELASSIGNMENT_INVALID;
            break;
        }

        audioAttributesEx.m_wKaraokeVersion       = (byAudioAttributes[7] & 0x0C) >> 2;

        audioAttributesEx.m_bMCIntroPresent       = (byAudioAttributes[7] & 0x02) ? true : false;
        audioAttributesEx.m_eKaraokeMode          = (byAudioAttributes[7] & 0x01) ? DVDKARAOKEMODE_DUET : DVDKARAOKEMODE_SOLO;

        break;
    }
    case DVDAUDIOAPPLICATIONMODE_SURROUND:
    {
        audioAttributesEx.m_bSuitableForDolbySurroundDecoding = (byAudioAttributes[7] & 0x08) ? true : false;
        break;
    }
    default:
        break;
    }
}

void dvdparse::getSubpictureAttributes(DVDSUBPICSTREAM & subpictureAttributes, const uint8_t bySubpictureAttributes[6], uint16_t wSubpicStreamNo)
{
    int nSoundApplicationMode                           = bySubpictureAttributes[5];

    subpictureAttributes.m_wCodingMode                  = (bySubpictureAttributes[0] & 0xE0) >> 5;
    subpictureAttributes.m_bLanguageCodePresent         = (((bySubpictureAttributes[0] & 0x03) == 1) && bySubpictureAttributes[2]) ? true : false;

    if (subpictureAttributes.m_bLanguageCodePresent)
    {
        getString2(subpictureAttributes.m_szLanguageCode, &bySubpictureAttributes[2]);
    }

    switch (nSoundApplicationMode)
    {
    case 0:
        subpictureAttributes.m_eCodeExt = DVDSUBPICCODEEXT_NOT_SPECIFIED;                       //  0 = not specified
        break;
    case 1:
        subpictureAttributes.m_eCodeExt = DVDSUBPICCODEEXT_NORMAL;                              //  1 = normal
        break;
    case 2:
        subpictureAttributes.m_eCodeExt = DVDSUBPICCODEEXT_LARGE;                               //  2 = large
        break;
    case 3:
        subpictureAttributes.m_eCodeExt = DVDSUBPICCODEEXT_CHILDREN;                            //  3 = children
        break;
    case 5:
        subpictureAttributes.m_eCodeExt = DVDSUBPICCODEEXT_NORMAL_CAPTIONS;                     //  5 = normal captions
        break;
    case 6:
        subpictureAttributes.m_eCodeExt = DVDSUBPICCODEEXT_LARGE_CAPTIONS;                      //  6 = large captions
        break;
    case 7:
        subpictureAttributes.m_eCodeExt = DVDSUBPICCODEEXT_CHILDRENS_CAPTION;                   //  7 = children's captions
        break;
    case 9:
        subpictureAttributes.m_eCodeExt = DVDSUBPICCODEEXT_FORCED;                              //  9 = forced
        break;
    case 13:
        subpictureAttributes.m_eCodeExt = DVDSUBPICCODEEXT_DIRECTORS_COMMENTS;                  // 13 = director comments
        break;
    case 14:
        subpictureAttributes.m_eCodeExt = DVDSUBPICCODEEXT_LARGE_DIRECTORS_COMMENTS;            // 14 = large director comments
        break;
    case 15:
        subpictureAttributes.m_eCodeExt = DVDSUBPICCODEEXT_DIRECTORS_COMMENTS_FOR_CHILDREN;     // 15 = director comments for children
        break;
    default:
        subpictureAttributes.m_eCodeExt = DVDSUBPICCODEEXT_INVALID;
        break;
    }

    subpictureAttributes.m_wID                         = 0x20 + wSubpicStreamNo - 1;
}

uint64_t dvdparse::getSize() const
{
    uint64_t size = 0;

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= getTitleSetCount(); wTitleSetNo++)
    {
        size += m_lstDvdTitle[wTitleSetNo - 1]->getSize();
    }

    return size;
}

uint64_t dvdparse::getPlayTime() const
{
    uint64_t qwPlayTime = 0;

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= getTitleSetCount(); wTitleSetNo++)
    {
        qwPlayTime += m_lstDvdTitle[wTitleSetNo - 1]->getPlayTime();
    }

    return qwPlayTime;
}

uint64_t dvdparse::getVirtSize() const
{
    uint64_t size = 0;

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= getDvdPttVmgCount(); wTitleSetNo++)
    {
        size += getDvdPttVmg(wTitleSetNo)->getSize();
    }

    return size;
}

uint64_t dvdparse::getVirtPlayTime() const
{
    uint64_t qwPlayTime = 0;

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= getDvdPttVmgCount(); wTitleSetNo++)
    {
        qwPlayTime += getDvdPttVmg(wTitleSetNo)->getPlayTime();
    }

    return qwPlayTime;
}

const uint8_t* dvdparse::readIFO(const string & strFilePath, time_t & ftime)
{
    string strError;
    void *fpi = NULL;
    DVDFILESTAT filestat;
    uint8_t* pData = NULL;

    m_strErrorString.clear();

    fpi = m_pOpenFile(strFilePath.c_str(), "rb", m_strProxy.c_str());
    if (fpi == NULL)
    {
        strError = "Error opening file: " + strFilePath + "\n";
        strError += strerror(errno);
        setError(strError, DVDERRORCODE_FILEOPEN);
        return NULL;
    }

    memset(&filestat, 0, sizeof(filestat));

    if (m_pFstatFile(fpi, &filestat) == -1)
    {
        strError = "Cannot stat file: " + strFilePath + "\n";
        strError += strerror(errno);
        setError(strError, DVDERRORCODE_FILESTAT);
    }
    else if (filestat.m_qwFileSize)
    {
        pData = new uint8_t[filestat.m_qwFileSize];
        if (pData != NULL)
        {
            memset(pData, 0, filestat.m_qwFileSize);
            if (m_pReadFile(pData, 1, filestat.m_qwFileSize, fpi) != (size_t)filestat.m_qwFileSize)
            {
                strError = "Error reading file: " + strFilePath + "\n";
                strError += strerror(errno);
                setError(strError, DVDERRORCODE_FILEREAD);
                delete [] pData;
                pData = NULL;
            }
        }

        ftime = filestat.m_FileTime;
    }
    else
    {
        strError = "Error opening file: " + strFilePath + "\nCould not get file size.";
        setError(strError, DVDERRORCODE_FILEOPEN);
        return NULL;
    }

    m_pCloseFile(fpi);

    return pData;
}

void dvdparse::setError(const std::string & strErrorString, DVDERRORCODE eErrorCode)
{
    m_strErrorString    = strErrorString;
    m_eErrorCode        = eErrorCode;
}

bool dvdparse::isLoaded() const
{
    return m_bIsLoaded;
}

string dvdparse::getCode() const
{
    std::string strCode;

    strCode = "DVD";
    strCode += toHexString(getDvdPttVmgCount());
    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= getDvdPttVmgCount(); wTitleSetNo++)
    {
        const dvdpttvmg *pDvdPttVmg = getDvdPttVmg(wTitleSetNo);

        strCode += toHexString(pDvdPttVmg->getPttVtsCount());
        for (uint16_t wPttNo = 1; wPttNo <= pDvdPttVmg->getPttVtsCount(); wPttNo++)
        {
            const dvdpttvts * pDvdPttVts = pDvdPttVmg->getDvdPttVts(wPttNo);
            strCode += ::toHexString(pDvdPttVts->getPlayTime());
        }
    }

    return strCode;
}

string dvdparse::getHash() const
{
    if (m_strHash.size())
    {
        return m_strHash; // Use stored
    }

    std::string strMD5;
    std::string strCode = getCode();

    MD5_CTX state;
    MD5_type digest[16];

    ::MD5_Init(&state);
    ::MD5_Update(&state, (const void *)strCode.c_str(), strCode.length());
    ::MD5_Final(digest, &state);

    for (size_t i = 0; i < sizeof(digest); i++)
    {
        strMD5 += ::toHexString(digest[i]);
    }

    return strMD5;
}

void dvdparse::setHash(const std::string & strHash)
{
    m_strHash = strHash;
}

const std::string & dvdparse::getAlbumArtist() const
{
    return m_strAlbumArtist;
}

void dvdparse::setAlbumArtist(const std::string & strAlbumArtist)
{
    m_strAlbumArtist = strAlbumArtist;
}

const std::string & dvdparse::getAlbum() const
{
    return m_strAlbum;
}

void dvdparse::setAlbum(const std::string & strAlbum)
{
    m_strAlbum = strAlbum;
}

const std::string & dvdparse::getOriginalAlbum() const
{
    return m_strOriginalAlbum;
}

void dvdparse::setOriginalAlbum(const std::string & strOriginalAlbum)
{
    m_strOriginalAlbum = strOriginalAlbum;
}

const std::string & dvdparse::getGenre() const
{
    return m_strGenre;
}

void dvdparse::setGenre(const std::string & strGenre)
{
    m_strGenre = strGenre;
}

const std::string & dvdparse::getCast() const
{
    return m_strCast;
}

void dvdparse::setCast(const std::string & strCast)
{
    m_strCast = strCast;
}

const std::string & dvdparse::getCrew() const
{
    return m_strCrew;
}

void dvdparse::setCrew(const std::string & strCrew)
{
    m_strCrew = strCrew;
}

const std::string & dvdparse::getDirector() const
{
    return m_strDirector;
}

void dvdparse::setDirector(const std::string & strDirector)
{
    m_strDirector = strDirector;
}

const std::string & dvdparse::getScreenplay() const
{
    return m_strScreenplay;
}

void dvdparse::setScreenplay(const std::string & strScreenplay)
{
    m_strScreenplay = strScreenplay;
}

const std::string & dvdparse::getProducer() const
{
    return m_strProducer;
}

void dvdparse::setProducer(const std::string & strProducer)
{
    m_strProducer = strProducer;
}

const std::string & dvdparse::getEditing() const
{
    return m_strEditing;
}

void dvdparse::setEditing(const std::string & strEditing)
{
    m_strEditing = strEditing;
}

const std::string & dvdparse::getCinematography() const
{
    return m_strCinematography;
}

void dvdparse::setCinematography(const std::string & strCinematography)
{
    m_strCinematography = strCinematography;
}

const std::string & dvdparse::getCountry() const
{
    return m_strCountry;
}

void dvdparse::setCountry(const std::string & strCountry)
{
    m_strCountry = strCountry;
}

const std::string & dvdparse::getOriginalLanguage() const
{
    return m_strOriginalLanguage;
}

void dvdparse::setOriginalLanguage(const std::string & strOriginalLanguage)
{
    m_strOriginalLanguage = strOriginalLanguage;
}

const std::string & dvdparse::getReleaseDate() const
{
    return m_strReleaseDate;
}

void dvdparse::setReleaseDate(const std::string & strReleaseDate)
{
    m_strReleaseDate = strReleaseDate;
}

const std::string & dvdparse::getSpecialFeatures() const
{
    return m_strSpecialFeatures;
}

void dvdparse::setSpecialFeatures(const std::string & strFeatures)
{
    m_strSpecialFeatures = strFeatures;
}

const std::string & dvdparse::getEAN_UPC() const
{
    return m_strEAN_UPC;
}

void dvdparse::setEAN_UPC(const std::string & strEAN_UPC)
{
    m_strEAN_UPC = strEAN_UPC;
}

const std::string & dvdparse::getStoryline() const
{
    return m_strStoryline;
}

void dvdparse::setStoryline(const std::string & strStoryline)
{
    m_strStoryline = strStoryline;
}

const std::string & dvdparse::getRemarks() const
{
    return m_strRemarks;
}

void dvdparse::setRemarks(const std::string & strRemarks)
{
    m_strRemarks = strRemarks;
}

const std::string & dvdparse::getSubmitter() const
{
    return m_strSubmitter;
}

int dvdparse::getRevision() const
{
    return m_nRevision;
}

void dvdparse::setRevision(int nRevision)
{
    m_nRevision = nRevision;
}

void dvdparse::setSubmitter(const std::string & strSubmitter)
{
    m_strSubmitter = strSubmitter;
}

const std::string & dvdparse::getSubmitterIP() const
{
    return m_strSubmitterIP;
}

void dvdparse::setSubmitterIP(const std::string & strSubmitterIP)
{
    m_strSubmitterIP = strSubmitterIP;
}

const std::string & dvdparse::getClient() const
{
    return m_strClient;
}

void dvdparse::setClient(const std::string & strClient)
{
    m_strClient = strClient;
}

const std::string & dvdparse::getDateCreated() const
{
    return m_strDateCreated;
}

void dvdparse::setDateCreated(const std::string & strDateCreated)
{
    m_strDateCreated = strDateCreated;
}

void dvdparse::setDateLastChanged(const std::string & strDateLastChanged)
{
    m_strDateLastChanged = strDateLastChanged;
}

const std::string & dvdparse::getDateLastChanged() const
{
    return m_strDateLastChanged;
}

const std::string & dvdparse::getKeywords() const
{
    return m_strKeywords;
}

void dvdparse::setKeywords(const std::string & strKeywords)
{
    m_strKeywords = strKeywords;
}

int dvdparse::getProtocolVersion() const
{
    return m_nProtocolVersion;
}

void dvdparse::setProtocolVersion(int nProtocolVersion)
{
    m_nProtocolVersion = nProtocolVersion;
}

const std::string & dvdparse::getLibraryVersion() const
{
    return m_strLibraryVersion;
}

void dvdparse::setLibraryVersion(const std::string & strLibraryVersion)
{
    m_strLibraryVersion = strLibraryVersion;
}

const std::string & dvdparse::getLibraryName() const
{
    return m_strLibraryName;
}

void dvdparse::setLibraryName(const std::string & strLibraryName)
{
    m_strLibraryName = strLibraryName;
}

const std::string & dvdparse::getClientName() const
{
    return m_strClientName;
}

void dvdparse::setClientName(const std::string & strClientName)
{
    m_strClientName = strClientName;
}

void dvdparse::update(const dvdparse * dvdParse)
{
    // Do not update: m_strHash
    m_strAlbumArtist        = dvdParse->m_strAlbumArtist;
    m_strAlbum              = dvdParse->m_strAlbum;
    m_strOriginalAlbum      = dvdParse->m_strOriginalAlbum;
    m_strGenre              = dvdParse->m_strGenre;
    m_strCast               = dvdParse->m_strCast;
    m_strCrew               = dvdParse->m_strCrew;
    m_strDirector           = dvdParse->m_strDirector;
    m_strScreenplay         = dvdParse->m_strScreenplay;
    m_strProducer           = dvdParse->m_strProducer;
    m_strEditing            = dvdParse->m_strEditing;
    m_strCinematography     = dvdParse->m_strCinematography;
    m_strCountry            = dvdParse->m_strCountry;
    m_strOriginalLanguage   = dvdParse->m_strOriginalLanguage;
    m_strReleaseDate        = dvdParse->m_strReleaseDate;
    m_strSpecialFeatures    = dvdParse->m_strSpecialFeatures;
    m_strEAN_UPC            = dvdParse->m_strEAN_UPC;
    m_strStoryline          = dvdParse->m_strStoryline;
    m_strRemarks            = dvdParse->m_strRemarks;
    m_nRevision             = dvdParse->m_nRevision;
    m_strSubmitter          = dvdParse->m_strSubmitter;
    m_strSubmitterIP        = dvdParse->m_strSubmitterIP;
    m_strClient             = dvdParse->m_strClient;
    m_strDateCreated        = dvdParse->m_strDateCreated;
    m_strDateLastChanged    = dvdParse->m_strDateLastChanged;
    m_strKeywords           = dvdParse->m_strKeywords;
    m_nProtocolVersion      = dvdParse->m_nProtocolVersion;
    m_strLibraryVersion     = dvdParse->m_strLibraryVersion;
    m_strLibraryName        = dvdParse->m_strLibraryName;
    m_strClientName         = dvdParse->m_strClientName;

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= getDvdPttVmgCount(); wTitleSetNo++)
    {
        dvdpttvmg *pDvdPttVmg = (dvdpttvmg *)getDvdPttVmg(wTitleSetNo);
        const dvdpttvmg *pDvdPttVmg2 = dvdParse->getDvdPttVmg(wTitleSetNo);

        if (pDvdPttVmg == NULL || pDvdPttVmg2 == NULL)
        {
            break;
        }

        pDvdPttVmg->m_strTitle = pDvdPttVmg2->m_strTitle;

        for (uint16_t wPttNo = 1; wPttNo <= pDvdPttVmg->getPttVtsCount(); wPttNo++)
        {
            dvdpttvts * pDvdPttVts = (dvdpttvts *)pDvdPttVmg->getDvdPttVts(wPttNo);
            const dvdpttvts * pDvdPttVts2 = pDvdPttVmg2->getDvdPttVts(wPttNo);

            if (pDvdPttVts == NULL || pDvdPttVts2 == NULL)
            {
                break;
            }

            pDvdPttVts->m_strArtist = pDvdPttVts2->m_strArtist;
            pDvdPttVts->m_strTitle = pDvdPttVts2->m_strTitle;
        }
    }
}

void dvdparse::setProxy(const std::string & strProxy)
{
    m_strProxy = strProxy;
}

string dvdparse::getProxy() const
{
    return m_strProxy;
}

