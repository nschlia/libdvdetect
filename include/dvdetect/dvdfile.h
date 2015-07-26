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
    friend class xmldocparser;

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

    //! Get the corresponding filename.
    /*!
     *  \return Name of file
     */
    std::string			getFileName() const;

    dvdfile& operator= (dvdfile const & source);

protected:
    DVDFILE             m_DVDFILE;				//!< DVDFILE structure (DVD part of title)
};

typedef vector_ptr<dvdfile> dvdfilelst;         //!< shortcut for a list of dvdfiles

#endif // DVDFILE_H
