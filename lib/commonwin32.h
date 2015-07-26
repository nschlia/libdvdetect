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

/*! \file commonwin32.h
 *
*   \brief Compatibility layer (windoze MSVC) */

#pragma once

#ifndef COMMONWIN32_H

#define COMMONWIN32_H

#ifndef WIN32
#error "Include this file for windows only"
#endif

// ***************** Special windoze stuff goes here *****************

#if defined(_UNICODE)
#define USE_WCHAR_T
#endif

#define _CRT_SECURE_NO_WARNINGS

// Including SDKDDKVer.h defines the highest available Windows _PLATFORM.

// If you wish to build your application for a previous Windows _PLATFORM, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the _PLATFORM you wish to support before including SDKDDKVer.h.

#include <SDKDDKVer.h>

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <Wspiapi.h>

#ifndef USE_WCHAR_T
#define TSTRING						string
#define TOFSTREAM					ofstream
#define TIFSTREAM					ifstream
#define TSTRINGSTREAM				stringstream
#define TCHAR						char
#define TCOUT						cout
#define TCERR						cerr
#define _T(x)						x
#define TSTRERROR					strerror
#define TUNLINK						_unlink
#define TGETADDRINFO				getaddrinfo
#define TADDRINFO					addrinfo
#define TFREEADDRINFO				freeaddrinfo
#define TASCTIME					asctime
#define	WVSPRINTF					vsprintf_s
#define WTOI						atoi
#define WTOL						atol
#define	WSPRINTF					sprintf_s
#define STRTOKEN					"%s"
#define TSTRSTR                     strstr
#define TSTRLEN                     strlen
#define TSTRTOUL                    strtoul
#else
#define TSTRING						wstring
#define TOFSTREAM					wofstream
#define TIFSTREAM					wifstream
#define TSTRINGSTREAM				wstringstream
#define TCHAR						wchar_t
#define TCOUT						wcout
#define TCERR						wcerr
#define _T(x)						L ## x
#define TSTRERROR					_wcserror
#define TUNLINK						_wunlink
#define TGETADDRINFO				GetAddrInfoW
#define TADDRINFO					ADDRINFOW
#define TFREEADDRINFO				FreeAddrInfoW
#define TASCTIME					_wasctime
#define	WVSPRINTF					vswprintf_s
#define WTOI						_wtoi
#define WTOL						_wtol
#define	WSPRINTF					swprintf_s
#define STRTOKEN					L"%s"
#define TSTRSTR                     _wcsstr
#define TSTRLEN                     _wcslen
#define TSTRTOUL                    _wcstoul
#endif

#define	POLL						WSAPoll

#if (_WIN32_WINNT < 0x0600)
typedef USHORT ADDRESS_FAMILY;
#endif

// Architecture

#if defined(_M_X64)

// Detected AMD 64 bit
#define ARCH						_T("amd64")
#define PLATFORM					_T("windows (") ARCH _T(")")

#elif defined(_M_IA64)

// Detected Intel 64 bit
#define ARCH						_T("IA64")
#define PLATFORM					_T("windows (") ARCH _T(")")

#elif defined(_M_IX86)

// Unknown
#define ARCH						_T("x86")
#define PLATFORM					_T("windows (") ARCH _T(")")

#else

// Unknown
#error "Unsupported architecture"

#endif

// ***************** End Special windoze stuff *****************

#endif // COMMONWIN32_H
