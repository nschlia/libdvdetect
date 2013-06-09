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

/*! \file dvdfile.cpp
 *
 *  \brief dvdfile class implementation
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>

#include "localutils.h"

dvdfile::dvdfile()
{
    memset(&m_DVDFILE, 0, sizeof(DVDFILE));
}

dvdfile::~dvdfile()
{
}

LPCDVDFILE dvdfile::getDVDFILE() const
{
    return &m_DVDFILE;
}

const std::type_info & dvdfile::classtype() const
{
    return typeid(*this);
}

