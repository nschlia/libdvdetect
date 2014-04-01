/*
  dvdetect DVD detection, analysis & DVDETECT lookup library

  Copyright (C) 2013-2014 Norbert Schlia <nschlia@dvdetect.de>

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

dvdcell& dvdcell::operator= (dvdcell const & source)
{
    if (this != &source)
    {
        memcpy(&m_DVDCELL, &source.m_DVDCELL, sizeof(DVDCELL));
        m_lstDvdUnit = source.m_lstDvdUnit;
    }
    return *this;
}

LPCDVDCELL dvdcell::getDVDCELL() const
{
    return &m_DVDCELL;
}

dvdunit *dvdcell::getDvdUnit(uint16_t wUnitNo) const
{
    if (!wUnitNo || wUnitNo > m_lstDvdUnit.size())
    {
        return NULL;
    }

    return m_lstDvdUnit[wUnitNo - 1];
}

uint16_t dvdcell::getUnitCount() const
{
    return (uint16_t)m_lstDvdUnit.size();
}

uint64_t dvdcell::getSize() const
{
    uint64_t size = 0;

    for (uint16_t wUnitNo = 1; wUnitNo <= getUnitCount(); wUnitNo++)
    {
        size += m_lstDvdUnit[wUnitNo - 1]->getSize();
    }

    return size;

    //return (dvdSector2bytes((m_DVDCELL.m_dwLastVOBUEndSector - m_DVDCELL.m_dwFirstVOBUStartSector)));
}
