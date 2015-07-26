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

dvdpttvts& dvdpttvts::operator= (dvdpttvts const & source)
{
    if (this != &source)
    {
        memcpy(&m_DVDPTTVTS, &source.m_DVDPTTVTS, sizeof(DVDPTTVTS));
        m_pDvdParse     = source.m_pDvdParse;
        m_strArtist     = source.m_strArtist;
        m_strTitle      = source.m_strTitle;
    }
    return *this;
}

LPCDVDPTTVTS dvdpttvts::getDVDPTTVTS() const
{
    return &m_DVDPTTVTS;
}

dvdprogram * dvdpttvts::getDvdChapter() const
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
    uint64_t size = 0;

    if (pDvdProgram == NULL)	// Safety net
    {
        return 0;
    }

    for (uint16_t wCellNo = 1; wCellNo <= pDvdProgram->getCellCount(); wCellNo++)
    {
        const dvdcell *pDvdCell = pDvdProgram->getDvdCell(wCellNo);
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
    uint64_t qwPlayTime = 0;

    if (pDvdProgram == NULL)	// Safety net
    {
        return 0;
    }

    for (uint16_t wCellNo = 1; wCellNo <= pDvdProgram->getCellCount(); wCellNo++)
    {
        const dvdcell *pDvdCell = pDvdProgram->getDvdCell(wCellNo);
        LPCDVDCELL pDVDCELL = pDvdCell->getDVDCELL();

        if (pDVDCELL->m_eCellType == CELLTYPE_NORMAL || pDVDCELL->m_eCellType == CELLTYPE_FIRST)
        {
            // Only count normal cells and the first of an angle to avoid duplicate times
            qwPlayTime += pDVDCELL->m_qwPlayTime;
        }
    }

    return qwPlayTime;
}

const std::string & dvdpttvts::getArtist() const
{
    return m_strArtist;
}

void dvdpttvts::setArtist(const std::string & strArtist)
{
    m_strArtist = strArtist;
}

const std::string & dvdpttvts::getTitle() const
{
    return m_strTitle;
}

void dvdpttvts::setTitle(const std::string & strTitle)
{
    m_strTitle = strTitle;
}
