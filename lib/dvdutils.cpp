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

/*! \file dvdutils.cpp
 *
 *  \brief Collection of libdvdetect utility function
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>

#include "dvdetect/dvdutils.h"

DLL_PUBLIC uint64_t dvdSector2bytes(uint32_t dwSectorAddress)
{
    return ((uint64_t)DVD_SECTOR_SIZE * (uint64_t)dwSectorAddress);
}

DLL_PUBLIC const char * dvdGetExtension(DVDFILETYPE eFileType)
{
    switch (eFileType)
    {
    case DVDFILETYPE_VMG_IFO:   // VIDEO_TS.IFO
    case DVDFILETYPE_VTS_IFO:   // VTS_##_0.IFO
        return "IFO";
        break;
    case DVDFILETYPE_VMG_VOB:   // VIDEO_TS.VOB
    case DVDFILETYPE_MENU_VOB:  // VTS_##_0.VOB
    case DVDFILETYPE_VTS_VOB:   // VTS_##_1.VOB to VTS_##_9.VOB
        return "VOB";
        break;
    case DVDFILETYPE_VMG_BUP:   // VIDEO_TS.BUP
    case DVDFILETYPE_VTS_BUP:   // VTS_##_0.BUP
        return "BUP";
        break;
    default:
        return "";
        break;
    }
}

DLL_PUBLIC const char * dvdGetFileType(DVDFILETYPE eFileType)
{
    switch (eFileType)
    {
    case DVDFILETYPE_VMG_IFO:   // VIDEO_TS.IFO
        return "VMG/IFO";
        break;
    case DVDFILETYPE_VMG_BUP:   // VIDEO_TS.BUP
        return "VMG/BUP";
        break;
    case DVDFILETYPE_VMG_VOB:   // VIDEO_TS.VOB
        return "VMG/VOB";
        break;
    case DVDFILETYPE_VTS_IFO:   // VTS_##_0.IFO
        return "VTS/IFO";
        break;
    case DVDFILETYPE_VTS_BUP:   // VTS_##_0.BUP
        return "VTS/BUP";
        break;
    case DVDFILETYPE_MENU_VOB:  // VTS_##_0.VOB
        return "MNU/VOB";
        break;
    case DVDFILETYPE_VTS_VOB:   // VTS_##_1.VOB to VTS_##_9.VOB
        return "VTS/VOB";
        break;
    default:
        return "";
        break;
    }
}
