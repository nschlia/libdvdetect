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

/*! \file xmldocbuilder.cpp
 *
 *  \brief
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>

#include "xmldocbuilder.h"

#include "localutils.h"

xmldocbuilder::xmldocbuilder(const std::string & strClientName)
    : xmldoc(strClientName)
    , m_bCondensedMode(false)
{
}

xmldocbuilder::~xmldocbuilder()
{
}

void xmldocbuilder::addRootAttributes(TiXmlElement* pRoot, XMLMODE eXmlMode) const
{
    safeSetAttribute(pRoot, "XmlMode", (int)(eXmlMode), true);
    safeSetAttribute(pRoot, "ProtocolVersion", LIBDVDETECT_PROTOCOL_VERSION, true);
    safeSetAttribute(pRoot, "LibraryVersion", LIBDVDETECT_VERSION, true);
    safeSetAttribute(pRoot, "LibraryName", LIBDVDETECT_LIBRARYNAME, true);
    safeSetAttribute(pRoot, "ClientName", m_strClientName, true);
}

void xmldocbuilder::addComment(TiXmlElement* pOwner, const char * pszComment) const
{
    if (!getCondensedMode())
    {
        TiXmlComment* pComment = new TiXmlComment();
        pComment->SetValue(pszComment);
        pOwner->LinkEndChild(pComment);
    }
}

void xmldocbuilder::addComment(TiXmlElement* pOwner, XMLMODE eXmlMode) const
{
    addComment(pOwner, LIBDVDETECT_XML_COMMENT);
    addComment(pOwner, LIBDVDETECT_COPYRIGHT);

    switch (eXmlMode)
    {
    case XMLMODE_QUERY:
        addComment(pOwner, " XMLMODE_QUERY ");
        break;
    case XMLMODE_QUERY_RESPONSE:
        addComment(pOwner, " XMLMODE_QUERY_RESPONSE ");
        break;
    case XMLMODE_SUBMIT:
        addComment(pOwner, " XMLMODE_SUBMIT ");
        break;
    case XMLMODE_SUBMIT_RESPONSE:
        addComment(pOwner, " XMLMODE_SUBMIT_RESPONSE");
        break;
    case XMLMODE_SEARCH:
        addComment(pOwner, " XMLMODE_SEARCH ");
        break;
    case XMLMODE_SEARCH_RESPONSE:
        addComment(pOwner, " XMLMODE_SEARCH_RESPONSE ");
        break;
    case XMLMODE_EXPORT:
        addComment(pOwner, " XMLMODE_EXPORT ");
        addComment(pOwner, " You may change anything you wish in here, BUT DO NOT MODIFY Hash ATTRIBUTE! ");
        break;
    case XMLMODE_INVALID:
        break;
    }
}

void xmldocbuilder::addDvdAttributes(TiXmlElement* pDVD, const dvdparse *pDvdParse, XMLMODE eXmlMode) const
{
    safeSetAttribute(pDVD, "Hash", pDvdParse->getHash(), true);

    if (eXmlMode != XMLMODE_QUERY)
    {
        safeSetAttribute(pDVD, "Revision", pDvdParse->getRevision());

        if (!getCondensedMode())
        {
            setValue(pDVD, "Album", pDvdParse->getAlbum());
            setValue(pDVD, "OriginalAlbum", pDvdParse->getOriginalAlbum());
            setValue(pDVD, "AlbumArtist", pDvdParse->getAlbumArtist());
            setValue(pDVD, "Genre", pDvdParse->getGenre());
            setValue(pDVD, "Cast", pDvdParse->getCast());
            setValue(pDVD, "Crew", pDvdParse->getCrew());
            setValue(pDVD, "Director", pDvdParse->getDirector());
            setValue(pDVD, "Screenplay", pDvdParse->getScreenplay());
            setValue(pDVD, "Producer", pDvdParse->getProducer());
            setValue(pDVD, "Editing", pDvdParse->getEditing());
            setValue(pDVD, "Cinematography", pDvdParse->getCinematography());
            setValue(pDVD, "Country", pDvdParse->getCountry());
            setValue(pDVD, "OriginalLanguage", pDvdParse->getOriginalLanguage());
            setValue(pDVD, "ReleaseDate", pDvdParse->getReleaseDate());
            setValue(pDVD, "SpecialFeatures", pDvdParse->getSpecialFeatures());
            setValue(pDVD, "EAN_UPC", pDvdParse->getEAN_UPC());
            setValue(pDVD, "Storyline", pDvdParse->getStoryline());
            setValue(pDVD, "Remarks", pDvdParse->getRemarks());
            setValue(pDVD, "Keywords", pDvdParse->getKeywords());
        }
        else
        {
            safeSetValue(pDVD, "Album", pDvdParse->getAlbum());
            safeSetValue(pDVD, "OriginalAlbum", pDvdParse->getOriginalAlbum());
            safeSetValue(pDVD, "AlbumArtist", pDvdParse->getAlbumArtist());
            safeSetValue(pDVD, "Genre", pDvdParse->getGenre());
            safeSetValue(pDVD, "Cast", pDvdParse->getCast());
            safeSetValue(pDVD, "Crew", pDvdParse->getCrew());
            safeSetValue(pDVD, "Director", pDvdParse->getDirector());
            safeSetValue(pDVD, "Screenplay", pDvdParse->getScreenplay());
            safeSetValue(pDVD, "Producer", pDvdParse->getProducer());
            safeSetValue(pDVD, "Editing", pDvdParse->getEditing());
            safeSetValue(pDVD, "Cinematography", pDvdParse->getCinematography());
            safeSetValue(pDVD, "Country", pDvdParse->getCountry());
            safeSetValue(pDVD, "OriginalLanguage", pDvdParse->getOriginalLanguage());
            safeSetValue(pDVD, "ReleaseDate", pDvdParse->getReleaseDate());
            safeSetValue(pDVD, "SpecialFeatures", pDvdParse->getSpecialFeatures());
            safeSetValue(pDVD, "EAN_UPC", pDvdParse->getEAN_UPC());
            safeSetValue(pDVD, "Storyline", pDvdParse->getStoryline());
            safeSetValue(pDVD, "Remarks", pDvdParse->getRemarks());
            safeSetValue(pDVD, "Keywords", pDvdParse->getKeywords());
        }
    }

    if (!getCondensedMode())
    {
        //setValue(pDVD, "Submitter", (eXmlMode != XMLMODE_QUERY) ? DEFSUBMITTER : pDvdParse->getSubmitter());
        setValue(pDVD, "Submitter", pDvdParse->getSubmitter());
        setValue(pDVD, "Client", m_strClientName);
    }
    else
    {
        //safeSetValue(pDVD, "Submitter", (eXmlMode != XMLMODE_QUERY) ? DEFSUBMITTER : pDvdParse->getSubmitter());
        safeSetValue(pDVD, "Submitter", pDvdParse->getSubmitter());
        safeSetValue(pDVD, "Client", m_strClientName);
    }
}

void xmldocbuilder::addDvdAttributesEx(TiXmlElement* pDVD, const dvdparse *pDvdParse) const
{
    const DVDVMGM *pDVDVMGM = pDvdParse->getDVDVMGM();

    for (int i = 0; i < 8; i++)
    {
        safeSetAttribute(pDVD, ("RegionProhibited" + ::toString(i + 1)).c_str(), pDVDVMGM->m_bRegionProhibited[i], true);
    }

    if (pDVDVMGM->m_wVersionNumberMajor)
    {
        safeSetAttribute(pDVD, "VersionNumberMajor", pDVDVMGM->m_wVersionNumberMajor, true);
        safeSetAttribute(pDVD, "VersionNumberMinor", pDVDVMGM->m_wVersionNumberMinor, true);
    }

    safeSetAttribute(pDVD, "NumberOfVolumes", pDVDVMGM->m_wNumberOfVolumes);
    safeSetAttribute(pDVD, "VolumeNumber", pDVDVMGM->m_wVolumeNumber);
    safeSetAttribute(pDVD, "SideID", pDVDVMGM->m_wSideID);
    safeSetAttribute(pDVD, "ProviderID", pDVDVMGM->m_szProviderID);
}

void xmldocbuilder::addVideoStream(TiXmlElement* pDVD, LPCDVDVIDEOSTREAM pDvdVideoStream, const string & strType /*= ""*/) const
{
    TiXmlElement * pVideoStream = new TiXmlElement("DVDVIDEOSTREAM");

    // No number: there is only one stream
    safeSetAttribute(pVideoStream, "Type", strType);

    safeSetAttribute(pVideoStream, "ID", pDvdVideoStream->m_wID);
    safeSetAttribute(pVideoStream, "VideoCodingMode", pDvdVideoStream->m_eCodingMode);
    safeSetAttribute(pVideoStream, "VideoStandard", pDvdVideoStream->m_eStandard);
    safeSetAttribute(pVideoStream, "VideoAspect", pDvdVideoStream->m_eAspect);
    safeSetAttribute(pVideoStream, "AutomaticPanScanDisallowed", pDvdVideoStream->m_bAutomaticPanScanDisallowed, true);
    safeSetAttribute(pVideoStream, "CCForLine21Field1InGOP", pDvdVideoStream->m_bCCForLine21Field1InGOP, true);
    safeSetAttribute(pVideoStream, "CCForLine21Field2InGOP", pDvdVideoStream->m_bCCForLine21Field2InGOP, true);
    safeSetAttribute(pVideoStream, "CBR", pDvdVideoStream->m_bCBR, true);
    safeSetAttribute(pVideoStream, "Resolution", &pDvdVideoStream->m_Resolution);
    safeSetAttribute(pVideoStream, "LetterBoxed", pDvdVideoStream->m_bLetterBoxed, true);
    safeSetAttribute(pVideoStream, "SourceFilm", pDvdVideoStream->m_bSourceFilm, true);

    pDVD->LinkEndChild(pVideoStream);
}

