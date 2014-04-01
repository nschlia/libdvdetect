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

/*! \file xmldoc.cpp
 *
 *  \brief
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>

#include "xmldoc.h"

#include "localutils.h"

xmldoc::xmldoc(const std::string &strClientName)
    : m_eErrorCode(DVDERRORCODE_SUCCESS)
    , m_strClientName(strClientName)
{
}

xmldoc::~xmldoc()
{
}

std::string xmldoc::saveGet(const char * psz) const
{
    return (psz != NULL ? psz : "");
}

void xmldoc::safeSetAttribute(TiXmlElement* pElement, const char *pszName, const std::string & strValue, bool bSaveEmpty) const
{
    if (!strValue.empty() || bSaveEmpty)
    {
        pElement->SetAttribute(pszName, strValue);
    }
}

void xmldoc::safeSetAttribute(TiXmlElement* pElement, const char *pszName, const char *pszValue, bool bSaveEmpty) const
{
    if (pszValue != NULL && (*pszValue || bSaveEmpty))
    {
        pElement->SetAttribute(pszName, pszValue);
    }
}

int xmldoc::setValue(TiXmlElement* pElement, const std::string& strNode, const char * pszValue, bool bCDATA /*= true*/) const
{
    TiXmlNode* pNode = new TiXmlElement(strNode);

    pElement->LinkEndChild(pNode);

    TiXmlText * pText = new TiXmlText(pszValue);

    pText->SetCDATA(bCDATA);
    pNode->LinkEndChild(pText);

    return 0;
}

int xmldoc::setValue(TiXmlElement* pElement, const std::string& strNode, const std::string & strValue, bool bCDATA /*= true*/) const
{
    TiXmlNode* pNode = new TiXmlElement(strNode);

    pElement->LinkEndChild(pNode);

    TiXmlText * pText = new TiXmlText(strValue);

    pText->SetCDATA(bCDATA);
    pNode->LinkEndChild(pText);

    return 0;
}

int xmldoc::safeSetValue(TiXmlElement* pElement, const std::string& strNode, const char * pszValue, bool bCDATA /*= true*/) const
{
    if (*pszValue)
    {
        TiXmlNode* pNode = new TiXmlElement(strNode);

        pElement->LinkEndChild(pNode);

        TiXmlText * pText = new TiXmlText(pszValue);

        pText->SetCDATA(bCDATA);
        pNode->LinkEndChild(pText);
    }

    return 0;
}

int xmldoc::safeSetValue(TiXmlElement* pElement, const std::string& strNode, const std::string & strValue, bool bCDATA /*= true*/) const
{
    if (!strValue.empty())
    {
        TiXmlNode* pNode = new TiXmlElement(strNode);

        pElement->LinkEndChild(pNode);

        TiXmlText * pText = new TiXmlText(strValue);

        pText->SetCDATA(bCDATA);
        pNode->LinkEndChild(pText);
    }

    return 0;
}

int xmldoc::queryText(TiXmlElement* pElement, const std::string& strNode, std::string * pstrValue) const
{
    TiXmlNode* pNode = pElement->FirstChild(strNode);
    if (pNode == NULL)
    {
        return -1;
    }

    pNode = pNode->FirstChild();
    if (pNode == NULL)
    {
        return -1;
    }

    TiXmlText * pText = pNode->ToText();
    if (pText == NULL)
    {
        return -1;
    }

    *pstrValue = pText->Value();
    return 0;
}

bool xmldoc::getXmlResponse(TiXmlHandle hRoot)
{
    TiXmlElement* pResponse = hRoot.FirstChild("response").Element();
    XMLRESULT eXmlResult = XMLRESULT_SUCCESS;

    if (pResponse != NULL)
    {
        int nResult = 0;
        pResponse->QueryIntAttribute("Result", &nResult);
        eXmlResult = (XMLRESULT)nResult;

        TiXmlElement* pResponseText = pResponse->FirstChildElement("ResponseText");
        const char *pszResponseText = NULL;
        if (pResponseText != NULL)
        {
            pszResponseText = pResponseText->GetText();
            if (pszResponseText)
            {
                setError(pszResponseText, eXmlResult);
            }
        }

        if (pszResponseText == NULL)
        {
            setError("Error from libdvdetect server.", eXmlResult);
        }
    }

    return (eXmlResult == XMLRESULT_SUCCESS);
}

