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

/*! \file dvdfile.h
 *
 *  \brief dvdfile class declaration
 *
 * Store information about a DVD file.
 */

#pragma once

#ifndef DVDFILE_H
#define DVDFILE_H

/*!
 *  @brief dvdfile class
 *
 * Contains all details about a DVD file (e.g, VIDEO_VTS.IFO, VTS_04_1.VOB etc.)
 */
class DLL_PUBLIC dvdfile : public dvdetectbase
{
    friend class dvdparse;

public:
    //! Constructor.
    /*!
     *  Construct a dvdfile element.
     */
    explicit dvdfile();
    //! Destructor.
    /*!
     *  Destruct a dvdfile element.
     */
    virtual ~dvdfile();

    //! Access the DVDFILE data structure.
    /*!
     *  \return Pointer to the underlying DVDFILE structure.
     *  \return It is guaranteed that a valid structure is returned (never NULL).
     */
    LPCDVDFILE          getDVDFILE() const;

    dvdfile& operator= (dvdfile const& rhs);

protected:
    DVDFILE             m_DVDFILE;				//!< DVDFILE structure (DVD part of title)
};

typedef std::vector<dvdfile> dvdfilelst;		//!< shortcut for a list of dvdfiles

#endif // DVDFILE_H
