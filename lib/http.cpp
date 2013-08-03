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

/*! \file http.cpp
 *
 *  \brief HTTP web server access class implementation
 *
 * TODO:
 * - Enable chunked transfer coding (see http://en.wikipedia.org/wiki/Chunked_transfer_encoding)
 * - Use Content-Size header
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>

#include "localutils.h"
#include "http.h"

#include <stdio.h>

bool getLoggingEnabled(int) { return false; }

http::http()
    : m_nResponse(0)
{
}

http::~http()
{
}

void http::setProxy(const TSTRING & strProxy)
{
    m_strProxyServer = strProxy;
}

int http::splitUrl(TSTRING & strUrl, TSTRING & strHost, TSTRING & strUri, int & iPort)
{
    size_t iSlash = 0;
    size_t iDots = 0;

    if (!strUrl.substr(0, 7).compare(_T("http://")))
    {
        strUrl = strUrl.substr(7);
    }

    // get port address
    if ((iDots = strUrl.rfind(':')) != TSTRING::npos)
    {
        iPort = WTOI(strUrl.substr(iDots + 1).c_str());
    }
    else
    {
        iPort = 80;	// Use default
    }

    if ((iSlash = strUrl.find('/')) != TSTRING::npos)
    {
        if (iDots != TSTRING::npos)
        {
            strHost = strUrl.substr(0, iDots);
        }
        else
        {
            strHost = strUrl.substr(0, iSlash);
        }
        strUri = strUrl.substr(iSlash);
    }
    else
    {
        if (iDots != TSTRING::npos)
        {
            strHost = strUrl.substr(0, iDots);
        }
        else
        {
            strHost = strUrl;
        }
        strUri = '/';
    }

    return 0;
}

int http::request(METHOD eMethod, const TSTRING & strUrl, const TSTRING & strData)
{
    SOCKET          ConnectSocket = INVALID_SOCKET;
    INET_ADDRESS    SocketAddr;
    TSTRING         strFullUrl = strUrl;
    TSTRING         strHost;
    TSTRING         strUri;
    TSTRING         strProxyUrl = m_strProxyServer;
    int             iPort = 80;
    int             iResult = 0;
    TSTRING         strSendBuf;
    bool            bUseProxy = strProxyUrl.length() != 0;

    m_strContent.clear();
    m_strErrorString.clear();

    if (eMethod == GET && strData.size())
    {
        strFullUrl += _T("?");
        strFullUrl += strData;
    }

    if (getLoggingEnabled(1))
    {
        printf(_T("request Url: ") STRTOKEN, strFullUrl.c_str());
        if (eMethod == POST && strData.size())
        {
            printf(_T("request data: ") STRTOKEN, strData.c_str());
        }
    }

    try
    {
        iResult = splitUrl(strFullUrl, strHost, strUri, iPort);
        if (iResult < 0)
        {
            throw iResult;
        }

        if (strProxyUrl.length())
        {
            TSTRING strDummy;

            iResult = splitUrl(strProxyUrl, strProxyUrl, strDummy, iPort);
            if (iResult < 0)
            {
                throw iResult;
            }
        }

        if (bUseProxy)
        {
            iResult = ::string2IP(strProxyUrl.c_str(), &SocketAddr);
        }
        else
        {
            iResult = ::string2IP(strHost.c_str(), &SocketAddr);
        }

        if (iResult != 0)
        {
            setError(_T("getaddrinfo"), iResult);
            throw -1;
        }

        if (SocketAddr.sa_family != AF_INET && SocketAddr.sa_family != AF_INET6)
        {
            setError(_T("getaddrinfo: Unknown family"));
            throw -1;
        }

        switch (SocketAddr.sa_family)
        {
        case AF_INET:
        {
            sockaddr_in  *sockaddr_ipv4 = (sockaddr_in *) &(SocketAddr.addr);
            sockaddr_ipv4->sin_port = htons( iPort );
            if (getLoggingEnabled(0))
            {
                if (bUseProxy)
                {
                    printf(_T("Proxy IPv4 address ") STRTOKEN, ::IP2String(&SocketAddr).c_str());
                }
                else
                {
                    printf(_T("Server IPv4 address ") STRTOKEN, ::IP2String(&SocketAddr).c_str());
                }
            }
            break;
        }
        case AF_INET6:
        {
            sockaddr_in6 *sockaddr_ipv6 = (sockaddr_in6 *) &(SocketAddr.addr);
            sockaddr_ipv6->sin6_port = htons( iPort );
            if (getLoggingEnabled(0))
            {
                if (bUseProxy)
                {
                    printf(_T("Proxy IPv6 address ") STRTOKEN, ::IP2String(&SocketAddr).c_str());
                }
                else
                {
                    printf(_T("Server IPv6 address ") STRTOKEN, ::IP2String(&SocketAddr).c_str());
                }
            }
            break;
        }
        }

        ConnectSocket = ::socket(SocketAddr.sa_family, SOCK_STREAM, IPPROTO_TCP);
        if (ConnectSocket == INVALID_SOCKET)
        {
            setError(_T("socket"));
            throw -1;
        }

        //----------------------
        // Connect to server.
        iResult = ::connect(ConnectSocket, (sockaddr*)&SocketAddr.addr, (int)SocketAddr.iAddrLen);

        if ( iResult == SOCKET_ERROR)
        {
            setError(_T("connect"));
            throw -1;
        }

        switch (eMethod)
        {
        case GET:
        {
            if (strProxyUrl.length() == 0)
            {
                strSendBuf = _T("GET ");
                strSendBuf += strUri;
                strSendBuf += _T(" HTTP/1.0\n");
                strSendBuf += _T("Host: ");
                strSendBuf += strHost;
                strSendBuf += _T("\n\n");
            }
            else
            {
                strSendBuf = _T("GET http://");
                strSendBuf += strFullUrl;
                strSendBuf += _T(" HTTP/1.1\n\n");
            }

            iResult = send( ConnectSocket, strSendBuf, 0 );
            if (iResult == SOCKET_ERROR)
            {
                setError(_T("send"));
                throw -1;
            }

            if (getLoggingEnabled(1))
            {
                printf(_T("Bytes sent: %i"), iResult);
            }
        }
            break;
        case POST:
        {
            if (strProxyUrl.length() == 0)
            {
                strSendBuf = _T("POST ");
                strSendBuf += strUri;
                strSendBuf += _T(" HTTP/1.1\r\n");
                strSendBuf += _T("Host: ");
                strSendBuf += strHost;
                strSendBuf += _T("\r\n");
            }
            else
            {
                strSendBuf = _T("POST http://");
                strSendBuf += strFullUrl;
                strSendBuf += _T(" HTTP/1.1\r\n");
            }

            if (strData.size())
            {
                //	strSendBuf += _T("Referer: $referer\n");
                strSendBuf += _T("Content-type: application/x-www-form-urlencoded\r\n");
                strSendBuf += _T("Content-length: ") + toString(strData.size());
                strSendBuf += _T("\r\n");
                strSendBuf += _T("Connection: close\r\n\r\n");
                strSendBuf += strData;
                strSendBuf += _T("\r\n");
            }
            else
            {
                strSendBuf += _T("\r\n");
            }

            iResult = send( ConnectSocket, strSendBuf, 0 );
            if (iResult == SOCKET_ERROR)
            {
                setError(_T("send"));
                throw -1;
            }

            if (getLoggingEnabled(1))
            {
                printf(_T("Bytes sent: %i"), iResult);
            }
        }
            break;
        }

        iResult = recv( ConnectSocket, m_strContent, 0 );
        if (iResult == SOCKET_ERROR)
        {
            throw -1;
        }
    }
    catch (int nResponse)
    {
        m_nResponse = nResponse;
    }

    if (ConnectSocket != INVALID_SOCKET)
    {
        ::closesocket(ConnectSocket);
    }

    if (!m_nResponse)
    {
        TSTRING strHeaders = getHeaders();
        size_t iPos = 0;

        if ((iPos = strHeaders.find(_T("HTTP/1.0"))) != TSTRING::npos)
        {
            m_nResponse = ::WTOL(strHeaders.substr(9).c_str());
            setError(strHeaders);
        }
        else if ((iPos = strHeaders.find(_T("HTTP/1.1"))) != TSTRING::npos)
        {
            m_nResponse = ::WTOL(strHeaders.substr(9).c_str());
            setError(strHeaders);
        }
        else
        {
            m_nResponse = 400;
            setError("HTTP Error 400");
        }
    }

    return m_nResponse;
}

int http::send(SOCKET s, const TSTRING & strBuffer, int flags) const
{
    return ::send(s, wstringToString(strBuffer).c_str(), (int)strBuffer.length(), flags);
}

int http::recv(SOCKET s, TSTRING & strBuffer, int /* flags */)
{
    char recvbuf[1024*50];
    int recvbuflen = countof(recvbuf);
    int iResult = 0;
    int iRetBytes = 0;
    struct pollfd ufds[1];
    int rv;

    memset(&ufds, 0, sizeof(ufds));

    ufds[0].fd = s;
    ufds[0].events = POLLIN; // check for just normal data

    // Receive until the peer closes the connection
    strBuffer.clear();
    do
    {
        int n = sizeof(ufds) / sizeof(pollfd);
        rv = ::POLL(ufds, n, 10);

        if (rv == -1)
        {
            // error occurred in poll()
            iRetBytes = SOCKET_ERROR;
            setError(_T("poll has failed"));
            break;
        }
        else if (rv == 0)
        {
            //            if (iRetBytes)
            //            {
            //                // Data received, ending loop
            //                //// Timeout, leaving loop
            //                ////printf(_T("recv has timed out"));
            //                break;
            //            }
            // No data received yet, continueing
            iResult = 1;
        }
        else
        {
            if (ufds[0].revents & POLLIN)
            {
                iResult = ::recv(s, recvbuf, recvbuflen - 1, 0);

                if ( iResult > 0 )
                {
                    if (getLoggingEnabled(1))
                    {
                        printf(_T("Bytes received: %i"), iResult);
                    }

                    recvbuf[iResult] = '\0';
                    strBuffer += stringToWString(recvbuf);
                    iRetBytes += iResult;
                }
                else if ( iResult == 0 )
                {
                    // No (more) data
                    break;
                }
                else
                {
                    // Set error
                    iRetBytes = iResult;
                    printf(_T("recv has failed, res %i"), iResult);
                    break;
                }
            }
            else
            {
                if (!iRetBytes)
                {
                    setError(_T("poll reported no data"));
                    iRetBytes = SOCKET_ERROR;
                    iResult = -1;
                }
                break;
            }
        }
    }
    while( iResult > 0 );

    return iRetBytes;
}

