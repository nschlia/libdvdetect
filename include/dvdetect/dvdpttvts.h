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
    friend class xmldocbuilder;
    friend class xmldocparser;

public:
    //! Constructor.
    /*!
     *  Construct a dvdpttvts element.
     */
    explicit dvdpttvts(dvdparse *pDvdParse = NULL);
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
     *  \return Success: Pointer to dvdprogram class.
     *  \return Fail: NULL if not class initialised/no DVD parsed.
     */
    dvdprogram *        getDvdChapter() const;

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

    //! Get title artist
    /*!
     * If DVD was successfully looked up, return the title artist
     *
     *  \return title artist or empty string if unknown
     */
    const std::string & getArtist() const;

    //! Set title artist
    /*!
     * Sets the title artist
     *
     *  \param strArtist const std::string & title artist
     */
    void                setArtist(const std::string & strArtist);

    //! Get title
    /*!
     * If DVD was successfully looked up, return the title
     *
     *  \return title or empty string if unknown
     */
    const std::string & getTitle() const;

    //! Set title
    /*!
     * Sets the title
     *
     *  \param strTitle const std::string & Title
     */
    void                setTitle(const std::string & strTitle);

    dvdpttvts& operator= (dvdpttvts const & source);

protected:
    DVDPTTVTS           m_DVDPTTVTS;            //!< DVDPTTVTS structure (DVD part of title)
    dvdparse *          m_pDvdParse;            //!< Owner DVD parse object

    // database data
    std::string         m_strArtist;            //!< Title artist
    std::string         m_strTitle;             //!< Title
};

typedef vector_ptr<dvdpttvts> dvdpttvtslst;     //!< shortcut for a list of dvdpttvts's

#endif // DVDPTTVTS_H