std::string xmldoc::getErrorString() const
{
    return m_strErrorString;
}

DVDERRORCODE xmldoc::getErrorCode() const
{
    return m_eErrorCode;
}

void xmldoc::setClientName(const std::string & strClientName)
{
    m_strClientName = strClientName;
}

std::string xmldoc::getClientName() const
{
    return m_strClientName;
}

std::string xmldoc::valueToString(DVDVIDEOCODINGMODE eCodingMode) const
{
    std::string strVideoCodingMode;

    switch (eCodingMode)
    {
    case DVDVIDEOCODINGMODE_MPEG1:
        strVideoCodingMode = "MPEG-1";
        break;
    case DVDVIDEOCODINGMODE_MPEG2:
        strVideoCodingMode = "MPEG-2";
        break;
    default:
        break;
    }

    return strVideoCodingMode;
}

std::string xmldoc::valueToString(DVDVIDEOTVSTANDARD eStandard) const
{
    std::string strVideoStandard;

    switch (eStandard)
    {
    case DVDVIDEOTVSTANDARD_NTSC:
        strVideoStandard = "NTSC";
        break;
    case DVDVIDEOTVSTANDARD_PAL:
        strVideoStandard = "PAL";
        break;
    default:
        break;
    }

    return strVideoStandard;
}

std::string xmldoc::valueToString(DVDVIDEOASPECT eAspect) const
{
    std::string strVideoAspect;

    switch (eAspect)
    {
    case DVDVIDEOASPECT_4_3:
        strVideoAspect = "4:3";
        break;
    case DVDVIDEOASPECT_16_9:
        strVideoAspect = "16:9";
        break;
    default:
        break;
    }

    return strVideoAspect;
}

std::string xmldoc::valueToString(DVDAUDIOCODINGMODE eAudioCodingMode) const
{
    std::string strCodingMode;

    switch (eAudioCodingMode)
    {
    case DVDAUDIOCODINGMODE_AC3:
        strCodingMode = "AC3";
        break;
    case DVDAUDIOCODINGMODE_MPEG1:
        strCodingMode = "MPEG-1";
        break;
    case DVDAUDIOCODINGMODE_MPEG2:
        strCodingMode = "MPEG-2";
        break;
    case DVDAUDIOCODINGMODE_LPCM:
        strCodingMode = "LPCM";
        break;
    case DVDAUDIOCODINGMODE_DTS:
        strCodingMode = "DTS";
        break;
    default:
        break;
    }

    return strCodingMode;
}

std::string xmldoc::valueToString(DVDAUDIOQUANTISATION eQuantisation) const
{
    std::string strQuantisation;

    switch (eQuantisation)
    {
    case DVDAUDIOQUANTISATION_16BPS:
        strQuantisation = "16 bps";
        break;
    case DVDAUDIOQUANTISATION_20BPS:
        strQuantisation = "20 bps";
        break;
    case DVDAUDIOQUANTISATION_24BPS:
        strQuantisation = "24 bps";
        break;
    case DVDAUDIOQUANTISATION_DRC:
        strQuantisation = "DRC";
        break;
    default:
        break;
    }
    return strQuantisation;
}

std::string xmldoc::valueToString(DVDAUDIOCHANNELASSIGNMENT eAudioChannelAssignment) const
{
    return ::toString(eAudioChannelAssignment); // Keep it simple: the description makes no sense to the reader anyway
}

std::string xmldoc::valueToString(DVDAUDIOAPPLICATIONMODE eAudioApplicationMode) const
{
    std::string strAudioApplicationMode;

    switch (eAudioApplicationMode)
    {
    case DVDAUDIOAPPLICATIONMODE_UNSPECIFIED:
        strAudioApplicationMode = "unspecified";
        break;
    case DVDAUDIOAPPLICATIONMODE_KARAOKE:
        strAudioApplicationMode = "karaoke";
        break;
    case DVDAUDIOAPPLICATIONMODE_SURROUND:
        strAudioApplicationMode = "surround";
        break;
    default:
        break;
    }
    return strAudioApplicationMode;
}

std::string xmldoc::valueToString(DVDAKARAOKEMODE eKaraokeMode) const
{
    std::string strKaraokeMode;

    switch (eKaraokeMode)
    {
    case DVDKARAOKEMODE_SOLO:
        strKaraokeMode = "solo";
        break;
    case DVDKARAOKEMODE_DUET:
        strKaraokeMode = "duet";
        break;
    default:
        break;
    }
    return strKaraokeMode;
}

