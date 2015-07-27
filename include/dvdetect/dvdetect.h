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

/*!
 * \mainpage libdvdetect, a C/C++ API for LIBDVDETECT server access and DVD structure analysis
 *
 *
 * \section intro_sec Introduction
 *
 * The DVD Detection Library (libdvdetect) is a library to lookup DVDs in a database, similar to
 * existing services for CDs like Gracenote, freeDB or MusicBrainz. and provide information about
 * e.g. title, UPC/EAN or even cover arts. It is OS independent and compiles for Windows or Unix
 * operating systems. Mac support is also planned.
 *
 * What the library does not provide is DVD ripping and decryption. This is not the intent, besides
 * cracking encyption is illegal in many countries. It merely reads the structure (e.g. titles and
 * chapters) and generates a special checksum to look up the database.
 *
 * \section build_sec Building
 *
 * For *nix, please make sure you have the tinyxml 2.6.1+ library installed first. For Windows, see
 * win/readme.txt for instructions how to install it.
 * 
 * Detailed instructions can be found in INSTALL at the top level directory.
 *
 * \subsection build_step1 Step 1: configure
 * \subsection build_step2 Step 2: make
 * \subsection build_step3 Optional Step 3: cd doc + run build (make doxygen documentation)
 *
 * \section install_sec Installation
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
typedef struct _tagDVDETECTHANDLE
{
    void*   m_pClassDvdParse;                       //!< Class "handle", internal value, do not manipulate

} DVDETECTHANDLE, *LPDVDETECTHANDLE;

typedef const DVDETECTHANDLE* LPCDVDETECTHANDLE;

//! libdvdetect database handle
/*!
 * A libdvdetect database handle...
 */
typedef struct _tagDVDBHANDLE
{
    void*   m_pClassDvdDatabase;                    //!< Class "handle", internal value, do not manipulate
    void*   m_pClassDvdParseLst;                    //!< Class "handle", internal value, do not manipulate

} DVDBHANDLE, *LPDVDBHANDLE;

typedef const LPDVDBHANDLE* LPCDVDBHANDLE;

//! Open library and get an dvdetect API handle.
/*! Open library and get an dvdetect API handle.
 *
 *  \return Success: LPDVDETECTHANDLE
 *  \return Fail: NULL
 */
DLL_PUBLIC LPDVDETECTHANDLE     dvdOpenLib();

//! Set or reset proxy server.
/*! Sets the proxy server or resets it (to no proxy) if pszProxyName is an empty string or NULL.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszProxyName const char * Proxy server name/IP and port (e.g. "proxy.mynet:3128")
 */
DLL_PUBLIC void                 dvdSetProxy(LPDVDETECTHANDLE pDvDetectHandle, const char * pszProxyName);

//! Parse a DVD.
/*! Parse a DVD.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszPath const char * Path to files. VIDEO_TS will be automatically added if necessesary.
 *  \return Success: returns 0
 *  \return Fail: returns nonzero value
 */
DLL_PUBLIC int                  dvdParse(LPDVDETECTHANDLE pDvDetectHandle, const char *pszPath);

//! Get number of title sets.
/*! Get number of title sets.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: returns number of title sets
 *  \return Fail: returns nonzero value
 */
DLL_PUBLIC int                  dvdGetTitleSetCount(LPDVDETECTHANDLE pDvDetectHandle);

//! Get number of program chains.
/*! Get number of program chains.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param wTitleSetNo uint16_t Index of title set (1...n)
 *  \return Success: returns number of program chains
 *  \return Fail: returns 0
 */
DLL_PUBLIC int                  dvdGetPgcCount(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo);

//! Get number of programs.
/*! Get number of programs.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param wTitleSetNo uint16_t Index of title set (1...n)
 *  \param wProgramChainNo uint16_t Index of program chain (1...n)
 *  \return Success: returns number of programs
 *  \return Fail: returns 0
 */
DLL_PUBLIC int                  dvdGetProgramCount(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo);

