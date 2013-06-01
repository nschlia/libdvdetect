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

/** \file dvdptt.cpp
 *
 *  \brief dvdptt class
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>

#include "localutils.h"

dvdptt::dvdptt()
{
    memset(&m_DVDPTT, 0, sizeof(DVDPTT));
}

dvdptt::~dvdptt()
{
}

LPCDVDPTT dvdptt::getDVDPTT() const
{
    return &m_DVDPTT;
}

const dvdprogram * dvdptt::getDvdProgram(uint16_t wProgram) const
{
    if (!wProgram || wProgram > m_dvdProgramLst.size())
    {
        return NULL;
    }

    return &m_dvdProgramLst[wProgram - 1];
}

uint16_t dvdptt::getProgramCount() const
{
    return (uint16_t)m_dvdProgramLst.size();
}

const std::type_info & dvdptt::classtype() const
{
    return typeid(*this);
}
