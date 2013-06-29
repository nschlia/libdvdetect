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

/*! \file dvdcell.h
 *
 *  \brief dvdcell class declaration
 *
 * Store information about a DVD cell.
 */

#pragma once

#ifndef DVDCELL_H
#define DVDCELL_H

#include <dvdetect/dvdunit.h>

/*!
 *  @brief dvdcell class
 *
 * Contains all details about a DVD cell.
 */
class DLL_PUBLIC dvdcell : public dvdetectbase
{
    friend class dvdparse;
    friend class xmldoc;

public:
    //! Constructor.
    /*!
     *  Construct a dvdcell element.
     */
    explicit dvdcell();
    //! Destructor.
    /*!
     *  Destruct a dvdcell element.
     */
    virtual ~dvdcell();

    //! Access the DVDCELL data structure.
    /*!
     *  \return Pointer to the underlying DVDCELL structure.
     *  \return It is guaranteed that a valid structure is returned (never NULL).
     */
    LPCDVDCELL          getDVDCELL() const;

    //! Get a DVD unit in this cell.
    /*!
     *  \param wUnit uint16_t Index of unit (1...n)
     *  \return Success: Pointer to dvdunit class
     *  \return Fail: NULL if wUnit out of bounds.
     */
    const dvdunit *     getDvdUnit(uint16_t wUnit) const;

    //! Get the number of units in this cell.
    /*!
     *  \return Number of units (1...n)
     */
    uint16_t            getUnitCount() const;

    //! Get the size (in bytes) of this cell.
    /*!
     *  \return Size (in bytes) of this cell.
     */
    uint64_t            getSize() const;

    dvdcell& operator= (dvdcell const& rhs);

protected:
    DVDCELL             m_DVDCELL;			//!< DVDCELL structure (DVD cell information)
    dvdunitlst          m_dvdUnitLst;		//!< list of units of this cell
};

typedef std::vector<dvdcell> dvdcelllst;	//!< shortcut for a list of dvdcells

#endif // DVDCELL_H
