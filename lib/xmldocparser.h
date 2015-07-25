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

/*! \file xmldocparser.h
 *
 *  \brief
 *
 *
 *
 */

#pragma once

#ifndef XMLDOCPARSER_H
#define XMLDOCPARSER_H

#include <dvdetect/dvdetectdll.h>
#include <dvdetect/dvdetectbase.h>

#include "xmldoc.h"

class dvdparse;

class DLL_LOCAL xmldocparser : public xmldoc
{
public:
    explicit xmldocparser(const std::string & strClientName);
    virtual ~xmldocparser();

    //! Parse XML response from server
    /*!
     *  \param strXML std::string & XML response to parse
     *  \param eXmlMode XMLMODE & XML mode of document
     *  \return 0 if successfull; nonzero on error
     */
    int                 	parseXmlResponse(const std::string & strXML, XMLMODE & eXmlMode);

    //! Parse XML from server
    /*!
     *  \param strXML std::string & XML document to parse
     *  \param eXmlMode XMLMODE & XML mode of document
     *  \param pLstDvdParse const dvdparselst * Returns dvdparse list read in
     *  \return 0 if successfull; nonzero on error
     */
    int                 	parseXml(const std::string & strXML, XMLMODE & eXmlMode, dvdparselst *pLstDvdParse);

protected:
    void                    getRootAttributes(TiXmlElement * pRoot, dvdparse *pDvdParse, int nProtocolVersion);
    // No need to read comments
    void                    getDvdAttributes(TiXmlElement* pDVD, dvdparse *pDvdParse) const;
    void                    getDvdAttributesEx(TiXmlElement *pDVD, dvdparse *pDvdParse) const;

    void                    getVideoStreamAttributes(TiXmlElement* pVideoStream, LPDVDVIDEOSTREAM pDvdVideoStream) const;
    int                     parseVideoStreams(TiXmlElement* pDVD, dvdparse *pDvdParse);
    int                     parseVideoStreams(TiXmlElement* pTitle, dvdtitle *pDvdTitle);

    void                    getAudioStreamAttributes(TiXmlElement* pAudioStream, LPDVDAUDIOSTREAM pDvdAudioStream) const;
    int                     parseAudioStreams(TiXmlElement* pDVD, dvdparse *pDvdParse);
    int                     parseAudioStreams(TiXmlElement* pTitle, dvdtitle *pDvdTitle);

    void                    getAudioStreamExAttributes(TiXmlElement* pAudioStreamEx, LPDVDAUDIOSTREAMEX pDvdAudioStreamEx) const;
    int                     parseAudioStreamExs(TiXmlElement* pTitle, dvdtitle *pDvdTitle);

    void                    getSubpicStreamAttributes(TiXmlElement* pSubpicStream, LPDVDSUBPICSTREAM pDvdSubpicStream) const;
    int                     parseSubpicStreams(TiXmlElement* pDVD, dvdparse *pDvdParse);
    int                     parseSubpicStreams(TiXmlElement* pTitle, dvdtitle *pDvdTitle);

    int                     parsePhysicalStructure(TiXmlElement* pOwner, dvdparse *pDvdParse);
    int                     parseVirtualStructure(TiXmlElement* pOwner, dvdparse *pDvdParse);

    void                    getFileSet(TiXmlElement* pFile, dvdfile *pDvdFile) const;
    int                     parseFileSet(TiXmlElement* pDVD, dvdparse *pDvdParse);
    int                     parseFileSet(TiXmlElement* pTitle, dvdtitle *pDvdTitle);
};

#endif // XMLDOCPARSER_H

