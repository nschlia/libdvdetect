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
 * \param wVtsNo uint16_t For VTS_* files, number of IF or VOB
 * \param pszFileName char * Buffer to receive filename
 * \param maxlen size_t Max. size in bytes of buffer
 * \return File name
 */
DLL_PUBLIC void         dvdGetFileName(DVDFILETYPE eFileType, uint16_t wTitleSetNo, uint16_t wVtsNo, char * pszFileName, size_t maxlen);

//! Open file
/*!
 *  Open file from disk or web
 *
 * \param filename const char * Filename to open
 * \param mode const char * Mode (see fopen) for disk files; web files ignore this value and open in "rb" mode
 * \param proxy const char * Null or proxy name/port for web files
 * \return Success: Pointer to an object that specifies an input stream
 * \return Fail: returns NULL
 */
DLL_PUBLIC void *       openFile(const char *filename, const char *mode, const char *proxy);

//! Read data from file.
/*!
 * Read data from file. The file can be on disk or a file on a web server.
 *
 * \param buffer void* Pointer to a block of memory with a size of at least (size*count) bytes, converted to a void*
 * \param size size_t Size, in bytes, of each element to be read.
 * \param count size_t Number of elements, each one with a size of size bytes.
 * \param stream void* Pointer to an object that specifies an input stream.
 * \return The total number of elements successfully read is returned.
 */
DLL_PUBLIC size_t       readFile(void* buffer, size_t size, size_t count, void* stream);

//! Write data to file
/*!
 * Write data to file. This function only supports disk files.
 *
 * \param buffer const void* Pointer to the array of elements to be written, converted to a const void*.
 * \param size size_t Size in bytes of each element to be written.
 * \param count size_t Number of elements, each one with a size of size bytes.
 * \param stream void* Pointer to an object that specifies an output stream.
 * \return The total number of elements successfully written is returned.
 */
DLL_PUBLIC int          writeFile(const void* buffer, size_t size, size_t count, void* stream);

//! Closes a file
/*!
 * Closes the file defined by the stream.
 *
 * \param stream void* Pointer to an object that specifies a stream.
 * \return Success: If the stream is successfully closed, a zero value is returned.
 * \return Fail: On failure, EOF is returned.
 */
DLL_PUBLIC int          closeFile(void *stream);

//! Get file stats
/*!
 * Get file stats for the file defined by the file name.
 *
 * \param filename const char * Filename to check
 * \param pFileStat LPDVDFILESTAT Structure to receive status data
 * \param proxy const char * Null or proxy name/port for web files
 * \return Success: On success, zero is returned.
 * \return Fail: On error, -1 is returned, and errno is set appropriately.
 */
DLL_PUBLIC int          statFile(const char *filename, LPDVDFILESTAT pFileStat, const char *proxy);

//! Get file stats
/*!
 * Get file stats for the file defined by the stream pointer.
 *
 * \param stream void* Pointer to an object that specifies a stream.
 * \param pFileStat LPDVDFILESTAT Structure to receive status data
 * \return Success: On success, zero is returned.
 * \return Fail: On error, -1 is returned, and errno is set appropriately.
 */
DLL_PUBLIC int          fstatFile(void* stream, LPDVDFILESTAT pFileStat);

#ifdef __cplusplus
}
#endif

#endif // DVDUTILS_H