std::string xmldoc::valueToString(DVDAUDIOCODEEXT eAudioCodeExt) const
{
    return ::toString(eAudioCodeExt); // Keep it simple: the description makes no sense to the reader anyway
}

std::string xmldoc::valueToString(DVDSUBPICCODEEXT eSubpictCodeExt) const
{
    return ::toString(eSubpictCodeExt); // Keep it simple: the description makes no sense to the reader anyway
}

std::string xmldoc::valueToString(CELLTYPE eCellType) const
{
    std::string strCellType;

    switch (eCellType)
    {
    case CELLTYPE_NORMAL:
        strCellType = "normal";
        break;
    case CELLTYPE_FIRST:
        strCellType = "first";
        break;
    case CELLTYPE_MIDDLE:
        strCellType = "middle";
        break;
    case CELLTYPE_LAST:
        strCellType = "last";
        break;
    default:
        break;
    }

    return strCellType;
}

std::string xmldoc::valueToString(BLOCKTYPE eBlockType) const
{
    std::string strBlockType;

    switch (eBlockType)
    {
    case BLOCKTYPE_NORMAL:
        strBlockType = "normal";
        break;
    case BLOCKTYPE_ANGLE:
        strBlockType = "angle";
        break;
    default:
        break;
    }
    return strBlockType;
}

string xmldoc::valueToString(DVDFILETYPE eDvdFileType) const
{
    return dvdGetFileType(eDvdFileType);
}

std::string xmldoc::valueToString(LPCDVDVIDEORESOLUTION pVideoResolution) const
{
    if (pVideoResolution != NULL && pVideoResolution->m_wX && pVideoResolution->m_wY)
    {
        return ::toString(pVideoResolution->m_wX) + "x" + ::toString(pVideoResolution->m_wY);
    }
    else
    {
        return "";
    }
}

std::string xmldoc::valueToString(const time_t *pUnixTime) const
{
#if 0 // Reentrant
    struct tm timeinfo;
    char szBuffer [80] = "";

    memset(&timeinfo, 0, sizeof(tm));

    gmtime_r(pUnixTime, &timeinfo);
#else
    struct tm *timeinfo;
    char szBuffer [80] = "";

    timeinfo = gmtime(pUnixTime);
#endif

    strftime(szBuffer, sizeof(szBuffer), DVD_DATEFORMAT, timeinfo);

    return szBuffer;
}

std::string xmldoc::languageToString(const char pszLangCode[]) const  // TODO
{
    return pszLangCode;
}

void xmldoc::fromString(LPDVDVIDEOCODINGMODE peVideoCodingMode, const std::string & strDvdVideoCodingMode) const
{
    if (strDvdVideoCodingMode == "MPEG-1")
    {
        *peVideoCodingMode = DVDVIDEOCODINGMODE_MPEG1;
    }
    else if (strDvdVideoCodingMode == "MPEG-2")
    {
        *peVideoCodingMode = DVDVIDEOCODINGMODE_MPEG2;
    }
    else
    {
        *peVideoCodingMode = DVDVIDEOCODINGMODE_INVALID;
    }
}

void xmldoc::fromString(LPDVDVIDEOTVSTANDARD peVideoStandard, const std::string & strDvdVideoStandard) const
{
    if (strDvdVideoStandard == "NTSC")
    {
        *peVideoStandard = DVDVIDEOTVSTANDARD_NTSC;
    }
    else if (strDvdVideoStandard == "PAL")
    {
        *peVideoStandard = DVDVIDEOTVSTANDARD_PAL;
    }
    else
    {
        *peVideoStandard = DVDVIDEOTVSTANDARD_INVALID;
    }
}

void xmldoc::fromString(LPDVDVIDEORESOLUTION pVideoResolution, const std::string & strResolution) const
{
    if (pVideoResolution == NULL)
    {
        return;
    }

    size_t nPos = strResolution.find('x');

    memset(pVideoResolution, 0, sizeof(DVDVIDEORESOLUTION));

    if (nPos != string::npos && nPos)
    {
        ::fromString(&pVideoResolution->m_wX, strResolution.substr(0, nPos));
        ::fromString(&pVideoResolution->m_wY, strResolution.substr(nPos + 1));
    }
}

