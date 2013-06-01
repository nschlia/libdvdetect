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

/**
 * \mainpage libdvdetect, a C/C++ API for LIBDVDETECT server access and DVD structure analysis
 *
 *
 * \section intro_sec Introduction
 *
 * This is the introduction.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Opening the box
 *
 * etc...
 */

/** \file dvdetect.h
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

/**
 * libdvdetect API handle
 */
typedef struct _tagDVDETECTHANDLE
{
    void*    m_pClass;                          /**< Class "handle", internal value, do not manipulate */

} DVDETECTHANDLE, *LPDVDETECTHANDLE;

typedef const DVDETECTHANDLE* LPCDVDETECTHANDLE;

/**
 *
 *
 *  @return LPDVDETECTHANDLE
 */

DLL_PUBLIC LPDVDETECTHANDLE     dvdOpenLib();

/**
 *
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @param pszPath const char *
 *  @return 0 on success; nonzero on error
 */

DLL_PUBLIC int                  dvdParse(LPDVDETECTHANDLE pDvDetectHandle, const char *pszPath);

/**
 *
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @return
 */

DLL_PUBLIC LPCDVDVMGM		dvdGetDVDVMGM(LPDVDETECTHANDLE pDvDetectHandle);

/**
 *
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @param wTitleSetNo uint16_t
 *  @return
 */

DLL_PUBLIC LPCDVDVTS		dvdGetDVDVTS(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo);

/**
 *
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @param wTitleSetNo uint16_t
 *  @param wProgramChainNo uint16_t
 *  @return
 */

DLL_PUBLIC LPCDVDPGC 		dvdGetDVDPGC(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo);

/**
 *
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @param wTitleSetNo uint16_t
 *  @param wProgramChainNo uint16_t
 *  @param wProgram uint16_t
 *  @return
 */

DLL_PUBLIC LPCDVDPROGRAM	dvdGetDVDPROGRAM(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram);

/**
 *
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @param wTitleSetNo uint16_t
 *  @param wProgramChainNo uint16_t
 *  @param wPtt uint16_t
 *  @return
 */

DLL_PUBLIC LPCDVDPTT		dvdGetDVDPTT(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wPtt);

/**
 *
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @param wTitleSetNo uint16_t
 *  @param wProgramChainNo uint16_t
 *  @param wProgram uint16_t
 *  @param wCell uint16_t
 *  @return
 */

DLL_PUBLIC LPCDVDCELL		dvdGetDVDCELL(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram, uint16_t wCell);

/**
 *
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @param wTitleSetNo uint16_t
 *  @param wProgramChainNo uint16_t
 *  @param wProgram uint16_t
 *  @param wCell uint16_t
 *  @param wUnit uint16_t
 *  @return
 */

DLL_PUBLIC LPCDVDUNIT		dvdGetDVDUNIT(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram, uint16_t wCell, uint16_t wUnit);

/**
 *
 *
 *  @param pDvDetectHandle LPDVDETECTHANDLE
 *  @return
 */

DLL_PUBLIC const char *         dvdGetErrorString(LPDVDETECTHANDLE pDvDetectHandle);

/**
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