void xmldocbuilder::addVideoStreamVMG(TiXmlElement* pDVD, LPCDVDVMGM pDVDVMGM) const
{
    addVideoStream(pDVD, &pDVDVMGM->m_VideoStream, "VMG");
}

void xmldocbuilder::addVideoStreamVTSM(TiXmlElement* pDVD, LPCDVDVTS pDVDVTS) const
{
    addVideoStream(pDVD, &pDVDVTS->m_VideoStreamVTSM, "VTSM");
}

void xmldocbuilder::addVideoStreamVTS(TiXmlElement* pDVD, LPCDVDVTS pDVDVTS) const
{
    addVideoStream(pDVD, &pDVDVTS->m_VideoStreamVTS, "VTS");
}

void xmldocbuilder::addAudioStream(TiXmlElement* pDVD, LPCDVDAUDIOSTREAM pDvdAudioStream, const std::string &strType, uint16_t wAudioStreamNo /*= 0*/) const
{
    TiXmlElement * pAudioStream = new TiXmlElement("DVDAUDIOSTREAM");

    safeSetAttribute(pAudioStream, "Number", wAudioStreamNo);
    safeSetAttribute(pAudioStream, "Type", strType, true);

    safeSetAttribute(pAudioStream, "ID", pDvdAudioStream->m_wID, true);
    safeSetAttribute(pAudioStream, "CodingMode", pDvdAudioStream->m_eCodingMode);
    safeSetAttribute(pAudioStream, "MultichannelExtPresent", pDvdAudioStream->m_bMultichannelExtPresent, true);
    safeSetAttribute(pAudioStream, "Quantisation", pDvdAudioStream->m_eQuantisation);
    safeSetAttribute(pAudioStream, "SampleRate", pDvdAudioStream->m_dwSampleRate, true);
    safeSetAttribute(pAudioStream, "Channels", pDvdAudioStream->m_wChannels, true);

    pDVD->LinkEndChild(pAudioStream);
}

