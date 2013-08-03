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

/*! \file dvdpgc.h
 *
 *  \brief dvdpgc class declaration
 *
 * Store information about a DVD program chain (PGC).
 */

#pragma once

#ifndef DVDPGC_H
#define DVDPGC_H

#include <dvdetect/dvdprogram.h>
#include <dvdetect/dvdpttvts.h>

/*!
 *  @brief dvdpgc class
 *
 * This class stores all data about a program chain (PGC).
 */
class DLL_PUBLIC dvdpgc : public dvdetectbase
{
    friend class dvdparse;
    friend class xmldocbuilder;
    friend class xmldocparser;

public:
    //! Constructor.
    /*!
     *  Construct a dvdpgc element.
     */
    explicit dvdpgc();
    //! Destructor.
    /*!
     *  Destruct a dvdpgc element.
     */
    virtual ~dvdpgc();

    //! Access the DVDPGC data structure.
    /*!
     *  \return Pointer to the underlying DVDPGC structure.
     *  \return It is guaranteed that a valid structure is returned (never NULL).
     */
    LPCDVDPGC           getDVDPGC() const;

    //! Get a DVD program in this program chain (PGC).
    /*!
     *  \param wProgramNo uint16_t Index of program (1...n)
     *  \return Success: Pointer to dvdprogram class;
     *  \return Fail: NULL if wProgramNo out of bounds.
     */
    dvdprogram *        getDvdProgram(uint16_t wProgramNo) const;

    //! Get the number of programs in this program chain (PGC).
    /*!
     *  \return Number of programs (1...n)
     */
    uint16_t            getProgramCount() const;

    //! Get the size (in bytes) of this program chain (PGC).
    /*!
     *  \return Size (in bytes) of this program chain (PGC).
     */
    uint64_t            getSize() const;

    //! Get the playtime (in ms) of this program chain (PGC).
    /*!
     *  \return Playtime (in ms) of this program chain (PGC).
     */
    uint64_t            getPlayTime() const;

    dvdpgc& operator= (dvdpgc const & source);

protected:
    DVDPGC              m_DVDPGC;				//!< DVDPGC structure (DVD program chain information)
    dvdprogramlst       m_lstDvdProgram;		//!< list of DVD programs of this PGC
};

typedef vector_ptr<dvdpgc> dvdpgclst;			//!< shortcut for a list of dvdpgcs

#endif // DVDPGC_H
