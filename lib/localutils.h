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

/*! \file localutils.h
 *
 * \brief Utility collection headers (private to library)
 */

#pragma once

#ifndef LOCALUTILS_H
#define LOCALUTILS_H

using namespace std;

#include "compat.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

    //! Split a string by a delimiter
    /*!
     * \param s TSTRING Source string
     * \param delim TCHAR Delimiter (e.g. ";")
     * \param elems vector<TSTRING> & Separated parts of the string
     * \return vector array of strings
     */
vector<TSTRING> &	split(const TSTRING & s, TCHAR delim, vector<TSTRING> & elems);

    //! Split a string by a delimiter
    /*!
     * \param s TSTRING Source string
     * \param delim TCHAR Delimiter (e.g. ";")
     * \return vector array of strings
     */
vector<TSTRING>		split(const TSTRING & s, TCHAR delim);

    //! Convert an IP address from string form to INET_ADDRESS
    /*!
     * \param strIP const TSTRING & IP address string
     * \param pSocketAddr INET_ADDRESS * IP address in binary format
     * \return Success: 0
     * \return Fail: nonzero error code, see man getaddrinfo()
     */
int					string2IP(const TSTRING & strIP, INET_ADDRESS *pSocketAddr);

    //! Convert binary IP address to string form
    /*!
     * \param pSocketAddr INET_ADDRESS * IP address in binary format
     * \return IP address string
     */
TSTRING				IP2String(const INET_ADDRESS *pSocketAddr);

    //! Get the directory application resides in
    /*!
     * Returns the directory application resides in. This is not the same
     * as the current path and may be different.
     *
     * \return Directory name
     */
TSTRING				getAppDir();

#ifdef _UNICODE
    // Prototype for conversion functions

    //! Convert standard string to wide string
    /*!
     * \param mbs const std::string& Standard (multibyte) string
     * \return Wide string
     */
std::wstring		stringToWString(const std::string& mbs);

    //! Convert wide string to standard string
    /*!
     * \param wcs const std::string& Wide string
     * \return Standard string
     */
std::string			wstringToString(const std::wstring& wcs);
#endif

    //! Pause execution
    /*!
     * \param seconds int Halt execution for number of seconds
     */
void				nap(int seconds);

    //! Remove whitespaces from right part of string
    /*!
     * \param source TSTRING &
     * \param t const TSTRING &
     */
void				trimRight(TSTRING &source, const TSTRING &t = _T(" "));

    //! Remove whitespaces from left part of string
    /*!
     * \param source TSTRING &
     * \param t const TSTRING &
     */
void				trimLeft(TSTRING &source, const TSTRING &t = _T(" "));

    //! Get error text for code
    /*!
     * \param error int Error code
     * \return Error description as text
     */
TSTRING				getError(int error);

    //! Decode uri
    /*!
     * Decodes an uri according to RFC1630, RFC1738, RFC2396
     *
     * \param sSrc const TSTRING &
     * \return Success:
     * \return Fail:
     */
TSTRING				uriDecode(const TSTRING & sSrc);

    //! Encode uri
    /*!
     * Encodes an uri according to RFC1630, RFC1738, RFC2396
     *
     * \param sSrc const TSTRING &
     * \return Success:
     * \return Fail:
     */
TSTRING				uriEncode(const TSTRING & sSrc);

    //! Convert big endian 16 bit integer to native format (for Intel: to little endian)
    /*!
     * \param dwData uint32_t
     * \return Success:
     * \return Fail:
     */
uint32_t            be2native(uint32_t dwData);

    //! Convert big endian 16 bit integer to native format (for Intel: to little endian)
    /*!
     * \param wData uint16_t
     * \return Success:
     * \return Fail:
     */
uint16_t            be2native(uint16_t wData);

    //! Convert byte to 16 bit integer (bytes can cause trible with modern compilers)
    /*!
     * \param bData uint8_t
     * \return Success:
     * \return Fail:
     */
uint16_t            getbyte(uint8_t bData);

    //! Convert DVD BCD coded time to integer
    /*!
     * \param ptr const uint8_t * BCD coded time
     * \return Decoded time as integer
     */
uint64_t            BCDtime(const uint8_t *ptr);

    //! Get frame from DVD BCD coded time
    /*!
     * \param ptr const uint8_t * BCD coded time
     * \return Frame rate (25 or 30)
     */
uint16_t            frameRate(const uint8_t* ptr);

