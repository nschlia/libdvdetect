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

/** \file dvdetectcglue.cpp
 *
 *  \brief Collection of C API functions for libdvdetect
 */

#include "compat.h"

#include <dvdetect/dvdetect.h>
#include <dvdetect/dvdetectc++.h>

static dvdparse *getSafeDvdParse(LPDVDETECTHANDLE pDvDetectHandle)
{
    if (pDvDetectHandle == NULL)
    {
        return NULL;
    }

    return (dvdparse*)pDvDetectHandle->m_pClass;
}

DLL_PUBLIC LPDVDETECTHANDLE dvdOpenLib()
{
    LPDVDETECTHANDLE pDvDetectHandle = new DVDETECTHANDLE;

    if (pDvDetectHandle == NULL)
    {
        return NULL;
    }

    pDvDetectHandle->m_pClass = new dvdparse;

    if (pDvDetectHandle->m_pClass == NULL)
    {
        delete pDvDetectHandle;
        return NULL;
    }

    return pDvDetectHandle;
} 

DLL_PUBLIC int dvdParse(LPDVDETECTHANDLE pDvDetectHandle, const char *pszPath)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return 1;
    }

    return (pDvdParse->parse(pszPath));
}

DLL_PUBLIC LPCDVDVMGM dvdGetDVDVMGM(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return pDvdParse->getDVDVMGM();
}

DLL_PUBLIC LPCDVDVTS dvdGetDVDVTS(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    const dvdtitle *pDvdTitle = pDvdParse->getDvdTitle(wTitleSetNo);

    if (pDvdTitle == NULL)
    {
        return NULL;
    }

    return pDvdTitle->getDVDVTS();
}

DLL_PUBLIC LPCDVDPGC dvdGetDVDPGC(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    const dvdpgc *pDvdPgc = pDvdParse->getDvdPgc(wTitleSetNo, wProgramChainNo);

    if (pDvdPgc == NULL)
    {
        return NULL;
    }

    return pDvdPgc->getDVDPGC();
}

DLL_PUBLIC LPCDVDPROGRAM dvdGetDVDPROGRAM(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    const dvdprogram *pDvdProgram = pDvdParse->getDvdProgram(wTitleSetNo, wProgramChainNo, wProgram);

    if (pDvdProgram == NULL)
    {
        return NULL;
    }

    return pDvdProgram->getDVDPROGRAM();
}

DLL_PUBLIC LPCDVDPTT dvdGetDVDPTT(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wPtt)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    const dvdptt *pDvdPtt = pDvdParse->getDvdPtt(wTitleSetNo, wProgramChainNo, wPtt);

    if (pDvdPtt == NULL)
    {
        return NULL;
    }

    return pDvdPtt->getDVDPTT();
}

DLL_PUBLIC LPCDVDCELL dvdGetDVDCELL(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram, uint16_t wCell)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    const dvdcell *pDvdCell = pDvdParse->getDvdCell(wTitleSetNo, wProgramChainNo, wProgram, wCell);

    if (pDvdCell == NULL)
    {
        return NULL;
    }

    return pDvdCell->getDVDCELL();
}

DLL_PUBLIC LPCDVDUNIT dvdGetDVDUNIT(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram, uint16_t wCell, uint16_t wUnit)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    const dvdunit *pDvdUnit = pDvdParse->getDvdUnit(wTitleSetNo, wProgramChainNo, wProgram, wCell, wUnit);

    if (pDvdUnit == NULL)
    {
        return NULL;
    }

    return pDvdUnit->getDVDUNIT();
}

DLL_PUBLIC const char * dvdGetErrorString(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return "Invalid handle";
    }

    return (pDvdParse->getErrorString().c_str());
}

DLL_PUBLIC void dvdCloseLib(LPDVDETECTHANDLE pDvDetectHandle)
{
    delete pDvDetectHandle;
}
