/*
  dvdetect DVD detection, analysis & DVDETECT lookup library

  Copyright (C) 2013-2014 Norbert Schlia <nschlia@dvdetect.de>

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

/*! \file xmldocparser.cpp
 *
 *  \brief
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>

#include "xmldocparser.h"

#include "localutils.h"

xmldocparser::xmldocparser(const std::string &strClientName)
    : xmldoc(strClientName)
{
}

xmldocparser::~xmldocparser()
{
}

void xmldocparser::getRootAttributes(TiXmlElement *pRoot, dvdparse *pDvdParse, int nProtocolVersion)
{
    pDvdParse->setProtocolVersion(nProtocolVersion);
    pDvdParse->setLibraryVersion(saveGet(pRoot->Attribute("LibraryVersion")));
    pDvdParse->setLibraryName(saveGet(pRoot->Attribute("LibraryName")));
    pDvdParse->setClientName(saveGet(pRoot->Attribute("ClientName")));
}

void xmldocparser::getDvdAttributes(TiXmlElement* pDVD, dvdparse *pDvdParse) const
{
    pDvdParse->setHash(saveGet(pDVD->Attribute("Hash")));

    int nRevision = 0;
    pDVD->QueryIntAttribute("Revision", &nRevision);
    pDvdParse->setRevision(nRevision);

    queryText(pDVD, "Album", &pDvdParse->m_strAlbum);
    queryText(pDVD, "OriginalAlbum", &pDvdParse->m_strOriginalAlbum);
    queryText(pDVD, "AlbumArtist", &pDvdParse->m_strAlbumArtist);
    queryText(pDVD, "Genre", &pDvdParse->m_strGenre);
    queryText(pDVD, "Cast", &pDvdParse->m_strCast);
    queryText(pDVD, "Crew", &pDvdParse->m_strCrew);
    queryText(pDVD, "Director", &pDvdParse->m_strDirector);
    queryText(pDVD, "Screenplay", &pDvdParse->m_strScreenplay);
    queryText(pDVD, "Producer", &pDvdParse->m_strProducer);
    queryText(pDVD, "Editing", &pDvdParse->m_strEditing);
    queryText(pDVD, "Cinematography", &pDvdParse->m_strCinematography);
    queryText(pDVD, "Country", &pDvdParse->m_strCountry);
    queryText(pDVD, "OriginalLanguage", &pDvdParse->m_strOriginalLanguage);
    queryText(pDVD, "ReleaseDate", &pDvdParse->m_strReleaseDate);
    queryText(pDVD, "SpecialFeatures", &pDvdParse->m_strSpecialFeatures);
    queryText(pDVD, "EAN_UPC", &pDvdParse->m_strEAN_UPC);
    queryText(pDVD, "Storyline", &pDvdParse->m_strStoryline);
    queryText(pDVD, "Remarks", &pDvdParse->m_strRemarks);
    queryText(pDVD, "Keywords", &pDvdParse->m_strKeywords);

    queryText(pDVD, "Submitter", &pDvdParse->m_strSubmitter);
    queryText(pDVD, "Client", &pDvdParse->m_strClient);

    queryText(pDVD, "SubmitterIP", &pDvdParse->m_strSubmitterIP);
    queryText(pDVD, "DateCreated", &pDvdParse->m_strDateCreated);
    queryText(pDVD, "DateLastChanged", &pDvdParse->m_strDateLastChanged);
}

void xmldocparser::getDvdAttributesEx(TiXmlElement* pDVD, dvdparse *pDvdParse) const
{
    DVDVMGM *pDVDVMGM = (DVDVMGM *)pDvdParse->getDVDVMGM();

    for (int i = 0; i < 8; i++)
    {
        pDVD->QueryValueAttribute(("RegionProhibited" + ::toString(i + 1)).c_str(), &pDVDVMGM->m_bRegionProhibited[i]);
    }
    pDVD->QueryValueAttribute("VersionNumberMajor", &pDVDVMGM->m_wVersionNumberMajor);
    pDVD->QueryValueAttribute("VersionNumberMinor", &pDVDVMGM->m_wVersionNumberMinor);
    pDVD->QueryValueAttribute("NumberOfVolumes", &pDVDVMGM->m_wNumberOfVolumes);
    pDVD->QueryValueAttribute("VolumeNumber", &pDVDVMGM->m_wVolumeNumber);
    pDVD->QueryValueAttribute("SideID", &pDVDVMGM->m_wSideID);

    safecpy(pDVDVMGM->m_szProviderID, saveGet(pDVD->Attribute("ProviderID")).c_str());
}

void xmldocparser::

getVideoStreamAttributes(TiXmlElement* pVideoStream, LPDVDVIDEOSTREAM pDvdVideoStream) const
{
    pVideoStream->QueryValueAttribute("ID", &pDvdVideoStream->m_wID);

    safeQueryAttribute(pVideoStream, "VideoCodingMode", &pDvdVideoStream->m_eCodingMode);
    safeQueryAttribute(pVideoStream, "VideoStandard", &pDvdVideoStream->m_eStandard);
    safeQueryAttribute(pVideoStream, "VideoAspect", &pDvdVideoStream->m_eAspect);
    pVideoStream->QueryValueAttribute("AutomaticPanScanDisallowed", &pDvdVideoStream->m_bAutomaticPanScanDisallowed);
    pVideoStream->QueryValueAttribute("AutomaticPanScanDisallowed", &pDvdVideoStream->m_bAutomaticPanScanDisallowed);
    pVideoStream->QueryValueAttribute("CCForLine21Field1InGOP", &pDvdVideoStream->m_bCCForLine21Field1InGOP);
    pVideoStream->QueryValueAttribute("CCForLine21Field2InGOP", &pDvdVideoStream->m_bCCForLine21Field2InGOP);
    pVideoStream->QueryValueAttribute("CBR", &pDvdVideoStream->m_bCBR);
    safeQueryAttribute(pVideoStream, "Resolution", &pDvdVideoStream->m_Resolution);
    pVideoStream->QueryValueAttribute("LetterBoxed", &pDvdVideoStream->m_bLetterBoxed);
    pVideoStream->QueryValueAttribute("SourceFilm", &pDvdVideoStream->m_bSourceFilm);
}

int xmldocparser::parseVideoStreams(TiXmlElement* pDVD, dvdparse *pDvdParse)
{
    int res = 0;

    try
    {
        for (TiXmlElement* pVideoStream = pDVD->FirstChildElement("DVDVIDEOSTREAM"); pVideoStream != NULL; pVideoStream = pVideoStream->NextSiblingElement("DVDVIDEOSTREAM"))
        {
            // Video manager video stream
            // There is exactly only one stream
            getVideoStreamAttributes(pVideoStream, &pDvdParse->m_DVDVMGM.m_VideoStream);
        }

    }
    catch (int _res)
    {
        res = _res;
    }
    return res;
}

int xmldocparser::parseVideoStreams(TiXmlElement* pTitle, dvdtitle *pDvdTitle)
{
    int res = 0;

    try
    {
        for (TiXmlElement* pVideoStream = pTitle->FirstChildElement("DVDVIDEOSTREAM"); pVideoStream != NULL; pVideoStream = pVideoStream->NextSiblingElement("DVDVIDEOSTREAM"))
        {
            string strType;

            pVideoStream->QueryValueAttribute("Type", &strType);

            if (strType == "VTSM")
            {
                getVideoStreamAttributes(pVideoStream, &pDvdTitle->m_DVDVTS.m_VideoStreamVTSM);
            }
            else if (strType == "VTS")
            {
                // Video title set video stream
                // There is exactly only one stream
                getVideoStreamAttributes(pVideoStream, &pDvdTitle->m_DVDVTS.m_VideoStreamVTS);
            }
            else
            {
                setError("Invalid video stream type '" + strType + "'", DVDERRORCODE_XML_INCONSISTENT_DATA);
                throw -1;
            }
        }

    }
    catch (int _res)
    {
        res = _res;
    }
    return res;
}

void xmldocparser::getAudioStreamAttributes(TiXmlElement* pAudioStream, LPDVDAUDIOSTREAM pDvdAudioStream) const
{
    pAudioStream->QueryValueAttribute("ID", &pDvdAudioStream->m_wID);
    safeQueryAttribute(pAudioStream, "CodingMode", &pDvdAudioStream->m_eCodingMode);
    pAudioStream->QueryValueAttribute("MultichannelExtPresent", &pDvdAudioStream->m_bMultichannelExtPresent);
    safeQueryAttribute(pAudioStream, "Quantisation", &pDvdAudioStream->m_eQuantisation);
    pAudioStream->QueryValueAttribute("SampleRate", &pDvdAudioStream->m_dwSampleRate);
    pAudioStream->QueryValueAttribute("Channels", &pDvdAudioStream->m_wChannels);
}

int xmldocparser::parseAudioStreams(TiXmlElement* pDVD, dvdparse *pDvdParse)
{
    int res = 0;

    try
    {
        for (TiXmlElement* pAudioStream = pDVD->FirstChildElement("DVDAUDIOSTREAM"); pAudioStream != NULL; pAudioStream = pAudioStream->NextSiblingElement("DVDAUDIOSTREAM"))
        {
            // Video manager audio stream
            // There can be 0 or 1 streams
            pDvdParse->m_DVDVMGM.m_wAudioStreamCount = 1;
            getAudioStreamAttributes(pAudioStream, &pDvdParse->m_DVDVMGM.m_AudioStream);
        }
    }
    catch (int _res)
    {
        res = _res;
    }
    return res;
}

int xmldocparser::parseAudioStreams(TiXmlElement* pTitle, dvdtitle *pDvdTitle)
{
    int res = 0;

    try
    {
        for (TiXmlElement* pAudioStream = pTitle->FirstChildElement("DVDAUDIOSTREAM"); pAudioStream != NULL; pAudioStream = pAudioStream->NextSiblingElement("DVDAUDIOSTREAM"))
        {
            string strType;

            pAudioStream->QueryValueAttribute("Type", &strType);

            if (strType == "VTSM")
            {
                // Video title set menu audio stream
                // There can be 0 or 1 streams
                pDvdTitle->m_DVDVTS.m_wAudioStreamCountVTSM = 1;
                getAudioStreamAttributes(pAudioStream, &pDvdTitle->m_DVDVTS.m_AudioStreamVTSM);
            }
            else if (strType == "VTS")
            {
                // Video title set audio stream
                // Get the right stream
                uint16_t wNumber = 0;

                pAudioStream->QueryValueAttribute("Number", &wNumber);

                if (wNumber > 0 && wNumber <= DVDVTS_MAXAUDIOSTREAM)
                {
                    pDvdTitle->m_DVDVTS.m_wAudioStreamCountVTS++;
                    getAudioStreamAttributes(pAudioStream, &pDvdTitle->m_DVDVTS.m_AudioStreamVTS[wNumber - 1]);
                }
                else
                {
                    setError("Too many audio streams.", DVDERRORCODE_XML_INCONSISTENT_DATA);
                    throw -1;
                }
            }
            else
            {
                setError("Invalid audio stream type '" + strType + "'", DVDERRORCODE_XML_INCONSISTENT_DATA);
                throw -1;
            }
        }
    }
    catch (int _res)
    {
        res = _res;
    }
    return res;
}

void xmldocparser::getAudioStreamExAttributes(TiXmlElement* pAudioStreamEx, LPDVDAUDIOSTREAMEX pDvdAudioStreamEx) const
{
    pAudioStreamEx->QueryValueAttribute("LanguageCodePresent", &pDvdAudioStreamEx->m_bLanguageCodePresent);
    if (pDvdAudioStreamEx->m_bLanguageCodePresent)
    {
        pAudioStreamEx->QueryValueAttribute("LanguageCode", &pDvdAudioStreamEx->m_szLanguageCode);
    }
    safeQueryAttribute(pAudioStreamEx, "CodeExt", &pDvdAudioStreamEx->m_eCodeExt);

    safeQueryAttribute(pAudioStreamEx, "ChannelAssignment", &pDvdAudioStreamEx->m_eChannelAssignment);
    pAudioStreamEx->QueryValueAttribute("MCIntroPresent", &pDvdAudioStreamEx->m_bMCIntroPresent);
    safeQueryAttribute(pAudioStreamEx, "ApplicationMode", &pDvdAudioStreamEx->m_eApplicationMode);
    pAudioStreamEx->QueryValueAttribute("KaraokeVersion", &pDvdAudioStreamEx->m_wKaraokeVersion);
    safeQueryAttribute(pAudioStreamEx, "KaraokeMode", &pDvdAudioStreamEx->m_eKaraokeMode);
    pAudioStreamEx->QueryValueAttribute("SuitableForDolbySurroundDecoding", &pDvdAudioStreamEx->m_bSuitableForDolbySurroundDecoding);

    //    `DvdKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDVMGM',
    //    `TitleSetNo` int(11) DEFAULT NULL COMMENT 'Number of title set',
    //    `Number` int(11) DEFAULT NULL COMMENT 'Number',
    //    `Type` char(10) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Type of stream VMG (Video Manager), VTSM (Video Title Manager), VTS (Video Title)',
    //    `SuitableForDolbySurroundDecoding` bit(1) DEFAULT b'0' COMMENT 'Suitable for Dolby surround decoding',
    //    `KaraokeVersion` int(11) DEFAULT NULL COMMENT 'Karaoke version',
    //    `ApplicationMode` char(10) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Audio application mode: undefined, karaoke, surround',
    //    `MCIntroPresent` bit(1) DEFAULT b'0' COMMENT 'MC Intro Present',
    //    `LanguageCodePresent` bit(1) DEFAULT b'0' COMMENT 'Language code is present',
    //    `LanguageCode` char(2) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Two byte language code',
    //    `CodeExtPresent` bit(1) DEFAULT b'0' COMMENT 'Language code extention is present',
    //    `CodeExt` int(11) DEFAULT NULL COMMENT 'unspecified/normal/for visually impaired/director''s comments/alternate director''s comments',
    //    `Type` char(10) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Type of stream VMG (Video Manager), VTSM (Video Title Manager), VTS (Video Title)',

}

int xmldocparser::parseAudioStreamExs(TiXmlElement* pDVD, dvdtitle *pDvdTitle)
{
    int res = 0;

    try
    {
        for (TiXmlElement* pAudioStreamEx = pDVD->FirstChildElement("DVDAUDIOSTREAMEX"); pAudioStreamEx != NULL; pAudioStreamEx = pAudioStreamEx->NextSiblingElement("DVDAUDIOSTREAMEX"))
        {
            string strType;

            pAudioStreamEx->QueryValueAttribute("Type", &strType);

            if (strType == "VTS")
            {
                // Video title set audio stream (extended attributes)
                // Get the right stream
                uint16_t wNumber = 0;

                pAudioStreamEx->QueryValueAttribute("Number", &wNumber);

                if (wNumber > 0 && wNumber <= DVDVTS_MAXAUDIOSTREAM)
                {
                    getAudioStreamExAttributes(pAudioStreamEx, &pDvdTitle->m_DVDVTS.m_AudioStreamExVTS[wNumber - 1]);
                }
                else
                {
                    setError("Too many audio streams.", DVDERRORCODE_XML_INCONSISTENT_DATA);
                    throw -1;
                }
            }
            else
            {
                setError("Invalid audio stream type '" + strType + "'", DVDERRORCODE_XML_INCONSISTENT_DATA);
                throw -1;
            }
        }
    }
    catch (int _res)
    {
        res = _res;
    }
    return res;
}

void xmldocparser::getSubpicStreamAttributes(TiXmlElement* pSubpicStream, LPDVDSUBPICSTREAM pDvdSubpicStream) const
{
    pSubpicStream->QueryValueAttribute("ID", &pDvdSubpicStream->m_wID);
    pSubpicStream->QueryValueAttribute("CodingMode", &pDvdSubpicStream->m_wCodingMode);
    pSubpicStream->QueryValueAttribute("LanguageCodePresent", &pDvdSubpicStream->m_bLanguageCodePresent);
    if (pDvdSubpicStream->m_bLanguageCodePresent)
    {
        string strLanguage;
        pSubpicStream->QueryValueAttribute("LanguageCode", &strLanguage);
        languageFromString(pDvdSubpicStream->m_szLanguageCode, strLanguage);
    }
    pSubpicStream->QueryValueAttribute("CodeExtPresent", &pDvdSubpicStream->m_bCodeExtPresent);
    if (pDvdSubpicStream->m_bCodeExtPresent)
    {
        safeQueryAttribute(pSubpicStream, "CodeExt", &pDvdSubpicStream->m_eCodeExt); // TODO DVDSUBPICCODEEXT
    }
}

int xmldocparser::parseSubpicStreams(TiXmlElement* pDVD, dvdparse *pDvdParse)
{
    int res = 0;

    try
    {
        for (TiXmlElement* pSubpicStream = pDVD->FirstChildElement("DVDSUBPICSTREAM"); pSubpicStream != NULL; pSubpicStream = pSubpicStream->NextSiblingElement("DVDSUBPICSTREAM"))
        {
            // Video manager subpicture stream
            // There can be 0 or 1 streams
            pDvdParse->m_DVDVMGM.m_wSubpicStreamCount = 1;
            getSubpicStreamAttributes(pSubpicStream, &pDvdParse->m_DVDVMGM.m_SubpicStream);
        }
    }
    catch (int _res)
    {
        res = _res;
    }
    return res;
}

int xmldocparser::parseSubpicStreams(TiXmlElement* pTitle, dvdtitle *pDvdTitle)
{
    int res = 0;

    try
    {
        for (TiXmlElement* pSubpicStream = pTitle->FirstChildElement("DVDSUBPICSTREAM"); pSubpicStream != NULL; pSubpicStream = pSubpicStream->NextSiblingElement("DVDSUBPICSTREAM"))
        {
            string strType;

            pSubpicStream->QueryValueAttribute("Type", &strType);

            if (strType == "VTSM")
            {
                // Video title set menu subpicture stream
                // There can be 0 or 1 streams
                pDvdTitle->m_DVDVTS.m_wSubpicStreamCountVTSM = 1;
                getSubpicStreamAttributes(pSubpicStream, &pDvdTitle->m_DVDVTS.m_SubpicStreamVTSM);
            }
            else if (strType == "VTS")
            {
                // Video title set subpicture stream
                // Get the right stream
                uint16_t wNumber = 0;

                pSubpicStream->QueryValueAttribute("Number", &wNumber);

                if (wNumber > 0 && wNumber <= DVDVTS_MAXSUBPICSTREAM)
                {
                    pDvdTitle->m_DVDVTS.m_wSubpicStreamCountVTS++;
                    getSubpicStreamAttributes(pSubpicStream, &pDvdTitle->m_DVDVTS.m_SubpicStreamVTS[wNumber - 1]);
                }
                else
                {
                    setError("Too many subpicture streams.", DVDERRORCODE_XML_INCONSISTENT_DATA);
                    throw -1;
                }
            }
            else
            {
                setError("Invalid subpicture stream type '" + strType + "'", DVDERRORCODE_XML_INCONSISTENT_DATA);
                throw -1;
            }
        }
    }
    catch (int _res)
    {
        res = _res;
    }
    return res;
}

void xmldocparser::getFileSet(TiXmlElement* pFile, dvdfile *pDvdFile) const
{
    safeQueryAttribute(pFile, "Type", &pDvdFile->m_DVDFILE.m_eDvdFileType);
    pFile->QueryValueAttribute("VobNo", &pDvdFile->m_DVDFILE.m_wVobNo);
    pFile->QueryValueAttribute("Size", &pDvdFile->m_DVDFILE.m_dwSize);
    safeQueryAttribute(pFile, "Date", &pDvdFile->m_DVDFILE.m_Date);
}

int xmldocparser::parseFileSet(TiXmlElement* pDVD, dvdparse *pDvdParse)
{
    if (pDVD == NULL)
    {
        setInvalidParameterError("TiXmlElement is NULL");
        return -1;
    }

    if (pDvdParse == NULL)
    {
        setInvalidParameterError("dvdparse is NULL");
        return -1;
    }

    for (TiXmlElement* pFileSet = pDVD->FirstChildElement("DVDFILE"); pFileSet != NULL; pFileSet = pFileSet->NextSiblingElement("DVDFILE"))
    {
        dvdfile *pDvdFile = new dvdfile;

        getFileSet(pFileSet, pDvdFile);

        // Belongs to the video manager
        pDvdParse->m_lstDvdFile.push_back(pDvdFile);
    }

    return 0;
}

int xmldocparser::parseFileSet(TiXmlElement* pTitle, dvdtitle *pDvdTitle)
{
    if (pTitle == NULL)
    {
        setInvalidParameterError("TiXmlElement is NULL");
        return -1;
    }

    if (pDvdTitle == NULL)
    {
        setInvalidParameterError("dvdtitle is NULL");
        return -1;
    }

    for (TiXmlElement* pFileSet = pTitle->FirstChildElement("DVDFILE"); pFileSet != NULL; pFileSet = pFileSet->NextSiblingElement("DVDFILE"))
    {
        dvdfile *pDvdFile = new dvdfile;

        getFileSet(pFileSet, pDvdFile);

        pDvdFile->m_DVDFILE.m_wTitleSetNo = pDvdTitle->m_DVDVTS.m_wTitleSetNo;

        pDvdTitle->m_lstDvdFile.push_back(pDvdFile);
    }

    return 0;
}


int xmldocparser::parsePhysicalStructure(TiXmlElement* pOwner, dvdparse *pDvdParse)
{
    int res = 0;

    if (pOwner == NULL)
    {
        return 0; // No error
    }

    try
    {
        for (TiXmlElement* pTitle = pOwner->FirstChildElement("DVDVTS"); pTitle != NULL; pTitle = pTitle->NextSiblingElement("DVDVTS"))
        {
            dvdtitle * pDvdTitle = new dvdtitle;

            // Get dvdtitle data

            pTitle->QueryValueAttribute("TitleSetNo", &pDvdTitle->m_DVDVTS.m_wTitleSetNo);
            pTitle->QueryValueAttribute("VersionNumberMajor", &pDvdTitle->m_DVDVTS.m_wVersionNumberMajor);
            pTitle->QueryValueAttribute("VersionNumberMinor", &pDvdTitle->m_DVDVTS.m_wVersionNumberMinor);

            for (TiXmlElement* pProgramChain = pTitle->FirstChildElement("DVDPGC"); pProgramChain != NULL; pProgramChain = pProgramChain->NextSiblingElement("DVDPGC"))
            {
                dvdpgc * pDvdPgc = new dvdpgc;

                // Set dvdpgc data

                pDvdPgc->m_DVDPGC.m_wTitleSetNo =  pDvdTitle->m_DVDVTS.m_wTitleSetNo;

                pProgramChain->QueryValueAttribute("Number", &pDvdPgc->m_DVDPGC.m_wProgramChainNo);
                pProgramChain->QueryValueAttribute("EntryPGC", &pDvdPgc->m_DVDPGC.m_bEntryPGC);

                for (TiXmlElement* pProgram = pProgramChain->FirstChildElement("DVDPROGRAM"); pProgram != NULL; pProgram = pProgram->NextSiblingElement("DVDPROGRAM"))
                {
                    dvdprogram * pDvdProgram = new dvdprogram;

                    // Set dvdprogram data

                    pDvdProgram->m_DVDPROGRAM.m_wTitleSetNo     = pDvdTitle->m_DVDVTS.m_wTitleSetNo;
                    pDvdProgram->m_DVDPROGRAM.m_wProgramChainNo = pDvdPgc->m_DVDPGC.m_wProgramChainNo;

                    pProgram->QueryValueAttribute("Number", &pDvdProgram->m_DVDPROGRAM.m_wProgramNo);

                    for (TiXmlElement* pCell = pProgram->FirstChildElement("DVDCELL"); pCell != NULL; pCell = pCell->NextSiblingElement("DVDCELL"))
                    {
                        dvdcell * pDvdCell = new dvdcell;

                        // Set dvdcell data

                        pDvdCell->m_DVDCELL.m_wTitleSetNo           = pDvdTitle->m_DVDVTS.m_wTitleSetNo;
                        pDvdCell->m_DVDCELL.m_wProgramChainNo       = pDvdPgc->m_DVDPGC.m_wProgramChainNo;
                        pDvdCell->m_DVDCELL.m_wProgramNo            = pDvdProgram->m_DVDPROGRAM.m_wProgramNo;

                        pCell->QueryValueAttribute("Number", &pDvdCell->m_DVDCELL.m_wCellNo);

                        safeQueryAttribute(pCell, "CellType", &pDvdCell->m_DVDCELL.m_eCellType);
                        safeQueryAttribute(pCell, "BlockType", &pDvdCell->m_DVDCELL.m_eBlockType);
                        pCell->QueryValueAttribute("SeamlessMultiplex", &pDvdCell->m_DVDCELL.m_bSeamlessMultiplex);
                        pCell->QueryValueAttribute("Interleaved", &pDvdCell->m_DVDCELL.m_bInterleaved);
                        pCell->QueryValueAttribute("SCRdiscontinuity", &pDvdCell->m_DVDCELL.m_bSCRdiscontinuity);
                        pCell->QueryValueAttribute("SeamlessAngleLinkedInDSI", &pDvdCell->m_DVDCELL.m_bSeamlessAngleLinkedInDSI);
                        pCell->QueryValueAttribute("VOBStillMode", &pDvdCell->m_DVDCELL.m_bVOBStillMode);
                        pCell->QueryValueAttribute("StopsTrickPlay", &pDvdCell->m_DVDCELL.m_bStopsTrickPlay);
                        pCell->QueryValueAttribute("CellStillTime", &pDvdCell->m_DVDCELL.m_wCellStillTime);
                        pCell->QueryValueAttribute("CellCommand", &pDvdCell->m_DVDCELL.m_wCellCommand);
                        pCell->QueryValueAttribute("PlayTime", &pDvdCell->m_DVDCELL.m_qwPlayTime);
                        pCell->QueryValueAttribute("FrameRate", &pDvdCell->m_DVDCELL.m_wFrameRate);
                        pCell->QueryValueAttribute("FirstVOBUStartSector", &pDvdCell->m_DVDCELL.m_dwFirstVOBUStartSector);
                        pCell->QueryValueAttribute("FirstILVUEndSector", &pDvdCell->m_DVDCELL.m_dwFirstILVUEndSector);
                        pCell->QueryValueAttribute("LastVOBUStartSector", &pDvdCell->m_DVDCELL.m_dwLastVOBUStartSector);
                        pCell->QueryValueAttribute("LastVOBUEndSector", &pDvdCell->m_DVDCELL.m_dwLastVOBUEndSector);
                        pCell->QueryValueAttribute("VOBidn", &pDvdCell->m_DVDCELL.m_wVOBidn);
                        pCell->QueryValueAttribute("CELLidn", &pDvdCell->m_DVDCELL.m_wCELLidn);
                        pCell->QueryValueAttribute("NumberOfVOBIds", &pDvdCell->m_DVDCELL.m_wNumberOfVOBIds);

                        for (TiXmlElement* pUnit = pCell->FirstChildElement("DVDUNIT"); pUnit != NULL; pUnit = pUnit->NextSiblingElement("DVDUNIT"))
                        {
                            dvdunit * pDvdUnit = new dvdunit;

                            // Set dvdunit data

                            pDvdUnit->m_DVDUNIT.m_wTitleSetNo       = pDvdTitle->m_DVDVTS.m_wTitleSetNo;
                            pDvdUnit->m_DVDUNIT.m_wProgramChainNo   = pDvdPgc->m_DVDPGC.m_wProgramChainNo;
                            pDvdUnit->m_DVDUNIT.m_wProgramNo        = pDvdProgram->m_DVDPROGRAM.m_wProgramNo;
                            pDvdUnit->m_DVDUNIT.m_wCellNo           = pDvdCell->m_DVDCELL.m_wCellNo;

                            pUnit->QueryValueAttribute("Number", &pDvdUnit->m_DVDUNIT.m_wUnitNo);

                            pUnit->QueryValueAttribute("StartSector", &pDvdUnit->m_DVDUNIT.m_dwStartSector);
                            pUnit->QueryValueAttribute("EndSector", &pDvdUnit->m_DVDUNIT.m_dwEndSector);

                            pDvdCell->m_lstDvdUnit.push_back(pDvdUnit);
                        }

                        pDvdProgram->m_lstDvdCell.push_back(pDvdCell);
                    }

                    pDvdPgc->m_lstDvdProgram.push_back(pDvdProgram);
                }

                pDvdTitle->m_lstDvdPgc.push_back(pDvdPgc);
            }

            // Get the streams
            res = parseVideoStreams(pTitle, pDvdTitle);
            if (res)
            {
                throw res;
            }

            res = parseAudioStreams(pTitle, pDvdTitle);
            if (res)
            {
                throw res;
            }

            res = parseAudioStreamExs(pTitle, pDvdTitle);
            if (res)
            {
                throw res;
            }

            res = parseSubpicStreams(pTitle, pDvdTitle);
            if (res)
            {
                throw res;
            }

            // Get the file sets
            res = parseFileSet(pTitle, pDvdTitle);
            if (res)
            {
                throw res;
            }

            pDvdParse->m_lstDvdTitle.push_back(pDvdTitle);
        }
    }
    catch (int _res)
    {
        res = _res;
    }
    return res;
}

int xmldocparser::parseVirtualStructure(TiXmlElement* pOwner, dvdparse *pDvdParse)
{
    if (pOwner == NULL)
    {
        return 0; // No error
    }

    for (TiXmlElement* pTitle = pOwner->FirstChildElement("DVDPTTVMG"); pTitle != NULL; pTitle = pTitle->NextSiblingElement("DVDPTTVMG"))
    {
        dvdpttvmg * pDvdPttVmg = new dvdpttvmg(pDvdParse);

        // Set dvdpttvmg data
        queryText(pTitle, "Title", &pDvdPttVmg->m_strTitle);

        pTitle->QueryValueAttribute("PlaybackType", &pDvdPttVmg->m_DVDPTTVMG.m_wPlaybackType);
        pTitle->QueryValueAttribute("NumberOfVideoAngles", &pDvdPttVmg->m_DVDPTTVMG.m_wNumberOfVideoAngles);
        //pDvdPttVmg->m_DVDPTTVMG.m_byParentalMgmMaskVMG;                                     //!< Parental management mask for VMG
        //pDvdPttVmg->m_DVDPTTVMG.m_byParentalMgmMaskVTS;                                     //!< Parental management mask for VTS
        pTitle->QueryValueAttribute("VideoTitleSetNo", &pDvdPttVmg->m_DVDPTTVMG.m_wVideoTitleSetNo);
        pTitle->QueryValueAttribute("TitleNo", &pDvdPttVmg->m_DVDPTTVMG.m_wTitleNo);

        // Set chapter data
        for (TiXmlElement* pChapter = pTitle->FirstChildElement("DVDPTTVTS"); pChapter != NULL; pChapter = pChapter->NextSiblingElement("DVDPTTVTS"))
        {
            dvdpttvts *pDvdPttVts = new dvdpttvts(pDvdParse);

            // Set dvdpttvts data

            queryText(pChapter, "Artist", &pDvdPttVts->m_strArtist);
            queryText(pChapter, "Title", &pDvdPttVts->m_strTitle);

            pChapter->QueryValueAttribute("Number", &pDvdPttVts->m_DVDPTTVTS.m_wPttChapterNo);
            pChapter->QueryValueAttribute("TitleSetNo", &pDvdPttVts->m_DVDPTTVTS.m_wTitleSetNo);
            pChapter->QueryValueAttribute("ProgramChainNo", &pDvdPttVts->m_DVDPTTVTS.m_wProgramChainNo);
            pChapter->QueryValueAttribute("ProgramNo", &pDvdPttVts->m_DVDPTTVTS.m_wProgramNo);
            pChapter->QueryValueAttribute("PttTitleSetNo", &pDvdPttVts->m_DVDPTTVTS.m_wPttTitleSetNo);

            pDvdPttVmg->m_lstDvdPttVts.push_back(pDvdPttVts);
        }

        pDvdParse->m_lstDvdPttVmg.push_back(pDvdPttVmg);
    }

    return 0;
}

int xmldocparser::parseXml(const std::string & strXML, XMLMODE & eXmlMode, dvdparselst *pLstDvdParse)
{
    TiXmlDocument	doc;
    int res = 0;

    pLstDvdParse->clear();

    try
    {
        doc.Parse(strXML.c_str(), NULL, TIXML_ENCODING_UTF8);

        if (doc.Error())
        {
            setError(doc.ErrorDesc(), DVDERRORCODE_XML_ERROR);
            throw -1;
        }

        TiXmlHandle		hDoc(&doc);
        TiXmlElement*	pRoot = NULL;
        TiXmlHandle		hRoot = 0;

        pRoot = hDoc.FirstChildElement().Element();
        // should always have a valid root but handle gracefully if it does not
        if (pRoot == NULL)
        {
            setError("Invalid XML document.", DVDERRORCODE_XML_ERROR);
            throw -1;
        }

        int nXmlMode = 0;
        pRoot->QueryIntAttribute("XmlMode", &nXmlMode);

        if (eXmlMode != XMLMODE_INVALID && (XMLMODE)nXmlMode != eXmlMode)
        {
            setError("Unable to load XML document. Invalid XML mode.", DVDERRORCODE_XML_WRONG_MODE);
            throw -1;
        }

        eXmlMode = (XMLMODE)nXmlMode;

        int nProtocolVersion = 0;
        pRoot->QueryIntAttribute("ProtocolVersion", &nProtocolVersion);

        if (nProtocolVersion > LIBDVDETECT_PROTOCOL_VERSION)
        {
            setError("Unable to load XML document. Unsupported protocol version " + ::toString(nProtocolVersion) + ", maximum supported is " + ::toString(LIBDVDETECT_PROTOCOL_VERSION) + ". Please update your client.", DVDERRORCODE_XML_UNSUPPORTED_VERSION);
            throw -1;
        }

        // save this for later
        hRoot  =  TiXmlHandle(pRoot);

        if (!getXmlResponse(hRoot))
        {
            throw -1;
        }

        for (TiXmlElement* pDVD = hRoot.FirstChild("DVD").Element(); pDVD != NULL; pDVD = pDVD->NextSiblingElement("DVD"))
        {
            dvdparse *pDvdParse = new dvdparse;

            // Get root attributes
            getRootAttributes(pRoot, pDvdParse, nProtocolVersion);

            // Get DVD attributes
            getDvdAttributes(pDVD, pDvdParse);

            switch (eXmlMode)
            {
            case XMLMODE_QUERY_RESPONSE:
            case XMLMODE_SEARCH_RESPONSE:
            case XMLMODE_EXPORT:
            {
                // Get (extended) DVD attributes
                getDvdAttributesEx(pDVD, pDvdParse);

                // Parse physical view
                res = parsePhysicalStructure(pDVD->FirstChildElement("physical"), pDvdParse);
                if (res)
                {
                    throw res;
                }

                // Get the streams
                res = parseVideoStreams(pDVD, pDvdParse);
                if (res)
                {
                    throw res;
                }

                res = parseAudioStreams(pDVD, pDvdParse);
                if (res)
                {
                    throw res;
                }

                res = parseSubpicStreams(pDVD, pDvdParse);
                if (res)
                {
                    throw res;
                }

                // Get the file sets
                res = parseFileSet(pDVD, pDvdParse);
                if (res)
                {
                    throw res;
                }

                // Parse virtual view
                res = parseVirtualStructure(pDVD->FirstChildElement("virtual"), pDvdParse);
                if (res)
                {
                    throw res;
                }
                break;
            }
            default:
                setError("Unknown XML mode " + ::toString(eXmlMode), DVDERRORCODE_XML_UNKNOWN_MODE);
                throw -1;
                break;
            }

            pDvdParse->m_bIsLoaded = true;

            pLstDvdParse->push_back(pDvdParse);
        }
    }
    catch (int _res)
    {
        res = _res;
    }
    return res;
}

int xmldocparser::parseXmlResponse(const std::string & strXML, XMLMODE & eXmlMode)
{
    TiXmlDocument	doc;
    int res = 0;

    try
    {
        doc.Parse(strXML.c_str(), NULL, TIXML_ENCODING_UTF8);

        if (doc.Error())
        {
            setError(doc.ErrorDesc(), DVDERRORCODE_XML_ERROR);
            throw -1;
        }

        TiXmlHandle		hDoc(&doc);
        TiXmlElement*	pRoot = NULL;

        pRoot = hDoc.FirstChildElement().Element();
        // should always have a valid root but handle gracefully if it does not
        if (pRoot == NULL)
        {
            setError("Invalid XML document.", DVDERRORCODE_XML_ERROR);
            throw -1;
        }

        int nXmlMode = 0;
        pRoot->QueryIntAttribute("XmlMode", &nXmlMode);
        eXmlMode = (XMLMODE)nXmlMode;

        if (!getXmlResponse(TiXmlHandle(pRoot)))
        {
            throw -1;
        }
    }
    catch (int _res)
    {
        res = _res;
    }
    return res;
}
