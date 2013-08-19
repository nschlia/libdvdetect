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

/*! \file xmldoc.h
 *
 *  \brief
 *
 *
 *
 */

#pragma once

#ifndef XMLDOC_H
#define XMLDOC_H

#include <dvdetect/dvdetectdll.h>
#include <dvdetect/dvdetectbase.h>

#include <tinyxml.h>

class DLL_LOCAL xmldoc : public dvdetectbase
{
public:
    explicit xmldoc(const std::string & strClientName);
    virtual ~xmldoc();

    //! If an error occurred, this string will return (English) details.
    /*!
     *  \return Error text.
     */
    std::string         	getErrorString() const;

    //! If an error occurred, this will return the code.
    /*!
     *  \return Error code.
     */
    DVDERRORCODE            getErrorCode() const;

    //! Set the client name
    /*!
     *  \param strClientName std::string
     */
    void                	setClientName(const std::string & strClientName);

    //! Get the client name
    /*!
     *  \return Client name.
     */
    std::string         	getClientName() const;

protected:
    std::string         	saveGet(const char * psz) const;
    void                	safeSetAttribute(TiXmlElement* pElement, const char *pszName, const std::string & strValue, bool bSaveEmpty = false) const;
    void                	safeSetAttribute(TiXmlElement* pElement, const char *pszName, const char *pszValue, bool bSaveEmpty = false) const;
    template<typename T>
    void                    safeSetAttribute(TiXmlElement* pElement, const char *pszName, const T & value, bool bSaveEmpty = false) const
    {
        if (value != 0 || bSaveEmpty)
        {
            pElement->SetAttribute(pszName, valueToString(value));
        }
    }

    template<typename T>
    int                     safeQueryAttribute(TiXmlElement* pElement, const char *pszName, T * value) const
    {
        std::string strValue;
        int res = pElement->QueryValueAttribute(pszName, &strValue);

        if (res != TIXML_SUCCESS)
        {
            return res;
        }

        fromString(value, strValue);

        return TIXML_SUCCESS;
    }

    template<typename T>
    int                     setValue(TiXmlElement* pElement, const std::string& strNode, const T & value, bool bCDATA = false) const
    {
        std::stringstream ss;

        ss << value;

        TiXmlNode* pNode = new TiXmlElement(strNode);

        pElement->LinkEndChild(pNode);

        TiXmlText * pText = new TiXmlText(ss.str());

        pText->SetCDATA(bCDATA);
        pNode->LinkEndChild(pText);

        return (ss.fail() ? -1 : 0);
    }
    int                     setValue(TiXmlElement* pElement, const std::string& strNode, const char * pszValue, bool bCDATA = true) const;
    int                     setValue(TiXmlElement* pElement, const std::string& strNode, const std::string & strValue, bool bCDATA = true) const;

    template<typename T>
    int                     safeSetValue(TiXmlElement* pElement, const std::string& strNode, const T & value, bool bCDATA = false) const
    {
        std::stringstream ss;

        ss << value;

        if (!ss.str().empty())
        {
            TiXmlNode* pNode = new TiXmlElement(strNode);

            pElement->LinkEndChild(pNode);

            TiXmlText * pText = new TiXmlText(ss.str());

            pText->SetCDATA(bCDATA);
            pNode->LinkEndChild(pText);
        }

        return (ss.fail() ? -1 : 0);
    }
    int                     safeSetValue(TiXmlElement* pElement, const std::string& strNode, const char * pszValue, bool bCDATA = true) const;
    int                     safeSetValue(TiXmlElement* pElement, const std::string& strNode, const std::string & strValue, bool bCDATA = true) const;

    template< typename T >
    int                     queryValue(TiXmlElement* pElement, const std::string& strNode, T* value) const
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
        std::stringstream ss(pText->Value());
        ss >> *value;

