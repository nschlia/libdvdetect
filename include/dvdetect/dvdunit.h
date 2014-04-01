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

/*! \file dvdunit.h
 *
 *  \brief dvdunit class declaration
 *
 * Store information about a DVD unit.
 */

#pragma once

#ifndef DVDUNIT_H
#define DVDUNIT_H

/*!
 * \brief dvdunit class
 *  This class stores all data about a unit.
 */
class DLL_PUBLIC dvdunit : public dvdetectbase
{
    friend class dvdparse;
    friend class xmldocbuilder;
    friend class xmldocparser;

public:
    //! Constructor.
    /*!
     *  Construct a dvdunit element.
     */
    explicit dvdunit();
    //! Destructor.
    /*!
     *  Destruct a dvdunit element.
     */
    virtual ~dvdunit();

    //! Access the DVDUNIT data structure.
    /*!
     *  \return Pointer to the underlying DVDUNIT structure.
     *  \return It is guaranteed that a valid structure is returned (never NULL).
     */
    LPCDVDUNIT          getDVDUNIT() const;

    //! Get the size (in bytes) of this unit.
    /*!
     *  \return Size (in bytes) of this unit.
     */
    uint64_t            getSize() const;

    dvdunit& operator= (dvdunit const & source);

protected:
    DVDUNIT             m_DVDUNIT;          //!< DVDUNIT structure (DVD unit information)
};

typedef vector_ptr<dvdunit> dvdunitlst;     //!< shortcut for a list of dvdunits

#endif // DVDUNIT_H
