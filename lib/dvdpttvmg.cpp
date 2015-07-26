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

/*! \file dvdpttvmg.cpp
 *
 *  \brief dvdpttvmg class implementation
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>

#include "localutils.h"

dvdpttvmg::dvdpttvmg(dvdparse *pDvdParse)
    : m_pDvdParse(pDvdParse)
{
    memset(&m_DVDPTTVMG, 0, sizeof(DVDPTTVMG));
}

dvdpttvmg::~dvdpttvmg()
{
    while (m_lstDvdPttVts.size())
    {
        delete m_lstDvdPttVts.back();
        m_lstDvdPttVts.pop_back();
    }
}

dvdpttvmg& dvdpttvmg::operator= (dvdpttvmg const & source)
{
    if (this != &source)
    {
        memcpy(&m_DVDPTTVMG, &source.m_DVDPTTVMG, sizeof(DVDPTTVMG));
        m_pDvdParse     = source.m_pDvdParse;
        m_lstDvdPttVts  = source.m_lstDvdPttVts;
        m_strTitle      = source.m_strTitle;
    }
    return *this;
}

LPCDVDPTTVMG dvdpttvmg::getDVDPTTVMG() const
{
    return &m_DVDPTTVMG;
}

dvdtitle * dvdpttvmg::getDvdTitle() const
{
    if (m_pDvdParse == NULL)
    {
        return NULL;
    }

    const dvdpttvts * pDvdPttVts = getDvdPttVts(1);

    if (pDvdPttVts == NULL)
    {
        return NULL;
    }

    LPCDVDPTTVTS pDVDPTTVTS = pDvdPttVts->getDVDPTTVTS();
    const dvdprogram * pDvdProgram = m_pDvdParse->getDvdProgram(pDVDPTTVTS);

    if (pDvdProgram == NULL)
    {
        return NULL;
    }

    LPCDVDPROGRAM pDVDPROGRAM = pDvdProgram->getDVDPROGRAM();
    return m_pDvdParse->getDvdTitle(pDVDPROGRAM->m_wTitleSetNo);
    //return m_pDvdParse->getDvdTitle(m_DVDPTTVMG.m_wRealTitleNo /*m_wVideoTitleSetNo*/);
}

dvdpttvts * dvdpttvmg::getDvdPttVts(uint16_t wPttNo) const
{
    if (!wPttNo || wPttNo > m_lstDvdPttVts.size())
    {
        return NULL;
    }

    return m_lstDvdPttVts[wPttNo - 1];
}

uint16_t dvdpttvmg::getPttVtsCount() const
{
    return m_lstDvdPttVts.size();
}

uint64_t dvdpttvmg::getSize() const
{
    uint64_t size = 0;

    for (uint16_t wPttVmgNo = 1; wPttVmgNo <= getPttVtsCount(); wPttVmgNo++)
    {
        size += m_lstDvdPttVts[wPttVmgNo - 1]->getSize();
    }

    return size;
}

uint64_t dvdpttvmg::getPlayTime() const
{
    uint64_t qwPlayTime = 0;

    for (uint16_t wPttVmgNo = 1; wPttVmgNo <= getPttVtsCount(); wPttVmgNo++)
    {
        qwPlayTime += m_lstDvdPttVts[wPttVmgNo - 1]->getPlayTime();
    }

    return qwPlayTime;
}

std::string dvdpttvmg::getTitle() const
{
    return m_strTitle;
}

void dvdpttvmg::setTitle(const std::string & strTitle)
{
    m_strTitle = strTitle;
}
