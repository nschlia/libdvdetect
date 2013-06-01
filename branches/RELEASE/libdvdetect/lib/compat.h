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

/** \file compat.h
 *
 *  \brief Compatibility layer (all playtforms)
 */

#pragma once

#ifndef COMPAT_H
#define COMPAT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// ***************** Special windoze stuff goes here *****************

#if defined(WIN32)
#define _WIN32_WINNT    0x0600
#endif

#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#endif

#ifdef HAVE_WS2TCPIP_H
#include <Ws2tcpip.h>
#endif

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

#ifdef __cplusplus

// Useful to unify ip4 and ip6 stuff
typedef struct _tagINET_ADDRESS
{
    size_t			iAddrLen;
    ADDRESS_FAMILY	sa_family;
    union addr
    {
        sockaddr_in		Ipv4;
        sockaddr_in6	Ipv6;
    } addr;
} INET_ADDRESS, *PINET_ADDRESS;

#endif

#ifdef AF_INET6
#define PROTOCOL					_T("IPv4/IPv6")
#else
#define PROTOCOL					_T("IPv4")
#endif

#if !defined(_countof)
#define _countof(_Array)			(sizeof(_Array) / sizeof(_Array[0]))
#endif

#define countof(x)					_countof(x)

#define DVD_SECTOR_SIZE             2048
#define DVD_MAX_VOB                 9
#define DVD_MAX_VOB_SIZE            ((1024*1024*1024) - DVD_SECTOR_SIZE)    // = 1.073.739.776 bytes

// Big/Little Endian
#ifdef WORDS_BIGENDIAN
#define O32_ORDER_STR               "BE"
#define O32_ORDER_STR_LONG          "big endian"
#else
#define O32_ORDER_STR               "LE"
#define O32_ORDER_STR_LONG          "little endian"
#endif

#define native2be                   be2native
#define native2le                   le2native

#endif // COMPAT_H
