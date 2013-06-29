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

/*! \file dvdetectbase.h
 *
 *  \brief dvdetectbase class declaration
 *
 * Base class for all dvd* classes. Provides type information..
 */

#pragma once

#ifndef DVDETECTBASE_H
#define DVDETECTBASE_H

#include <typeinfo>

/*!
 *  @brief libdvdetect abstract base class
 *
 * All libdvdetect classes derive from this class. Cannot be instanciated
 * by itself (abstract class).
 * Derived classes should implement a classtype member funtion to
 * identify themselves.
 */
class DLL_PUBLIC dvdetectbase
{
public:
    //! Constructor.
    /*!
     *  Construct a dvdtitle element.
     */
    explicit dvdetectbase();
    //! Destructor.
    /*!
     *  Destruct a dvdtitle element.
     */
    virtual ~dvdetectbase();

    //! Get the type_info of this class.
    /*!
     *  \return type_info of this class.
     */
    virtual const std::type_info & classtype() const;
};

#endif // DVDETECTBASE_H