//! Get number of cells.
/*! Get number of cells.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param wTitleSetNo uint16_t Index of title set (1...n)
 *  \param wProgramChainNo uint16_t Index of program chain (1...n)
 *  \param wProgramNo uint16_t Index of program (1...n)
 *  \return Success: returns number of cells
 *  \return Fail: returns 0
 */
DLL_PUBLIC int                  dvdGetCellCount(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo);

//! Get number of units.
/*! Get number of units.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param wTitleSetNo uint16_t Index of title set (1...n)
 *  \param wProgramChainNo uint16_t Index of program chain (1...n)
 *  \param wProgramNo uint16_t Index of program (1...n)
 *  \param wCellNo uint16_t Index of cell (1...n)
 *  \return Success: returns number of units
 *  \return Fail: returns 0
 */
DLL_PUBLIC int                  dvdGetUnitCount(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo, uint16_t wCellNo);

//! Get number of virtual title sets.
/*! Get number of virtual title sets.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: returns number of programs
 *  \return Fail: returns 0
 */
DLL_PUBLIC int                  dvdGetDvdPttVmgCount(LPDVDETECTHANDLE pDvDetectHandle);

//! Get number of virtual programs/chapters.
/*! Get number of virtual programs/chapters.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param wTitleSetNo uint16_t Index of title set (1...n)
 *  \return Success: returns number of virtual programs/chapters
 *  \return Fail: returns 0
 */
DLL_PUBLIC int                  dvdGetPttVtsCount(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo);

//! Get a pointer to a DVD video manager structure.
/*! Get a pointer to a DVD video manager structure.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: Pointer to the underlying DVDVMGM (DVD Video Manager) structure.
 *  \return Fail: NULL
 */
DLL_PUBLIC LPCDVDVMGM			dvdGetDVDVMGM(LPDVDETECTHANDLE pDvDetectHandle);

//! Get a pointer to a DVD Video Title Set structure.
/*! Get a pointer to a DVD Video Title Set structure.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param wTitleSetNo uint16_t Index of title set (1...n)
 *  \return Success: Pointer to the underlying DVDVTS (DVD Video Title Set) structure.
 *  \return Fail: NULL
 */
DLL_PUBLIC LPCDVDVTS			dvdGetDVDVTS(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo);

//! Get a pointer to a DVD program chain structure.
/*! Get a pointer to a DVD program chain structure
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param wTitleSetNo uint16_t Index of title set (1...n)
 *  \param wProgramChainNo uint16_t Index of program chain (1...n)
 *  \return Success: Pointer to the underlying DVDPGC (DVD Program Chain) structure.
 *  \return Fail: NULL
 */
DLL_PUBLIC LPCDVDPGC 			dvdGetDVDPGC(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo);

//! Get a pointer to a DVD program structure.
/*! Get a pointer to a DVD program structure.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param wTitleSetNo uint16_t Index of title set (1...n)
 *  \param wProgramChainNo uint16_t Index of program chain (1...n)
 *  \param wProgramNo uint16_t Index of program (1...n)
 *  \return Success: Pointer to the underlying DVDPROGRAM (DVD Program) structure.
 *  \return Fail: NULL
 */
DLL_PUBLIC LPCDVDPROGRAM		dvdGetDVDPROGRAM(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo);

//! Get a pointer to a DVD video manager part of program structure.
/*! Get a pointer to a DVD video manager part of program structure.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param wTitleSetNo uint16_t Index of title set (1...n)
 *  \return Success: Pointer to the underlying DVDPTTVMG (DVD Virtual Video Manager) structure.
 *  \return Fail: NULL
 */
DLL_PUBLIC LPCDVDPTTVMG			dvdGetDVDPTTVMG(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo);

//! Get a pointer to a DVD video title set part of program structure.
/*! Get a pointer to a DVD video title set part of program structure.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param wTitleSetNo uint16_t Index of title set (1...n)
 *  \param wPttNo uint16_t
 *  \return Success: Pointer to the underlying DVDPTTVTS (DVD Virtual Title Set) structure.
 *  \return Fail: NULL
 */
DLL_PUBLIC LPCDVDPTTVTS         dvdGetDVDPTTVTS(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wPttNo);

