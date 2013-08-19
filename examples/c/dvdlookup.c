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
 *  \brief DVD online db lookup example (c version)
 *
 * This example reads structure of a DVD, tries to look it up on the
 * server and displays the information (or simply that it is not found). 
 * The purpose of this example is to demonstrate how to get the 
 * information from the server and then traverse through the DVD 
 * structure to display it. There is also an option to write the data 
 * retrieved into an xml file that can be edited and submitted to the 
 * server (using the dvdsubmit example program).
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
 * Print playtime (and optionally frame rate)
 *
 *  \param qwPlaytimems uint32_t Play time in milliseconds
 *  \param wFrameRate uint16_t Frame rate (25/30) or -1 to not display
 *  \return Adress in bytes
 */
static void playTime(uint64_t qwPlaytimems, uint16_t wFrameRate)
{
    uint64_t qwPlayTimeS = qwPlaytimems / 1000;

    printf("Playback Time       : %02i:%02i:%02i.%03i",
           (int)(qwPlayTimeS) / (60 * 60),
           (int)((qwPlayTimeS) / 60) % 60,
           (int)(qwPlayTimeS) % 60,
           (int)(qwPlaytimems - (qwPlayTimeS * 1000)));
    if (wFrameRate != (uint16_t)-1)
    {
        printf(" %i fps", wFrameRate);
    }

    printf("\n");
}

/*!
 * Show usage info
 */
static void usage()
{
    printf("\n");
    printf("Usage:\n\n");
    printf("%s [-x] -d PATH-TO-DVD   list DVD structure\n", pszProgramName);
    printf("%s -x proxy:port         enable proxy\n", pszProgramName);
    printf("%s -h                    show help\n", pszProgramName);
    printf("\n");

    printf("Example:\n\n");

#ifdef _WIN32
    printf("%s -d F:\n", pszProgramName);
#else
    printf("%s -d /mnt/cdrom1/\n", pszProgramName);
#endif
}

static void printl(const char *pszFormat, const char *pszText)
{
    if (pszText != NULL)
    {
        printf(pszFormat, pszText);
        dvdFree((void **)&pszText);
    }
}

static int showStructure(LPDVDETECTHANDLE pDvDetectHandle)
{
    uint16_t wTitleSetNo = 1;
    int res = 0;

    printl("Album Artist        : %s\n", dvdGetAlbumArtist(pDvDetectHandle));
    printl("Album               : %s\n", dvdGetAlbum(pDvDetectHandle));
    printl("Genre               : %s\n", dvdGetGenre(pDvDetectHandle));
    printl("Keywords            : %s\n", dvdGetKeywords(pDvDetectHandle));
    printl("Storyline/Synopsis  : %s\n", dvdGetStoryline(pDvDetectHandle));
    printf("Revision            : %u\n", dvdGetRevision(pDvDetectHandle));
    printl("Submitter           : %s\n", dvdGetSubmitter(pDvDetectHandle));
    printl("Submitter IP        : %s\n", dvdGetSubmitterIP(pDvDetectHandle));
    printl("Submission Client   : %s\n", dvdGetClient(pDvDetectHandle));
    printl("Database Hash       : %s\n", dvdGetHash(pDvDetectHandle));

    for (wTitleSetNo = 1; wTitleSetNo <= dvdGetTitleSetCount(pDvDetectHandle); wTitleSetNo++)
    {
        uint16_t wPttNo = 1;

        printf("******************************** Title %02u ********************************\n", wTitleSetNo);

        printf("Chapters            : %u\n", dvdGetPttVtsCount(pDvDetectHandle, wTitleSetNo));
        printl("Title               : %s\n", dvdGetPttVmgTitle(pDvDetectHandle, wTitleSetNo));

        for (wPttNo = 1; wPttNo <= dvdGetPttVtsCount(pDvDetectHandle, wTitleSetNo); wPttNo++)
        {
            LPCDVDPTTVTS pDVDPTTVTS = dvdGetDVDPTTVTS(pDvDetectHandle, wTitleSetNo, wPttNo);

            printf("Chapter #           : %u\n", pDVDPTTVTS->m_wProgramNo);
            printl("Artist              : %s\n", dvdGetPttVtsArtist(pDvDetectHandle, wTitleSetNo, wPttNo));
            printl("Title               : %s\n", dvdGetPttVtsTitle(pDvDetectHandle, wTitleSetNo, wPttNo));
        }
    }

    printf("\n");
    printf("Size                : %"PRIu64" MB\n", dvdGetVirtSize(pDvDetectHandle) / (1024*1024));
    playTime(dvdGetVirtPlayTime(pDvDetectHandle), -1);

    printf("**************************************************************************\n");

    return res;
}

