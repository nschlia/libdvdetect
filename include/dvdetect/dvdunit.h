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

/** \file dvdunit.h
 *
 *  \brief dvdunit class
 */

#pragma once

#ifndef DVDUNIT_H
#define DVDUNIT_H

/*!
 *  @brief dvdunit class
 *
 * This class stores all data about a unit.
 */
class DLL_PUBLIC dvdunit : public dvdetectbase
{
    friend class dvdparse;

public:
    explicit dvdunit();
    virtual ~dvdunit();

    LPCDVDUNIT          getDVDUNIT() const;

    uint64_t            getSize() const;

//    dvdunit& operator= (dvdunit const& rhs);

    virtual const std::type_info & classtype() const;

protected:
    DVDUNIT             m_DVDUNIT;
};

typedef std::vector<dvdunit> dvdunitlst;

#endif // DVDUNIT_H
