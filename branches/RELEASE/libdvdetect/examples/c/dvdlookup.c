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
 *  \brief DVD online db lookup example (c++ version)
 *
 * This example reads the information of a DVD and looks up that DVD
 * in the online database. If found, the DVD name, chapter list and
 * other information is displayed.
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
 *  \param wFrameRate uint16_t Frame rate (25/30) or 0 to not display
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
    printf("%s [-p] -d PATH-TO-DVD   list DVD structure\n", pszProgramName);
    printf("%s -h                    show help\n", pszProgramName);
    printf("\n");

    printf("Example:\n\n");

#ifdef _WIN32
    printf("dvdinfo -d F:\n");
#else
    printf("dvdinfo -d /mnt/cdrom1/\n");
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
    LPCDVDVMGM pDVDVMGM = dvdGetDVDVMGM(pDvDetectHandle);
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

    for (wTitleSetNo = 1; wTitleSetNo <= pDVDVMGM->m_wPTTNumberOfTitles; wTitleSetNo++)
    {
        LPCDVDPTTVMG pDVDPTTVMG = dvdGetDVDPTTVMG(pDvDetectHandle, wTitleSetNo);
        uint16_t wPtt = 1;

        printf("******************************** Title %02u ********************************\n", wTitleSetNo);

        printf("Chapters            : %u\n", pDVDPTTVMG->m_wNumberOfChapters);
        printl("Title               : %s\n", dvdGetPttVmgTitle(pDvDetectHandle, wTitleSetNo));

        for (wPtt = 1; wPtt <= pDVDPTTVMG->m_wNumberOfChapters; wPtt++)
        {
            LPCDVDPTTVTS pDVDPTTVTS = dvdGetDVDPTTVTS(pDvDetectHandle, wTitleSetNo, wPtt);

            printf("Chapter #           : %u\n", pDVDPTTVTS->m_wProgram);
            printl("Artist              : %s\n", dvdGetPttVtsArtist(pDvDetectHandle, wTitleSetNo, wPtt));
            printl("Title               : %s\n", dvdGetPttVtsTitle(pDvDetectHandle, wTitleSetNo, wPtt));
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
    LPDVDETECTHANDLE pDvDetectHandle = NULL;
    bool bMissingParameter = true;
    int res = 0;
    int c;

    pszProgramName = getFileName(argv[0]);

    printf("%s V%s\n", pszProgramName, LIBDVDETECT_VERSION);
    printf("%s\n\n", LIBDVDETECT_COPYRIGHT);

    opterr = 0;

    while ((c = getopt (argc, argv, "hd:")) != -1)
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
        case '?':
            if (optopt == 'd')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Unknown option character `\\x%x'.\n",
                         optopt);

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

#ifdef _WIN32
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
        fprintf(stderr, "WSAStartup failed with error:\n");
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
        }
    }
#endif

    printf("TESTPATH: %s\n\n", pszPath);

    pDvDetectHandle = dvdOpenLib();
    if (pDvDetectHandle != NULL)
    {
        res = dvdParse(pDvDetectHandle, pszPath);

        if (res == 0)
        {
            // dvddatabase dvdDatabase(pszProgramName);
            // dvdparselst dvdParseLst;
            LPDVDBHANDLE pDvDBHandle = NULL;

            pDvDBHandle = dvdOpenDB(pszProgramName);

            if (pDvDBHandle != NULL)
            {
                printf("Querying DVD...\n");

                // dvdDatabase.setProxy("proxyserver:3128");
                dvdSetDBProxy(pDvDBHandle, "proxyserver:3128");

                // res = dvdDatabase.query(dvdParseLst, dvd);
                res = dvdQueryDB(pDvDBHandle, pDvDetectHandle);

                if (res != 0)
                {
                    fprintf(stderr, "%s\n", dvdGetErrorString(pDvDetectHandle));
                }
                else if (dvdDBResults(pDvDBHandle))
                {
                    // dvd.update(dvdParseLst[0]);
                    dvdUpdate(pDvDBHandle, pDvDetectHandle);
                    res = showStructure(pDvDetectHandle);
                }
                else
                {
                    fprintf(stderr, "DVD not found in database\n");
                }

                dvdCloseDB(pDvDBHandle);
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

        dvdCloseLib(pDvDetectHandle);
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