int main(int argc, char *argv[])
{
    const char *pszPath = "";
    const char *pszProxy = "";
    const char *pszOutFile = "";
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

    while ((c = getopt (argc, argv, "hx:d:o:")) != -1)
    {
        switch (c)
        {
        case 'h':
            usage();
            return 1;
            break;
        case 'd':
            pszPath = optarg;
            bMissingParameter = false;
            break;
        case 'x':
            pszProxy = optarg;
            bUseProxy = true;
            bMissingParameter = false;
            break;
        case 'o':
            pszOutFile = optarg;
            bMissingParameter = false;
            break;
        case '?':
            if (optopt == 'd' || optopt == 'x' || optopt == 'o')
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
        if (bUseProxy)
        {
            printf("Using proxy server: %s\n", pszProxy);
            dvdSetProxy(pDvDetectHandle, pszProxy);
        }

        res = dvdParse(pDvDetectHandle, pszPath);

        if (res == 0)
        {
            LPDVDBHANDLE pDvDBHandle = NULL;

            pDvDBHandle = dvdDBOpen(pszProgramName);

            if (pDvDBHandle != NULL)
            {
                printf("Querying DVD...\n");

                if (bUseProxy)
                {
                    printf("Using proxy server: %s\n", pszProxy);
                    dvdDBSetProxy(pDvDBHandle, pszProxy);
                }

                res = dvdDBQuery(pDvDBHandle, pDvDetectHandle);

                if (res != 0)
                {
                    fprintf(stderr, "DVD query error. %s\n", dvdGetErrorString(pDvDetectHandle));
                }
                else if (dvdDBResults(pDvDBHandle))
                {
                    dvdUpdate(pDvDBHandle, pDvDetectHandle);
                    if (*pszOutFile)
                    {
                        res = dvdDBwrite(pDvDBHandle, pDvDetectHandle, pszOutFile);
                        if (!res)
                        {
                            printf("Result saved to %s.\n", pszOutFile);
                        }
                        else
                        {
                            fprintf(stderr, "Error writing output file. %s\n", dvdGetErrorString(pDvDetectHandle));
                        }

                        if (res == 0)
                        {
                            printf("Album Artist        : %s\n", dvdGetAlbumArtist(pDvDetectHandle));
                            printf("Album               : %s\n", dvdGetAlbum(pDvDetectHandle));
                            printf("Revision            : %i\n", dvdGetRevision(pDvDetectHandle));
                        }
                    }
                    else
                    {
                        res = showStructure(pDvDetectHandle);

                        printf("Revision            : %i\n", dvdGetRevision(pDvDetectHandle));
                        printf("Protocol Version    : %i\n", dvdGetProtocolVersion(pDvDetectHandle));
                        printf("Library Versiom     : %s\n", dvdGetLibraryVersion(pDvDetectHandle));
                        printf("Library Name        : %s\n", dvdGetLibraryName(pDvDetectHandle));
                        printf("Client Name         : %s\n", dvdGetClientName(pDvDetectHandle));
                    }
                }
                else
                {
                    fprintf(stderr, "DVD not found in database.\n");
                    if (*pszOutFile)
                    {
                        printf("Result saved to %s.\n", pszOutFile);
                    }
                }

                dvdDBClose(&pDvDBHandle);
            }
            else
            {
                fprintf(stderr, "Unable to open database\n");
            }
        }
        else
        {
            fprintf(stderr, "%s\n", dvdGetErrorString(pDvDetectHandle));
        }

        dvdCloseLib(&pDvDetectHandle);
    }
    else
    {
        perror("Error opening dvdetect library");
        res = 1;
    }

#ifdef _WIN32
    WSACleanup();
#endif

    return res;
}

