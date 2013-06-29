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
 * For *nix, please make sure you have the tinyxml 2.6.1+ library installed first. For Windows, see
 * win/readme.txt for instructions how to install it.
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
    void*   m_pClassDvdParse;                       //!< Class "handle", internal value, do not manipulate

} DVDETECTHANDLE, *LPDVDETECTHANDLE;

typedef const DVDETECTHANDLE* LPCDVDETECTHANDLE;

//! libdvdetect database handle
/*!
 * A libdvdetect database handle...
 */
typedef struct
{
    void*   m_pClassDvdDatabase;                    //!< Class "handle", internal value, do not manipulate
    void*   m_pClassDvdParseLst;                    //!< Class "handle", internal value, do not manipulate

} DVDBHANDLE, *LPDVDBHANDLE;

typedef const LPDVDBHANDLE* LPCDVDBHANDLE;


/*! Open library and get an dvdetect API handle
 *
 *  \return LPDVDETECTHANDLE
 */
DLL_PUBLIC LPDVDETECTHANDLE     dvdOpenLib();

/*! Parse a DVD
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \param pszPath const char *
 *  \return 0 on success; nonzero on error
 */
DLL_PUBLIC int                  dvdParse(LPDVDETECTHANDLE pDvDetectHandle, const char *pszPath);

/*! Get a pointer to a DVD video manager structure
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \return Pointer to the underlying DVDVMGM (DVD Video Manager) structure.
 *  \return It is guaranteed that a valid structure is returned (never NULL).
 */
DLL_PUBLIC LPCDVDVMGM			dvdGetDVDVMGM(LPDVDETECTHANDLE pDvDetectHandle);

/*! Get a pointer to a DVD Video Title Set structure
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \param wTitleSetNo uint16_t
 *  \return
 */
DLL_PUBLIC LPCDVDVTS			dvdGetDVDVTS(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo);

/*! Get a pointer to a DVD program chain structure
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \param wTitleSetNo uint16_t
 *  \param wProgramChainNo uint16_t
 *  \return
 */
DLL_PUBLIC LPCDVDPGC 			dvdGetDVDPGC(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo);

/*! Get a pointer to a DVD program structure
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \param wTitleSetNo uint16_t
 *  \param wProgramChainNo uint16_t
 *  \param wProgram uint16_t
 *  \return
 */
DLL_PUBLIC LPCDVDPROGRAM		dvdGetDVDPROGRAM(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram);

/*! Get a pointer to a DVD video manager part of program structure
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \param wTitleSetNo uint16_t
 *  \param wProgramChainNo uint16_t
 *  \param wPtt uint16_t
 *  \return
 */
DLL_PUBLIC LPCDVDPTTVMG			dvdGetDVDPTTVMG(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo);

/*! Get a pointer to a DVD video title set part of program structure
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \param wTitleSetNo uint16_t
 *  \param wProgramChainNo uint16_t
 *  \param wPtt uint16_t
 *  \return
 */
DLL_PUBLIC LPCDVDPTTVTS         dvdGetDVDPTTVTS(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wPtt);

/*! Get a pointer to a DVD cell structure
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \param wTitleSetNo uint16_t
 *  \param wProgramChainNo uint16_t
 *  \param wProgram uint16_t
 *  \param wCell uint16_t
 *  \return
 */
DLL_PUBLIC LPCDVDCELL			dvdGetDVDCELL(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram, uint16_t wCell);

/*! Get a pointer to a DVD unit structure
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \param wTitleSetNo uint16_t
 *  \param wProgramChainNo uint16_t
 *  \param wProgram uint16_t
 *  \param wCell uint16_t
 *  \param wUnit uint16_t
 *  \return
 */
DLL_PUBLIC LPCDVDUNIT			dvdGetDVDUNIT(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram, uint16_t wCell, uint16_t wUnit);

/*! Get the virtual size of the DVD in bytes
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \return Size in bytes
 */
DLL_PUBLIC uint64_t				dvdGetVirtSize(LPDVDETECTHANDLE pDvDetectHandle);

