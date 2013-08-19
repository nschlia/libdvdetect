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

#include "localutils.h"
#include "http.h"
#include "dvdetect/dvdutils.h"

#include <fstream>

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

DLL_PUBLIC void dvdGetFileName(DVDFILETYPE eFileType, uint16_t wTitleSetNo, uint16_t wVtsNo, char * pszFileName, size_t maxlen)
{
    *pszFileName = '\0';
    strncat(pszFileName, getDvdFileName(eFileType, wTitleSetNo, wVtsNo).c_str(), maxlen);
    *(pszFileName + (maxlen - 1)) = '\0';
}

typedef struct
{
    std::string m_strFileName;
    FILE*       m_pFile;
    http*       m_pHttpServer;
    bool        m_bWebFile;

} DVDFILEHANDLE, *LPDVDFILEHANDLE;

DLL_PUBLIC void * openFile(const char *filename, const char *mode, const char *proxy)
{
    LPDVDFILEHANDLE pHandle = new DVDFILEHANDLE;

    if (pHandle == NULL)
    {
        return NULL;
    }

    pHandle->m_strFileName = filename;

    if (!isUrl(pHandle->m_strFileName))
    {
#ifdef _WIN32
        wchar_t szUtf16Filename[MAX_PATH];
        wchar_t szMode[20];
        utf8ToUtf16(szMode, mode);
        if (utf8ToUtf16(szUtf16Filename, filename))
        {
            pHandle->m_pFile = _wfopen(szUtf16Filename, szMode);
        }
        else
        {
            pHandle->m_pFile = NULL;
        }
#else
        pHandle->m_pFile = fopen(filename, mode);
#endif
        if (pHandle->m_pFile != NULL)
        {
            pHandle->m_pHttpServer = NULL;
            pHandle->m_bWebFile = false;
        }
        else
        {
            closeFile(pHandle);
            pHandle = NULL;
        }
    }
    else
    {
        pHandle->m_pFile = NULL;
        pHandle->m_pHttpServer = new http;
        pHandle->m_bWebFile = true;

        if (proxy != NULL)
        {
            pHandle->m_pHttpServer->setProxy(proxy);
        }

        int res = pHandle->m_pHttpServer->request(http::GET, pHandle->m_strFileName);

        if (res != 200)
        {
            closeFile(pHandle);
            pHandle = NULL;
        }
    }

    return pHandle;
}

DLL_PUBLIC size_t readFile(void* buffer, size_t size, size_t count, void* stream)
{
    LPDVDFILEHANDLE pHandle = (LPDVDFILEHANDLE)stream;

    if (pHandle == NULL)
    {
        return 0;
    }

    if (!pHandle->m_bWebFile)
    {
        return fread(buffer, size, count, pHandle->m_pFile);
    }
    else if (pHandle->m_pHttpServer != NULL)
    {
        return pHandle->m_pHttpServer->getContent((char *)buffer, size * count);
    }

    return 0;
}

DLL_PUBLIC int writeFile(const void* buffer, size_t size, size_t count, void* stream)
{
    LPDVDFILEHANDLE pHandle = (LPDVDFILEHANDLE)stream;

    if (pHandle == NULL)
    {
        return 0;
    }

    if (!pHandle->m_bWebFile)
    {
        return fwrite(buffer, size, count, pHandle->m_pFile);
    }
    else
    {
        return 0;
    }
}

DLL_PUBLIC int closeFile(void *stream)
{
    LPDVDFILEHANDLE pHandle = (LPDVDFILEHANDLE)stream;

    if (pHandle == NULL)
    {
        return EOF;
    }

    int res =  0;

    if (!pHandle->m_bWebFile)
    {
        res =  fclose(pHandle->m_pFile);
    }

    delete pHandle->m_pHttpServer;
    delete pHandle;

    return res;
}

DLL_PUBLIC int statFile(const char *filename, LPDVDFILESTAT pFileStat, const char *proxy)
{
    memset(pFileStat, 0, sizeof(DVDFILESTAT));

    if (!isUrl(filename))
    {
        // Local files
#ifdef _WIN32
        struct _stat buf;
        wchar_t szUtf16Filename[MAX_PATH];
        if (utf8ToUtf16(szUtf16Filename, filename))
        {
            if (_wstat(szUtf16Filename, &buf) == -1)
            {
                return -1;
            }
        }
        else
        {
            return -1;
        }
#else
        struct stat buf;

        if (stat(filename, &buf) == -1)
        {
            return -1;
        }
#endif

        pFileStat->m_FileTime       = buf.st_mtime;
        pFileStat->m_qwFileSize     = buf.st_size;
        pFileStat->m_bIsDirectory   = S_ISDIR(buf.st_mode) ? true : false;
    }
    else
    {
        http httpserver;

        if (proxy != NULL)
        {
            httpserver.setProxy(proxy);
        }

        int res = httpserver.request(http::GET_HEADERSONLY, filename);

        if (res != 200)
        {
            return -1;
        }

        pFileStat->m_FileTime       = httpserver.getTimeStamp();
        pFileStat->m_qwFileSize     = httpserver.getContentSize();
    }

    return 0;
}

DLL_PUBLIC int fstatFile(void* stream, LPDVDFILESTAT pFileStat)
{
    LPDVDFILEHANDLE pHandle = (LPDVDFILEHANDLE)stream;

    if (pHandle == NULL && pFileStat != NULL)
    {
        return -1;
    }

    memset(pFileStat, 0, sizeof(DVDFILESTAT));

    if (!isUrl(pHandle->m_strFileName))
    {
        // Local files
        struct stat buf;

        if (fstat(fileno(pHandle->m_pFile), &buf) == -1)
        {
            return -1;
        }

        pFileStat->m_FileTime       = buf.st_mtime;
        pFileStat->m_qwFileSize     = buf.st_size;
        pFileStat->m_bIsDirectory   = S_ISDIR(buf.st_mode) ? true : false;
    }
    else if (pHandle->m_pHttpServer != NULL)
    {
        pFileStat->m_FileTime       = pHandle->m_pHttpServer->getTimeStamp();
        pFileStat->m_qwFileSize     = pHandle->m_pHttpServer->getContentSize();
    }
    return 0;
}