//! Get a pointer to a DVD cell structure.
/*! Get a pointer to a DVD cell structure.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param wTitleSetNo uint16_t Index of title set (1...n)
 *  \param wProgramChainNo uint16_t Index of program chain (1...n)
 *  \param wProgramNo uint16_t Index of program (1...n)
 *  \param wCellNo uint16_t Index of cell (1...n)
 *  \return Success: Pointer to the underlying DVDCELL (DVD Cell) structure.
 *  \return Fail: NULL
 */
DLL_PUBLIC LPCDVDCELL			dvdGetDVDCELL(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo, uint16_t wCellNo);

//! Get a pointer to a DVD unit structure.
/*! Get a pointer to a DVD unit structure.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param wTitleSetNo uint16_t Index of title set (1...n)
 *  \param wProgramChainNo uint16_t Index of program chain (1...n)
 *  \param wProgramNo uint16_t Index of program (1...n)
 *  \param wCellNo uint16_t Index of cell (1...n)
 *  \param wUnitNo uint16_t Index of unit (1...n)
 *  \return Success: Pointer to the underlying DVDUNIT (DVD Unit) structure.
 *  \return Fail: NULL
 */
DLL_PUBLIC LPCDVDUNIT			dvdGetDVDUNIT(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo, uint16_t wCellNo, uint16_t wUnitNo);

//! Get the virtual size of the DVD in bytes.
/*! Get the virtual size of the DVD in bytes.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: Virtual size in bytes
 *  \return Fail: 0 if no DVD parsed
 */
DLL_PUBLIC uint64_t				dvdGetVirtSize(LPDVDETECTHANDLE pDvDetectHandle);

//! Get the virtual playtime of the DVD in ms.
/*! Get the virtual playtime of the DVD in ms.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: Virtual playtime in ms
 *  \return Fail: 0 if no DVD parsed
 */
DLL_PUBLIC uint64_t				dvdGetVirtPlayTime(LPDVDETECTHANDLE pDvDetectHandle);

//! Get the size of the DVD in bytes.
/*! Get the size of the DVD in bytes.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: Size in bytes
 *  \return Fail: 0 if no DVD parsed
 */
DLL_PUBLIC uint64_t				dvdGetSize(LPDVDETECTHANDLE pDvDetectHandle);

//! Get the playtime of the DVD in ms.
/*! Get the playtime of the DVD in ms.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: Playtime in ms
 *  \return Fail: 0 if no DVD parsed
 */
DLL_PUBLIC uint64_t				dvdGetPlayTime(LPDVDETECTHANDLE pDvDetectHandle);

//! Get the DVD code.
/*! Get the DVD code (base code for the md5 disk hash).
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return DVD code string if successful
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetCode(LPDVDETECTHANDLE pDvDetectHandle);

//! Calculate the disk hash (md5 code).
/*! Calculate the disk hash (md5 code).
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return DVD hash string if successful
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetHash(LPDVDETECTHANDLE pDvDetectHandle);

//! Get disk album artist.
/*!
 * If DVD was successfully looked up, return the album artist.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Album artist or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetAlbumArtist(LPDVDETECTHANDLE pDvDetectHandle);

//! Set disk album artist.
/*!
 * Sets the album artist.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszAlbumArtist  const char * Album artist
 */
DLL_PUBLIC void                 dvdSetAlbumArtist(LPDVDETECTHANDLE pDvDetectHandle, const char * pszAlbumArtist);

//! Get disk album name.
/*!
 * If DVD was successfully looked up, return the album.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: album or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetAlbum(LPDVDETECTHANDLE pDvDetectHandle);

//! Set disk album name.
/*!
 * Sets the album name.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszAlbum  const char * Album name
 */
DLL_PUBLIC void                 dvdSetAlbum(LPDVDETECTHANDLE pDvDetectHandle, const char * pszAlbum);

//! Get the DVD genre.
/*!
 * If DVD was successfully looked up, return DVD genre.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: DVD genre or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetGenre(LPDVDETECTHANDLE pDvDetectHandle);

//! Set the DVD genre
/*!
 * Sets the DVD genre
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszGenre const char * DVD genre
 */