/*! Get the virtual playtime of the DVD in ms
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \return Playtime in ms
 */
DLL_PUBLIC uint64_t				dvdGetVirtPlayTime(LPDVDETECTHANDLE pDvDetectHandle);

/*! Get the size of the DVD in bytes
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \return Size in bytes
 */
DLL_PUBLIC uint64_t				dvdGetSize(LPDVDETECTHANDLE pDvDetectHandle);

/*! Get the playtime of the DVD in ms
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \return Playtime in ms
 */
DLL_PUBLIC uint64_t				dvdGetPlayTime(LPDVDETECTHANDLE pDvDetectHandle);

//!
/*!
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \return
 */
DLL_PUBLIC const char *         dvdGetCode(LPDVDETECTHANDLE pDvDetectHandle);

//! Calculate the disk hash (md5 code)
/*!
 * The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \return Disk hash
 */
DLL_PUBLIC const char *         dvdGetHash(LPDVDETECTHANDLE pDvDetectHandle);

//! Get disk album artist
/*!
 * If DVD was successfully looked up, return the album artist. The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \return album artist or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetAlbumArtist(LPDVDETECTHANDLE pDvDetectHandle);

//! Set disk album artist
/*!
 * Sets the album artist
 *
 *  \param strAlbumArtist  const char * Album artist
 */
DLL_PUBLIC void                 dvdSetAlbumArtist(LPDVDETECTHANDLE pDvDetectHandle, const char * pszAlbumArtist);

//! Get disk album name
/*!
 * If DVD was successfully looked up, return the album. The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \return album or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetAlbum(LPDVDETECTHANDLE pDvDetectHandle);

//! Set disk album name
/*!
 * Sets the album name
 *
 *  \param strAlbum  const char * Album name
 */
DLL_PUBLIC void                 dvdSetAlbum(LPDVDETECTHANDLE pDvDetectHandle, const char * pszAlbum);

//! Get the DVD genre
/*!
 * If DVD was successfully looked up, return DVD genre. The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \return DVD genre or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetGenre(LPDVDETECTHANDLE pDvDetectHandle);

//! Set the DVD genre
/*!
 * Sets the DVD genre
 *
 *  \param strGenre  const char * DVD genre
 */
DLL_PUBLIC void                 dvdSetGenre(LPDVDETECTHANDLE pDvDetectHandle, const char * pszGenre);

// TODO: Cover arts
//std::string                   getCover(LPDVDETECTHANDLE pDvDetectHandle);
//void                          setCover(LPDVDETECTHANDLE pDvDetectHandle);

//! Get DVD movie cast
/*!
 * If DVD was successfully looked up, return DVD movie cast. The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \return DVD movie cast or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetCast(LPDVDETECTHANDLE pDvDetectHandle);

//! Set the DVD movie cast
/*!
 * Sets the DVD movie cast
 *
 *  \param strCast  const char * DVD movie cast
 */
DLL_PUBLIC void                 dvdSetCast(LPDVDETECTHANDLE pDvDetectHandle, const char * pszCast);

//! Get DVD movie crew
/*!
 * If DVD was successfully looked up, return DVD movie crew. The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \return DVD movie crew or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetCrew(LPDVDETECTHANDLE pDvDetectHandle);

//! Set the DVD movie crew
/*!
 * Sets the DVD movie crew
 *
 *  \param strCrew  const char * DVD movie crew
 */
DLL_PUBLIC void                 dvdSetCrew(LPDVDETECTHANDLE pDvDetectHandle, const char * pszCrew);

//! Get the DVD movie director
/*!
 * If DVD was successfully looked up, return DVD movie director. The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \return DVD movie director or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetDirector(LPDVDETECTHANDLE pDvDetectHandle);

//! Set the DVD movie director
/*!
 * Sets the DVD movie director
 *
 *  \param strDirector  const char * DVD movie director
 */
DLL_PUBLIC void                 dvdSetDirector(LPDVDETECTHANDLE pDvDetectHandle, const char * pszDirector);

