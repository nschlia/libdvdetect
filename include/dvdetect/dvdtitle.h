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

/*! \file dvdtitle.h
 *
 *  \brief dvdtitle class declaration
 *
 * Store information about a DVD title.
 */

#pragma once

#ifndef DVDTITLE_H
#define DVDTITLE_H

#include <vector>

#include <dvdetect/dvdpgc.h>

/*!
 *  @brief dvdtitle class
 *
 * This class stores all data about a DVD title set.
 */
class DLL_PUBLIC dvdtitle : public dvdetectbase
{
    friend class dvdparse;
    friend class xmldocbuilder;
    friend class xmldocparser;

public:
    //! Constructor.
    /*!
     *  Construct a dvdtitle element.
     */
    explicit dvdtitle();
    //! Destructor.
    /*!
     *  Destruct a dvdtitle element.
     */
    virtual ~dvdtitle();

    //! Access the DVDVTS (video title set) data structure.
    /*!
     *  \return Pointer to the underlying DVDVTS (video title set) structure.
     *  \return It is guaranteed that a valid structure is returned (never NULL).
     */
    LPCDVDVTS           getDVDVTS() const;

    //! Get a DVD program chain (PGC) in this title set.
    /*!
     *  \param wProgramChainNo uint16_t Index of program chain (PGC) (1...n)
     *  \return Success: Pointer to dvdpgc class
     *  \return Fail: NULL if wProgramChainNo out of bounds.
     */
    dvdpgc *            getDvdPgc(uint16_t wProgramChainNo) const;

    //! Get the number of program chains (PGC) in this title set.
    /*!
     *  \return Number of program chains (PGC) (1...n)
     */
    uint16_t            getPgcCount() const;

    //! Get a DVD file in this title set.
    /*!
     *  \param wFileNo uint16_t Index of file (1...n)
     *  \return Success: Pointer to wFileNo class
     *  \return Fail: NULL if wFileNo out of bounds.
     */
    dvdfile *           getDvdFile(uint16_t wFileNo) const;

    //! Get the number of files in this title set.
    /*!
     *  \return Number of files (1...n)
     */
    uint16_t            getFileCount() const;

    //! Get the size (in bytes) of this title set.
    /*! Get the size (in bytes) of this title set.
     * This is not the combined size of all VOBS. This is calculated
     * by the size of all cells in this program, so this may (and will)
     * differ from the file size, but reflect the size of the actual
     * title.
     *
     *  \return Size (in bytes) of this title set.
     */
    uint64_t            getSize() const;

    //! Get the playtime (in ms) of this title set.
    /*!
     *  \return Playtime (in ms) of this title set.
     */
    uint64_t            getPlayTime() const;

    dvdtitle& operator= (dvdtitle const & source);

protected:
    DVDVTS              m_DVDVTS;				//!< DVDVTS structure (DVD video title set)
    dvdpgclst           m_lstDvdPgc;			//!< list of DVD pogram chains (PGC) in this title set
    dvdfilelst          m_lstDvdFile;			//!< list of DVD files in this title set
};

typedef vector_ptr<dvdtitle> dvdtitlelst;		//!< shortcut for a list of dvdtitles

#endif // DVDTITLE_H
