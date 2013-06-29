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

/*! \file localutils.cpp
 *
 *  \brief Utility collection (private to library)
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>

#include "localutils.h"

#include <iostream>
#include <limits.h>
#include <stdio.h>

#ifdef _UNICODE
#pragma warning (disable : 4244)
#endif

vector<TSTRING> & split(const TSTRING & s, TCHAR delim, vector<TSTRING> & elems)
{
    TSTRINGSTREAM ss(s);
    TSTRING item;

    while (getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

vector<TSTRING> split(const TSTRING & s, TCHAR delim)
{
    vector<TSTRING> elems;
    return split(s, delim, elems);
}

int string2IP(const TSTRING & strIP, INET_ADDRESS *pSocketAddr)
{
    TADDRINFO *pResult = NULL;
    TADDRINFO hints;
    int iResult = 0;

    memset(pSocketAddr, 0, sizeof(INET_ADDRESS));
    memset(&hints, 0, sizeof(hints));

    //hints.ai_flags = AI_NUMERICHOST;
    //hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

#ifdef _WIN32
    iResult = ::TGETADDRINFO(strIP.c_str(), NULL, &hints, &pResult);
#else
    iResult = ::TGETADDRINFO(wstringToString(strIP).c_str(), NULL, &hints, &pResult);
#endif

    if (iResult == 0 && pResult != NULL)
    {
        pSocketAddr->iAddrLen = pResult->ai_addrlen;
        memcpy(&pSocketAddr->addr, pResult->ai_addr, pSocketAddr->iAddrLen);
        pSocketAddr->sa_family = pResult->ai_addr->sa_family;
    }

    if (pResult != NULL)
    {
        ::TFREEADDRINFO(pResult);
    }

    return iResult;
}

TSTRING	IP2String(const INET_ADDRESS *pSocketAddr)
{
#ifdef WIN32
    TCHAR szBuffer[INET6_ADDRSTRLEN] = _T("");
    DWORD dwSize = countof(szBuffer);

    if (!::WSAAddressToString((LPSOCKADDR)&pSocketAddr->addr, (DWORD)pSocketAddr->iAddrLen, NULL, szBuffer, &dwSize))
    {
        return szBuffer;
    }
    else
    {
        return _T("");
    }
#else
    char szBuffer[INET6_ADDRSTRLEN] = "";
    switch(pSocketAddr->sa_family)
    {
    case AF_INET:
        inet_ntop(AF_INET, &(((sockaddr_in *)&pSocketAddr->addr)->sin_addr), szBuffer, countof(szBuffer));
        break;
#ifdef AF_INET6
    case AF_INET6:
        inet_ntop(AF_INET6, &(((sockaddr_in6 *)&pSocketAddr->addr)->sin6_addr), szBuffer, countof(szBuffer));
        break;
#endif
    }

    return stringToWString(szBuffer);
#endif
}

#if !defined(WIN32)
#if defined(__FreeBSD__)
#define READLINKPATH	"/proc/curproc/file"
#else
#define READLINKPATH	"/proc/self/exe"
#endif
#endif

TSTRING	getAppDir()
{
#ifdef WIN32
    TCHAR pResult[MAX_PATH] = _T("");
    return TSTRING( pResult, GetCurrentDirectory(MAX_PATH, pResult));
#else
    char pResult[PATH_MAX] = "";
    ssize_t count = readlink(READLINKPATH, pResult, PATH_MAX);
    TSTRING strAppDir = stringToWString(string(pResult, (count > 0) ? count : 0));
    size_t nPos = strAppDir.rfind('/');
    if (nPos != TSTRING::npos)
    {
        return strAppDir.substr(0, nPos);
    }
    else
    {
        return strAppDir;
    }
#endif
}

#ifdef _UNICODE

std::wstring stringToWString(const std::string& mbs)
{
    std::vector<wchar_t> wcs(mbs.length());
    mbstowcs(&wcs[0], mbs.c_str(), mbs.length());
    return std::wstring(wcs.begin(), wcs.end());
}

std::string wstringToString(const std::wstring& wcs)
{
    std::vector<char> mbs(wcs.length());
    wcstombs(&mbs[0], wcs.c_str(), wcs.length());
    return std::string(mbs.begin(), mbs.end());
}

#endif

void nap(int seconds)
{
#ifdef WIN32
    for (int n = 0; n < seconds * 4; n++)
    {
        Sleep(1000 / 4);
    }
#else
    for (int n = 0; n < seconds; n++)
    {
        sleep(1);
    }
#endif
}

void trimRight(TSTRING &source, const TSTRING &t)
{
    source.erase(source.find_last_not_of(t)+1);
}

void trimLeft(TSTRING &source, const TSTRING &t)
{
    source.erase(0, source.find_first_not_of(t));
}

TSTRING getError(int error)
{
    return stringToWString(strerror(error));
}

// Uri encode and decode.
// RFC1630, RFC1738, RFC2396

static const char HEX2DEC[256] =
{
    /*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
    /* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,

    /* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

    /* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

    /* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
};

std::string uriDecode(const std::string & sSrc)
{
    // Note from RFC1630:  "Sequences which start with a percent sign
    // but are not followed by two hexadecimal characters (0-9, A-F) are reserved
    // for future extension"

    const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
    const size_t SRC_LEN = sSrc.length();
    const unsigned char * const SRC_END = pSrc + SRC_LEN;
    const unsigned char * const SRC_LAST_DEC = SRC_END - 2;   // last decodable '%'

    char * const pStart = new char[SRC_LEN];
    char * pEnd = pStart;

    while (pSrc < SRC_LAST_DEC)
    {
        if (*pSrc == '%')
        {
            char dec1, dec2;
            if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
                    && -1 != (dec2 = HEX2DEC[*(pSrc + 2)]))
            {
                *pEnd++ = (dec1 << 4) + dec2;
                pSrc += 3;
                continue;
            }
        }

        *pEnd++ = *pSrc++;
    }

    // the last 2- chars
    while (pSrc < SRC_END)
    {
        *pEnd++ = *pSrc++;
    }

    std::string sResult(pStart, pEnd);
    delete [] pStart;
    return sResult;
}

// Only alphanum is safe.
static const char SAFE[256] =
{
    /*      0 1 2 3  4 5 6 7  8 9 A B  C D E F */
    /* 0 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* 1 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* 2 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1,
    /* 3 */ 1,1,1,1, 1,1,1,1, 1,1,0,0, 0,0,0,0,

    /* 4 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
    /* 5 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,
    /* 6 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
    /* 7 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,

    /* 8 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* 9 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* A */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* B */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

    /* C */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* D */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* E */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* F */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
};

