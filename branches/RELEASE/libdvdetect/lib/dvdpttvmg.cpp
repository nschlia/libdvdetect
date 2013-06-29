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
}

dvdpttvmg& dvdpttvmg::operator= (dvdpttvmg const& rhs)
{
    if (this != &rhs)
    {
        memcpy(&m_DVDPTTVMG, &rhs.m_DVDPTTVMG, sizeof(DVDPTTVMG));
        m_pDvdParse     = rhs.m_pDvdParse;
        m_dvdPttVtsLst  = rhs.m_dvdPttVtsLst;
    }
    return *this;
}

LPCDVDPTTVMG dvdpttvmg::getDVDPTTVMG() const
{
    return &m_DVDPTTVMG;
}

const dvdtitle * dvdpttvmg::getDvdTitle() const
{
    if (m_pDvdParse == NULL)
    {
        return NULL;
    }

    return m_pDvdParse->getDvdTitle(m_DVDPTTVMG.m_byVideoTitleSet);
}

const dvdpttvts * dvdpttvmg::getDvdPttVts(uint16_t wPtt) const
{
    if (!wPtt || wPtt > m_dvdPttVtsLst.size())
    {
        return NULL;
    }

    return &m_dvdPttVtsLst[wPtt - 1];
}

uint16_t dvdpttvmg::getPttCount() const
{
    return m_dvdPttVtsLst.size();
}

uint64_t dvdpttvmg::getSize() const
{
    uint64_t size = 0;

    for (uint16_t wPttVmgNo = 1; wPttVmgNo <= getPttCount(); wPttVmgNo++)
    {
        size += m_dvdPttVtsLst[wPttVmgNo - 1].getSize();
    }

    return size;
}

uint64_t dvdpttvmg::getPlayTime() const
{
    uint64_t qwPlayTime = 0;

    for (uint16_t wPttVmgNo = 1; wPttVmgNo <= getPttCount(); wPttVmgNo++)
    {
        qwPlayTime += m_dvdPttVtsLst[wPttVmgNo - 1].getPlayTime();
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
