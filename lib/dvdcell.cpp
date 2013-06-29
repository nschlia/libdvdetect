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

/*! \file dvdcell.cpp
 *
 *  \brief dvdell class implementation
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>

#include "localutils.h"

dvdcell::dvdcell()
{
    memset(&m_DVDCELL, 0, sizeof(DVDCELL));
}

dvdcell::~dvdcell()
{
}

dvdcell& dvdcell::operator= (dvdcell const& rhs)
{
    if (this != &rhs)
    {
        memcpy(&m_DVDCELL, &rhs.m_DVDCELL, sizeof(DVDCELL));
        m_dvdUnitLst = rhs.m_dvdUnitLst;
    }
    return *this;
}

LPCDVDCELL dvdcell::getDVDCELL() const
{
    return &m_DVDCELL;
}

const dvdunit * dvdcell::getDvdUnit(uint16_t wUnit) const
{
    if (!wUnit || wUnit > m_dvdUnitLst.size())
    {
        return NULL;
    }

    return &m_dvdUnitLst[wUnit - 1];
}

uint16_t dvdcell::getUnitCount() const
{
    return (uint16_t)m_dvdUnitLst.size();
}

uint64_t dvdcell::getSize() const
{
    uint64_t size = 0;

    for (uint16_t wUnit = 1; wUnit <= getUnitCount(); wUnit++)
    {
        size += m_dvdUnitLst[wUnit - 1].getSize();
    }

    return size;

    //return (dvdSector2bytes((m_DVDCELL.m_dwLastVOBUEndSector - m_DVDCELL.m_dwFirstVOBUStartSector)));
}
