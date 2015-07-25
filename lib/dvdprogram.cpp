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

dvdprogram& dvdprogram::operator= (dvdprogram const & source)
{
    if (this != &source)
    {
        memcpy(&m_DVDPROGRAM, &source.m_DVDPROGRAM, sizeof(DVDPROGRAM));
        m_lstDvdCell = source.m_lstDvdCell;
    }
    return *this;
}

LPCDVDPROGRAM dvdprogram::getDVDPROGRAM() const
{
    return &m_DVDPROGRAM;
}

dvdcell * dvdprogram::getDvdCell(uint16_t wCellNo) const
{
    if (!wCellNo || wCellNo > m_lstDvdCell.size())
    {
        return NULL;
    }

    return m_lstDvdCell[wCellNo - 1];
}

uint16_t dvdprogram::getCellCount() const
{
    return m_lstDvdCell.size();
}

uint64_t dvdprogram::getSize() const
{
    uint64_t size = 0;

    for (uint16_t wCellNo = 1; wCellNo <= getCellCount(); wCellNo++)
    {
        const dvdcell *pDvdCell = getDvdCell(wCellNo);

        if (pDvdCell == NULL)	// Safety net
        {
            break;
        }

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

    for (uint16_t wCellNo = 1; wCellNo <= getCellCount(); wCellNo++)
    {
        const dvdcell *pDvdCell = getDvdCell(wCellNo);
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
    if (!m_lstDvdCell.size())
    {
        return 0;
    }
    return m_lstDvdCell[0]->getDVDCELL()->m_dwFirstVOBUStartSector;
}

uint32_t dvdprogram::getEndSector() const
{
    int nCell = m_lstDvdCell.size();
    if (!nCell)
    {
        return 0;
    }
    return m_lstDvdCell[nCell- 1]->getDVDCELL()->m_dwLastVOBUEndSector;
}
