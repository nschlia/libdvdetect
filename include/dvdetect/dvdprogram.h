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

/** \file dvdprogram.h
 *
 *  \brief dvdprogram class
 */

#pragma once

#ifndef DVDPROGRAM_H
#define DVDPROGRAM_H

#include <dvdetect/dvdcell.h>
#include <dvdetect/dvdfile.h>

/*!
 *  @brief dvdprogram class
 *
 * This class stores all data about a program chain (aka "chapter").
 */
class DLL_PUBLIC dvdprogram : public dvdetectbase
{
    friend class dvdparse;

public:
    explicit dvdprogram();
    virtual ~dvdprogram();

    LPCDVDPROGRAM       getDVDPROGRAM() const;
    const dvdcell *     getDvdCell(uint16_t wCell) const;
    uint16_t            getCellCount() const;

    uint64_t            getSize() const;
    uint64_t            getPlayTime() const;
    uint32_t            getStartSector() const;
    uint32_t            getEndSector() const;

    virtual const std::type_info & classtype() const;

protected:
    DVDPROGRAM          m_DVDPROGRAM;
    dvdcelllst          m_dvdCellLst;
};

typedef std::vector<dvdprogram> dvdprogramlst;

#endif // DVDPROGRAM_H
