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

/*! \file dvdutils.h
 *
 *  \brief Public utility collection
 *
 * Several handy utility functions to be used in accessing DVD structure data.
 * These are created as C style functions (e.g., no C++ decoration) so the same
 * they can also be used directly as part of the C API.
 */

#pragma once

#ifndef DVDUTILS_H
#define DVDUTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Convert sector (LBA/Logical Block Adress) to bytes
 *
 * \param dwSectorAddress uint32_t
 * \return Adress in bytes
 */

DLL_PUBLIC uint64_t dvdSector2bytes(uint32_t dwSectorAddress);

/*!
 * Get file extension for DVD file type
 *
 * \param eFileType DVDFILETYPE
 * \return File extension string (e.g. "IFO")
 */

DLL_PUBLIC const char * dvdGetExtension(DVDFILETYPE eFileType);

/*!
 * Get textual description of DVD file type
 *
 * \param eFileType DVDFILETYPE
 * \return File type string (e.g. "VMG/VOB")
 */

DLL_PUBLIC const char * dvdGetFileType(DVDFILETYPE eFileType);

//! Get a DVD file name
/*!
     * \param eFileType DVDFILETYPE File type, e.g. VIDEO_TS.IFO
     * \param wTitleSetNo uint16_t For VTS_* files, number of title set
     * \param wVtsNo  uint16_t For VTS_* files, number of IF or VOB
     * \return File name
     */
DLL_PUBLIC void         dvdGetFileName(DVDFILETYPE eFileType, uint16_t wTitleSetNo, uint16_t wVtsNo, char * pszFileName, size_t maxlen);

#ifdef __cplusplus
}
#endif

#endif // DVDUTILS_H