DLL_PUBLIC void                 dvdSetGenre(LPDVDETECTHANDLE pDvDetectHandle, const char * pszGenre);

//! Get DVD movie cast
/*!
 * If DVD was successfully looked up, return DVD movie cast.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: DVD movie cast or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetCast(LPDVDETECTHANDLE pDvDetectHandle);

//! Set the DVD movie cast
/*!
 * Sets the DVD movie cast
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszCast  const char * DVD movie cast
 */
DLL_PUBLIC void                 dvdSetCast(LPDVDETECTHANDLE pDvDetectHandle, const char * pszCast);

//! Get DVD movie crew
/*!
 * If DVD was successfully looked up, return DVD movie crew.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: DVD movie crew or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetCrew(LPDVDETECTHANDLE pDvDetectHandle);

//! Set the DVD movie crew
/*!
 * Sets the DVD movie crew
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszCrew  const char * DVD movie crew
 */
DLL_PUBLIC void                 dvdSetCrew(LPDVDETECTHANDLE pDvDetectHandle, const char * pszCrew);

//! Get the DVD movie director
/*!
 * If DVD was successfully looked up, return DVD movie director.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: DVD movie director or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetDirector(LPDVDETECTHANDLE pDvDetectHandle);

//! Set the DVD movie director
/*!
 * Sets the DVD movie director
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszDirector  const char * DVD movie director
 */
DLL_PUBLIC void                 dvdSetDirector(LPDVDETECTHANDLE pDvDetectHandle, const char * pszDirector);

//! Get screen play writer
/*!
 * If DVD was successfully looked up, return screen play writer.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: screen play writer or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *dvdGetScreenplay(LPDVDETECTHANDLE pDvDetectHandle);

//! Set screen play writer
/*!
 * Sets screen play writer
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszScreenplay const char * screen play writer
 */
DLL_PUBLIC void dvdSetScreenplay(LPDVDETECTHANDLE pDvDetectHandle, const char * pszScreenplay);

//! Get producer
/*!
 * If DVD was successfully looked up, return producer.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: producer or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *dvdGetProducer(LPDVDETECTHANDLE pDvDetectHandle);

//! Set producer
/*!
 * Sets producer
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszProducer const char * Producer
 */
DLL_PUBLIC void dvdSetProducer(LPDVDETECTHANDLE pDvDetectHandle, const char * pszProducer);

//! Get editor
/*!
 * If DVD was successfully looked up, return editor.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: editor or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *dvdGetEditing(LPDVDETECTHANDLE pDvDetectHandle);

//! Set editor
/*!
 * Sets editor
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszEditing const char * Editor
 */
DLL_PUBLIC void dvdSetEditing(LPDVDETECTHANDLE pDvDetectHandle, const char * pszEditing);

//! Get cinematographer
/*!
 * If DVD was successfully looked up, return cinematographer.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: cinematographer or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *dvdGetCinematography(LPDVDETECTHANDLE pDvDetectHandle);

//! Set cinematographer
/*!
 * Sets cinematographer
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszCinematography const char * Cinematographer
 */
DLL_PUBLIC void dvdSetCinematography(LPDVDETECTHANDLE pDvDetectHandle, const char * pszCinematography);

//! Get country of origin
/*!
 * If DVD was successfully looked up, return country of origin.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: country of origin or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetCountry(LPDVDETECTHANDLE pDvDetectHandle);

//! Set the country of origin
/*!
 * Sets the country of origin
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszCountry  const char * country of origin
 */
DLL_PUBLIC void                 dvdSetCountry(LPDVDETECTHANDLE pDvDetectHandle, const char * pszCountry);

//! Get original language
/*!
 * If DVD was successfully looked up, return original language.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: original language or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetOriginalLanguage(LPDVDETECTHANDLE pDvDetectHandle);

//! Set the original language
/*!
 * Sets the original language
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszOriginalLanguage const char * original language
 */
