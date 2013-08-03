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
 *  \brief Compatibility layer (all playtforms)
*/

#pragma once

#ifndef COMMON_H
#define COMMON_H

#if !defined(WIN32)
typedef int	SOCKET;
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET				(-1)
#endif
#define SOCKET_ERROR				(-1)

#ifndef _WIN32
#define closesocket					close
#endif

#ifdef _GLIBCXX_USE_WCHAR_T
#define _UNICODE
#endif

#ifndef _UNICODE
#define TSTRING						string
#define TOFSTREAM					ofstream
#define TIFSTREAM					ifstream
#define TSTRINGSTREAM				stringstream
#define TCHAR						char
#define TCOUT						cout
#define TCERR						cerr
#define _T(x)						x
#define TSTRERROR					strerror
#define TUNLINK						unlink
#define TGETADDRINFO				getaddrinfo
#define TADDRINFO					addrinfo
#define TFREEADDRINFO				freeaddrinfo
#define TASCTIME					asctime
#define	WVSPRINTF					vsprintf
#define WTOI						atoi
#define WTOL						atol
#define	WSPRINTF					sprintf
#define stringToWString				std::string
#define wstringToString				std::string
#define STRTOKEN					"%s"
#else
#define TSTRING						wstring
#define TOFSTREAM					ofstream
#define TIFSTREAM					ifstream
#define TSTRINGSTREAM				wstringstream
#define TCHAR						wchar_t
#define TCOUT						wcout
#define TCERR						wcerr
#define _T(x)						L ## x
#define TSTRERROR(err)				stringToWString(strerror(err)).c_str()
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

// Detected FreeBSD
#if defined(__FreeBSD__)

#define _PLATFORM					_T("FreeBSD")

// Detected CygWIN
#elif defined(__CYGWIN32__)

#define _PLATFORM					_T("cygwin")
#ifdef _GLIBCXX_USE_WCHAR_T
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

#if defined(__amd64) || defined(__amd64__) || defined(__x86_64) || defined(__x86_64__)

// Detected 64 bit
#define ARCH						_T("amd64")
#define PLATFORM					_PLATFORM _T(" (") ARCH _T(")")

#else

// Unknown
#define ARCH						_T("")
#define PLATFORM					_PLATFORM

#endif

/*! If the submitter name is set to this value, it will never be written
 * into the database. In fact it will be treated like NULL. This can be
 * used to mark it in xml to be changed.
 */
#define DEFSUBMITTER    "***ChangeMe!***"

typedef enum
{
    XMLMODE_INVALID,                    //!< Invalid query
    XMLMODE_QUERY,                      //!< Query DVD
    XMLMODE_QUERY_RESPONSE,             //!< Response
    XMLMODE_SEARCH,                     //!< Search DVD
    XMLMODE_SEARCH_RESPONSE,            //!< Response
    XMLMODE_SUBMIT,                     //!< Submit DVD
    XMLMODE_SUBMIT_RESPONSE,            //!< Response
    XMLMODE_EXPORT                      //!< Export DVD

} XMLMODE, *LPXMLMODE;

typedef const XMLMODE* LPCXMLMODE;      //!< constant version

typedef enum
{
    XMLRESULT_SUCCESS,                  //!< Success
    XMLRESULT_NOTFOUND,                 //!< Query not successful
    XMLRESULT_NOT_IMPLEMENTED,          //!< Not implemented
    XMLRESULT_SQL_ERROR,                //!< SQL error, description see error string
    XMLRESULT_DUPLICATE_SUBMISSION,     //!< DBD already in database
    XMLRESULT_XML_ERROR,                //!< XML error, description see error string
    XMLRESULT_UNSUPPORTED_VERSION       //!< Unsupported XML version

} XMLRESULT, *LPXMLRESULT;

typedef const XMLRESULT* LPCXMLRESULT;  //!< constant version

#define setInvalidParameterError(text)  setError(__PRETTY_FUNCTION__ + std::string("\nInvalid parameter: ") + std::string(text), DVDERRORCODE_INVALID_PARAMETER)
#define setInternalError(text)          setError(__PRETTY_FUNCTION__ + std::string("\nInternal error: ") + std::string(text), DVDERRORCODE_INTERNAL_ERROR)

#endif // COMMON_H