void xmldocbuilder::addAudioStreamEx(TiXmlElement* pTitle, LPCDVDAUDIOSTREAMEX pDvdAudioStreamEx, uint16_t wAudioStreamNo) const
{
    TiXmlElement * pAudioStreamEx = new TiXmlElement("DVDAUDIOSTREAMEX");

    safeSetAttribute(pAudioStreamEx, "Number", wAudioStreamNo, true);
    safeSetAttribute(pAudioStreamEx, "Type", "VTS", true);    // Fixed

    safeSetAttribute(pAudioStreamEx, "LanguageCodePresent", pDvdAudioStreamEx->m_bLanguageCodePresent, true);
    if (pDvdAudioStreamEx->m_bLanguageCodePresent)
    {
        safeSetAttribute(pAudioStreamEx, "LanguageCode", languageToString(pDvdAudioStreamEx->m_szLanguageCode), true);
    }

    safeSetAttribute(pAudioStreamEx, "CodeExtPresent", pDvdAudioStreamEx->m_bCodeExtPresent, true);
    if (pDvdAudioStreamEx->m_bCodeExtPresent)
    {
        safeSetAttribute(pAudioStreamEx, "CodeExt", pDvdAudioStreamEx->m_eCodeExt, true);
    }

    safeSetAttribute(pAudioStreamEx, "ChannelAssignment", pDvdAudioStreamEx->m_eChannelAssignment);
    safeSetAttribute(pAudioStreamEx, "MCIntroPresent", pDvdAudioStreamEx->m_bMCIntroPresent, true);
    safeSetAttribute(pAudioStreamEx, "ApplicationMode", pDvdAudioStreamEx->m_eApplicationMode);
    safeSetAttribute(pAudioStreamEx, "KaraokeVersion", pDvdAudioStreamEx->m_wKaraokeVersion, true);
    safeSetAttribute(pAudioStreamEx, "KaraokeMode", pDvdAudioStreamEx->m_eKaraokeMode);
    safeSetAttribute(pAudioStreamEx, "SuitableForDolbySurroundDecoding", pDvdAudioStreamEx->m_bSuitableForDolbySurroundDecoding, true);

    pTitle->LinkEndChild(pAudioStreamEx);
}

