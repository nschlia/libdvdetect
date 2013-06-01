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

/** \file dvdptt.h
 *
 *  \brief dvdptt class
 */

#pragma once

#ifndef DVDPTT_H
#define DVDPTT_H

#include <dvdetect/dvdprogram.h>

/*!
 *  @brief dvdptt class
 *
 * This class stores all data about a "part of program" (PTT).
 */
class DLL_PUBLIC dvdptt : public dvdetectbase
{
    friend class dvdparse;

public:
    explicit dvdptt();
    virtual ~dvdptt();

    LPCDVDPTT           getDVDPTT() const;
    const dvdprogram *  getDvdProgram(uint16_t wProgram) const;
    uint16_t            getProgramCount() const;

    virtual const std::type_info & classtype() const;

protected:
    DVDPTT              m_DVDPTT;
    dvdprogramlst       m_dvdProgramLst;
};

typedef std::vector<dvdptt> dvdpttlst;

#endif // DVDPTT_H
