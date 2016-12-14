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

/*! \file compat.h
 *
 *  \brief Compatibility layer (all platforms)
 */

#pragma once

#ifndef COMPAT_H

#define COMPAT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// ***************** Special windoze stuff goes here *****************

#if defined(WIN32) && !defined(_WIN32_WINNT)
#define _WIN32_WINNT _WIN32_WINNT_VISTA
//#define _WIN32_WINNT _WIN32_WINNT_WIN2K
//#define _WIN32_WINNT 0x0600
#endif

#ifndef __CYGWIN__
#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#endif

#ifdef HAVE_WS2TCPIP_H
#include <ws2tcpip.h>
#endif
#endif //!__CYGWIN__

// ***************** Special *nix stuff goes here *****************

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_POLL_H
#include <poll.h>
#endif

#ifdef HAVE_NETINET_H
#include <netinet/in.h>
#endif

#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#include <errno.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif

/*
#ifndef __cplusplus
#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#else
#define false 0
#define true 1
typedef int bool;
#endif
#endif
*/

#if defined(WIN32) && defined(_MSC_VER)
#include "commonwin32.h"
#else
#include "common.h"
#endif

//#if defined(WIN32) && defined(__GNUC__)
//#include <ws2ipdef.h>
//#include <ws2tcpip.h>
//#endif

#ifdef __cplusplus

//! IPv4 and IPv6 Address
/*!
 * Useful to unify ip4 and ip6 stuff
 */
typedef struct _tagINET_ADDRESS
{
    size_t              iAddrLen;
    ADDRESS_FAMILY      sa_family;
    union addr
    {
        sockaddr_in     Ipv4;
        sockaddr_in6	Ipv6;
    } addr;
} INET_ADDRESS, *PINET_ADDRESS;

/*!
 * \typedef PINET_ADDRESS
 * \brief Pointer to INET_ADDRESS
 *
 * \typedef LPCINET_ADDRESS
 * \brief Constant pointer to INET_ADDRESS
 */
typedef const INET_ADDRESS* LPCINET_ADDRESS;

#endif

#ifdef AF_INET6
#define PROTOCOL                        _T("IPv4/IPv6")
#else
#define PROTOCOL                        _T("IPv4")
#endif

#if !defined(_countof)
#define _countof(_Array)                (sizeof(_Array) / sizeof(_Array[0]))
#endif

#define countof(x)                      _countof(x)

// Big/Little Endian
#ifdef WORDS_BIGENDIAN
#define O32_ORDER_STR                   "BE"
#define O32_ORDER_STR_LONG              "big endian"
#define ARCH_IS_BIG_ENDIAN              1
#else
#define O32_ORDER_STR                   "LE"
#define O32_ORDER_STR_LONG              "little endian"
#define ARCH_IS_BIG_ENDIAN              0
#endif

#define native2be                       be2native
#define native2le                       le2native

#ifndef HAVE_STRPTIME
extern "C" char * strptime (const char *buf, const char *format, struct tm *tm);
#endif

#define setInvalidParameterError(text)  setError(__PRETTY_FUNCTION__ + std::string("\nInvalid parameter: ") + std::string(text), DVDERRORCODE_INVALID_PARAMETER)
#define setInternalError(text)          setError(__PRETTY_FUNCTION__ + std::string("\nInternal error: ") + std::string(text), DVDERRORCODE_INTERNAL_ERROR)

#ifndef BOOL
#define BOOL int

#ifndef TRUE
#define TRUE (-1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif
#endif

#ifndef POLLRDNORM
#ifdef __cplusplus
extern "C" {
#endif

typedef struct pollfd {
  SOCKET fd;
  short  events;
  short  revents;
} WSAPOLLFD, *PWSAPOLLFD, *LPWSAPOLLFD;

#define POLLRDNORM 0x0100
#define POLLRDBAND 0x0200
#define POLLIN    (POLLRDNORM | POLLRDBAND)
#define POLLPRI    0x0400

#define POLLWRNORM 0x0010
#define POLLOUT   (POLLWRNORM)
#define POLLWRBAND 0x0020

#define POLLERR    0x0001
#define POLLHUP    0x0002
#define POLLNVAL   0x0004

int WSAAPI WSAPoll(
  WSAPOLLFD fdarray[],
  ULONG nfds,
  INT timeout
);

typedef struct addrinfo  ADDRINFOA, *PADDRINFOA;

int WSAAPI getaddrinfo(
    PCSTR pNodeName,
    PCSTR pServiceName,
    const ADDRINFOA *pHints,
       PADDRINFOA *ppResult
);

void WSAAPI freeaddrinfo(
    struct addrinfo *ai
);

#ifdef __cplusplus
}
#endif
#endif

#endif // COMPAT_H
