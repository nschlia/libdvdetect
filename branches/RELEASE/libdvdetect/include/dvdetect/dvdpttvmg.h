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

/*! \file dvdpttvmg.h
 *
 *  \brief dvdpttvmg class declaration
 *
 * Store information about a DVD part of title (PTT).
 */

#pragma once

#ifndef DVDPTTVMG_H

#define DVDPTTVMG_H


#include <dvdetect/dvdprogram.h>

class dvdparse;

/*!
 *  @brief dvdpttvmg class
 *
 * This class stores all data about a "part of title" (PTT).
 */
class DLL_PUBLIC dvdpttvmg : public dvdetectbase
{
    friend class dvdparse;
    friend class xmldoc;

public:
    //! Constructor.
    /*!
     *  Construct a dvdpttvmg element.
     */
    explicit dvdpttvmg(dvdparse *pDvdParse);
    //! Destructor.
    /*!
     *  Destruct a dvdpttvmg element.
     */
    virtual ~dvdpttvmg();

    //! Access the DVDPTTVMG data structure.
    /*!
     *  \return Pointer to the underlying DVDPTTVMG structure.
     *  \return It is guaranteed that a valid structure is returned (never NULL).
     */
    LPCDVDPTTVMG        getDVDPTTVMG() const;

    //! Get a DVD title set of this part of title (PTT).
    /*!
     *  \return Success: Pointer to dvdtitle class
     *  \return Fail: NULL if not found.
     */
    const dvdtitle *    getDvdTitle() const;

    //! Get a DVD part of program (PTT) in this video manager PTT.
    /*!
     *  \param wPtt uint16_t Index of PTT (1...n)
     *  \return Success: Pointer to dvdpttvts class
     *  \return Fail: NULL if wPtt out of bounds.
     */
    const dvdpttvts *   getDvdPttVts(uint16_t wPtt) const;

    //! Get the number of PTTs in this video manager PTT.
    /*!
     *  \return Number of units (1...n)
     */
    uint16_t            getPttCount() const;

    //! Get the size (in bytes) of this virtual title.
    /*!
     *  \return Size (in bytes) of this virtual title.
     */
    uint64_t            getSize() const;

    //! Get the playtime (in ms) of this virtual title.
    /*!
     *  \return Playtime (in ms) of this virtual title.
     */
    uint64_t            getPlayTime() const;

    //! Get disk chapter title
    /*!
     * If DVD was successfully looked up, return the chapter title
     *
     *  \return chapter title or empty string if unknown
     */
    std::string         getTitle() const;

    //! Set chapter title
    /*!
     * Sets the chapter title
     *
     *  \param strTitle const std::string & chapter title
     */
    void                setTitle(const std::string & strTitle);

    dvdpttvmg& operator= (dvdpttvmg const& rhs);

protected:
    DVDPTTVMG           m_DVDPTTVMG;			//!< DVDPTTVMG structure (DVD part of title)
    dvdparse *          m_pDvdParse;			//!< Owner DVD parse object
    dvdpttvtslst        m_dvdPttVtsLst;			//!< list of DVD part-of-programs (PTT)

    // database data
    std::string         m_strTitle;             //!<
};

typedef std::vector<dvdpttvmg> dvdpttvmglst;	//!< shortcut for a list of dvdpttvmgs

#endif // DVDPTTVMG_H