DLL_PUBLIC void                 dvdSetOriginalLanguage(LPDVDETECTHANDLE pDvDetectHandle, const char * pszOriginalLanguage);

//! Get release date
/*!
 * If DVD was successfully looked up, return release date.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 * Returned in ISO format (YYYY-MM-DD HH:MM:SS).
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: release date or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetReleaseDate(LPDVDETECTHANDLE pDvDetectHandle);

//! Set release date
/*!
 * Sets the release date
 * Must be in ISO format (YYYY-MM-DD HH:MM:SS). Time can be left empty.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszReleaseDate  const char * release date
 */
DLL_PUBLIC void                 dvdSetReleaseDate(LPDVDETECTHANDLE pDvDetectHandle, const char * pszReleaseDate);

//! Get special feature list
/*!
 * If DVD was successfully looked up, return special feature list
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: Special feature list or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetSpecialFeatures(LPDVDETECTHANDLE pDvDetectHandle);

//! Set the special feature list
/*!
 * Sets the special feature list
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszFeatures  const char * special feature list.
 */
DLL_PUBLIC void                 dvdSetSpecialFeatures(LPDVDETECTHANDLE pDvDetectHandle, const char * pszFeatures);

//! Get EAN/UPC
/*!
 * If DVD was successfully looked up, return EAN/UPC
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: EAN/UPC or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetEAN_UPC(LPDVDETECTHANDLE pDvDetectHandle);

//! Set EAN/UPC
/*!
 * Sets the EAN/UPC
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszEAN_UPC  const char * EAN/UPC.
 */
DLL_PUBLIC void                 dvdSetEAN_UPC(LPDVDETECTHANDLE pDvDetectHandle, const char * pszEAN_UPC);

//! Get storyline/synopsis
/*!
 * If DVD was successfully looked up, return storyline/synopsis
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: storyline/synopsis or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetStoryline(LPDVDETECTHANDLE pDvDetectHandle);

//! Set storyline/synopsis
/*!
 * Sets the storyline/synopsis
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszStoryline  const char * Storyline/synopsis
 */
DLL_PUBLIC void                 dvdSetStoryline(LPDVDETECTHANDLE pDvDetectHandle, const char * pszStoryline);

//! Get remarks
/*!
 * If DVD was successfully looked up, return remarks
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: remarks or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetRemarks(LPDVDETECTHANDLE pDvDetectHandle);

//! Set remarks
/*!
 * Sets the remarks
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszRemarks  const char * Remarks
 */
DLL_PUBLIC void                 dvdSetRemarks(LPDVDETECTHANDLE pDvDetectHandle, const char * pszRemarks);

//! Get revision
/*!
 * If DVD was successfully looked up, return revision.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: revision or 0 if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC int                  dvdGetRevision(LPDVDETECTHANDLE pDvDetectHandle);

//! Get submitter
/*!
 * If DVD was successfully looked up, return submitter.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: submitter or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetSubmitter(LPDVDETECTHANDLE pDvDetectHandle);

//! Set submitter
/*!
 * Sets the submitter
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszSubmitter  const char * Submitter
 */
DLL_PUBLIC void                 dvdSetSubmitter(LPDVDETECTHANDLE pDvDetectHandle, const char * pszSubmitter);

//! Get submitter IP
/*!
 * If DVD was successfully looked up, return submitter IP.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: submitter IP or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetSubmitterIP(LPDVDETECTHANDLE pDvDetectHandle);

//! Set client
/*!
 * Sets the client
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszClient const char * Client
 */
DLL_PUBLIC void 				dvdSetClient(LPDVDETECTHANDLE pDvDetectHandle, const char * pszClient);

//! Get submission client
/*!
 * If DVD was successfully looked up, return submission client.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: submission client or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetClient(LPDVDETECTHANDLE pDvDetectHandle);

//! Set keywords
/*!
 * Sets the keywords
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszKeywords const char * Keywords
 */
DLL_PUBLIC void                 dvdSetKeywords(LPDVDETECTHANDLE pDvDetectHandle, const char * pszKeywords);