std::string uriEncode(const std::string & sSrc)
{
    const char DEC2HEX[16 + 1] = "0123456789ABCDEF";
    const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
    const size_t SRC_LEN = sSrc.length();
    unsigned char * const pStart = new unsigned char[SRC_LEN * 3];
    unsigned char * pEnd = pStart;
    const unsigned char * const SRC_END = pSrc + SRC_LEN;

    for (; pSrc < SRC_END; ++pSrc)
    {
        if (SAFE[*pSrc])
        {
            *pEnd++ = *pSrc;
        }
        else
        {
            // escape this char
            *pEnd++ = '%';
            *pEnd++ = DEC2HEX[*pSrc >> 4];
            *pEnd++ = DEC2HEX[*pSrc & 0x0F];
        }
    }

    std::string sResult((char *)pStart, (char *)pEnd);
    delete [] pStart;
    return sResult;
}

#ifdef _UNICODE

TSTRING	uriDecode(const TSTRING & sSrc)
{
    return stringToWString(uriDecode(wstringToString(sSrc)));
}

TSTRING	uriEncode(const TSTRING & sSrc)
{
    return stringToWString(uriEncode(wstringToString(sSrc)));
}

#endif

#ifdef WORDS_BIGENDIAN

uint32_t be2native(uint32_t dwData)
{
    return dwData;
}

uint16_t be2native(uint16_t wData)
{
    return wData;
}

uint32_t native2le(uint32_t dwData)
{
    const uint8_t * pData = (const uint8_t *)&dwData;

    return (pData[3] & 0xFF) | ((pData[2] & 0xFF) << 8) | ((pData[1] & 0xFF) << 16) | ((pData[0] & 0xFF) << 24);
}

uint16_t native2le(uint16_t wData)
{
    const uint8_t * pData = (const uint8_t *)&wData;

    return (pData[1] & 0xFF) | ((pData[0] & 0xFF) << 8);
}
#else
uint32_t be2native(uint32_t dwData)
{
    const uint8_t * pData = (const uint8_t *)&dwData;

    return (pData[3] & 0xFF) | ((pData[2] & 0xFF) << 8) | ((pData[1] & 0xFF) << 16) | ((pData[0] & 0xFF) << 24);
}

uint16_t be2native(uint16_t wData)
{
    const uint8_t * pData = (const uint8_t *)&wData;

    return (pData[1] & 0xFF) | ((pData[0] & 0xFF) << 8);
}

uint32_t native2le(uint32_t dwData)
{
    return dwData;
}

uint16_t native2le(uint16_t wData)
{
    return wData;
}

#endif

uint16_t getbyte(uint8_t bData)
{
    return (bData & 0xFF);
}