void xmldocbuilder::addAudioStreamVMG(TiXmlElement* pDVD, LPCDVDVMGM pDVDVMGM) const
{
    if (pDVDVMGM->m_wAudioStreamCount)
    {
        addAudioStream(pDVD, &pDVDVMGM->m_AudioStream, "VMG");
    }
}

void xmldocbuilder::addAudioStreamVTSM(TiXmlElement* pTitle, LPCDVDVTS pDVDVTS) const
{
    if (pDVDVTS->m_wAudioStreamCountVTSM)
    {
        addAudioStream(pTitle, &pDVDVTS->m_AudioStreamVTSM, "VTSM");
    }
}

void xmldocbuilder::addAudioStreamVTS(TiXmlElement* pTitle, LPCDVDVTS pDVDVTS) const
{
    if (pDVDVTS->m_wAudioStreamCountVTS)
    {
        for (uint16_t wAudioStreamNo = 1; wAudioStreamNo <= pDVDVTS->m_wAudioStreamCountVTS; wAudioStreamNo++)
        {
            addAudioStream(pTitle, &pDVDVTS->m_AudioStreamVTS[wAudioStreamNo - 1], "VTS", wAudioStreamNo);
        }

        for (uint16_t wAudioStreamNo = 1; wAudioStreamNo <= pDVDVTS->m_wAudioStreamCountVTS; wAudioStreamNo++)
        {
            addAudioStreamEx(pTitle, &pDVDVTS->m_AudioStreamExVTS[wAudioStreamNo - 1], wAudioStreamNo);
        }
    }
}

void xmldocbuilder::addSubpicStream(TiXmlElement* pDVD, LPCDVDSUBPICSTREAM pDvdSubpicStream, const std::string & strType, uint16_t wSubpicStreamNo /*= 0*/) const
{
    TiXmlElement * pSubpicStream = new TiXmlElement("DVDSUBPICSTREAM");

    safeSetAttribute(pSubpicStream, "Number", wSubpicStreamNo);
    safeSetAttribute(pSubpicStream, "Type", strType);

    safeSetAttribute(pSubpicStream, "ID", pDvdSubpicStream->m_wID);
    safeSetAttribute(pSubpicStream, "CodingMode", pDvdSubpicStream->m_wCodingMode, true);
    safeSetAttribute(pSubpicStream, "LanguageCodePresent", pDvdSubpicStream->m_bLanguageCodePresent, true);
    if (pDvdSubpicStream->m_bLanguageCodePresent)
    {
        safeSetAttribute(pSubpicStream, "LanguageCode", languageToString(pDvdSubpicStream->m_szLanguageCode));
    }
    safeSetAttribute(pSubpicStream, "CodeExtPresent", pDvdSubpicStream->m_bCodeExtPresent, true);
    if (pDvdSubpicStream->m_bCodeExtPresent)
    {
        safeSetAttribute(pSubpicStream, "CodeExt", pDvdSubpicStream->m_eCodeExt);
    }

    pDVD->LinkEndChild(pSubpicStream);
}

