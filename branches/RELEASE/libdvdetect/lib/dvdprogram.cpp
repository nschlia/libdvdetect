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

/*! \file dvdprogram.cpp
 *
 *  \brief dvdprogram class implementation
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>

#include "localutils.h"

dvdprogram::dvdprogram()
{
    memset(&m_DVDPROGRAM, 0, sizeof(DVDPROGRAM));
}

dvdprogram::~dvdprogram()
{
}

dvdprogram& dvdprogram::operator= (dvdprogram const& rhs)
{
    if (this != &rhs)
    {
        memcpy(&m_DVDPROGRAM, &rhs.m_DVDPROGRAM, sizeof(DVDPROGRAM));
        m_dvdCellLst = rhs.m_dvdCellLst;
    }
    return *this;
}

LPCDVDPROGRAM dvdprogram::getDVDPROGRAM() const
{
    return &m_DVDPROGRAM;
}

const dvdcell * dvdprogram::getDvdCell(uint16_t wCell) const
{
    if (!wCell || wCell > m_dvdCellLst.size())
    {
        return NULL;
    }

    return &m_dvdCellLst[wCell - 1];
}

uint16_t dvdprogram::getCellCount() const
{
    return m_DVDPROGRAM.m_wCells;
}

uint64_t dvdprogram::getSize() const
{
    uint64_t size = 0;

    for (uint16_t wCell = 1; wCell <= m_DVDPROGRAM.m_wCells; wCell++)
    {
        const dvdcell *pDvdCell = getDvdCell(wCell);
        LPCDVDCELL pDVDCELL = pDvdCell->getDVDCELL();

        if (pDVDCELL->m_eCellType == CELLTYPE_NORMAL || pDVDCELL->m_eCellType == CELLTYPE_FIRST)
        {
            // Only count normal cells and the first of an angle to avoid duplicate sizes
            size += pDvdCell->getSize();
        }
    }

    return size;
}

uint64_t dvdprogram::getPlayTime() const
{
    uint64_t qwPlayTime = 0;

    for (uint16_t wCell = 1; wCell <= m_DVDPROGRAM.m_wCells; wCell++)
    {
        const dvdcell *pDvdCell = getDvdCell(wCell);
        LPCDVDCELL pDVDCELL = pDvdCell->getDVDCELL();

        if (pDVDCELL->m_eCellType == CELLTYPE_NORMAL || pDVDCELL->m_eCellType == CELLTYPE_FIRST)
        {
            // Only count normal cells and the first of an angle to avoid duplicate times
            qwPlayTime += pDVDCELL->m_qwPlayTime;
        }
    }

    return qwPlayTime;
}

uint32_t dvdprogram::getStartSector() const
{
    if (!m_dvdCellLst.size())
    {
        return 0;
    }
    return m_dvdCellLst[0].getDVDCELL()->m_dwFirstVOBUStartSector;
}

uint32_t dvdprogram::getEndSector() const
{
    int nCell = m_dvdCellLst.size();
    if (!nCell)
    {
        return 0;
    }
    return m_dvdCellLst[nCell- 1].getDVDCELL()->m_dwLastVOBUEndSector;
}
