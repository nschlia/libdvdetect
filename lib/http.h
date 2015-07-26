/*
  dvdetect DVD detection, analysis & DVDETECT lookup library

  Copyright (C) 2013-2015 Norbert Schlia <nschlia@dvdetect.de>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU LESSER GENERAL PUBLIC LICENSE for more details.

  You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*! \file http.h
 *
 *  \brief HTTP web server access class declaration
 */

#pragma once

#ifndef HTTP_H

#define HTTP_H


using namespace std;

#include <iostream>

#include "compat.h"

/*!
 *  \brief http class
 *
 * HTTP web server access class
 */
class DLL_LOCAL http
{
public:
    //! Http method to be used.
    typedef enum
    {
        GET,                            //!< http get
        POST,                           //!< http post
        GET_HEADERSONLY,                //!< http get - retrieve headers only
        POST_HEADERSONLY                //!< http post - retrieve headers only
    } METHOD;
public:
    //! Constructor.
    /*!
     *  Construct a http element.
     */
    explicit http();
    //! Destructor.
    /*!
     *  Destruct a http element.
     */
    virtual ~http();

    /*!
     *  \brief Set proxy name
     *
     *  \param strProxy TSTRING & DNS name or IP of proxy and optional port (e.g. proxy:3128)
     */
    void		setProxy(const TSTRING & strProxy);

    /*!
     *  \brief Send request to server
     *
     * Send a request to the server with optional data.
     *
     *  \param eMethod METHOD Request method (post or get)
     *  \param strUrl TSTRING & Url to send request to
     *  \param strData TSTRING & Data to send
     *  \return http result code, e.g, 404 not found or 200 OK.
     */
    int         request(METHOD eMethod, const TSTRING & strUrl, const TSTRING & strData = _T(""));

    /*!
     *  \brief Get the http headers
     *
     * Gets the http headers returned by the server.
     *
     *  \return Header
     */
    TSTRING		getHeaders() const;

    /*!
     *  \brief Get the http content
     *
     * Get the http content (aka the web page) without any headers
     *
     *  \return Content
     */
    TSTRING		getContent() const;

    /*!
     *  \brief Get the header size
     *
     * Get the header size
     *
     *  \return Header size
     */
    int         getHeaderSize() const;

    /*!
     *  \brief Get the http content
     *
     * Get the http content without any headers (normally used for binary data)
     *
     *  \return Content
     */
    int64_t     getContent(char *pContent, int64_t qwSize) const;

    /*!
     *  \brief Get the content size
     *
     * Get the content size without any headers
     *
     *  \return Content size
     */
    int64_t     getContentSize() const;

    /*!
     *  \brief Get time stamp of the content
     *
     * Get time stamp of the content
     *
     *  \return Unix time stamp or -1 if unknown/not supported
     */
    time_t      getTimeStamp() const;

    /*!
     *  \brief Get the http response
     *
     * Get the http response code, e.g. 404 (not found), 200 (OK) etc.
     *
     *  \return Response code
     */
    int         getResponse() const;

    /*!
     *  \brief Return the last error
     *
     * If an error occurred, returns the last error in text form.
     *
     *  \return Error text
     */
    TSTRING		getErrorString() const;

protected:

    /*!
     *  \brief Split an url
     *
     * Splits url into separate parts: hortname, uri and port:
     *
     * http://www.dvdetect.de/index.php:8080 ->
     *
     *  hostname = www.dvdetect.de
     *
     *  uri = index.php
     *
     *  port = 8080
     *
     * Port part can be omitted and will be set to default (80).
     *
     *  \return Success: 0
     *  \return Fail: nonzero
     *
     */
    static int	splitUrl(TSTRING & strUrl, TSTRING & strHost, TSTRING & strUri, int & iPort);

    /*!
     *  \brief Set the error text
     *
     *  \param strError TSTRING & Error text
     *  \param nErrorCode int Error code (see "man errno")
     */
    void		setError(const TSTRING & strError, int nErrorCode);

    /*!
     *  \brief Set the error text
     *
     *  \param strError TSTRING & Error text
     */
    void		setError(const TSTRING & strError);

    /*!
     *  \brief Send data via tcp
     *
     * Encapsulates the low level send() function
     *
     *  \param s SOCKET Connected socket to send data on
     *  \param strBuffer const TSTRING & Data to sent
     *  \param flags int Flags (see "man send" for details)
     *  \return Success: Number of bytes sent
     *  \return Fail: -1
     */
    int         send(SOCKET s, const TSTRING & strBuffer, int flags);

    /*!
     *  \brief Receive data via tcp
     *
     * Encapsulates the low level recv() function.
     * If the block is fragmented, the function waits until there is actually
     * no more data to receive (that means, it is almost guaranteed that the
     * complete web page transmitted will be returned).
     * ppBuffer is allocated and must be freed with "free" if not NULL.
     *
     *  \param s SOCKET Connected socket to send data on
     *  \param ppBuffer char ** Buffer to store received data in
     *  \param flags int Flags (see "man recv" for details)
     *  \param eMethod METHOD Request method: POST, GET or POST/GET without data
     *  \return Number of bytes received
     */
    int         recv(SOCKET s, char ** ppBuffer, int flags, METHOD eMethod);

    /*!
     *  \brief Allocate memory for content
     *
     * Allocates memory for the content.
     *
     *  \param qwSize int64_t Size in bytes of the content
     */
    void        allocContent(int64_t qwSize);

    /*!
     *  \brief Free memory allocated for content
     *
     * Free memory allocated for the content.
     */
    void        deleteContent();

    // DOCUMENTATION ???
    int64_t     extractHeaders(const char* pBuffer);
    std::string extractHeader(const char *pszHeader) const;
    time_t      parseLastModified(const std::string &str) const;

protected:
    TSTRING		m_strProxyServer;       //!< Proxy server name
    std::string m_strHeaders;           //!< Headers received from server
    int         m_nHeaderSize;          //!< Size of received headers
    char *      m_pContent;             //!< Content received from server
    int64_t     m_qwContentSize;        //!< Size of received content
    time_t      m_TimeStamp;            //!< Time stamp of content (if supported by server)
    int         m_nResponse;            //!< http result code, e.g, 404 not found or 200 OK.
    TSTRING		m_strErrorString;       //!< Error code
};

#endif // HTTP_H
