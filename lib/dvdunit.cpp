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

/*! \file dvdunit.cpp
 *
 *  \brief dvdunit class implementation
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>

#include "localutils.h"

dvdunit::dvdunit()
{
    memset(&m_DVDUNIT, 0, sizeof(DVDUNIT));
}

dvdunit::~dvdunit()
{
}

dvdunit& dvdunit::operator= (dvdunit const & source)
{
    if (this != &source)
    {
        memcpy(&m_DVDUNIT, &source.m_DVDUNIT, sizeof(DVDUNIT));
    }
    return *this;
}

LPCDVDUNIT dvdunit::getDVDUNIT() const
{
    return &m_DVDUNIT;
}

uint64_t dvdunit::getSize() const
{
    return (dvdSector2bytes((m_DVDUNIT.m_dwEndSector - m_DVDUNIT.m_dwStartSector)));
}
