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

/*! \file dvdpgc.cpp
 *
 *  \brief dvdpgc class implementation
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>

#include "localutils.h"

dvdpgc::dvdpgc()
{
    memset(&m_DVDPGC, 0, sizeof(DVDPGC));
}

dvdpgc::~dvdpgc()
{
}

dvdpgc& dvdpgc::operator= (dvdpgc const& rhs)
{
    if (this != &rhs)
    {
        memcpy(&m_DVDPGC, &rhs.m_DVDPGC, sizeof(DVDPGC));
        m_dvdProgramLst = rhs.m_dvdProgramLst;
    }
    return *this;
}

LPCDVDPGC dvdpgc::getDVDPGC() const
{
    return &m_DVDPGC;
}

const dvdprogram * dvdpgc::getDvdProgram(uint16_t wProgram) const
{
    if (!wProgram || wProgram > m_dvdProgramLst.size())
    {
        return NULL;
    }

    return &m_dvdProgramLst[wProgram - 1];
}

uint16_t dvdpgc::getProgramCount() const
{
    return m_DVDPGC.m_wNumberOfPrograms;
}

uint16_t dvdpgc::getCellCount() const
{
    return m_DVDPGC.m_wNumberOfCells;
}

uint64_t dvdpgc::getSize() const
{
    uint64_t size = 0;

    for (uint16_t wProgram = 1; wProgram <= m_DVDPGC.m_wNumberOfPrograms; wProgram++)
    {
        size += m_dvdProgramLst[wProgram - 1].getSize();
    }

    return size;
}

uint64_t dvdpgc::getPlayTime() const
{
    uint64_t qwPlayTime = 0;

    for (uint16_t wProgram = 1; wProgram <= m_DVDPGC.m_wNumberOfPrograms; wProgram++)
    {
        qwPlayTime += m_dvdProgramLst[wProgram - 1].getPlayTime();
    }

    return qwPlayTime;
}
