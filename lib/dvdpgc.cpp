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

dvdpgc& dvdpgc::operator= (dvdpgc const & source)
{
    if (this != &source)
    {
        memcpy(&m_DVDPGC, &source.m_DVDPGC, sizeof(DVDPGC));
        m_lstDvdProgram = source.m_lstDvdProgram;
    }
    return *this;
}

LPCDVDPGC dvdpgc::getDVDPGC() const
{
    return &m_DVDPGC;
}

dvdprogram * dvdpgc::getDvdProgram(uint16_t wProgramNo) const
{
    if (!wProgramNo || wProgramNo > getProgramCount())
    {
        return NULL;
    }

    return m_lstDvdProgram[wProgramNo - 1];
}

uint16_t dvdpgc::getProgramCount() const
{
    return m_lstDvdProgram.size();
}

uint64_t dvdpgc::getSize() const
{
    uint64_t size = 0;

    for (uint16_t wProgramNo = 1; wProgramNo <= getProgramCount(); wProgramNo++)
    {
        size += m_lstDvdProgram[wProgramNo - 1]->getSize();
    }

    return size;
}

uint64_t dvdpgc::getPlayTime() const
{
    uint64_t qwPlayTime = 0;

    for (uint16_t wProgramNo = 1; wProgramNo <= getProgramCount(); wProgramNo++)
    {
        qwPlayTime += m_lstDvdProgram[wProgramNo - 1]->getPlayTime();
    }

    return qwPlayTime;
}
