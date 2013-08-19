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

/*! \file common.h
 *
 *  \brief Compatibility layer (all platforms)
*/

#pragma once

#ifndef COMMON_H
#define COMMON_H

#if defined(_MSC_VER)
#error "Do not include this file for MS visual C++"
#endif

// ***************** Special *nix stuff goes here *****************

#ifndef ___WSA_SOCKET_TYPES_H
typedef int	SOCKET;
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET				(-1)
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR				(-1)
#endif

#ifndef _WIN32
#define closesocket					close
#endif

#ifdef _UNICODE
#ifndef _GLIBCXX_USE_WCHAR_T
#define _GLIBCXX_USE_WCHAR_T
#endif
#define USE_WCHAR_T
#endif

#ifndef USE_WCHAR_T
#define TSTRING						string
#define TOFSTREAM					ofstream
#define TIFSTREAM					ifstream
#define TSTRINGSTREAM				stringstream
#define TCHAR						char
#define TCOUT						cout
#define TCERR						cerr
#define _T(x)						x
#define TSTRERROR					gai_strerror
#define TUNLINK						unlink
#define TGETADDRINFO				getaddrinfo
#define TADDRINFO					addrinfo
#define TFREEADDRINFO				freeaddrinfo
#define TASCTIME					asctime
#define	WVSPRINTF					vsprintf
#define WTOI						atoi
#define WTOL						atol
#define	WSPRINTF					sprintf
#define STRTOKEN					"%s"
#define TSTRSTR                     strstr
#define TSTRLEN                     strlen
#define TSTRTOUL                    strtoul
#else
#define TSTRING						wstring
#define TOFSTREAM					ofstream
#define TIFSTREAM					ifstream
#define TSTRINGSTREAM				wstringstream
#define TCHAR						wchar_t
#define TCOUT						wcout
#define TCERR						wcerr
#define _T(x)						L ## x
#define TSTRERROR(err)				stringToWString(gai_strerror(err)).c_str()
#define TUNLINK(x)					unlink(wstringToString(x).c_str())
#define TGETADDRINFO				getaddrinfo
#define TADDRINFO					addrinfo
#define TFREEADDRINFO				freeaddrinfo
#define TASCTIME(tm)				stringToWString(asctime(tm)).c_str()
#define	WVSPRINTF(ws, format, ap)	vswprintf(ws, sizeof(ws), format, ap)
#define WTOI(x)						(int)wcstol(x, (wchar_t **) NULL, 10)
#define WTOL(x)						wcstol(x, (wchar_t **) NULL, 10)
#define	WSPRINTF					swprintf
#define STRTOKEN					L"%ls"
#define TSTRSTR                     wcsstr
#define TSTRLEN                     wcslen
#define TSTRTOUL                    wcstoul
#endif

#ifndef _WIN32
#define	POLL						poll
#else
#define	POLL						WSAPoll
#endif

#if (_WIN32)
typedef USHORT ADDRESS_FAMILY;
#else
typedef sa_family_t ADDRESS_FAMILY;
#endif

// Architecture

// Detected FreeBSD
#if defined(__FreeBSD__)

#define _PLATFORM					_T("FreeBSD")

// Detected CygWIN
#elif defined(__CYGWIN32__)

#define _PLATFORM					_T("cygwin")
#ifdef USE_WCHAR_T
#error "std::wstring not supported with cygwin"
#endif

// Any linux
#elif defined(__gnu_linux__) || defined(__linux) || defined(__linux__) || defined(linux)

#define _PLATFORM					_T("linux")

// Any unix
#elif defined(__unix) || defined(__unix__) || defined(unix)

#define _PLATFORM					_T("*nix")

// Unknown
#else

#define _PLATFORM					_T("unknown")

#endif

#if defined(__amd64) || defined(__amd64__) || defined(__x86_64) || defined(__x86_64__)

// Detected 64 bit
#define ARCH						_T("amd64")
#define PLATFORM					_PLATFORM _T(" (") ARCH _T(")")

#else

// Unknown
#define ARCH						_T("")
#define PLATFORM					_PLATFORM

#endif

// ***************** End Special *nix stuff *****************

#endif // COMMON_H