void xmldocbuilder::addSubpicStreamVMG(TiXmlElement* pDVD, LPCDVDVMGM pDVDVMGM) const
{
    if (pDVDVMGM->m_wSubpicStreamCount)
    {
        addSubpicStream(pDVD, &pDVDVMGM->m_SubpicStream, "VMG");
    }
}

void xmldocbuilder::addSubpicStreamVTSM(TiXmlElement* pTitle, LPCDVDVTS pDVDVTS) const
{
    if (pDVDVTS->m_wSubpicStreamCountVTSM)
    {
        addSubpicStream(pTitle, &pDVDVTS->m_SubpicStreamVTSM, "VTSM");
    }
}

void xmldocbuilder::addSubpicStreamVTS(TiXmlElement* pTitle, LPCDVDVTS pDVDVTS) const
{
    if (pDVDVTS->m_wSubpicStreamCountVTS)
    {
        for (uint16_t wSubpicStreamNo = 1; wSubpicStreamNo <= pDVDVTS->m_wSubpicStreamCountVTS; wSubpicStreamNo++)
        {
            addSubpicStream(pTitle, &pDVDVTS->m_SubpicStreamVTS[wSubpicStreamNo - 1], "VTS", wSubpicStreamNo);
        }
    }
}

void xmldocbuilder::addFileSet(TiXmlElement* pDVD, const dvdparse *pDvdParse) const
{
    if (pDvdParse->getFileCount())
    {
        addComment(pDVD, "Video Mananger (VMG) File Set");
    }

    buildFileSetTpl(pDVD, pDvdParse);
}

void xmldocbuilder::addFileSet(TiXmlElement* pDVD, const dvdtitle *pDvdTitle) const
{
    if (pDvdTitle->getFileCount())
    {
        addComment(pDVD, "Title (VTS) File Set");
    }

    buildFileSetTpl(pDVD, pDvdTitle);
}

