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

#define HTML_TNEWLINE           _T("\r\n")
#define HTML_NEWLINE            "\r\n"
#define HTML_HEADER_END         HTML_NEWLINE HTML_NEWLINE
#define HTML_LAST_MODIFIED      "Last-Modified: "
#define HTML_CONTENT_LENGTH     "Content-Length: "
#define HTML_SIZE(t)            (countof(t) - 1)

#define RECVBUFFSIZE            (1024*50)           // 50 kB

#define POLLDELAY               10                  // 10 ms
#define POLLTIMEOUT             (1000/POLLDELAY)    // 5000 ms

bool getLoggingEnabled(int) { return false; }

http::http()
    : m_nHeaderSize(0)
    , m_pContent(NULL)
    , m_qwContentSize(0)
    , m_TimeStamp(-1)
    , m_nResponse(0)
{
}

http::~http()
{
    deleteContent();
}

void http::allocContent(int64_t qwSize)
{
    deleteContent();

    if (qwSize)
    {
        m_qwContentSize = qwSize;
        m_pContent = new char[qwSize];
    }
}

void http::deleteContent()
{
    delete [] m_pContent;
    m_qwContentSize = 0;
    m_pContent = NULL;
}

void http::setProxy(const TSTRING & strProxy)
{
    m_strProxyServer = strProxy;
}