uint64_t BCDtime(const uint8_t * ptr)
{
    const uint8_t * pTime = ptr;
    uint8_t         time[4];
    int             nFrameRate = 30;    // assume 30fps

    time[0] = pTime[0];                 // Hour
    time[1] = pTime[1];                 // Minute
    time[2] = pTime[2];                 // Second
    time[3] = pTime[3] & 0x3F;          // Number of frame

    switch (pTime[3] >> 6)
    {
    case 1: // PAL
    {
        nFrameRate = 25; // fps
        break;
    }
    case 3: // NTSC
    {
        nFrameRate = 30; // fps
        break;
    }
    default: //Invalid code
    {
        break;
    }
    }

    // convert bcd (two digits) to binary
    for (int j = 0; j < 4; j++)
    {
        time[j] = ((time[j] & 0xf0) >> 4) * 10 + (time[j] & 0x0f);
    }

    return (1000 * ((int)time[0] * 3600 + (int)time[1] * 60 + time[2]) + (1000 * (int)time[3]) / nFrameRate);
}

uint16_t frameRate(const uint8_t * ptr)
{
    uint16_t wFrameRate = 0;
    // 11 = 30 fps, 10 = illegal, 01 = 25 fps, 00 = illegal
    unsigned fps = ((ptr[3] & 0xC0) >> 6) & 0x03;
    switch (fps)
    {
    case 3:
        wFrameRate = 30;
        break;
    case 1:
        wFrameRate = 25;
        break;
    default:
        wFrameRate = 0;
        break;
    }

    return wFrameRate;
}

#ifdef _WIN32
// Windoze does not support utf-8 file names. Using the short file path trick to open them anyway.

std::string getWin32ShortFilePath(const string& strFilePath)
{
    wchar_t szLongName[MAX_PATH];
    wchar_t szShortName[MAX_PATH];
    char szShortName2[MAX_PATH];

    int result1 = ::MultiByteToWideChar(
                CP_UTF8,                        // convert from UTF-8
                MB_ERR_INVALID_CHARS,           // error on invalid chars
                strFilePath.c_str(),            // source UTF-8 string
                -1,                             // automatic length
                szLongName,                     // destination buffer
                MAX_PATH                        // size of destination buffer, in wchar_ts
                );

    if (!result1)
    {
        return "";
    }

    DWORD result2 = ::GetShortPathNameW(
                szLongName,
                szShortName,
                MAX_PATH
                );

    //DWORD dwError =  GetLastError();

    if (!result2)
    {
        return "";
    }

    //
    // WC_ERR_INVALID_CHARS flag is set to fail if invalid input character
    // is encountered.
    // This flag is supported on Windows Vista and later.
    // Don't use it on Windows XP and previous.
    //

#if (WINVER >= 0x0600)
    DWORD dwConversionFlags = WC_ERR_INVALID_CHARS;
#else
    DWORD dwConversionFlags = 0;
#endif

    int result3 = ::WideCharToMultiByte(
                CP_UTF8,                    // convert to UTF-8
                dwConversionFlags,          // specify conversion behavior
                szShortName,                // source UTF-16 string
                -1,                         // automatic length
                szShortName2,               // destination buffer
                MAX_PATH,                   // destination buffer size, in bytes
                NULL, NULL                  // unused
                );

    if (!result3)
    {
        return "";
    }

    return szShortName2;
}
#endif

void removeFileSpec(std::string & strPath)
{
    std::size_t found = std::string::npos;

    found = strPath.rfind('/');

    if (found != std::string::npos)
    {
        strPath = strPath.substr(0, found + 1);
        return;
    }

    found = strPath.rfind('\\');

    if (found != std::string::npos)
    {
        strPath = strPath.substr(0, found + 1);
    }
}

void addSeparator(std::string & strPath)
{
    size_t nSize = strPath.size();

    if (nSize)
    {
        char ch = strPath.at(nSize - 1);

        if (ch != '\\' && ch != '/')
        {
            strPath.push_back('/');
        }
    }
    //    else
    //    {
    //        strPath = "/";
    //    }
}

std::string getDvdFileName(DVDFILETYPE eFileType, uint16_t wTitleSetNo, uint16_t wVtsNo)
{
    stringstream strFileName;

    switch (eFileType)
    {
    case DVDFILETYPE_VMG_IFO:   // VIDEO_TS.IFO
    case DVDFILETYPE_VMG_BUP:   // VIDEO_TS.BUP
    case DVDFILETYPE_VMG_VOB:   // VIDEO_TS.VOB
        strFileName << "VIDEO_TS." << dvdGetExtension(eFileType);
        break;
    case DVDFILETYPE_VTS_IFO:   // VTS_##_0.IFO
    case DVDFILETYPE_VTS_BUP:   // VTS_##_0.BUP
    case DVDFILETYPE_MENU_VOB:  // VTS_##_0.VOB
    case DVDFILETYPE_VTS_VOB:   // VTS_##_1.VOB to VTS_##_9.VOB
        strFileName << "VTS_" << setfill('0') << setw(2) << wTitleSetNo << "_" << wVtsNo << "." << dvdGetExtension(eFileType);
        break;
    default:
        break;
    }

    return strFileName.str();
}