TSTRING http::getHeaders() const
{
    size_t iPos = m_strContent.find(_T("\r\n\r\n"));
    if (iPos != TSTRING::npos)
    {
        return m_strContent.substr(0, iPos);
    }
    else
    {
        return m_strContent;
    }
}

TSTRING http::getContent() const
{
    size_t iPos = m_strContent.find(_T("\r\n\r\n"));
    if (iPos != TSTRING::npos)
    {
        return m_strContent.substr(iPos + 4);
    }
    else
    {
        return m_strContent;
    }
}

int http::getResponse() const
{
    return m_nResponse;
}

void http::setError(const TSTRING & strError, int nErrorCode)
{
#ifdef WIN32
    TCHAR szBuffer[1024] = _T("");

    if (!nErrorCode)
    {
        nErrorCode = WSAGetLastError();
    }

    if (!nErrorCode)
    {
        WSPRINTF(szBuffer, STRTOKEN, strError.c_str());
    }
    else
    {
        WSPRINTF(szBuffer, STRTOKEN _T(": %i"), strError.c_str(), nErrorCode);
    }

    m_strErrorString = szBuffer;
#else
    m_strErrorString = strError;
    m_strErrorString += _T(": ");
    if (!nErrorCode)
    {
        m_strErrorString += TSTRERROR(errno);
    }
    else
    {
        m_strErrorString += TSTRERROR(nErrorCode);
    }
#endif
}

TSTRING http::getErrorString() const
{
    return m_strErrorString;
}