int http::splitUrl(TSTRING & strUrl, TSTRING & strHost, TSTRING & strUri, int & nPort)
{
    size_t nSlash = 0;
    size_t nDots = 0;

    if (!strUrl.substr(0, 7).compare(_T("http://")))
    {
        strUrl = strUrl.substr(7);
    }

    // get port address
    if ((nDots = strUrl.rfind(':')) != TSTRING::npos)
    {
        nPort = WTOI(strUrl.substr(nDots + 1).c_str());
    }
    else
    {
        nPort = 80;	// Use default
    }

    if ((nSlash = strUrl.find('/')) != TSTRING::npos)
    {
        if (nDots != TSTRING::npos)
        {
            strHost = strUrl.substr(0, nDots);
        }
        else
        {
            strHost = strUrl.substr(0, nSlash);
        }
        strUri = strUrl.substr(nSlash);
    }
    else
    {
        if (nDots != TSTRING::npos)
        {
            strHost = strUrl.substr(0, nDots);
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
    SOCKET                  connectSocket = INVALID_SOCKET;
    vector<INET_ADDRESS>    lstSocketAddr;
    TSTRING                 strFullUrl(strUrl);
    TSTRING                 strHost;
    TSTRING                 strUri;
    TSTRING                 strProxyUrl(m_strProxyServer);
    TSTRING                 strMode;
    int                     nPort = 80;
    int                     nResult = 0;
    TSTRING                 strSendBuf;
    bool                    bUseProxy = (strProxyUrl.length() != 0) ? true : false;
    bool                    bMultiIP = false;

    m_strHeaders.clear();
    m_strErrorString.clear();
    deleteContent();

    if (eMethod == GET && strData.size())
    {
        strFullUrl += _T("?");
        strFullUrl += strData;
    }

    if (getLoggingEnabled(1))
    {
        printf(_T("request Url: ") STRTOKEN, strFullUrl.c_str());
        if ((eMethod == POST || eMethod == POST_HEADERSONLY) && strData.size())
        {
            printf(_T("request data: ") STRTOKEN, strData.c_str());
        }
    }

    try
    {
        nResult = splitUrl(strFullUrl, strHost, strUri, nPort);
        if (nResult < 0)
        {
            throw nResult;
        }

        if (strProxyUrl.length())
        {
            TSTRING strDummy;

            nResult = splitUrl(strProxyUrl, strProxyUrl, strDummy, nPort);
            if (nResult < 0)
            {
                throw nResult;
            }
        }

        if (bUseProxy)
        {
            nResult = ::string2IP(strProxyUrl.c_str(), lstSocketAddr);
            strMode = "proxy";
        }
        else
        {
            nResult = ::string2IP(strHost.c_str(), lstSocketAddr);
            strMode = "server";
        }

        if (nResult != 0)
        {
            setError(_T("getaddrinfo"), nResult);
            throw -1;
        }

        bMultiIP = lstSocketAddr.size() > 1;

        if (bMultiIP)
        {
            strMode = "Trying " + strMode;
        }
        else
        {
            strMode = "Using " + strMode;
        }

        for (std::vector<INET_ADDRESS>::iterator it = lstSocketAddr.begin() ; it != lstSocketAddr.end(); ++it)
        {
            switch ((*it).sa_family)
            {
            case AF_INET:
            {
                sockaddr_in  *sockaddr_ipv4 = (sockaddr_in *) &((*it).addr);
                sockaddr_ipv4->sin_port = htons( nPort );
                if (getLoggingEnabled(1))
                {
                    printf(STRTOKEN, (strMode + _T(" IPv4 address ") + ::IP2String(&(*it))).c_str());
                }
                break;
            }
            case AF_INET6:
            {
                sockaddr_in6 *sockaddr_ipv6 = (sockaddr_in6 *) &((*it).addr);
                sockaddr_ipv6->sin6_port = htons( nPort );
                if (getLoggingEnabled(1))
                {
                    printf(STRTOKEN, (strMode + _T(" IPv6 address ") + ::IP2String(&(*it))).c_str());
                }
                break;
            }
            }

            connectSocket = ::socket((*it).sa_family, SOCK_STREAM, IPPROTO_TCP);
            if (connectSocket == (SOCKET)INVALID_SOCKET)
            {
                setError(_T("socket open failed"), 0);
                throw -1;
            }

            //----------------------
            // Connect to server.
            nResult = ::connect(connectSocket, (sockaddr*)&(*it).addr, (int)(*it).iAddrLen);

            if (nResult != SOCKET_ERROR)
            {
                break;
            }
        }

        if (nResult == SOCKET_ERROR)
        {
            setError(_T("connect faild"), 0);
            throw -1;
        }

#if 1
    	int iOptVal = 0;
    	socklen_t iOptLen = sizeof (int);

    	nResult = getsockopt(connectSocket, SOL_SOCKET, SO_KEEPALIVE, (char *) &iOptVal, &iOptLen);
        if (nResult == SOCKET_ERROR)
        {
            setError(_T("getsockopt for SO_KEEPALIVE failed"), 0);
    	}
//    	else
//    	{
//			cout << "SO_KEEPALIVE Value:" << iOptVal << endl;
//		}
#endif

  		BOOL bOptVal = FALSE;
  	  	int bOptLen = sizeof (BOOL);

        bOptVal = TRUE;

    	nResult = ::setsockopt(connectSocket, SOL_SOCKET, SO_KEEPALIVE, (char *) &bOptVal, bOptLen);
        if (nResult == SOCKET_ERROR)
        {
            setError(_T("setsockopt for SO_KEEPALIVE failed"), 0);
            throw -1;
        }


#if 1
iOptVal = 0;
    	nResult = getsockopt(connectSocket, SOL_SOCKET, SO_KEEPALIVE, (char *) &iOptVal, &iOptLen);
        if (nResult == SOCKET_ERROR)
        {
            setError(_T("getsockopt for SO_KEEPALIVE failed"), 0);
    	}
//    	else
//    	{
//			cout << "NEW SO_KEEPALIVE Value:" << iOptVal << endl;
//		}
#endif

        switch (eMethod)
        {
        case GET:
        case GET_HEADERSONLY:
        {
            if (strProxyUrl.length() == 0)
            {
                strSendBuf = _T("GET ");
                strSendBuf += strUri;
                strSendBuf += _T(" HTTP/1.0") HTML_TNEWLINE;
                strSendBuf += _T("Host: ");
                strSendBuf += strHost;
                strSendBuf += HTML_TNEWLINE;
                strSendBuf += "Proxy-Connection: Keep-Alive";
                strSendBuf += HTML_TNEWLINE;
            }
            else
            {
                strSendBuf = _T("GET http://");
                strSendBuf += strFullUrl;
                strSendBuf += _T(" HTTP/1.1") HTML_TNEWLINE;
            }

            strSendBuf += _T("Connection: close") HTML_TNEWLINE HTML_TNEWLINE;

            nResult = send( connectSocket, strSendBuf, 0 );
            if (nResult == SOCKET_ERROR)
            {
                throw -1;
            }

            if (getLoggingEnabled(1))
            {
                printf(_T("Bytes sent: %i"), nResult);
            }
        }
            break;
        case POST:
        case POST_HEADERSONLY:
        {
            if (strProxyUrl.length() == 0)
            {
                strSendBuf = _T("POST ");
                strSendBuf += strUri;
                strSendBuf += _T(" HTTP/1.1") HTML_TNEWLINE;
                strSendBuf += _T("Host: ");
                strSendBuf += strHost;
                strSendBuf += HTML_TNEWLINE;
                strSendBuf += "Proxy-Connection: Keep-Alive";
                strSendBuf += HTML_TNEWLINE;
            }
            else
            {
                strSendBuf = _T("POST http://");
                strSendBuf += strFullUrl;
                strSendBuf += _T(" HTTP/1.1") HTML_TNEWLINE;
            }

            if (strData.size())
            {
                strSendBuf += _T("Content-type: application/x-www-form-urlencoded") HTML_TNEWLINE;
                strSendBuf += _T("Content-length: ") + toString(strData.size());
                strSendBuf += HTML_TNEWLINE;
                strSendBuf += _T("Connection: close") HTML_TNEWLINE HTML_TNEWLINE;
                strSendBuf += strData;
                strSendBuf += HTML_TNEWLINE;
            }
            else
            {
                strSendBuf += _T("Connection: close") HTML_TNEWLINE HTML_TNEWLINE;
            }

            nResult = send( connectSocket, strSendBuf, 0 );
            if (nResult == SOCKET_ERROR)
            {
                throw -1;
            }

            if (getLoggingEnabled(1))
            {
                printf(_T("Bytes sent: %i"), nResult);
            }
        }
            break;
        }

        char *pBuffer = NULL;
        nResult = recv(connectSocket, &pBuffer, 0, eMethod);
        if (nResult == SOCKET_ERROR)
        {
            if (pBuffer != NULL)
            {
                free(pBuffer);
            }
            throw -1;
        }

        if (!nResult)
        {
            m_strErrorString = _T("recv: No data");
            throw -1;
        }

        if (!m_nHeaderSize)
        {
            m_strErrorString = _T("recv: Did not receive a content header");
            throw -1;
        }

        {
            int64_t qwContentSize = nResult - m_nHeaderSize;

            if (qwContentSize)
            {
                allocContent(qwContentSize);
                memcpy(m_pContent, pBuffer + m_nHeaderSize, qwContentSize);
            }
        }

        ::free(pBuffer);
    }
    catch (int nResponse)
    {
        m_nResponse = nResponse;
    }

    if (connectSocket != (SOCKET)INVALID_SOCKET)
    {
        ::closesocket(connectSocket);
    }

    if (!m_nResponse)
    {
        size_t iPos = 0;

        if ((iPos = m_strHeaders.find("HTTP/1.0")) != TSTRING::npos)
        {
            m_nResponse = ::atol(m_strHeaders.substr(9).c_str());
            setError(getHeaders());
        }
        else if ((iPos = m_strHeaders.find("HTTP/1.1")) != TSTRING::npos)
        {
            m_nResponse = ::atol(m_strHeaders.substr(9).c_str());
            setError(getHeaders());
        }
        else
        {
            setError(_T("HTTP ") + ::toString(m_nResponse));
        }
    }

    return m_nResponse;
}

int http::send(SOCKET s, const TSTRING & strBuffer, int flags)
{
    int nResult  = ::send(s, wstringToString(strBuffer).c_str(), (int)strBuffer.length(), flags);
    if (nResult == SOCKET_ERROR)
    {
        setError(_T("send"), 0);
    }
    return nResult;
}

int http::recv(SOCKET s, char ** ppBuffer, int flags, METHOD eMethod)
{
    struct pollfd ufds[1];
    int     nResult = 0;
    int     nRetBytes = 0;
//    int     nNoDataCount = 0;
    int64_t qwMaxSize = -1;
    bool    bHeadersOnly = (eMethod == GET_HEADERSONLY || eMethod == POST_HEADERSONLY) ? true : false;

    m_nHeaderSize = 0;

    memset(&ufds, 0, sizeof(ufds));

    ufds[0].fd = s;
    ufds[0].events = POLLIN; // check for just normal data

    // Receive until the peer closes the connection
    do
    {
        int n = sizeof(ufds) / sizeof(pollfd);
        int rv;

        rv = ::POLL(ufds, n, POLLDELAY);

        if (rv == -1)
        {
            // error occurred in poll()
            nRetBytes = SOCKET_ERROR;
            setError(_T("poll has failed"), 0);
            break;
        }
        else if (rv == 0)
        {
//            if (++nNoDataCount > POLLTIMEOUT)
//            {
//                // Timeout, leaving loop
//                nRetBytes = SOCKET_ERROR;
//                setError(_T("recv has timed out"));
//                break;
//            }

            if (qwMaxSize != -1 && nRetBytes >= qwMaxSize)
            {
                // If server has sent all data, close connection
                // In case of proxy, this should be done by the proxy, but this provides
                // an extra safety net to prevent lock-ups and time-outs.
                break;
            }

            // No data received yet, continueing
            nResult = 1;
        }
        else
        {
            if (ufds[0].revents & POLLIN)
            {
                char recvbuf[RECVBUFFSIZE];

                nResult = ::recv(s, recvbuf, countof(recvbuf), flags);

                if ( nResult > 0 )
                {
//                    nNoDataCount = 0;

                    if (getLoggingEnabled(1))
                    {
                        printf(_T("Bytes received: %i"), nResult);
                    }

                    *ppBuffer = (char*)::realloc(*ppBuffer, nRetBytes + nResult);
                    memcpy((*ppBuffer) + nRetBytes, recvbuf, nResult);
                    nRetBytes += nResult;

                    if (!m_nHeaderSize)
                    {
                        qwMaxSize = extractHeaders(*ppBuffer);
                    }

                    if (bHeadersOnly && m_nHeaderSize)
                    {
                        // Stop at header
                        nRetBytes = m_nHeaderSize;
                        m_qwContentSize = (qwMaxSize == -1) ? 0 : (qwMaxSize - m_nHeaderSize);
                        break;
                    }
                }
                else if ( nResult == 0 )
                {
                    // No (more) data
                    break;
                }
                else
                {
//        			if (WSAECONNRESET == WSAGetLastError())
//        			{
 //                   	break;
//					}

                    // Set error
                    nRetBytes = nResult;
                    setError(_T("recv has failed"), 0);
                    break;
                }
            }
            else
            {
                if (!nRetBytes)
                {
                    setError(_T("poll reported no data"));
                    nRetBytes = SOCKET_ERROR;
                    nResult = -1;
                }
                break;
            }
        }
    }
    while( nResult > 0 );

    return nRetBytes;
}

TSTRING http::getHeaders() const
{
    return stringToWString(m_strHeaders);
}

int http::getHeaderSize() const
{
    return m_nHeaderSize;
}

TSTRING http::getContent() const
{
    if (m_pContent == NULL || !m_qwContentSize)
    {
        return _T("");
    }

    std::string strContent;

    strContent.assign(m_pContent, getContentSize());    // Convert to text

    return stringToWString(strContent);
}

int64_t http::getContent(char *pContent, int64_t qwSize) const
{
    if (m_pContent == NULL || !m_qwContentSize)
    {
        return -1;
    }

    if (qwSize > m_qwContentSize)
    {
        qwSize = m_qwContentSize;
    }

    memcpy(pContent, m_pContent, qwSize);

    return qwSize;
}

int64_t http::getContentSize() const
{
    return m_qwContentSize;
}

time_t http::getTimeStamp() const
{
    return m_TimeStamp;
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

void http::setError(const TSTRING & strError)
{
    m_strErrorString = strError;
}

TSTRING http::getErrorString() const
{
    return m_strErrorString;
}

/*
    RFC 1123

    5. DATE AND TIME SPECIFICATION

    5.1. SYNTAX


    date-time   =  [ day "," ] date time     ; dd mm yy
                                             ;  hh:mm:ss zzz

    day         =  "Mon"  / "Tue" /  "Wed"  / "Thu"
             /  "Fri"  / "Sat" /  "Sun"

    date        =  1*2DIGIT month 2DIGIT     ; day month year
                                             ;  e.g. 20 Jun 82

    month       =  "Jan"  /  "Feb" /  "Mar"  /  "Apr"
             /  "May"  /  "Jun" /  "Jul"  /  "Aug"
             /  "Sep"  /  "Oct" /  "Nov"  /  "Dec"

    time        =  hour zone                 ; ANSI and Military

    hour        =  2DIGIT ":" 2DIGIT [":" 2DIGIT]
                                             ; 00:00:00 - 23:59:59

    zone        =  "UT"  / "GMT"                ; Universal Time
                                             ; North American : UT
             /  "EST" / "EDT"                ;  Eastern:  - 5/ - 4
             /  "CST" / "CDT"                ;  Central:  - 6/ - 5
             /  "MST" / "MDT"                ;  Mountain: - 7/ - 6
             /  "PST" / "PDT"                ;  Pacific:  - 8/ - 7
             /  1ALPHA                       ; Military: Z = UT;
                                             ;  A:-1; (J not used)
                                             ;  M:-12; N:+1; Y:+12
             / ( ("+" / "-") 4DIGIT )        ; Local differential
                                             ;  hours+min. (HHMM)
*/

#define ASCTIME_FORMAT  "%a %b %d %H:%M:%S %Y"
#define RFC_850_FORMAT  "%A, %d-%b-%y %H:%M:%S %Z"
#define RFC_1123_FORMAT "%a, %d %b %Y %H:%M:%S %Z"

time_t http::parseLastModified(const string &str) const
{
    char *cp = NULL;
    struct tm filetime;
    time_t TimeStamp = -1;

    memset(&filetime, 0, sizeof(filetime));

    setlocale(LC_TIME, "en_US");

    // Try all three forms:
    //
    // Sun, 06 Nov 1994 08:49:37 GMT  ; RFC 822, updated by RFC 1123
    // Sunday, 06-Nov-94 08:49:37 GMT ; RFC 850, obsoleted by RFC 1036
    // Sun Nov  6 08:49:37 1994       ; ANSI C's asctime() format

#ifdef _WIN32
    char *p = _strdup(str.c_str());
#else
    char *p = strdup(str.c_str());
#endif

    cp = strptime(p, RFC_1123_FORMAT, &filetime);
    if (cp == NULL)
    {
        cp = strptime(p, RFC_850_FORMAT, &filetime);
    }

    if (cp == NULL)
    {
        cp = strptime(p, ASCTIME_FORMAT, &filetime);
    }

    free(p);

    if (cp != NULL)
    {
        TimeStamp = getgmtime(&filetime);
    }

    setlocale(LC_TIME, "");

    return TimeStamp;
}

int64_t http::extractHeaders(const char *pBuffer)
{
    const char *ptr = strstr(pBuffer, HTML_HEADER_END);
    int64_t qwMaxSize = 0;

    if (ptr != NULL)
    {
        string str;

        m_nHeaderSize = (int)((ptr - pBuffer) + HTML_SIZE(HTML_HEADER_END)); // Cast OK, header will never be > 2GB

        m_strHeaders.assign(pBuffer, m_nHeaderSize);

        str = extractHeader(HTML_CONTENT_LENGTH);
        if (str.size())
        {
            char * endptr = NULL;
            qwMaxSize = strtoul(str.c_str(), &endptr, 0) + m_nHeaderSize;
        }

        str = extractHeader(HTML_LAST_MODIFIED);
        if (str.size())
        {
            m_TimeStamp = parseLastModified(str);
        }
        else
        {
            m_TimeStamp = -1;
        }
    }
    return qwMaxSize;
}

std::string http::extractHeader(const char *pszHeader) const
{
    size_t iPosStart = m_strHeaders.find(pszHeader);
    if (iPosStart != string::npos)
    {
        size_t iPosEnd = m_strHeaders.find(HTML_NEWLINE, iPosStart);

        iPosStart += strlen(pszHeader);
        if (iPosEnd != string::npos)
        {
            iPosEnd -= iPosStart;
        }

        return m_strHeaders.substr(iPosStart, iPosEnd);
    }
    return "";
}