//! Get keywords
/*!
 * If DVD was successfully looked up, return keywords.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: Keywords or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetKeywords(LPDVDETECTHANDLE pDvDetectHandle);

//! Get protocol version
/*!
 * If DVD was successfully looked up, return protocol version
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Protocol version or 0 if unknown
 */
DLL_PUBLIC int                  dvdGetProtocolVersion(LPDVDETECTHANDLE pDvDetectHandle);

//! Get library version
/*!
 * If DVD was successfully looked up, return library version
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: Library version or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetLibraryVersion(LPDVDETECTHANDLE pDvDetectHandle);

//! Get library name
/*!
 * If DVD was successfully looked up, return library name
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: Library name or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetLibraryName(LPDVDETECTHANDLE pDvDetectHandle);

//! Client name
/*!
 * If DVD was successfully looked up, return client name
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: Client name or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetClientName(LPDVDETECTHANDLE pDvDetectHandle);

//! Get date created
/*!
 * If DVD was successfully looked up, return date created.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: date created or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetDateCreated(LPDVDETECTHANDLE pDvDetectHandle);

//! Get date last changed
/*!
 * If DVD was successfully looked up, return date last changed.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: date last changed or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetDateLastChanged(LPDVDETECTHANDLE pDvDetectHandle);

//! Get title
/*!
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param wTitleSetNo uint16_t Index of title set (1...n)
 *  \return Success: title or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetPttVmgTitle(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo);

//! Get artist
/*!
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param wTitleSetNo uint16_t Index of title set (1...n)
 *  \param wPttNo uint16_t Index of part of title (1...n)
 *  \return Success: title or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetPttVtsArtist(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wPttNo);

//! Get title
/*!
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param wTitleSetNo uint16_t Index of title set (1...n)
 *  \param wPttNo uint16_t Index of part of title (1...n)
 *  \return Success: title or empty string if unknown
 *  \return Fail: NULL
 */
DLL_PUBLIC const char *         dvdGetPttVtsTitle(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wPttNo);

//! Allocate memory
/*!
 * Allocates memory and returns a pointer to it
 *
 *  \return Success: Returns pointer to allocated memory
 *  \return Fail: NULL
 */
DLL_PUBLIC void *               dvdMalloc(size_t size);

//! Free memory
/*!
 * Previously by dvdMalloc() allocated memory. Do not pass a pointer allocated
 * by new or other methods. The pointer will be set to NULL.
 *
 *  \param ppMemory void ** Pointer to memory to be freed
 */
DLL_PUBLIC void                 dvdFree(void ** ppMemory);

//! Get a string with the last error.
/*! Get a string with the last error.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: Error string
 *  \return Fail: NULL (only if out of memory)
 */
DLL_PUBLIC const char *         dvdGetErrorString(LPDVDETECTHANDLE pDvDetectHandle);

//! Close the library handle.
/*! Close the library handle.
 *
 * Closes the library handle and frees all resources. The pointer will be set to NULL.
 * The handle cannot be used any longer after this call.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 */
DLL_PUBLIC void                 dvdCloseLib(LPDVDETECTHANDLE *pDvDetectHandle);

// ---------------------------------------------------------------------------------------------------
// Database access
// ---------------------------------------------------------------------------------------------------

//! Open DVD database connection.
/*! Open DVD database connection.
 *
 *  \param  pszPogramName const char * Name of (your) program
 *  \return Success: LPDVDBHANDLE handle to database
 *  \return Fail: NULL
 */
DLL_PUBLIC LPDVDBHANDLE         dvdDBOpen(const char * pszPogramName);

//! Set or reset proxy server.
/*! Sets the proxy server or resets it (to no proxy) if pszProxyName is an empty string or NULL.
 *
 *  \param pDvDBHandle LPDVDBHANDLE Handle to DVD database previously obtained by dvdDBOpen
 *  \param pszProxyName const char * Proxy server name/IP and port (e.g. "proxy.mynet:3128")
 */
DLL_PUBLIC void                 dvdDBSetProxy(LPDVDBHANDLE pDvDBHandle, const char * pszProxyName);

