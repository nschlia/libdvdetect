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

/*! \file dvdlookup.c
 *
 *  \brief DVD online db submit example (c version)
 *
 * This example reads the information of a DVD from an xml file, and
 * submits it to the server. The purpose of this example is to
 * demonstrate how to submit information to the server.
 * If you want a more clearly arranged look, better check out the
 * Qt GUI example instead.
 */

#include <dvdetect/dvdetect.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

const char *pszProgramName = NULL;

#ifdef _WIN32
// Windoze requires the TCP stack explicitly to be initialised, so let's do this here
static int WSAInit()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        fprintf(stderr, "WSAStartup failed with error: %i\n", err);
        return err;
    }
    else
    {
        /* Confirm that the WinSock DLL supports 2.2.*/
        /* Note that if the DLL supports versions greater    */
        /* than 2.2 in addition to 2.2, it will still return */
        /* 2.2 in wVersion since that is the version we      */
        /* requested.                                        */

        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        {
            /* Tell the user that we could not find a usable */
            /* WinSock DLL.                                  */
            fprintf(stderr, "Could not find a usable version of Winsock.dll\n");
            WSACleanup();
            return 1;
        }

    }
    return 0;
}
#endif

static const char * getFileName(const char *pszFilePath)
{
    const char *ptr = strrchr(pszFilePath, '\\');
    if (ptr != NULL)
    {
        return ++ptr;
    }
    ptr = strrchr(pszFilePath, '/');
    if (ptr != NULL)
    {
        return ++ptr;
    }
    return pszFilePath;
}

/*!
 * Show usage info
 */
static void usage()
{
    printf("\n");
    printf("Usage:\n\n");
    printf("%s [-x] -i input.xml     xml to send\n", pszProgramName);
    printf("%s -x proxy:port         enable proxy\n", pszProgramName);
    printf("%s -h                    show help\n", pszProgramName);
    printf("\n");

    printf("Example:\n\n");

    printf("%s -i mydvd.xml\n", pszProgramName);
}

static void printl(const char *pszFormat, const char *pszText)
{
    if (pszText != NULL)
    {
        printf(pszFormat, pszText);
        dvdFree((void **)&pszText);
    }
}

int main(int argc, char *argv[])
{
    const char *pszInputFile = "";
    const char *pszProxy = "";
    LPDVDETECTHANDLE pDvDetectHandle = NULL;
    bool bMissingParameter = true;
    bool bUseProxy = false;
    int res = 0;
    int c;

    pszProgramName = getFileName(argv[0]);

    printf("%s V%s\n", pszProgramName, LIBDVDETECT_VERSION);
    printf("%s\n\n", LIBDVDETECT_COPYRIGHT);

#ifdef _WIN32
    res = WSAInit();
    if (res)
    {
        return res;
    }
#endif

    opterr = 0;

    while ((c = getopt (argc, argv, "hi:")) != -1)
    {
        switch (c)
        {
        case 'h':
            usage();
            return 1;
            break;
        case 'i':
            pszInputFile = optarg;
            bMissingParameter = false;
            break;
        case 'x':
            pszProxy = optarg;
            bUseProxy = true;
            bMissingParameter = false;
            break;
        case '?':
            if (optopt == 'i' || optopt == 'x')
            {
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            }
            else if (isprint (optopt))
            {
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            }
            else
            {
                fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
            }

            usage();
            return 1;
            break;
        default:
            abort ();
        }
    }

    if (bMissingParameter)
    {
        fprintf(stderr, "Missing parameter.\n");
        usage();
        return 1;
    }

    pDvDetectHandle = dvdOpenLib();
    if (pDvDetectHandle != NULL)
    {
        LPDVDBHANDLE pDvDBHandle = NULL;

        pDvDBHandle = dvdDBOpen(LIBDVDETECT_VERSION);

        if (pDvDBHandle != NULL)
        {
            if (bUseProxy)
            {
                printf("Using proxy server: %s\n", pszProxy);
                dvdDBSetProxy(pDvDBHandle, pszProxy);
            }

            res = dvdDBread(pDvDBHandle, pDvDetectHandle, pszInputFile);

            if (res != 0)
            {
                fprintf(stderr, "DVD read error. %s\n", dvdGetErrorString(pDvDetectHandle));
            }
            else
            {
                printl("Submit Hash         : %s\n", dvdGetHash(pDvDetectHandle));
                printf("Revision            : %i\n\n", dvdGetRevision(pDvDetectHandle));

                printf("Submitting DVD...\n\n");

                res = dvdDBSubmit(pDvDBHandle, pDvDetectHandle);

                if (res != 0)
                {
                    fprintf(stderr, "DVD submit error. %s\n", dvdDBGetErrorString(pDvDBHandle));
                }
            }
        }
        else
        {
            fprintf(stderr, "Unable to open database\n");
        }

        if (res == 0)
        {
            res = dvdDBQuery(pDvDBHandle, pDvDetectHandle);

            if (res)
            {
                fprintf(stderr, "DVD query error. %s\n", dvdDBGetErrorString(pDvDBHandle));
            }
            else if (dvdDBResults(pDvDBHandle))
            {
                dvdUpdate(pDvDBHandle, pDvDetectHandle);

                printl("Album Artist        : %s\n", dvdGetAlbumArtist(pDvDetectHandle));
                printl("Album               : %s\n", dvdGetAlbum(pDvDetectHandle));
                printf("New Revision at     : %i\n\n", dvdGetRevision(pDvDetectHandle));

                printf("Submit successful.\n");
            }
            else
            {
                fprintf(stderr, "Requery failed - no results received.\n");
            }
        }
        dvdDBClose(&pDvDBHandle);
    }

    dvdCloseLib(&pDvDetectHandle);

#ifdef _WIN32
    WSACleanup();
#endif

    return res;
}