        return (ss.fail() ? -1 : 0);
    }
    int                     queryText(TiXmlElement* pElement, const std::string& strNode, std::string * pstrValue) const;

    bool                    getXmlResponse(TiXmlHandle hRoot);

    // Video
    std::string         	valueToString(DVDVIDEOCODINGMODE eDvdVideoCodingMode) const;
    std::string         	valueToString(DVDVIDEOTVSTANDARD eDvdVideoStandard) const;
    std::string         	valueToString(DVDVIDEOASPECT eDvdVideoAspect) const;
    std::string 			valueToString(LPCDVDVIDEORESOLUTION pVideoResolution) const;
    // Audio
    std::string         	valueToString(DVDAUDIOCODINGMODE eAudioCodingMode) const;
    std::string         	valueToString(DVDAUDIOQUANTISATION eQuantisation) const;
    std::string         	valueToString(DVDAUDIOCHANNELASSIGNMENT eAudioChannelAssignment) const;
    std::string         	valueToString(DVDAUDIOAPPLICATIONMODE eAudioApplicationMode) const;
    std::string         	valueToString(DVDAKARAOKEMODE eKaraokeMode) const;
    std::string         	valueToString(DVDAUDIOCODEEXT eAudioCodeExt) const;
    // Subpicture
    std::string         	valueToString(DVDSUBPICCODEEXT eSubpictCodeExt) const;
    // Cells
    std::string             valueToString(CELLTYPE eCellType) const;
    std::string             valueToString(BLOCKTYPE eBlockType) const;
    // Files
    std::string             valueToString(DVDFILETYPE eDvdFileType) const;

    std::string 			valueToString(const time_t *pUnixTime) const;
    template<class T>
    std::string             valueToString(T i) const    // Anything else
    {
        std::stringstream ss;
        ss << i;
        return ss.str();
    }
    std::string         	languageToString(const char pszLangCode[]) const;

    // Video
    void                    fromString(LPDVDVIDEOCODINGMODE peVideoCodingMode, const std::string & strDvdVideoCodingMode) const;
    void                    fromString(LPDVDVIDEOTVSTANDARD peVideoStandard, const std::string & strDvdVideoStandard) const;
    void                    fromString(LPDVDVIDEORESOLUTION pVideoResolution, const std::string & strResolution) const;
    void                    fromString(LPDVDVIDEOASPECT peVideoAspect, const std::string & strDvdVideoAspect) const;
    // Audio
    void                    fromString(LPDVDAUDIOCODINGMODE peAudioCodingMode, const std::string & strAudioCodingMode) const;
    void                    fromString(LPDVDAUDIOQUANTISATION peAudioQuantisation, const std::string & strQuantisation) const;
    void                    fromString(LPDVDAUDIOCHANNELASSIGNMENT peAudioChannelAssignment, const std::string & strAudioChannelAssignment) const;
    void                    fromString(LPDVDAUDIOAPPLICATIONMODE peAudioApplicationMode, const std::string & strAudioApplicationMode) const;
    void                    fromString(LPDVDAKARAOKEMODE peKaraokeMode, const std::string & strKaraokeMode) const;
    void                    fromString(LPDVDAUDIOCODEEXT peAudioCodeExt, const std::string & strAudioCodeExt) const;
    // Subpicture
    void                    fromString(LPDVDSUBPICCODEEXT peSubpictCodeExt, const std::string & strSubpictCodeExt) const;
    // Cells
    void                    fromString(LPCELLTYPE peCellType, const std::string & strCellType) const;
    void                    fromString(LPBLOCKTYPE peBlockType, const std::string & strBlockType) const;
    // Files
    void                    fromString(LPDVDFILETYPE peDvdFileType, const std::string & strFileType) const;

    void                    languageFromString(char pszLangCode[DVD_LANGCODELEN + 1], const std::string & strLanguage) const;
    void                    fromString(time_t *pUnixTime, const std::string & strDate) const;

    void                    setError(const std::string & strErrorString, DVDERRORCODE eErrorCode);
    void                    setError(const std::string & strErrorString, XMLRESULT eXmlResult);

protected:
    std::string         	m_strErrorString;			//!< Last error as clear text (English)
    DVDERRORCODE            m_eErrorCode;               //!< Error code
    std::string         	m_strClientName;            //!< Name of client
};

#endif // XMLDOC_H