//! Query DVD database.
/*! Query DVD database for a DVD.
 *
 *  \param pDvDBHandle LPDVDBHANDLE Handle to DVD database previously obtained by dvdDBOpen
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: returns 0
 *  \return Fail: returns nonzero value
 */
DLL_PUBLIC int                  dvdDBQuery(LPDVDBHANDLE pDvDBHandle, LPDVDETECTHANDLE pDvDetectHandle);

//! Get number or results of last dvdDBQuery call.
/*! Get number or results of last dvdDBQuery call.
 *
 *  \param pDvDBHandle LPDVDBHANDLE Handle to DVD database previously obtained by dvdDBOpen
 *  \return Number of found results
 */
DLL_PUBLIC int                  dvdDBResults(LPDVDBHANDLE pDvDBHandle);

//! Search DVD in database.
/*! Search a DVD in the database.
 *
 *  \param pDvDBHandle LPDVDBHANDLE Handle to DVD database previously obtained by dvdDBOpen
 *  \param pszSearch const char * Search pattern
 *  \return Success: returns 0
 *  \return Fail: returns nonzero value
 */
DLL_PUBLIC int                  dvdDBSearch(LPDVDBHANDLE pDvDBHandle, const char * pszSearch);

//! Submit DVD to database.
/*! Submit a DVD to the database.
 *
 *  \param pDvDBHandle LPDVDBHANDLE Handle to DVD database previously obtained by dvdDBOpen
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \return Success: returns 0
 *  \return Fail: returns nonzero value
 */
DLL_PUBLIC int                  dvdDBSubmit(LPDVDBHANDLE pDvDBHandle, LPDVDETECTHANDLE pDvDetectHandle);

//! Update album name etc.
/*!
 * Updates the album name, artist etc. from the parse data received.
 *
 *  \param pDvDBHandle LPDVDBHANDLE Handle to DVD database previously obtained by dvdDBOpen
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 */
DLL_PUBLIC void                 dvdUpdate(LPDVDBHANDLE pDvDBHandle, LPDVDETECTHANDLE pDvDetectHandle);

//! Write DVD structure to XML.
/*! Write DVD structure to XML.
 *
 *  \param pDvDBHandle LPDVDBHANDLE Handle to DVD database previously obtained by dvdDBOpen
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszOutFile const char * Name of output file
 *  \return Success: returns 0
 *  \return Fail: returns nonzero value
 */
DLL_PUBLIC int                  dvdDBwrite(LPDVDBHANDLE pDvDBHandle, LPDVDETECTHANDLE pDvDetectHandle, const char * pszOutFile);

//! Read DVD structure from XML.
/*! Read DVD structure from XML.
 *
 *  \param pDvDBHandle LPDVDBHANDLE Handle to DVD database previously obtained by dvdDBOpen
 *  \param pDvDetectHandle LPDVDETECTHANDLE libdvdetect handle previously obtained by dvdOpenLib
 *  \param pszInFile const char * Name of input file
 *  \return Success: returns 0
 *  \return Fail: returns nonzero value
 */
DLL_PUBLIC int                  dvdDBread(LPDVDBHANDLE pDvDBHandle, LPDVDETECTHANDLE pDvDetectHandle, const char * pszInFile);

//! Get a string with the last error.
/*! Get a string with the last error.
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \param pDvDBHandle LPDVDBHANDLE Handle to DVD database previously obtained by dvdDBOpen
 *  \return Success: Error string
 *  \return Fail: NULL (only if out of memory)
 */
DLL_PUBLIC const char *         dvdDBGetErrorString(LPDVDBHANDLE pDvDBHandle);

//! Close the database handle.
/*! Close the database handle.
 *
 * Closes the database handle and frees all resources. The pointer will be set to NULL.
 * The handle cannot be used any longer after this call.
 *
 *  \param pDvDBHandle LPDVDBHANDLE * Handle to DVD database previously obtained by dvdDBOpen
 */
DLL_PUBLIC void                 dvdDBClose(LPDVDBHANDLE * pDvDBHandle);

#ifdef __cplusplus
}
#endif

#endif // DVDETECT_H
