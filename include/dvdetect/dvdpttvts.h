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

/*! \file dvdpttvts.h
 *
 *  \brief dvdpttvts class declaration
 *
 * Store information about a DVD part of title (PTT).
 */

#pragma once

#ifndef DVDPTTVTS_H

#define DVDPTTVTS_H


#include <dvdetect/dvdprogram.h>

class dvdparse;

/*!
 *  @brief dvdpttvts class
 *
 * This class stores all data about a "part of title" (PTT).
 */
class DLL_PUBLIC dvdpttvts : public dvdetectbase
{
    friend class dvdparse;

public:
    //! Constructor.
    /*!
     *  Construct a dvdpttvts element.
     */
    explicit dvdpttvts(dvdparse *pDvdParse);
    //! Destructor.
    /*!
     *  Destruct a dvdpttvts element.
     */
    virtual ~dvdpttvts();

    //! Access the DVDPTTVTS data structure.
    /*!
     *  \return Pointer to the underlying DVDPTTVTS structure.
     *  \return It is guaranteed that a valid structure is returned (never NULL).
     */
    LPCDVDPTTVTS        getDVDPTTVTS() const;

    //! Get a DVD chapter (aka program) in this part of title (PTT).
    /*!
     *  \param wChapterNo uint16_t Index of chapter (1...n)
     *  \return Success: Pointer to dvdprogram class.
     *  \return Fail: NULL if wChapterNo out of bounds.
     */
    const dvdprogram *  getDvdChapter() const;

    //! Get the number of programs in this part of title (PTT).
    /*!
     *  \return Number of chapters (1...n)
     */
    uint16_t            getChapterCount() const;

    //! Get the size (in bytes) of this virtual chapter.
    /*!
     *  \return Size (in bytes) of this virtual chapter.
     */
    uint64_t            getSize() const;

    //! Get the playtime (in ms) of this virtual chapter.
    /*!
     *  \return Playtime (in ms) of this virtual chapter.
     */
    uint64_t            getPlayTime() const;

//    dvdpttvts& operator= (dvdpttvts const& rhs);

    //! Get the type_info of this class.
    /*!
     *  \return type_info of this class.
     */
    virtual const std::type_info & classtype() const;

protected:
    DVDPTTVTS           m_DVDPTTVTS;            //!< DVDPTTVTS structure (DVD part of title)
    dvdparse *          m_pDvdParse;            //!< DVD parse object
};

typedef std::vector<dvdpttvts> dvdpttvtslst;	//!< shortcut for a list of dvdpttvts's

#endif // DVDPTTVTS_H