//! Get country of origin
/*!
 * If DVD was successfully looked up, return country of origin. The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \return country of origin or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetCountry(LPDVDETECTHANDLE pDvDetectHandle);

//! Set the country of origin
/*!
 * Sets the country of origin
 *
 *  \param strCountry  const char * country of origin
 */
DLL_PUBLIC void                 dvdSetCountry(LPDVDETECTHANDLE pDvDetectHandle, const char * pszCountry);

//! Get release date
/*!
 * If DVD was successfully looked up, return release date. The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 * Returned in ISO format (YYYY-MM-DD HH:MM:SS).
 *
 *  \return release date or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetReleaseDate(LPDVDETECTHANDLE pDvDetectHandle);

//! Set release date
/*!
 * Sets the release date
 * Must be in ISO format (YYYY-MM-DD HH:MM:SS). Time can be left empty.
 *
 *  \param strReleaseDate  const char * release date
 */
DLL_PUBLIC void                 dvdSetReleaseDate(LPDVDETECTHANDLE pDvDetectHandle, const char * pszReleaseDate);

//! Get special feature list
/*!
 * If DVD was successfully looked up, return special feature list
 *
 *  \return Special feature list or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetSpecialFeatures(LPDVDETECTHANDLE pDvDetectHandle);

//! Set the special feature list
/*!
 * Sets the special feature list
 *
 *  \param strFeatures  const char * special feature list. The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 */
DLL_PUBLIC void                 dvdSetSpecialFeatures(LPDVDETECTHANDLE pDvDetectHandle, const char * pszFeatures);

//! Get EAN/UPC
/*!
 * If DVD was successfully looked up, return EAN/UPC
 *
 *  \return EAN/UPC or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetEAN_UPC(LPDVDETECTHANDLE pDvDetectHandle);

//! Set EAN/UPC
/*!
 * Sets the EAN/UPC
 *
 *  \param strEAN_UPC  const char * EAN/UPC. The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 */
DLL_PUBLIC void                 dvdSetEAN_UPC(LPDVDETECTHANDLE pDvDetectHandle, const char * pszEAN_UPC);

//! Get storyline/synopsis
/*!
 * If DVD was successfully looked up, return storyline/synopsis
 *
 *  \return storyline/synopsis or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetStoryline(LPDVDETECTHANDLE pDvDetectHandle);

//! Set storyline/synopsis
/*!
 * Sets the storyline/synopsis
 *
 *  \param strStoryline  const char * Storyline/synopsis
 */
DLL_PUBLIC void                 dvdSetStoryline(LPDVDETECTHANDLE pDvDetectHandle, const char * pszStoryline);

//! Get remarks
/*!
 * If DVD was successfully looked up, return remarks
 *
 *  \return remarks or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetRemarks(LPDVDETECTHANDLE pDvDetectHandle);

//! Set remarks
/*!
 * Sets the remarks
 *
 *  \param strRemarks  const char * Remarks
 */
DLL_PUBLIC void                 dvdSetRemarks(LPDVDETECTHANDLE pDvDetectHandle, const char * pszRemarks);

//! Get revision
/*!
 * If DVD was successfully looked up, return revision.
 *
 *  \return revision or 0 if unknown
 */
DLL_PUBLIC int                  dvdGetRevision(LPDVDETECTHANDLE pDvDetectHandle);

//! Set revision
/*!
 * Sets the revision
 *
 *  \param strRevision  const char * Revision
 */
DLL_PUBLIC void                 dvdSetRevision(LPDVDETECTHANDLE pDvDetectHandle, int nRevision);

//! Get submitter
/*!
 * If DVD was successfully looked up, return submitter. The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \return submitter or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetSubmitter(LPDVDETECTHANDLE pDvDetectHandle);

//! Set submitter
/*!
 * Sets the submitter
 *
 *  \param strSubmitter  const char * Submitter
 */
DLL_PUBLIC void                 dvdSetSubmitter(LPDVDETECTHANDLE pDvDetectHandle, const char * pszSubmitter);