#ifdef _WIN32
    //! Windows: Windoze does not support utf-8 file names. Using the short file path trick to open them anyway.
    //! other: dos nothing
    /*!
     * \param strFilePath const string &
     * \return Success: Short file path
     * \return Fail: Empty string. Call GetLastError() for details.
     */
std::string         getWin32ShortFilePath(const string & strFilePath);
#else
#define             getWin32ShortFilePath
#endif

    //! Get string
    /*!
     * Reads a not zero terminated string from input string up to the maximum size
     * of the input string.
     *
     * \param byDataIn const uint8_t & Input string
     * \return Converted string
     */
template <size_t size> std::string getString(const uint8_t (&byDataIn)[size])
{
    std::string buffer;
    char * p = new char[size + 1];

    memcpy(p, byDataIn, size);
    *(p + size) = '\0';

    // Clear if invalid
    if (*p == (char)0xFF)
    {
        memset(p, 0, size);
    }

    buffer = p;

    delete [] p;

    return buffer;
}

    //! Get string
    /*!
     * Reads a not zero terminated string from input string up to the maximum size
     * of the input string. If output string is too small, the string will be
     * truncated. A trailing "\0" will always be added.
     *
     * \param szDataOut char & Output string
     * \param byDataIn const uint8_t & Input string
     */
template <size_t sizeout, size_t sizein> void getString(char (&szDataOut)[sizeout], const uint8_t (&byDataIn)[sizein])
{
    int size = (sizein <= sizeout) ? sizein : sizeout;

    memcpy(szDataOut, byDataIn, size);
    szDataOut[sizeout - 1] = '\0';

    // Clear if invalid
    if (szDataOut[0] == (char)0xFF)
    {
        memset(szDataOut, 0, sizeof(szDataOut));
    }
}

    //! Get string
    /*!
     * Reads a not zero terminated string from input string. If output string is too
     * small, the string will be truncated. A trailing "\0" will always be added.
     *
     * \param szDataOut char & Output string
     * \param pbyDataIn const uint8_t* Input string
     */
template <size_t sizeout> void getString2(char (&szDataOut)[sizeout], const uint8_t* pbyDataIn)
{
    memcpy(szDataOut, pbyDataIn, sizeout);
    szDataOut[sizeout - 1] = '\0';

    // Clear if invalid
    if (szDataOut[0] == (char)0xFF)
    {
        memset(szDataOut, 0, sizeof(szDataOut));
    }
}

    //! Safely copy input string to output
    /*!
     * Reads a zero terminated string from input string. If output string is too
     * small, the string will be truncated. A trailing "\0" will always be added.
     *
     * \param szDataOut char & Output string
     * \param pszDataIn const char* Input string
     */
template <size_t sizeout> void safecpy(char (&szDataOut)[sizeout], const char* pszDataIn)
{
    szDataOut[0] = '\0';
    strncat(szDataOut, pszDataIn, sizeout);
    szDataOut[sizeout - 1] = '\0';
}

    //! Remove the filename from a path
    /*!
     * \param strPath std::string & Path
     */
void                removeFileSpec(std::string & strPath);

    //! Adds a path separator at the end of the path if missing
    /*!
     * \param strPath std::string & Path
     */
void                addSeparator(std::string & strPath);

    //! Get a DVD file name
    /*!
     * \param eFileType DVDFILETYPE File type, e.g. VIDEO_TS.IFO
     * \param wTitleSetNo uint16_t For VTS_* files, number of title set
     * \param wVtsNo  uint16_t For VTS_* files, number of IF or VOB
     * \return File name
     */
std::string         getDvdFileName(DVDFILETYPE eFileType, uint16_t wTitleSetNo = 0, uint16_t wVtsNo = 0);

//! Convert (preferrably a numeric value) to std::string
/*!
 * \param i T Value to convert
 * \return String representation
 */
template<class T> std::string toString(T i)
{
    std::stringstream ss;
    std::string s;
    ss << i;
    s = ss.str();

    return s;
}

//! Convert numeric value into hexadecimal represented std::string
/*!
 * \param i T Value to convert
 * \return String representation
 */
template<class T> std::string toHexString(T i)
{
    std::stringstream ss;
    std::string s;
    ss << hex << std::setw(sizeof(i)) << std::setfill('0') << (uint64_t)i;
    s = ss.str();

    return s;
}

#endif // LOCALUTILS_H
