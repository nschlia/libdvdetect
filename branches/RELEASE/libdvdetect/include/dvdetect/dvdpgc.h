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

/** \file dvdpgc.h
 *
 *  \brief dvdpgc class
 */

#pragma once

#ifndef DVDPGC_H
#define DVDPGC_H

#include <dvdetect/dvdprogram.h>
#include <dvdetect/dvdptt.h>

/*!
 *  @brief dvdpgc class
 *
 * This class stores all data about a program chain (PGC).
 */
class DLL_PUBLIC dvdpgc : public dvdetectbase
{
    friend class dvdparse;

public:
    explicit dvdpgc();
    virtual ~dvdpgc();

    LPCDVDPGC           getDVDPGC() const;
    const dvdprogram *  getDvdProgram(uint16_t wProgram) const;
    uint16_t            getProgramCount() const;
    uint16_t            getCellCount() const;
    const dvdptt *      getDvdPtt(uint16_t wPtt) const;
    uint16_t            getPttCount() const;

    uint64_t            getSize() const;
    uint64_t            getPlayTime() const;

    virtual const std::type_info & classtype() const;

protected:
    DVDPGC              m_DVDPGC;
    dvdprogramlst       m_dvdProgramLst;
    dvdpttlst           m_dvdPttLst;
};

typedef std::vector<dvdpgc> dvdpgclst;

#endif // DVDPGC_H