//! Get submitter IP
/*!
 * If DVD was successfully looked up, return submitter IP. The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \return submitter IP or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetSubmitterIP(LPDVDETECTHANDLE pDvDetectHandle);

//! Get submission client
/*!
 * If DVD was successfully looked up, return submission client. The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \return submission client or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetClient(LPDVDETECTHANDLE pDvDetectHandle);

//! Set keywords
/*!
 * Sets the keywords
 *
 *  \param strKeywords const char * Keywords
 */
DLL_PUBLIC void                 dvdSetKeywords(LPDVDETECTHANDLE pDvDetectHandle, const char * pszKeywords);

//! Get keywords
/*!
 * If DVD was successfully looked up, return keywords. The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \return Keywords or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetKeywords(LPDVDETECTHANDLE pDvDetectHandle);

//! Get date created
/*!
 * If DVD was successfully looked up, return date created. The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \return date created or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetDateCreated(LPDVDETECTHANDLE pDvDetectHandle);

//! Get date last changed
/*!
 * If DVD was successfully looked up, return date last changed. The memory area is allocated. Remember to call dvdFree() when you are finished with it.
 *
 *  \return date last changed or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetDateLastChanged(LPDVDETECTHANDLE pDvDetectHandle);

//! Get title
/*!
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \param wTitleSetNo uint16_t
 *  \return title or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetPttVmgTitle(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo);

//! Get artist
/*!
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \param wTitleSetNo uint16_t
 *  \param wPtt uint16_t
 *  \return title or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetPttVtsArtist(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wPtt);

//! Get title
/*!
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \param wTitleSetNo uint16_t
 *  \param wPtt uint16_t
 *  \return title or empty string if unknown/NULL on error
 */
DLL_PUBLIC const char *         dvdGetPttVtsTitle(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wPtt);

//! Allocate memory
/*!
 * Allocates memory
 *
 *  \return Returns pointer to allocated memory, if successful
 *  \return NULL on error
 */
DLL_PUBLIC void *               dvdMalloc(size_t size);

//! Free memory
/*!
 * Previously by dvdMalloc() allocated memory. Do not pass a pointer allocated
 * by new or other methods.
 *
 *  \param ppMemory void ** Pointer to memory to be freed
 */
DLL_PUBLIC void                 dvdFree(void ** ppMemory);

/*! Get a string with the last error
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \return Last error or empty string of no error
 */
DLL_PUBLIC const char *         dvdGetErrorString(LPDVDETECTHANDLE pDvDetectHandle);

/*! Close the library handle
 *
 * Closes the library handle and frees all resources.
 * The handle cannot be used any longer after this call.
 *
 *  \param pDvDetectHandle LPDVDETECTHANDLE
 *  \return
 */
DLL_PUBLIC void                 dvdCloseLib(LPDVDETECTHANDLE pDvDetectHandle);

// TODO: Comments/doxygen!
DLL_PUBLIC LPDVDBHANDLE         dvdOpenDB(const char * pszPogramName);
DLL_PUBLIC void                 dvdSetDBProxy(LPDVDBHANDLE pDvDBHandle, const char * pszProxyName);
DLL_PUBLIC int                  dvdDBResults(LPDVDBHANDLE pDvDBHandle);
DLL_PUBLIC int                  dvdQueryDB(LPDVDBHANDLE pDvDBHandle, LPDVDETECTHANDLE pDvDetectHandle);
DLL_PUBLIC int                  dvdSearchDB(LPDVDBHANDLE pDvDBHandle, LPDVDETECTHANDLE pDvDetectHandle);
DLL_PUBLIC int                  dvdSubmitDB(LPDVDBHANDLE pDvDBHandle, LPDVDETECTHANDLE pDvDetectHandle);

//! Update album name etc.
/*!
 * Updates the album name, artist etc. from the parse data received.
 *
 *  \param dvdParse const dvdparse & Data received from server
 */
DLL_PUBLIC void                 dvdUpdate(LPDVDBHANDLE pDvDBHandle, LPDVDETECTHANDLE pDvDetectHandle);
DLL_PUBLIC void                 dvdCloseDB(LPDVDBHANDLE pDvDBHandle);

#ifdef __cplusplus
}
#endif

#endif // DVDETECT_H
