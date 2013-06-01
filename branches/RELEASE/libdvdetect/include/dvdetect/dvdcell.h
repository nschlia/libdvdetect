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

/** \file dvdcell.h
 *
 *  \brief dvdcell class
 */

#pragma once

#ifndef DVDCELL_H
#define DVDCELL_H

#include <dvdetect/dvdunit.h>

/*!
 *  @brief dvdcell class
 *
 * Contains all details about a DVD cell.
 */
class DLL_PUBLIC dvdcell : public dvdetectbase
{
    friend class dvdparse;

public:
    explicit dvdcell();
    virtual ~dvdcell();

    LPCDVDCELL          getDVDCELL() const;
    const dvdunit *     getDvdUnit(uint16_t wUnit) const;
    uint16_t            getUnitCount() const;

    uint64_t            getSize() const;

//    dvdcell& operator= (dvdcell const& rhs);

    virtual const std::type_info & classtype() const;

protected:
    DVDCELL             m_DVDCELL;
    dvdunitlst          m_dvdUnitLst;
};

typedef std::vector<dvdcell> dvdcelllst;

#endif // DVDCELL_H
