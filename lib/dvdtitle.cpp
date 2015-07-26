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

dvdtitle& dvdtitle::operator= (dvdtitle const & source)
{
    if (this != &source)
    {
        memcpy(&m_DVDVTS, &source.m_DVDVTS, sizeof(DVDVTS));
        m_lstDvdPgc     = source.m_lstDvdPgc;
        m_lstDvdFile    = source.m_lstDvdFile;
    }
    return *this;
}

LPCDVDVTS dvdtitle::getDVDVTS() const
{
    return &m_DVDVTS;
}

dvdpgc *dvdtitle::getDvdPgc(uint16_t wProgramChainNo) const
{
    if (!wProgramChainNo || wProgramChainNo > getPgcCount())
    {
        return NULL;
    }

    return m_lstDvdPgc[wProgramChainNo - 1];
}

uint16_t dvdtitle::getPgcCount() const
{
    return m_lstDvdPgc.size();
}

dvdfile * dvdtitle::getDvdFile(uint16_t wFileNo) const
{
    if (wFileNo >= m_lstDvdFile.size())
    {
        return NULL;
    }

    return m_lstDvdFile[wFileNo];
}

uint16_t dvdtitle::getFileCount() const
{
    return (uint16_t)m_lstDvdFile.size();
}

uint64_t dvdtitle::getSize() const
{
    uint64_t size = 0;

    for (uint16_t wProgramChainNo = 1; wProgramChainNo <= getPgcCount(); wProgramChainNo++)
    {
        size += m_lstDvdPgc[wProgramChainNo - 1]->getSize();
    }

    return size;
}

uint64_t dvdtitle::getPlayTime() const
{
    uint64_t qwPlayTime = 0;

    for (uint16_t wProgramChainNo = 1; wProgramChainNo <= getPgcCount(); wProgramChainNo++)
    {
        qwPlayTime += m_lstDvdPgc[wProgramChainNo - 1]->getPlayTime();
    }

    return qwPlayTime;
}
