/*
  dvdetect DVD detection, analysis & DVDETECT lookup library

  Copyright (C) 2013-2014 Norbert Schlia <nschlia@dvdetect.de>

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

/*! \file dvdprogram.h
 *
 *  \brief dvdprogram class declaration
 *
 * Store information about a DVD program.
 */

#pragma once

#ifndef DVDPROGRAM_H
#define DVDPROGRAM_H

#include <dvdetect/dvdcell.h>
#include <dvdetect/dvdfile.h>

/*!
 *  @brief dvdprogram class
 *
 * This class stores all data about a program chain (aka "chapter").
 */
class DLL_PUBLIC dvdprogram : public dvdetectbase
{
    friend class dvdparse;
    friend class xmldocbuilder;
    friend class xmldocparser;

public:
    //! Constructor.
    /*!
     *  Construct a dvdprogram element.
     */
    explicit dvdprogram();
    //! Destructor.
    /*!
     *  Destruct a dvdprogram element.
     */
    virtual ~dvdprogram();

    //! Access the DVDPROGRAM data structure.
    /*!
     *  \return Pointer to the underlying DVDPROGRAM structure.
     *  \return It is guaranteed that a valid structure is returned (never NULL).
     */
    LPCDVDPROGRAM       getDVDPROGRAM() const;

    //! Get a DVD cell in this program.
    /*!
     *  \param wCellNo uint16_t Index of cell (1...n)
     *  \return Success: Pointer to dvdcell class
     *  \return Fail: NULL if wCellNo out of bounds.
     */
    dvdcell *           getDvdCell(uint16_t wCellNo) const;

    //! Get the number of cells in this program.
    /*!
     *  \return Number of cells (1...n)
     */
    uint16_t            getCellCount() const;

    //! Get the size (in bytes) of this program.
    /*!
     *  \return Size (in bytes) of this program.
     */
    uint64_t            getSize() const;

    //! Get the playtime (in ms) of this program.
    /*!
     *  \return Playtime (in ms) of this program.
     */
    uint64_t            getPlayTime() const;

    //! Get the start sector (LBA/Logical Block Adress) of this program.
    /*!
     *  \return Start sector (LBA/Logical Block Adress) of this program.
     */
    uint32_t            getStartSector() const;

    //! Get the end sector (LBA/Logical Block Adress) of this program.
    /*!
     *  \return end sector (LBA/Logical Block Adress) of this program.
     */
    uint32_t            getEndSector() const;

    dvdprogram& operator= (dvdprogram const & source);

protected:
    DVDPROGRAM          m_DVDPROGRAM;				//!< DVDPROGRAM structure (DVD program)
    dvdcelllst          m_lstDvdCell;				//!< list of DVD cells of this program
};

typedef vector_ptr<dvdprogram> dvdprogramlst;		//!< shortcut for a list of dvdprograms

#endif // DVDPROGRAM_H