void xmldocbuilder::buildPhysicalStructure(TiXmlElement *pOwner, const dvdparse *pDvdParse, XMLMODE /*eXmlMode*/) const
{
    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= pDvdParse->getTitleSetCount(); wTitleSetNo++)
    {
        const dvdtitle *pDvdTitle = pDvdParse->getDvdTitle(wTitleSetNo);
        LPCDVDVTS pDVDVTS = pDvdTitle->getDVDVTS();

        addComment(pOwner, "Title Set");

        TiXmlElement * pTitle = new TiXmlElement("DVDVTS");

        safeSetAttribute(pTitle, "TitleSetNo", pDVDVTS->m_wTitleSetNo);
        safeSetAttribute(pTitle, "VersionNumberMajor", pDVDVTS->m_wVersionNumberMajor, true);
        safeSetAttribute(pTitle, "VersionNumberMinor", pDVDVTS->m_wVersionNumberMinor, true);

        if (pDvdTitle->getPgcCount())
        {
            addComment(pTitle, "Program Chain (PGC)");
        }

        for (uint16_t wProgramChainNo = 1; wProgramChainNo <= pDvdTitle->getPgcCount(); wProgramChainNo++)
        {
            const dvdpgc *pDvdPgc = pDvdTitle->getDvdPgc(wProgramChainNo);
            LPCDVDPGC pDVDPGC = pDvdPgc->getDVDPGC();

            TiXmlElement* pProgramChain = new TiXmlElement("DVDPGC");

            safeSetAttribute(pProgramChain, "Number", pDVDPGC->m_wProgramChainNo);
            safeSetAttribute(pProgramChain, "EntryPGC", pDVDPGC->m_bEntryPGC, true);

            if (pDvdPgc->getProgramCount())
            {
                addComment(pProgramChain, "Programs");
            }

            for (uint16_t wProgramNo = 1; wProgramNo <= pDvdPgc->getProgramCount(); wProgramNo++)
            {
                const dvdprogram * pDvdProgram = pDvdPgc->getDvdProgram(wProgramNo);
                LPCDVDPROGRAM pDVDPROGRAM = pDvdProgram->getDVDPROGRAM();

                TiXmlElement* pProgram = new TiXmlElement("DVDPROGRAM");

                pProgram->SetAttribute("Number", pDVDPROGRAM->m_wProgramNo);

                if (pDvdProgram->getCellCount())
                {
                    addComment(pProgram, "Cells");
                }

                for (uint16_t wCellNo = 1; wCellNo <= pDvdProgram->getCellCount(); wCellNo++)
                {
                    const dvdcell * pDvdCell = pDvdProgram->getDvdCell(wCellNo);
                    LPCDVDCELL pDVDCELL = pDvdCell->getDVDCELL();

                    TiXmlElement* pCell = new TiXmlElement("DVDCELL");

                    safeSetAttribute(pCell, "Number", pDVDCELL->m_wCellNo);

                    safeSetAttribute(pCell, "CellType", pDVDCELL->m_eCellType);
                    safeSetAttribute(pCell, "BlockType", pDVDCELL->m_eBlockType);
                    safeSetAttribute(pCell, "SeamlessMultiplex", pDVDCELL->m_bSeamlessMultiplex, true);
                    safeSetAttribute(pCell, "Interleaved", pDVDCELL->m_bInterleaved, true);
                    safeSetAttribute(pCell, "SCRdiscontinuity", pDVDCELL->m_bSCRdiscontinuity, true);
                    safeSetAttribute(pCell, "SeamlessAngleLinkedInDSI", pDVDCELL->m_bSeamlessAngleLinkedInDSI, true);
                    safeSetAttribute(pCell, "VOBStillMode", pDVDCELL->m_bVOBStillMode, true);
                    safeSetAttribute(pCell, "StopsTrickPlay", pDVDCELL->m_bStopsTrickPlay, true);
                    safeSetAttribute(pCell, "CellStillTime", pDVDCELL->m_wCellStillTime, true);
                    safeSetAttribute(pCell, "CellCommand", pDVDCELL->m_wCellCommand, true);
                    safeSetAttribute(pCell, "PlayTime", pDVDCELL->m_qwPlayTime);
                    safeSetAttribute(pCell, "FrameRate", pDVDCELL->m_wFrameRate);
                    safeSetAttribute(pCell, "FirstVOBUStartSector", pDVDCELL->m_dwFirstVOBUStartSector, true);
                    safeSetAttribute(pCell, "FirstILVUEndSector", pDVDCELL->m_dwFirstILVUEndSector, true);
                    safeSetAttribute(pCell, "LastVOBUStartSector", pDVDCELL->m_dwLastVOBUStartSector, true);
                    safeSetAttribute(pCell, "LastVOBUEndSector", pDVDCELL->m_dwLastVOBUEndSector, true);
                    safeSetAttribute(pCell, "VOBidn", pDVDCELL->m_wVOBidn, true);
                    safeSetAttribute(pCell, "CELLidn", pDVDCELL->m_wCELLidn, true);
                    safeSetAttribute(pCell, "NumberOfVOBIds", pDVDCELL->m_wNumberOfVOBIds, true);

                    if (pDvdCell->getUnitCount())
                    {
                        addComment(pCell, "Units");
                    }

                    for (uint16_t wUnitNo = 1; wUnitNo <= pDvdCell->getUnitCount(); wUnitNo++)
                    {
                        const dvdunit * pDvdUnit = pDvdCell->getDvdUnit(wUnitNo);
                        LPCDVDUNIT pDVDUNIT = pDvdUnit->getDVDUNIT();

                        TiXmlElement* pUnit = new TiXmlElement("DVDUNIT");

                        safeSetAttribute(pUnit, "Number", pDVDUNIT->m_wUnitNo);

                        safeSetAttribute(pUnit, "StartSector", pDVDUNIT->m_dwStartSector, true);
                        safeSetAttribute(pUnit, "EndSector", pDVDUNIT->m_dwEndSector, true);

                        pCell->LinkEndChild(pUnit);
                    }

                    pProgram->LinkEndChild(pCell);
                }

                pProgramChain->LinkEndChild(pProgram);
            }

            pTitle->LinkEndChild(pProgramChain);
        }

        // Add streams
        addComment(pTitle, "VTSM/VTS Video Streams");
        addVideoStreamVTSM(pTitle, pDVDVTS);
        addVideoStreamVTS(pTitle, pDVDVTS);

        addComment(pTitle, "VTSM/VTS Audio Streams");
        addAudioStreamVTSM(pTitle, pDVDVTS);
        addAudioStreamVTS(pTitle, pDVDVTS);

        addComment(pTitle, "VTSM/VTS Subpicture Streams");
        addSubpicStreamVTSM(pTitle, pDVDVTS);
        addSubpicStreamVTS(pTitle, pDVDVTS);

        // Add files
        addFileSet(pTitle, pDvdTitle);

        pOwner->LinkEndChild(pTitle);
    }
}

