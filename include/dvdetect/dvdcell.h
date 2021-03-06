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
    friend class xmldocbuilder;
    friend class xmldocparser;

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
     *  \param wUnitNo uint16_t Index of unit (1...n)
     *  \return Success: Pointer to dvdunit class
     *  \return Fail: NULL if wUnitNo out of bounds.
     */
    dvdunit *           getDvdUnit(uint16_t wUnitNo) const;

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

    dvdcell& operator= (dvdcell const & source);

protected:
    DVDCELL             m_DVDCELL;			//!< DVDCELL structure (DVD cell information)
    dvdunitlst          m_lstDvdUnit;		//!< list of units of this cell
};

typedef vector_ptr<dvdcell> dvdcelllst;     //!< shortcut for a list of dvdcells

#endif // DVDCELL_H
