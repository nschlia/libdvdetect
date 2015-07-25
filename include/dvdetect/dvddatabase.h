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
class xmldocparser;
class xmldocbuilder;
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
    explicit dvddatabase(const std::string & strClientName,
                         void *  (*openFile)(const char *, const char *, const char *) = ::openFile,
                         size_t  (*readFile)(void*, size_t, size_t, void*) = ::readFile,
                         int     (*writeFile)(const void*, size_t, size_t, void*) = ::writeFile,
                         int     (*closeFile)(void *) = ::closeFile,
                         int     (*statFile)(const char *, LPDVDFILESTAT, const char *) = ::statFile,
                         int     (*fstatFile)(void*, LPDVDFILESTAT) = ::fstatFile);
    //! Destructor.
    /*!
     *  Destruct a dvddatabase element.
     */
    virtual ~dvddatabase();

    //! Query current DVD from database.
    /*!
     *  \return
     */
    int                 query(dvdparselst * pLstDvdParse, const dvdparse * pDvdParse);

    //! Search DVD in database.
    /*! Search DVD in database.
     *
     *  \return Success: 0
     *  \return Fail: generally non-zero value; if > 0 a http code (404, 500, etc); -1 otherwise. Call getErrorString() and getErrorCode() for details.
     */
    int                 search(dvdparselst * pLstDvdParse, const std::string & strSearch);

    //! Submit current DVD to database.
    /*! Submit current DVD to database (single DVD submit).
     *
     *  \return Success: 0
     *  \return Fail: generally non-zero value; if > 0 a http code (404, 500, etc); -1 otherwise. Call getErrorString() and getErrorCode() for details.
     */
    int                 submit(const dvdparse *pDvdParse);

    //! Submit current DVD to database.
    /*! Submit current DVD to database (submit several DVDs at once).
     *
     *  \return Success: 0
     *  \return Fail: generally non-zero value; if > 0 a http code (404, 500, etc); -1 otherwise. Call getErrorString() and getErrorCode() for details.
     */
    int                 submit(const dvdparselst *pLstDvdParse);

    //! Read a xml file into memory.
    /*! Read a xml file into memory.
     *
     *  \return Success: 0
     *  \return Fail: generally non-zero value; if > 0 a http code (404, 500, etc); -1 otherwise. Call getErrorString() and getErrorCode() for details.
     */
    int                 read(dvdparselst *pLstDvdParse, const std::string & strInFile);

    //! Write a xml file to disk.
    /*! Write a xml file to disk (single DVD).
     *
     *  \return Success: 0
     *  \return Fail: generally non-zero value. Call getErrorString() and getErrorCode() for details.
     */
    int                 write(const dvdparse *pDvdParse, const std::string & strOutFile);

    //! Write a xml file to disk.
    /*! Write a xml file to disk (several DVDs).
     *
     *  \return Success: 0
     *  \return Fail: generally non-zero value. Call getErrorString() and getErrorCode() for details.
     */
    int                 write(const dvdparselst *pLstDvdParse, const std::string & strOutFile);

    //! If an error occurred, this string will return (English) details.
    /*!
     *  \return Error text.
     */
    const std::string & getErrorString() const;

    //! If an error occurred, this will return the code.
    /*!
     *  \return Error code.
     */
    DVDERRORCODE        getErrorCode() const;

    dvddatabase& operator= (dvddatabase const & source);

    //! Set the client name.
    /*! Set the client name.
     */
    void                setClientName(const std::string & strClientName);

    //! Get the client name.
    /*! Get the client name.
     *
     *  \return Client name.
     */
    const std::string & getClientName() const;

    //! Set the server url.
    /*! Set the server url.
     */
    void                setServerUrl(const std::string & strServerUrl);

    //! Get the server url.
    /*! Get the server url.
     *
     *  \return Server url.
     */
    const std::string & getServerUrl() const;

    //! Set the proxy name (and optionally port), e.g. proxy:3128
    void                setProxy(const std::string & strProxy);
    std::string         getProxy() const;

protected:
    void                setError(const std::string & strErrorString, DVDERRORCODE eErrorCode);
    void                setError(const xmldocparser & xmlDoc);
    void                setError(const xmldocbuilder & xmlDoc);
    void                setError(const http & httpServer);

protected:
    std::string         m_strErrorString;			//!< Last error as clear text (English)
    DVDERRORCODE        m_eErrorCode;               //!< Error code
    std::string         m_strServerUrl;             //!< Url of dvdetect server
    std::string         m_strClientName;            //!< Name of client
    std::string         m_strProxy;                 //!< Proxy server address/port

    // I/O functions
    void *              (*m_pOpenFile)(const char *, const char *, const char *);
    size_t              (*m_pReadFile)(void*, size_t, size_t, void*);
    int                 (*m_pWriteFile)(const void*, size_t, size_t, void*);
    int                 (*m_pCloseFile)(void *);
    int                 (*m_pStatFile)(const char *, LPDVDFILESTAT, const char *);
    int                 (*m_pFstatFile)(void*, LPDVDFILESTAT);
};

#endif // DVDDATABASE_H