void xmldocbuilder::buildVirtualStructure(TiXmlElement* pOwner, const dvdparse *pDvdParse, XMLMODE eXmlMode) const
{
    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= pDvdParse->getDvdPttVmgCount(); wTitleSetNo++)
    {
        const dvdpttvmg *pDvdPttVmg = pDvdParse->getDvdPttVmg(wTitleSetNo);
        LPCDVDPTTVMG pDVDPTTVMG = pDvdPttVmg->getDVDPTTVMG();

        addComment(pOwner, "Title Set");

        TiXmlElement * pTitle = new TiXmlElement("DVDPTTVMG");

        safeSetAttribute(pTitle, "TitleSetNo", wTitleSetNo);

        if (!getCondensedMode())
        {
            setValue(pTitle, "Title", pDvdPttVmg->getTitle());
        }
        else
        {
            safeSetValue(pTitle, "Title", pDvdPttVmg->getTitle());
        }

        if (eXmlMode == XMLMODE_SUBMIT || eXmlMode == XMLMODE_EXPORT)
        {
            safeSetAttribute(pTitle, "PlaybackType", pDVDPTTVMG->m_wPlaybackType, true);
            safeSetAttribute(pTitle, "NumberOfVideoAngles", pDVDPTTVMG->m_wNumberOfVideoAngles, true);
            safeSetAttribute(pTitle, "ParentalMgmMaskVMG", (int)pDVDPTTVMG->m_byParentalMgmMaskVMG);
            safeSetAttribute(pTitle, "ParentalMgmMaskVTS", (int)pDVDPTTVMG->m_byParentalMgmMaskVTS);
            safeSetAttribute(pTitle, "VideoTitleSetNo", pDVDPTTVMG->m_wVideoTitleSetNo);
            safeSetAttribute(pTitle, "TitleNo", pDVDPTTVMG->m_wTitleNo);
        }

        if (pDvdPttVmg->getPttVtsCount())
        {
            addComment(pTitle, "Chapters (A.K.A. Programs)");
        }

        for (uint16_t wPttNo = 1; wPttNo <= pDvdPttVmg->getPttVtsCount(); wPttNo++)
        {
            const dvdpttvts * pDvdPttVts = pDvdPttVmg->getDvdPttVts(wPttNo);
            LPCDVDPTTVTS pDVDPTTVTS = pDvdPttVts->getDVDPTTVTS();

            TiXmlElement* pChapter = new TiXmlElement("DVDPTTVTS");

            if (!getCondensedMode())
            {
                setValue(pChapter, "Artist", pDvdPttVts->getArtist());
                setValue(pChapter, "Title", pDvdPttVts->getTitle());
            }
            else
            {
                safeSetValue(pChapter, "Artist", pDvdPttVts->getArtist());
                safeSetValue(pChapter, "Title", pDvdPttVts->getTitle());
            }

            safeSetAttribute(pChapter, "Number", pDVDPTTVTS->m_wPttChapterNo);
            safeSetAttribute(pChapter, "TitleSetNo", pDVDPTTVTS->m_wTitleSetNo);
            safeSetAttribute(pChapter, "ProgramChainNo", pDVDPTTVTS->m_wProgramChainNo);
            safeSetAttribute(pChapter, "ProgramNo", pDVDPTTVTS->m_wProgramNo);
            safeSetAttribute(pChapter, "PttTitleSetNo", pDVDPTTVTS->m_wPttTitleSetNo);

            pTitle->LinkEndChild(pChapter);
        }

        pOwner->LinkEndChild(pTitle);
    }
}

int xmldocbuilder::buildXml(std::string & strXML, XMLMODE eXmlMode, const dvdparse * pDvdParse, const std::string & strSearch /*= ""*/)
{
    dvdparselst lstDvdParseIn;
    dvdparse *pDvdParseTmp = new dvdparse; // No leak: gets deleted by dvdparselst destructor

    *pDvdParseTmp = *pDvdParse;

    lstDvdParseIn.push_back(pDvdParseTmp);

    return buildXml(strXML, eXmlMode, &lstDvdParseIn, strSearch);
}

