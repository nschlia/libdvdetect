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

/*! \file dvdtitle.cpp
 *
 *  \brief dvdtitle class implementation
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>

#include "localutils.h"

dvdtitle::dvdtitle()
{
    memset(&m_DVDVTS, 0, sizeof(DVDVTS));
}

dvdtitle::~dvdtitle()
{
}

dvdtitle& dvdtitle::operator= (dvdtitle const& rhs)
{
    if (this != &rhs)
    {
        memcpy(&m_DVDVTS, &rhs.m_DVDVTS, sizeof(DVDVTS));
        m_dvdPgcLst     = rhs.m_dvdPgcLst;
        m_dvdFileLst    = rhs.m_dvdFileLst;
    }
    return *this;
}

LPCDVDVTS dvdtitle::getDVDVTS() const
{
    return &m_DVDVTS;
}

const dvdpgc *dvdtitle::getDvdPgc(uint16_t wProgramChainNo) const
{
    if (!wProgramChainNo || wProgramChainNo > m_dvdPgcLst.size())
    {
        return NULL;
    }

    return &m_dvdPgcLst[wProgramChainNo - 1];
}

uint16_t dvdtitle::getPgcCount() const
{
    return m_DVDVTS.m_wNumberOfProgramChains;
}

const dvdfile * dvdtitle::getDvdFile(uint16_t wFileNo) const
{
    if (wFileNo >= m_dvdFileLst.size())
    {
        return NULL;
    }

    return &m_dvdFileLst[wFileNo];
}

uint16_t dvdtitle::getFileCount() const
{
    return (uint16_t)m_dvdFileLst.size();
}

uint64_t dvdtitle::getSize() const
{
    uint64_t size = 0;

    for (uint16_t wProgramChainNo = 1; wProgramChainNo <= m_DVDVTS.m_wNumberOfProgramChains;  wProgramChainNo++)
    {
        size += m_dvdPgcLst[wProgramChainNo - 1].getSize();
    }

    return size;
}

uint64_t dvdtitle::getPlayTime() const
{
    uint64_t qwPlayTime = 0;

    for (uint16_t wProgramChainNo = 1; wProgramChainNo <= m_DVDVTS.m_wNumberOfProgramChains;  wProgramChainNo++)
    {
        qwPlayTime += m_dvdPgcLst[wProgramChainNo - 1].getPlayTime();
    }

    return qwPlayTime;
}
