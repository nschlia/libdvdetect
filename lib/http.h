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

/** \file http.h
 *
 *  \brief HTTP web server access class
 */

#pragma once

#ifndef HTTP_H
#define HTTP_H

using namespace std;

#include <iostream>

#include "compat.h"

/*!
 *  @brief THIS IS A CLASS...
 *
 * SOME DETAILS ABOUT THE CLASS...
 */

class DLL_LOCAL http
{
public:
    typedef enum _tagMETHOD
    {
        GET,
        POST
    } METHOD;
public:
    http();
    virtual ~http();

/*!
 *  @brief A member function.
 *
 * More details...
 *
 *  @param strProxy TSTRING
 *  @exception std::out_of_range parameter is out of range.
 *  @return error.
 */

    void		setProxy(const TSTRING & strProxy);

    int         request(METHOD eMethod, const TSTRING & strUrl, const TSTRING & strData = _T(""));

    TSTRING		getHeaders() const;
    TSTRING		getContent() const;
    int         getResponse() const;
    TSTRING		getError() const;

protected:
    static int	splitUrl(TSTRING & strUrl, TSTRING & strHost, TSTRING & strUri, int & iPort);
    void		setError(const TSTRING & strError);

    int         send(SOCKET s, const TSTRING & strBuffer, int flags) const;
    int         recv(SOCKET s, TSTRING & strBuffer, int flags) const;

protected:
    TSTRING		m_strProxyServer;
    TSTRING		m_strContent;
    int         m_nResponse;
    /*! @brief Error code */
    TSTRING		m_strErrorString;
};

#endif // HTTP_H
