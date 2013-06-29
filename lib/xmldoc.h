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

class dvdparse;
class TiXmlHandle;
class TiXmlElement;

typedef enum
{
    XMLMODE_INVALID,                    //!< Invalid query
    XMLMODE_QUERY,                      //!< Query DVD
    XMLMODE_SEARCH,                     //!< Search DVD
    XMLMODE_SUBMIT_CONDENSED,           //!< Submit DVD (small data set)
    XMLMODE_SUBMIT_VERBOSE,             //!< Submit DVD (large data set)
    XMLMODE_RESPONSE                    //!< Response

} XMLMODE, *LPXMLMODE;

typedef const XMLMODE* LPCXMLMODE;      //!< constant version

typedef enum
{
    XMLRESULT_SUCCESS,                  //!< Success
    XMLRESULT_NOTFOUND,                 //!< Query not successful
    XMLRESULT_NOT_IMPLEMENTED,          //!< Not implemented
    XMLRESULT_SQL_ERROR,                //!< SQL error, description see error string
    XMLRESULT_DUPLICATE_SUBMISSION,     //!< DBD already in database
    XMLRESULT_XML_ERROR                 //!< XML error, description see error string

} XMLRESULT, *LPXMLRESULT;

typedef const XMLRESULT* LPCXMLRESULT;  //!< constant version

class DLL_LOCAL xmldoc : public dvdetectbase
{
public:
    explicit xmldoc(const std::string & strClientName);
    virtual ~xmldoc();

    //! Query current DVD from database.
    /*!
     *  \param strXML std::string
     *  \return
     */
    int                 query(std::string & strXML, const dvdparse & dvdParseSearch);

    //! Search DVD in database.
    /*!
     *  \param strXML std::string
     *  \return
     */
    int                 search(std::string & strXML, dvdparselst & dvdParseLstResponse, const std::string & strSearch);

    //! Submit current DVD to database.
    /*!
     *  \param strXML std::string
     *  \return
     */
    int                 submit(std::string & strXML, const dvdparselst & dvdParseLst, bool bVerbose);

    //! If an error occurred, this string will return (English) details.
    /*!
     *  \return Error text.
     */
    std::string         getErrorString() const;

    //! Set the client name
    /*!
     *  \param strClientName std::string
     *  \return Error text.
     */
    void                setClientName(const std::string &strClientName);

    //! Get the client name
    /*!
     *  \return Client name.
     */
    std::string         getClientName() const;

    //!
    /*!
     *  \param strXML std::string
     *  \return Client name.
     */
    int                 parseXmlResponse(const std::string & strXMLIn, XMLMODE & eXmlModeOut);

    //!
    /*!
     *  \param strXML std::string
     *  \return Client name.
     */
    int                 parseXml(const std::string & strXMLIn, XMLMODE & eXmlModeOut, dvdparselst & dvdParseLstOut);

    //!
    /*!
     *  \param strXML std::string
     *  \return Client name.
     */
    int                 buildXml(std::string & strXMLOut, XMLMODE eXmlModeIn, const dvdparselst & dvdParseLstIn, const std::string & strSearchIn = "");

protected:
    int                 getResponse(TiXmlHandle hRoot);
    std::string         getVideoCodingMode(DVDVIDEOCODINGMODE eDvdVideoCodingMode) const;
    std::string         getVideoStandard(DVDVIDEOTVSTANDARD eDvdVideoStandard) const;
    std::string         getVideoAspect(DVDVIDEOASPECT eDvdVideoAspect) const;
    std::string         getAudioCodingMode(DVDAUDIOCODINGMODE eAudioCodingMode) const;
    std::string         getAudioQuantisation(DVDAUDIOQUANTISATION eQuantisation) const;
    std::string         getLanguage(const char *pszLanguage) const;
    void                saveSetAttribute(TiXmlElement* element, const char *name, const std::string &strValue) const;

protected:
    std::string         m_strErrorString;			//!< Last error as clear text (English)
    std::string         m_strClientName;            //!< Name of client
};

#endif // XMLDOC_H