void xmldoc::fromString(LPDVDVIDEOASPECT peVideoAspect, const std::string & strDvdVideoAspect) const
{
    if (strDvdVideoAspect == "4:3")
    {
        *peVideoAspect = DVDVIDEOASPECT_4_3;
    }
    else if (strDvdVideoAspect == "16:9")
    {
        *peVideoAspect = DVDVIDEOASPECT_16_9;
    }
    else
    {
        *peVideoAspect = DVDVIDEOASPECT_INVALID;
    }
}

void xmldoc::fromString(LPDVDAUDIOCODINGMODE peAudioCodingMode, const std::string & strAudioCodingMode) const
{
    if (strAudioCodingMode == "AC3")
    {
        *peAudioCodingMode = DVDAUDIOCODINGMODE_AC3;
    }
    else if (strAudioCodingMode == "MPEG-1")
    {
        *peAudioCodingMode = DVDAUDIOCODINGMODE_MPEG1;
    }
    else if (strAudioCodingMode == "MPEG-2")
    {
        *peAudioCodingMode = DVDAUDIOCODINGMODE_MPEG2;
    }
    else if (strAudioCodingMode == "LPCM")
    {
        *peAudioCodingMode = DVDAUDIOCODINGMODE_LPCM;
    }
    else if (strAudioCodingMode == "DTS")
    {
        *peAudioCodingMode = DVDAUDIOCODINGMODE_DTS;
    }
    else
    {
        *peAudioCodingMode = DVDAUDIOCODINGMODE_INVALID;
    }
}

void xmldoc::fromString(LPDVDAUDIOQUANTISATION peAudioQuantisation, const std::string & strQuantisation) const
{
    if (strQuantisation == "16 bps")
    {
        *peAudioQuantisation = DVDAUDIOQUANTISATION_16BPS;
    }
    else if (strQuantisation == "20 bps")
    {
        *peAudioQuantisation = DVDAUDIOQUANTISATION_20BPS;
    }
    else if (strQuantisation == "24 bps")
    {
        *peAudioQuantisation = DVDAUDIOQUANTISATION_24BPS;
    }
    else if (strQuantisation == "DRC")
    {
        *peAudioQuantisation = DVDAUDIOQUANTISATION_DRC;
    }
    else
    {
        *peAudioQuantisation = DVDAUDIOQUANTISATION_INVALID;
    }
}

void xmldoc::fromString(LPDVDAUDIOCHANNELASSIGNMENT peAudioChannelAssignment, const std::string & strAudioChannelAssignment) const
{
    ::fromString((int *)peAudioChannelAssignment, strAudioChannelAssignment);
}

void xmldoc::fromString(LPDVDAUDIOAPPLICATIONMODE peAudioApplicationMode, const std::string & strAudioApplicationMode) const
{
    if (strAudioApplicationMode == "unspecified")
    {
        *peAudioApplicationMode = DVDAUDIOAPPLICATIONMODE_UNSPECIFIED;
    }
    else if (strAudioApplicationMode == "karaoke")
    {
        *peAudioApplicationMode = DVDAUDIOAPPLICATIONMODE_KARAOKE;
    }
    else if (strAudioApplicationMode == "surround")
    {
        *peAudioApplicationMode = DVDAUDIOAPPLICATIONMODE_SURROUND;
    }
    else
    {
        *peAudioApplicationMode = DVDAUDIOAPPLICATIONMODE_INVALID;
    }
}

void xmldoc::fromString(LPDVDAKARAOKEMODE peKaraokeMode, const std::string & strKaraokeMode) const
{
    if (strKaraokeMode == "solo")
    {
        *peKaraokeMode = DVDKARAOKEMODE_SOLO;
    }
    else if (strKaraokeMode == "duet")
    {
        *peKaraokeMode = DVDKARAOKEMODE_DUET;
    }
    else
    {
        *peKaraokeMode = DVDKARAOKEMODE_INVALID;
    }
}

void xmldoc::fromString(LPDVDAUDIOCODEEXT peAudioCodeExt, const std::string & strAudioCodeExt) const
{
    ::fromString((int *)peAudioCodeExt, strAudioCodeExt);
}

void xmldoc::fromString(LPDVDSUBPICCODEEXT peSubpictCodeExt, const std::string & strSubpictCodeExt) const
{
    ::fromString((int *)peSubpictCodeExt, strSubpictCodeExt);
}

