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

/*!
 * \mainpage libdvdetect, a C/C++ API for LIBDVDETECT server access and DVD structure analysis
 *
 *
 * \section intro_sec Introduction
 *
 * The DVD Detection Library (libdvdetect) is a library to lookup DVDs in a database, similar to
 * existing services for CDs like Gracenote, freeDB or MusicBrainz. and provide information about
 * e.g. title, UPC/EAN or even cover arts. It is OS-independent and compiles for Windows or Unix
 * operating systems. Mac support is also planned.
 *
 * What the library does not provide is DVD ripping and decryption. This is not the intent, besides
 * cracking encyption is illegal in many countries. It merely reads the structure (e.g. titles and
 * chapters) and generates a special checksum to look up the database.
 *
 * <b>Note
 *
 * The current pre-alpha version does not provide database lookup! Please be patient and stay tuned,
 * this will be implemented very soon!</b>
 *
 * \section build_sec Building
 *
 * \subsection build_step1 Step 1: configure
 * \subsection build_step2 Step 2: make
 * \subsection build_step3 Optional Step 3: make doxygen
 *
 * \section install_sec Installation
 *
 * <b>DO NOT INSTALL THIS VERSION!<br>
 * The current version is a pre-alpha version and should not be installed on a production system.
 * Installation has not been fully tested and is not guaranteed to work properly. If a later release
 * is installed remains of this version may remain and be in the way.</b>
 *
 * \subsection install_step1 Step 1: follow the instructions in "Building"
 * \subsection install_step2 Step 2: make install
 */

/*! \file dvdetect.h
 *
 *  \brief LIBDVDETECT C API
 */

#pragma once

#ifndef DVDETECT_H
#define DVDETECT_H

#include <dvdetect/types.h>
#include <dvdetect/version.h>
#include <dvdetect/dvdetectdll.h>
#include <dvdetect/dvdutils.h>

#ifdef __cplusplus
extern "C" {
#endif

//! libdvdetect API handle
/*!
 * A libdvdetect API handle...
 */
typedef struct
{
    void*    m_pClass;                          //!< Class "handle", internal value, do not manipulate

} DVDETECTHANDLE, *LPDVDETECTHANDLE;

typedef const DVDETECTHANDLE* LPCDVDETECTHANDLE;

/*! Open library and get an dvdetect API handle
 *
 *  @return LPDVDETECTHANDLE
 */

DLL_PUBLIC LPDVDETECTHANDLE     dvdOpenLib();

/*! Parse a DVD
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @param pszPath const char *
 *  @return 0 on success; nonzero on error
 */

DLL_PUBLIC int                  dvdParse(LPDVDETECTHANDLE pDvDetectHandle, const char *pszPath);

/*! Get a pointer to a DVD video manager structure
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  \return Pointer to the underlying DVDVMGM (DVD Video Manager) structure.
 *  \return It is guaranteed that a valid structure is returned (never NULL).
 */

DLL_PUBLIC LPCDVDVMGM			dvdGetDVDVMGM(LPDVDETECTHANDLE pDvDetectHandle);

/*! Get a pointer to a DVD Video Title Set structure
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @param wTitleSetNo uint16_t
 *  @return
 */

DLL_PUBLIC LPCDVDVTS			dvdGetDVDVTS(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo);

/*! Get a pointer to a DVD program chain structure
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @param wTitleSetNo uint16_t
 *  @param wProgramChainNo uint16_t
 *  @return
 */

DLL_PUBLIC LPCDVDPGC 			dvdGetDVDPGC(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo);

/*! Get a pointer to a DVD program structure
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @param wTitleSetNo uint16_t
 *  @param wProgramChainNo uint16_t
 *  @param wProgram uint16_t
 *  @return
 */

DLL_PUBLIC LPCDVDPROGRAM		dvdGetDVDPROGRAM(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram);

/*! Get a pointer to a DVD video manager part of program structure
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @param wTitleSetNo uint16_t
 *  @param wProgramChainNo uint16_t
 *  @param wPtt uint16_t
 *  @return
 */

DLL_PUBLIC LPCDVDPTTVMG			dvdGetDVDPTTVMG(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo);

/*! Get a pointer to a DVD video title set part of program structure
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @param wTitleSetNo uint16_t
 *  @param wProgramChainNo uint16_t
 *  @param wPtt uint16_t
 *  @return
 */

DLL_PUBLIC LPCDVDPTTVTS         dvdGetDVDPTTVTS(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wPtt);

/*! Get a pointer to a DVD cell structure
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @param wTitleSetNo uint16_t
 *  @param wProgramChainNo uint16_t
 *  @param wProgram uint16_t
 *  @param wCell uint16_t
 *  @return
 */

DLL_PUBLIC LPCDVDCELL			dvdGetDVDCELL(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram, uint16_t wCell);

/*! Get a pointer to a DVD unit structure
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @param wTitleSetNo uint16_t
 *  @param wProgramChainNo uint16_t
 *  @param wProgram uint16_t
 *  @param wCell uint16_t
 *  @param wUnit uint16_t
 *  @return
 */

DLL_PUBLIC LPCDVDUNIT			dvdGetDVDUNIT(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram, uint16_t wCell, uint16_t wUnit);

/*! Get a string with the last error
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @return
 */

DLL_PUBLIC const char *         dvdGetErrorString(LPDVDETECTHANDLE pDvDetectHandle);

/*!
 *
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @return
 */

DLL_PUBLIC void                 dvdCloseLib(LPDVDETECTHANDLE pDvDetectHandle);

#ifdef __cplusplus
}
#endif

#endif // DVDETECT_H