int xmldocbuilder::buildXml(std::string & strXML, XMLMODE eXmlMode, const dvdparselst * pDvdParseLst, const std::string & strSearch /*= ""*/)
{
    TiXmlDocument doc;
    int res = 0;

    strXML.empty();

    try
    {
        if (eXmlMode == XMLMODE_INVALID)
        {
            setError("Internal error: Invalid parameter", DVDERRORCODE_INVALID_PARAMETER);
            throw -1;
        }

        // Create document
        TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
        doc.LinkEndChild(decl);

        // Create header
        TiXmlElement* pRoot = new TiXmlElement("xml");
        doc.LinkEndChild(pRoot);

        // Set root attributes
        addRootAttributes(pRoot, eXmlMode);

        // Add comments
        addComment(pRoot, eXmlMode);

        // Create contents
        switch (eXmlMode)
        {
        case XMLMODE_QUERY:     // Send out query
        {
            dvdparselst::const_iterator itDvdParse;
            for (itDvdParse = pDvdParseLst->begin(); itDvdParse != pDvdParseLst->end(); ++itDvdParse)
            {
                TiXmlElement * pDVD = new TiXmlElement("DVD");

                // Add header information
                addDvdAttributes(pDVD, *itDvdParse, eXmlMode);

                pRoot->LinkEndChild(pDVD);
            }
            break;
        }
        case XMLMODE_EXPORT:    // Export and submit share the same format
        case XMLMODE_SUBMIT:
        {
            setCondensedMode(eXmlMode != XMLMODE_EXPORT);

            dvdparselst::const_iterator itDvdParse;
            for (itDvdParse = pDvdParseLst->begin(); itDvdParse != pDvdParseLst->end(); ++itDvdParse)
            {
                TiXmlElement * pDVD = new TiXmlElement("DVD");

                // Add (extended) header information
                addDvdAttributes(pDVD, *itDvdParse, eXmlMode);
                addDvdAttributesEx(pDVD, *itDvdParse);

                // Add physical view
                addComment(pDVD, "Physical structure");
                TiXmlElement * pPhysical = new TiXmlElement("physical");
                buildPhysicalStructure(pPhysical, *itDvdParse, eXmlMode);
                pDVD->LinkEndChild(pPhysical);

                // Add virtual view
                addComment(pDVD, "Virtual structure");
                TiXmlElement * pVirtual = new TiXmlElement("virtual");
                buildVirtualStructure(pVirtual, *itDvdParse, eXmlMode);
                pDVD->LinkEndChild(pVirtual);

                // VMG video stream
                addComment(pDVD, "VMG Video Stream");
                LPCDVDVMGM pDVDVMGM = &(*itDvdParse)->m_DVDVMGM;
                addVideoStreamVMG(pDVD, pDVDVMGM);

                // VMG audio streams
                addComment(pDVD, "VMG Audio Streams");
                addAudioStreamVMG(pDVD, pDVDVMGM);

                // VMG subpicture streams
                addComment(pDVD, "Subpicture Streams");
                addSubpicStreamVMG(pDVD, pDVDVMGM);

                // Add files
                addFileSet(pDVD, *itDvdParse);

                // Close this DVD
                pRoot->LinkEndChild(pDVD);
            }
            break;
        }
        case XMLMODE_SEARCH:        // Send search request
        {
            if (strSearch.length())
            {
                TiXmlElement * search = new TiXmlElement("Search");
                search->LinkEndChild(new TiXmlText(strSearch));
                pRoot->LinkEndChild(search);
            }
            break;
        }
        default:
            setError("Unknown XML mode " + ::toString(eXmlMode), DVDERRORCODE_XML_UNKNOWN_MODE);
            throw -1;
            break;
        }

        // Get document
        TiXmlPrinter printer;
        doc.Accept(&printer);

        if (doc.Error())
        {
            setError(doc.ErrorDesc(), DVDERRORCODE_XML_ERROR);
            throw -1;
        }

        strXML = printer.CStr();
    }
    catch (int _res)
    {
        res = _res;
    }
    return res;
}

bool xmldocbuilder::getCondensedMode() const
{
    return m_bCondensedMode;
}

void xmldocbuilder::setCondensedMode(bool bCondensedMode)
{
    m_bCondensedMode = bCondensedMode;
}