void xmldoc::fromString(LPCELLTYPE peCellType, const std::string & strCellType) const
{
    if (strCellType == "normal")
    {
        *peCellType = CELLTYPE_NORMAL;
    }
    else if (strCellType == "first")
    {
        *peCellType = CELLTYPE_FIRST;
    }
    else if (strCellType == "middle")
    {
        *peCellType = CELLTYPE_MIDDLE;
    }
    else if (strCellType == "last")
    {
        *peCellType = CELLTYPE_LAST;
    }
    else
    {
        *peCellType = CELLTYPE_INVALID;
    }
}

void xmldoc::fromString(LPBLOCKTYPE peBlockType, const std::string & strBlockType) const
{
    if (strBlockType == "normal")
    {
        *peBlockType = BLOCKTYPE_NORMAL;
    }
    else if (strBlockType == "angle")
    {
        *peBlockType = BLOCKTYPE_ANGLE;
    }
    else
    {
        *peBlockType = BLOCKTYPE_INVALID;
    }
}

void xmldoc::fromString(LPDVDFILETYPE peDvdFileType, const std::string & strFileType) const
{
    if (strFileType == "VMG/IFO")
    {
        *peDvdFileType = DVDFILETYPE_VMG_IFO;   // VIDEO_TS.IFO
    }
    else if (strFileType == "VMG/BUP")
    {
        *peDvdFileType = DVDFILETYPE_VMG_BUP;   // VIDEO_TS.BUP
    }
    else if (strFileType == "VMG/VOB")
    {
        *peDvdFileType = DVDFILETYPE_VMG_VOB;   // VIDEO_TS.VOB
    }
    else if (strFileType == "VTS/IFO")
    {
        *peDvdFileType = DVDFILETYPE_VTS_IFO;   // VTS_##_0.IFO
    }
    else if (strFileType == "VTS/BUP")
    {
        *peDvdFileType = DVDFILETYPE_VTS_BUP;   // VTS_##_0.BUP
    }
    else if (strFileType == "MNU/VOB")
    {
        *peDvdFileType = DVDFILETYPE_MENU_VOB;  // VTS_##_0.VOB
    }
    else if (strFileType == "VTS/VOB")
    {
        *peDvdFileType = DVDFILETYPE_VTS_VOB;   // VTS_##_1.VOB to VTS_##_9.VOB
    }
    else
    {
        *peDvdFileType = DVDFILETYPE_INVALID;
    }
}

void xmldoc::languageFromString(char *pszLanguage, const std::string & strLanguage) const
{
    strcpy(pszLanguage, strLanguage.c_str());// TODO
}

void xmldoc::fromString(time_t * pUnixTime, const std::string & strDate) const
{
    struct tm time;
    memset(&time, 0, sizeof(struct tm));

    strptime(strDate.c_str(), DVD_DATEFORMAT, &time);

    *pUnixTime = getgmtime(&time);
}

void xmldoc::setError(const std::string & strErrorString, DVDERRORCODE eErrorCode)
{
    m_strErrorString    = strErrorString;
    m_eErrorCode        = eErrorCode;
}

void xmldoc::setError(const std::string & strErrorString, XMLRESULT eXmlResult)
{
    m_strErrorString    = strErrorString;

    switch (eXmlResult)
    {
    case XMLRESULT_SUCCESS:
        m_eErrorCode = DVDERRORCODE_SUCCESS;
        break;
    case XMLRESULT_NOT_FOUND:
        m_eErrorCode = DVDERRORCODE_NOT_FOUND;
        break;
    case XMLRESULT_NOT_IMPLEMENTED:
        m_eErrorCode = DVDERRORCODE_NOT_IMPLEMENTED;
        break;
    case XMLRESULT_SQL_ERROR:
        m_eErrorCode = DVDERRORCODE_SQL_ERROR;
        break;
    case XMLRESULT_DUPLICATE_SUBMISSION:
        m_eErrorCode = DVDERRORCODE_DUPICLATE_SUBMISSION;
        break;
    case XMLRESULT_XML_ERROR:
        m_eErrorCode = DVDERRORCODE_XML_ERROR;
        break;
    case XMLRESULT_UNSUPPORTED_VERSION:
        m_eErrorCode = DVDERRORCODE_UNSUPPORTED_VERSION;
        break;
    }
}


