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

/*! \file dvddatabase.h
 *
 *  \brief dvddatabase class declaration
 *
 * Lookup information about a DVD.
 */

#pragma once
#ifndef DVDDATABASE_H
#define DVDDATABASE_H

class dvdparse;
class xmldoc;
class http;

/*!
 *  @brief dvddatabase class
 *
 * This class stores all data about a DVD title set.
 */
class DLL_PUBLIC dvddatabase : public dvdetectbase
{
public:
    //! Constructor.
    /*!
     *  Construct a dvddatabase element.
     */
    explicit dvddatabase(const std::string & strClientName);
    //! Destructor.
    /*!
     *  Destruct a dvddatabase element.
     */
    virtual ~dvddatabase();

    //! Query current DVD from database.
    /*!
     *  \return
     */
    int                 query(dvdparselst & dvdParseLstIn, const dvdparse & dvdParseOut);

    //! Search DVD in database.
    /*!
     *  \return
     */
    int                 search(dvdparselst & dvdParseLstIn, const std::string & strSearchOut);

    //! Submit current DVD to database.
    /*!
     *  \return
     */
    int                 submit(const dvdparselst & dvdParseLstOut);

    //! If an error occurred, this string will return (English) details.
    /*!
     *  \return Error text.
     */
    std::string         getErrorString() const;

    //! If an error occurred, this will return the code.
    /*!
     *  \return Error code.
     */
    DVDERRORCODE        getErrorCode() const;

    dvddatabase& operator= (dvddatabase const& rhs);

    //! Set the client name
    /*!
     *  \return Error text.
     */
    void                setClientName(const std::string & strClientName);

    //! Get the client name
    /*!
     *  \return Client name.
     */
    std::string         getClientName() const;

    //! Set the proxy name (and optionally port), e.g. proxy:3128
    void                setProxy(const std::string & strProxy);

protected:
    void                setError(const std::string & strErrorString, DVDERRORCODE eErrorCode);
    void                setError(const xmldoc & xmlDoc);
    void                setError(const http & httpServer);

protected:
    std::string         m_strErrorString;			//!< Last error as clear text (English)
    DVDERRORCODE        m_eErrorCode;               //!< Error code
    std::string         m_strClientName;            //!< Name of client
    std::string         m_strProxy;                 //!< Proxy server
};

#endif // DVDDATABASE_H
