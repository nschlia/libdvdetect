/*
  dvdetect DVD detection, analysis & DVDETECT lookup library

  Copyright (C) 2013-2014 Norbert Schlia <nschlia@dvdetect.de>

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

/*! \file dvdetectcglue.cpp
 *
 *  \brief Collection of C API functions for libdvdetect
 *
 * TODO: This file is still quite ugly. A bit of refactoring could do well...
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

    return (dvdparse*)pDvDetectHandle->m_pClassDvdParse;
}

static dvddatabase *getSafeDvDatabase(LPDVDBHANDLE pDvDBHandle)
{
    if (pDvDBHandle == NULL)
    {
        return NULL;
    }

    return (dvddatabase*)pDvDBHandle->m_pClassDvdDatabase;
}

static dvdparselst *getSafeDvdParseLst(LPDVDBHANDLE pDvDBHandle)
{
    if (pDvDBHandle == NULL)
    {
        return NULL;
    }

    return (dvdparselst*)pDvDBHandle->m_pClassDvdParseLst;
}

DLL_PUBLIC void *dvdMalloc(size_t size)
{
    return malloc(size);
}

DLL_PUBLIC void dvdFree(void ** ppMemory)
{
    if (*ppMemory != NULL)
    {
        free(*ppMemory);
        ppMemory = NULL;
    }
}

DLL_PUBLIC LPDVDETECTHANDLE dvdOpenLib()
{
    LPDVDETECTHANDLE pDvDetectHandle = new DVDETECTHANDLE;

    if (pDvDetectHandle == NULL)
    {
        return NULL;
    }

    pDvDetectHandle->m_pClassDvdParse = new dvdparse;

    if (pDvDetectHandle->m_pClassDvdParse == NULL)
    {
        delete pDvDetectHandle;
        return NULL;
    }

    return pDvDetectHandle;
}

DLL_PUBLIC void dvdSetProxy(LPDVDETECTHANDLE pDvDetectHandle, const char * pszProxyName)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    if (pszProxyName == NULL)
    {
        pDvdParse->setProxy("");
    }
    else
    {
        pDvdParse->setProxy(pszProxyName);
    }
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

DLL_PUBLIC int dvdGetTitleSetCount(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return 1;
    }

    return (pDvdParse->getTitleSetCount());
}

DLL_PUBLIC int dvdGetPgcCount(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return 0;
    }

    const dvdtitle *pDvdTitle = pDvdParse->getDvdTitle(wTitleSetNo);

    if (pDvdTitle == NULL)
    {
        return 0;
    }

    return pDvdTitle->getPgcCount();
}

DLL_PUBLIC int dvdGetProgramCount(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return 0;
    }

    const dvdpgc *pDvdPgc = pDvdParse->getDvdPgc(wTitleSetNo, wProgramChainNo);

    if (pDvdPgc == NULL)
    {
        return 0;
    }

    return pDvdPgc->getProgramCount();
}

DLL_PUBLIC int dvdGetCellCount(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return 0;
    }

    const dvdprogram *pDvdProgram = pDvdParse->getDvdProgram(wTitleSetNo, wProgramChainNo, wProgramNo);

    if (pDvdProgram == NULL)
    {
        return 0;
    }

    return pDvdProgram->getCellCount();
}

DLL_PUBLIC int dvdGetUnitCount(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo, uint16_t wCellNo)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return 0;
    }

    const dvdcell *pDvdCell = pDvdParse->getDvdCell(wTitleSetNo, wProgramChainNo, wProgramNo, wCellNo);

    if (pDvdCell == NULL)
    {
        return 0;
    }

    return pDvdCell->getUnitCount();
}


DLL_PUBLIC int dvdGetDvdPttVmgCount(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return 1;
    }

    return (pDvdParse->getDvdPttVmgCount());
}

DLL_PUBLIC int dvdGetPttVtsCount(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return 0;
    }

    const dvdpttvmg *pDvdPttVmg = pDvdParse->getDvdPttVmg(wTitleSetNo);

    if (pDvdPttVmg == NULL)
    {
        return 0;
    }

    return pDvdPttVmg->getPttVtsCount();
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

DLL_PUBLIC LPCDVDPROGRAM dvdGetDVDPROGRAM(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    const dvdprogram *pDvdProgram = pDvdParse->getDvdProgram(wTitleSetNo, wProgramChainNo, wProgramNo);

    if (pDvdProgram == NULL)
    {
        return NULL;
    }

    return pDvdProgram->getDVDPROGRAM();
}

DLL_PUBLIC LPCDVDPTTVMG dvdGetDVDPTTVMG(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    const dvdpttvmg *pDvdPttVmg = pDvdParse->getDvdPttVmg(wTitleSetNo);

    if (pDvdPttVmg == NULL)
    {
        return NULL;
    }

    return pDvdPttVmg->getDVDPTTVMG();
}

DLL_PUBLIC LPCDVDPTTVTS dvdGetDVDPTTVTS(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wPttNo)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    const dvdpttvts *pDvdPttVts = pDvdParse->getDvdPttVts(wTitleSetNo, wPttNo);

    if (pDvdPttVts == NULL)
    {
        return NULL;
    }

    return pDvdPttVts->getDVDPTTVTS();
}

DLL_PUBLIC LPCDVDCELL dvdGetDVDCELL(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo, uint16_t wCellNo)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    const dvdcell *pDvdCell = pDvdParse->getDvdCell(wTitleSetNo, wProgramChainNo, wProgramNo, wCellNo);

    if (pDvdCell == NULL)
    {
        return NULL;
    }

    return pDvdCell->getDVDCELL();
}

DLL_PUBLIC LPCDVDUNIT dvdGetDVDUNIT(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo, uint16_t wCellNo, uint16_t wUnitNo)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    const dvdunit *pDvdUnit = pDvdParse->getDvdUnit(wTitleSetNo, wProgramChainNo, wProgramNo, wCellNo, wUnitNo);

    if (pDvdUnit == NULL)
    {
        return NULL;
    }

    return pDvdUnit->getDVDUNIT();
}

DLL_PUBLIC uint64_t	dvdGetVirtSize(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return 0;
    }

    return pDvdParse->getVirtSize();
}

DLL_PUBLIC uint64_t	dvdGetVirtPlayTime(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return 0;
    }

    return pDvdParse->getVirtPlayTime();
}

DLL_PUBLIC uint64_t dvdGetSize(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return 0;
    }

    return pDvdParse->getSize();
}

DLL_PUBLIC uint64_t	 dvdGetPlayTime(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return 0;
    }

    return pDvdParse->getPlayTime();
}

DLL_PUBLIC const char *dvdGetPttVmgTitle(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    const dvdpttvmg *pDvdPttVmg = pDvdParse->getDvdPttVmg(wTitleSetNo);

    if (pDvdPttVmg == NULL)
    {
        return NULL;
    }

    return strdup(pDvdPttVmg->getTitle().c_str());
}

DLL_PUBLIC const char *dvdGetPttVtsArtist(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wPttNo)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    const dvdpttvts *pDvdPttVts = pDvdParse->getDvdPttVts(wTitleSetNo, wPttNo);

    if (pDvdPttVts == NULL)
    {
        return NULL;
    }

    return strdup(pDvdPttVts->getArtist().c_str());
}

DLL_PUBLIC const char *dvdGetPttVtsTitle(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wPttNo)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    const dvdpttvts *pDvdPttVts = pDvdParse->getDvdPttVts(wTitleSetNo, wPttNo);

    if (pDvdPttVts == NULL)
    {
        return NULL;
    }

    return strdup(pDvdPttVts->getTitle().c_str());
}

DLL_PUBLIC const char *dvdGetCode(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getCode().c_str());
}

DLL_PUBLIC const char *dvdGetHash(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getHash().c_str());
}

DLL_PUBLIC const char *dvdGetAlbumArtist(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getAlbumArtist().c_str());
}

DLL_PUBLIC void dvdSetAlbumArtist(LPDVDETECTHANDLE pDvDetectHandle, const char * pszAlbumArtist)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setAlbumArtist(pszAlbumArtist);
}

DLL_PUBLIC const char *dvdGetAlbum(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getAlbum().c_str());
}

DLL_PUBLIC void dvdSetAlbum(LPDVDETECTHANDLE pDvDetectHandle, const char * pszAlbum)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setAlbum(pszAlbum);
}

DLL_PUBLIC const char *dvdGetGenre(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getGenre().c_str());
}

DLL_PUBLIC void dvdSetGenre(LPDVDETECTHANDLE pDvDetectHandle, const char * pszGenre)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setGenre(pszGenre);
}

DLL_PUBLIC const char *dvdGetCast(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getCast().c_str());
}

DLL_PUBLIC void dvdSetCast(LPDVDETECTHANDLE pDvDetectHandle, const char * pszCast)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setCast(pszCast);
}

DLL_PUBLIC const char *dvdGetCrew(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getCrew().c_str());
}

DLL_PUBLIC void dvdSetCrew(LPDVDETECTHANDLE pDvDetectHandle, const char * pszCrew)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setCrew(pszCrew);
}

DLL_PUBLIC const char *dvdGetDirector(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getDirector().c_str());
}

DLL_PUBLIC void dvdSetDirector(LPDVDETECTHANDLE pDvDetectHandle, const char * pszDirector)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setDirector(pszDirector);
}

DLL_PUBLIC const char *dvdGetScreenplay(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getScreenplay().c_str());
}

DLL_PUBLIC void dvdSetScreenplay(LPDVDETECTHANDLE pDvDetectHandle, const char * pszScreenplay)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setScreenplay(pszScreenplay);
}

DLL_PUBLIC const char *dvdGetProducer(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getProducer().c_str());
}

DLL_PUBLIC void dvdSetProducer(LPDVDETECTHANDLE pDvDetectHandle, const char * pszProducer)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setProducer(pszProducer);
}

DLL_PUBLIC const char *dvdGetEditing(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getEditing().c_str());
}

DLL_PUBLIC void dvdSetEditing(LPDVDETECTHANDLE pDvDetectHandle, const char * pszEditing)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setEditing(pszEditing);
}

DLL_PUBLIC const char *dvdGetCinematography(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getCinematography().c_str());
}

DLL_PUBLIC void dvdSetCinematography(LPDVDETECTHANDLE pDvDetectHandle, const char * pszCinematography)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setCinematography(pszCinematography);
}

DLL_PUBLIC const char *dvdGetCountry(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getCountry().c_str());
}

DLL_PUBLIC void dvdSetCountry(LPDVDETECTHANDLE pDvDetectHandle, const char * pszCountry)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setCountry(pszCountry);
}

DLL_PUBLIC const char *dvdGetOriginalLanguage(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getOriginalLanguage().c_str());
}

DLL_PUBLIC void dvdSetOriginalLanguage(LPDVDETECTHANDLE pDvDetectHandle, const char * pszOriginalLanguage)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setOriginalLanguage(pszOriginalLanguage);
}

DLL_PUBLIC const char *dvdGetReleaseDate(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getReleaseDate().c_str());
}

DLL_PUBLIC void dvdSetReleaseDate(LPDVDETECTHANDLE pDvDetectHandle, const char * pszReleaseDate)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setReleaseDate(pszReleaseDate);
}

DLL_PUBLIC const char *dvdGetSpecialFeatures(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getSpecialFeatures().c_str());
}

DLL_PUBLIC void dvdSetSpecialFeatures(LPDVDETECTHANDLE pDvDetectHandle, const char * pszFeatures)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setSpecialFeatures(pszFeatures);
}

DLL_PUBLIC const char *dvdGetEAN_UPC(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getEAN_UPC().c_str());
}

DLL_PUBLIC void dvdSetEAN_UPC(LPDVDETECTHANDLE pDvDetectHandle, const char * pszEAN_UPC)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setEAN_UPC(pszEAN_UPC);
}

DLL_PUBLIC const char *dvdGetStoryline(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getStoryline().c_str());
}

DLL_PUBLIC void dvdSetStoryline(LPDVDETECTHANDLE pDvDetectHandle, const char * pszStoryline)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setStoryline(pszStoryline);
}

DLL_PUBLIC const char *dvdGetRemarks(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getRemarks().c_str());
}

DLL_PUBLIC void dvdSetRemarks(LPDVDETECTHANDLE pDvDetectHandle, const char * pszRemarks)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setRemarks(pszRemarks);
}

DLL_PUBLIC int dvdGetRevision(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return 0;
    }

    return pDvdParse->getRevision();
}

DLL_PUBLIC const char *dvdGetSubmitter(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getSubmitter().c_str());
}

DLL_PUBLIC void dvdSetSubmitter(LPDVDETECTHANDLE pDvDetectHandle, const char * pszSubmitter)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setSubmitter(pszSubmitter);
}

DLL_PUBLIC const char *dvdGetSubmitterIP(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getSubmitterIP().c_str());
}

DLL_PUBLIC void dvdSetClient(LPDVDETECTHANDLE pDvDetectHandle, const char * pszClient)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setClient(pszClient);
}

DLL_PUBLIC const char *dvdGetClient(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getClient().c_str());
}

DLL_PUBLIC void dvdSetKeywords(LPDVDETECTHANDLE pDvDetectHandle, const char * pszKeywords)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    pDvdParse->setKeywords(pszKeywords);
}

DLL_PUBLIC const char * dvdGetKeywords(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getKeywords().c_str());
}

DLL_PUBLIC int dvdGetProtocolVersion(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return 0;
    }

    return pDvdParse->getProtocolVersion();
}

DLL_PUBLIC const char * dvdGetLibraryVersion(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getLibraryVersion().c_str());
}

DLL_PUBLIC const char * dvdGetLibraryName(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getLibraryName().c_str());
}

DLL_PUBLIC const char * dvdGetClientName(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getClientName().c_str());
}

DLL_PUBLIC const char * dvdGetDateCreated(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getDateCreated().c_str());
}

DLL_PUBLIC const char * dvdGetDateLastChanged(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return NULL;
    }

    return strdup(pDvdParse->getDateLastChanged().c_str());
}

DLL_PUBLIC const char * dvdGetErrorString(LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return strdup("Invalid handle");
    }

    return strdup(pDvdParse->getErrorString().c_str());
}

DLL_PUBLIC void dvdCloseLib(LPDVDETECTHANDLE *ppDvDetectHandle)
{
    if (*ppDvDetectHandle != NULL)
    {
        delete (dvdparse*)(*ppDvDetectHandle)->m_pClassDvdParse;
        delete *ppDvDetectHandle;
        ppDvDetectHandle = NULL;
    }
}

DLL_PUBLIC LPDVDBHANDLE dvdDBOpen(const char * pszPogramName)
{
    if (pszPogramName == NULL)
    {
        return NULL;
    }

    LPDVDBHANDLE pDvDBHandle = new DVDBHANDLE;

    if (pDvDBHandle == NULL)
    {
        return NULL;
    }

    pDvDBHandle->m_pClassDvdDatabase = new dvddatabase(pszPogramName);

    if (pDvDBHandle->m_pClassDvdDatabase == NULL)
    {
        delete pDvDBHandle;
        return NULL;
    }

    pDvDBHandle->m_pClassDvdParseLst = new dvdparselst;

    if (pDvDBHandle->m_pClassDvdParseLst == NULL)
    {
        delete (dvddatabase*)pDvDBHandle->m_pClassDvdDatabase;
        delete pDvDBHandle;
        return NULL;
    }

    return pDvDBHandle;
}

DLL_PUBLIC void dvdDBSetProxy(LPDVDBHANDLE pDvDBHandle, const char * pszProxyName)
{
    dvddatabase *pDvdDatabase = getSafeDvDatabase(pDvDBHandle);

    if (pDvdDatabase == NULL)
    {
        return; // "Invalid handle";
    }

    if (pszProxyName == NULL)
    {
        pDvdDatabase->setProxy("");
    }
    else
    {
        pDvdDatabase->setProxy(pszProxyName);
    }
}

DLL_PUBLIC int dvdDBQuery(LPDVDBHANDLE pDvDBHandle, LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return -1;
    }

    dvddatabase *pDvdDatabase = getSafeDvDatabase(pDvDBHandle);

    if (pDvdDatabase == NULL)
    {
        return -1;
    }

    dvdparselst *pDvdParseLst = getSafeDvdParseLst(pDvDBHandle);

    if (pDvdParseLst == NULL)
    {
        return -1;
    }

    return pDvdDatabase->query(pDvdParseLst, pDvdParse);
}

DLL_PUBLIC int dvdDBResults(LPDVDBHANDLE pDvDBHandle)
{
    dvdparselst *pDvdParseLst = getSafeDvdParseLst(pDvDBHandle);

    if (pDvdParseLst == NULL)
    {
        return 0;
    }

    return pDvdParseLst->size();
}

DLL_PUBLIC int dvdDBSearch(LPDVDBHANDLE pDvDBHandle, const char *pszSearch)
{
    if (pszSearch == NULL)
    {
        return -1;
    }

    dvddatabase *pDvdDatabase = getSafeDvDatabase(pDvDBHandle);

    if (pDvdDatabase == NULL)
    {
        return -1;
    }

    dvdparselst *pDvdParseLst = getSafeDvdParseLst(pDvDBHandle);

    if (pDvdParseLst == NULL)
    {
        return -1;
    }

    return pDvdDatabase->search(pDvdParseLst, pszSearch);
}

DLL_PUBLIC int dvdDBSubmit(LPDVDBHANDLE pDvDBHandle, LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return -1;
    }

    dvddatabase *pDvdDatabase = getSafeDvDatabase(pDvDBHandle);

    if (pDvdDatabase == NULL)
    {
        return -1;
    }

    //    dvdparselst *pDvdParseLst = getSafeDvdParseLst(pDvDBHandle);

    //    if (pDvdParseLst == NULL)
    //    {
    //        return -1;
    //    }

    return pDvdDatabase->submit(pDvdParse);
}

DLL_PUBLIC void dvdUpdate(LPDVDBHANDLE pDvDBHandle, LPDVDETECTHANDLE pDvDetectHandle)
{
    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return;
    }

    dvdparselst *pDvdParseLst = getSafeDvdParseLst(pDvDBHandle);

    if (pDvdParseLst == NULL)
    {
        return;
    }

    if (!pDvdParseLst->size())
    {
        return;
    }

    pDvdParse->update((*pDvdParseLst)[0]);
}

DLL_PUBLIC int dvdDBwrite(LPDVDBHANDLE pDvDBHandle, LPDVDETECTHANDLE pDvDetectHandle, const char * pszOutFile)
{
    if (pszOutFile == NULL)
    {
        return -1;
    }

    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return -1;
    }

    dvddatabase *pDvdDatabase = getSafeDvDatabase(pDvDBHandle);

    if (pDvdDatabase == NULL)
    {
        return -1;
    }

    return pDvdDatabase->write(pDvdParse, pszOutFile);
}

DLL_PUBLIC int dvdDBread(LPDVDBHANDLE pDvDBHandle, LPDVDETECTHANDLE pDvDetectHandle, const char * pszInFile)
{
    if (pszInFile == NULL)
    {
        return -1;
    }

    dvdparse *pDvdParse = getSafeDvdParse(pDvDetectHandle);

    if (pDvdParse == NULL)
    {
        return -1;
    }

    dvddatabase *pDvdDatabase = getSafeDvDatabase(pDvDBHandle);

    if (pDvdDatabase == NULL)
    {
        return -1;
    }

    dvdparselst dvdParseLst;

    int res = pDvdDatabase->read(&dvdParseLst, pszInFile);

    if (res)
    {
        return res;
    }

    if (!dvdParseLst.size())
    {
        return -1;
    }

    *pDvdParse = *dvdParseLst[0];

    return 0;
}

DLL_PUBLIC const char * dvdDBGetErrorString(LPDVDBHANDLE pDvDBHandle)
{
    dvddatabase *pDvdDatabase = getSafeDvDatabase(pDvDBHandle);

    if (pDvdDatabase == NULL)
    {
        return strdup("Invalid handle");
    }

    return strdup(pDvdDatabase->getErrorString().c_str());

}

DLL_PUBLIC void dvdDBClose(LPDVDBHANDLE *ppDvDBHandle)
{
    if (*ppDvDBHandle != NULL)
    {
        delete (dvddatabase*)(*ppDvDBHandle)->m_pClassDvdDatabase;
        delete (dvdparselst*)(*ppDvDBHandle)->m_pClassDvdParseLst;
        delete *ppDvDBHandle;
        ppDvDBHandle = NULL;
    }
}
