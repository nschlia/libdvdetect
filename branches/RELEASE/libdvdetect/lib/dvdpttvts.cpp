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

/*! \file dvdpttvts.cpp
 *
 *  \brief dvdpttvts class implementation
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>

#include "localutils.h"

dvdpttvts::dvdpttvts(dvdparse *pDvdParse)
    : m_pDvdParse(pDvdParse)
{
    memset(&m_DVDPTTVTS, 0, sizeof(DVDPTTVTS));
}

dvdpttvts::~dvdpttvts()
{
}

LPCDVDPTTVTS dvdpttvts::getDVDPTTVTS() const
{
    return &m_DVDPTTVTS;
}

const dvdprogram * dvdpttvts::getDvdChapter() const
{
    if (m_pDvdParse == NULL)
    {
        return NULL;
    }

    return m_pDvdParse->getDvdProgram(&m_DVDPTTVTS);
}

uint64_t dvdpttvts::getSize() const
{
    const dvdprogram *pDvdProgram = getDvdChapter();
    LPCDVDPROGRAM pDVDPROGRAM = pDvdProgram->getDVDPROGRAM();
    uint64_t size = 0;

    for (uint16_t wCell = 1; wCell <= pDVDPROGRAM->m_wCells; wCell++)
    {
        const dvdcell *pDvdCell = pDvdProgram->getDvdCell(wCell);
        LPCDVDCELL pDVDCELL = pDvdCell->getDVDCELL();

        if (pDVDCELL->m_eCellType == CELLTYPE_NORMAL || pDVDCELL->m_eCellType == CELLTYPE_FIRST)
        {
            // Only count normal cells and the first of an angle to avoid duplicate sizes
            size += pDvdCell->getSize();
        }
    }

    return size;
}

uint64_t dvdpttvts::getPlayTime() const
{
    const dvdprogram *pDvdProgram = getDvdChapter();
    LPCDVDPROGRAM pDVDPROGRAM = pDvdProgram->getDVDPROGRAM();
    uint64_t qwPlayTime = 0;

    for (uint16_t wCell = 1; wCell <= pDVDPROGRAM->m_wCells; wCell++)
    {
        const dvdcell *pDvdCell = pDvdProgram->getDvdCell(wCell);
        LPCDVDCELL pDVDCELL = pDvdCell->getDVDCELL();

        if (pDVDCELL->m_eCellType == CELLTYPE_NORMAL || pDVDCELL->m_eCellType == CELLTYPE_FIRST)
        {
            // Only count normal cells and the first of an angle to avoid duplicate times
            qwPlayTime += pDVDCELL->m_qwPlayTime;
        }
    }

    return qwPlayTime;
}

const std::type_info & dvdpttvts::classtype() const
{
    return typeid(*this);
}
