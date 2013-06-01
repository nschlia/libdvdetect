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

/** \file localutils.h
 *
 *  \brief Utility collection headers (private to library)
 */

#pragma once

#ifndef UTILS_H
#define UTILS_H

using namespace std;

#include "compat.h"

#include <iostream>
#include <vector>

vector<TSTRING> &	split(const TSTRING & s, TCHAR delim, vector<TSTRING> & elems);
vector<TSTRING>		split(const TSTRING & s, TCHAR delim);

int					string2IP(const TSTRING & strIP, INET_ADDRESS *pSocketAddr);
TSTRING				IP2String(const INET_ADDRESS *pSocketAddr);

TSTRING				getAppDir();

#ifdef _UNICODE
// Prototype for conversion functions
std::wstring		stringToWString(const std::string& mbs);
std::string			wstringToString(const std::wstring& wcs);
#endif

void				nap(int seconds);

void				trimRight(TSTRING &source, const TSTRING &t = _T(" "));
void				trimLeft(TSTRING &source, const TSTRING &t = _T(" "));

TSTRING				getError(int error);

TSTRING				uriDecode(const TSTRING & sSrc);
TSTRING				uriEncode(const TSTRING & sSrc);


uint32_t            be2native(uint32_t dwData);
uint16_t            be2native(uint16_t wData);
uint16_t            getbyte(uint8_t bData);
uint64_t            BCDtime(const uint8_t *ptr);
uint16_t            frameRate(const uint8_t* ptr);

#ifdef _WIN32
// Windoze does not support utf-8 file names. Using the short file path trick to open them anyway.
std::string         getWin32ShortFilePath(const string & strFilePath);
#else
#define             getWin32ShortFilePath
#endif

const uint8_t *readData(const string & strFilePath, string & strErrorString, time_t &ftime);

template <size_t size> std::string getString(const uint8_t (&byDataIn)[size])
{
    char * p = new char[size + 1];

    memcpy(p, byDataIn, size);
    *(p + size) = '\0';

    return p;
}

template <size_t sizeout, size_t sizein> void getString(char (&szDataOut)[sizeout], const uint8_t (&byDataIn)[sizein])
{
    int size = (sizein <= sizeout) ? sizein : sizeout;

    memcpy(szDataOut, byDataIn, size);
    szDataOut[sizeout - 1] = '\0';
}

template <size_t sizeout> void getString2(char (&szDataOut)[sizeout], const uint8_t* pbyDataIn)
{
    memcpy(szDataOut, pbyDataIn, sizeout);
    szDataOut[sizeout - 1] = '\0';
}

template <size_t sizeout> void safecpy(char (&szDataOut)[sizeout], const char* pszDataIn)
{
    szDataOut[0] = '\0';
    strncat(szDataOut, pszDataIn, sizeout);
    szDataOut[sizeout - 1] = '\0';
}

void                addSeparator(std::string & strPath);

std::string         getDvdFileName(DVDFILETYPE eFileType, uint16_t wTitleSetNo = 0, uint16_t wVtsNo = 0);

#endif // UTILS_H

