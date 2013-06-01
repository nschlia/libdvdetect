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

/** \file dvdtitle.h
 *
 *  \brief dvdtitle class
 */

#pragma once

#ifndef DVDTITLE_H
#define DVDTITLE_H

#include <vector>

#include <dvdetect/dvdpgc.h>

/*!
 *  @brief dvdtitle class
 *
 * This class stores all data about a DVD title.
 */
class DLL_PUBLIC dvdtitle : public dvdetectbase
{
    friend class dvdparse;

public:
    explicit dvdtitle();
    virtual ~dvdtitle();

    LPCDVDVTS           getDVDVTS() const;
    const dvdpgc *      getDvdPgc(uint16_t wProgramChainNo) const;
    uint16_t            getPgcCount() const;
    const dvdfile *     getDvdFile(uint16_t wFileNo) const;
    uint16_t            getFileCount() const;

    uint64_t            getSize() const;
    uint64_t            getPlayTime() const;

    virtual const std::type_info & classtype() const;

protected:
    DVDVTS              m_DVDVTS;
    dvdpgclst           m_dvdPgcLst;
    dvdfilelst          m_dvdFileLst;
};

typedef std::vector<dvdtitle> dvdtitlelst;

#endif // DVDTITLE_H
