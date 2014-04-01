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

/*! \file xmldocbuilder.h
 *
 *  \brief
 *
 *
 *
 */

#pragma once

#ifndef XMLDOCBUILDER_H
#define XMLDOCBUILDER_H

#include <dvdetect/dvdetectdll.h>
#include <dvdetect/dvdetectbase.h>

#include "xmldoc.h"

class dvdparse;

class DLL_LOCAL xmldocbuilder : public xmldoc
{
public:
    explicit xmldocbuilder(const std::string & strClientName);
    virtual ~xmldocbuilder();

    //! Build XML
    /*!
     *  \param strXML std::string & Newly created XML document
     *  \param eXmlMode XMLMODE & XML mode to use (export, search, etc.)
     *  \param pDvdParse const dvdparse * dvdparse object
     *  \param strSearch const std::string & For XMLODE_SEARCH only: search text
     *  \return 0 if successfull; nonzero on error
     */
    int                 	buildXml(std::string & strXML, XMLMODE eXmlMode, const dvdparse *pDvdParse, const std::string & strSearch = "");

    //! Build XML
    /*!
     *  \param strXML std::string & Newly created XML document
     *  \param eXmlMode XMLMODE & XML mode to use (export, search, etc.)
     *  \param pDvdParseLst const dvdparselst * dvdparse list of objects to write
     *  \param strSearch const std::string & For XMLODE_SEARCH only: search text
     *  \return 0 if successfull; nonzero on error
     */
    int                 	buildXml(std::string & strXML, XMLMODE eXmlMode, const dvdparselst *pDvdParseLst, const std::string & strSearch = "");

    bool                    getCondensedMode() const;
    void                    setCondensedMode(bool bCondensedMode);

protected:
//    void                    additionalInfos(TiXmlElement* pDVD, const dvdparse *pDvdParse);
    void                    addRootAttributes(TiXmlElement* pRoot, XMLMODE eXmlMode) const;

    void                    addComment(TiXmlElement *pOwner, XMLMODE eXmlMode) const;
    void                    addComment(TiXmlElement *pOwner, const char *pszComment) const;

    void                    addDvdAttributes(TiXmlElement* pDVD, const dvdparse *pDvdParse, XMLMODE eXmlMode) const;
    void                    addDvdAttributesEx(TiXmlElement* pDVD, const dvdparse *pDvdParse) const;

    void                    addVideoStreamVMG(TiXmlElement* pDVD, LPCDVDVMGM pDVDVMGM) const;
    void                    addVideoStream(TiXmlElement* pDVD, LPCDVDVIDEOSTREAM pDvdVideoStream, const std::string &strType) const;
    void                    addVideoStreamVTSM(TiXmlElement* pDVD, LPCDVDVTS pDVDVTS) const;
    void                    addVideoStreamVTS(TiXmlElement* pDVD, LPCDVDVTS pDVDVTS) const;

    void                    addAudioStream(TiXmlElement* pDVD, LPCDVDAUDIOSTREAM pDvdAudioStream, const std::string &strType, uint16_t wAudioStreamNo = 0) const;
    void                    addAudioStreamEx(TiXmlElement* pDVD, LPCDVDAUDIOSTREAMEX pDvdAudioStreamEx, uint16_t wAudioStreamNo) const;
    void                    addAudioStreamVMG(TiXmlElement* pDVD, LPCDVDVMGM pDVDVMGM) const;
    void                    addAudioStreamVTSM(TiXmlElement* pDVD, LPCDVDVTS pDVDVTS) const;
    void                    addAudioStreamVTS(TiXmlElement* pDVD, LPCDVDVTS pDVDVTS) const;

    void                    addSubpicStream(TiXmlElement* pDVD, LPCDVDSUBPICSTREAM pDvdSubpicStream, const std::string &strType, uint16_t wSubpicStreamNo = 0) const;
    void                    addSubpicStreamVMG(TiXmlElement* pDVD, LPCDVDVMGM pDVDVMGM) const;
    void                    addSubpicStreamVTSM(TiXmlElement* pDVD, LPCDVDVTS pDVDVTS) const;
    void                    addSubpicStreamVTS(TiXmlElement* pDVD, LPCDVDVTS pDVDVTS) const;

//    void                    buildTitle(TiXmlElement* pDVD, LPCDVDVTS pDVDVTS) const;
    void                    buildPhysicalStructure(TiXmlElement* pOwner, const dvdparse *pDvdParse, XMLMODE eXmlMode) const;
    void                    buildVirtualStructure(TiXmlElement* pOwner, const dvdparse *pDvdParse, XMLMODE eXmlMode) const;
    void                    addFileSet(TiXmlElement* pDVD, const dvdparse *pDvdParse) const;
    void                    addFileSet(TiXmlElement* pDVD, const dvdtitle *pDvdTitle) const;

    template<class T>
    void                    buildFileSetTpl(TiXmlElement* pDVD, const T *pClass) const
    {
        for (uint16_t wFileNo = 1; wFileNo <= pClass->getFileCount(); wFileNo++)
        {
            const dvdfile *pDvdFile = pClass->getDvdFile(wFileNo - 1);
            LPCDVDFILE pDVDFILE = pDvdFile->getDVDFILE();

            TiXmlElement * pFile = new TiXmlElement("DVDFILE");

            safeSetAttribute(pFile, "Number", wFileNo, true);
            safeSetAttribute(pFile, "Type", pDVDFILE->m_eDvdFileType);
            if (pDVDFILE->m_eDvdFileType == DVDFILETYPE_VMG_VOB ||
                    pDVDFILE->m_eDvdFileType == DVDFILETYPE_MENU_VOB ||
                    pDVDFILE->m_eDvdFileType == DVDFILETYPE_VTS_VOB)
            {
                safeSetAttribute(pFile, "VobNo", pDVDFILE->m_wVobNo);
            }
            safeSetAttribute(pFile, "Size", pDVDFILE->m_dwSize, true);
            safeSetAttribute(pFile, "Date", &pDVDFILE->m_Date);

            pDVD->LinkEndChild(pFile);
        }
    }

protected:
    bool        m_bCondensedMode;
};

#endif // XMLDOCBUILDER_H

