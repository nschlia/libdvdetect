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

/*! \file dvdinfo.c
 *
 *  \brief DVD info example (c version)
 *
 * This example reads the information of a DVD and displays them in a rather
 * confusing manner. The purpose of this example is to demonstrate how to
 * traverse through the DVD structure. If you want a more clearly arranged
 * look, better check out the Qt GUI example instead.
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
    printf("%s [-px] -d PATH-TO-DVD  list DVD structure\n", pszProgramName);
    printf("%s -x proxy:port         enable proxy\n", pszProgramName);
    printf("%s -h                    show help\n", pszProgramName);
    printf("\nTo list the physical structure, use \"-p\", otherwise the virtual structure will\nbe shown.\n");
    printf("\n");

    printf("Example:\n\n");

#ifdef _WIN32
    printf("%s -d F:\n", pszProgramName);
#else
    printf("%s -d /mnt/cdrom1/\n", pszProgramName);
#endif
}

static void showHeader(LPCDVDVMGM pDVDVMGM)
{
    printf("**************************************************************************\n");
    printf("Version             : %u.%u\n", pDVDVMGM->m_wVersionNumberMajor, pDVDVMGM->m_wVersionNumberMinor);
    printf("Volumes/Volume No.  : %u/%u\n", pDVDVMGM->m_wNumberOfVolumes, pDVDVMGM->m_wVolumeNumber);
    printf("Side ID             : %u\n", pDVDVMGM->m_wSideID);
    printf("Provider ID         : %s\n", pDVDVMGM->m_szProviderID);
}

static void showCellsAndUnits(LPDVDETECTHANDLE pDvDetectHandle, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo)
{
    uint16_t wCell = 1;

    for (wCell = 1; wCell <= dvdGetCellCount(pDvDetectHandle, wTitleSetNo, wProgramChainNo, wProgramNo); wCell++)
    {
        LPCDVDCELL pDVDCELL = dvdGetDVDCELL(pDvDetectHandle, wTitleSetNo, wProgramChainNo, wProgramNo, wCell);
        uint16_t wUnit = 1;

        printf("\n");
        printf("Cell                : %u\n", pDVDCELL->m_wCellNo);
        printf("Seamless Multiplex  : %u\n", pDVDCELL->m_bSeamlessMultiplex);
        printf("Interleaved         : %u\n", pDVDCELL->m_bInterleaved);
        printf("STC Discontinuity   : %u\n", pDVDCELL->m_bSCRdiscontinuity);
        printf("Seamless Angle      : %u\n", pDVDCELL->m_bSeamlessAngleLinkedInDSI);
        printf("VOB Still Mode      : %u\n", pDVDCELL->m_bVOBStillMode);
        printf("Stops Trick Play    : %u\n", pDVDCELL->m_bStopsTrickPlay);
        printf("Cell Still Time     : %u\n", pDVDCELL->m_wCellStillTime);
        printf("Cell Command        : %u\n", pDVDCELL->m_wCellCommand);

        printf("Cell Type           : ");
        switch (pDVDCELL->m_eCellType)
        {
        case CELLTYPE_NORMAL:
            printf("normal");
            break;
        case CELLTYPE_FIRST:
            printf("first of angle block");
            break;
        case CELLTYPE_MIDDLE:
            printf("middle of angle block");
            break;
        case CELLTYPE_LAST:
            printf("last of angle block");
            break;
        default:
            printf("invalid");
            break;
        }
        printf("\n");

        printf("Block Type          : ");
        switch (pDVDCELL->m_eBlockType)
        {
        case BLOCKTYPE_NORMAL:
            printf("normal");
            break;
        case BLOCKTYPE_ANGLE:
            printf("angle block");
            break;
        default:
            printf("invalid");
            break;
        }
        printf("\n");

        playTime(pDVDCELL->m_qwPlayTime, pDVDCELL->m_wFrameRate);

        printf("1st VOBU Start Sec. : %u\n", pDVDCELL->m_dwFirstVOBUStartSector);
        printf("1st ILVU End Sector : %u\n", pDVDCELL->m_dwFirstILVUEndSector);
        printf("Last VOBU Start Sec.: %u\n", pDVDCELL->m_dwLastVOBUStartSector);
        printf("Last VOBU End Sector: %u\n", pDVDCELL->m_dwLastVOBUEndSector);

        printf("VOB ID              : %u\n", pDVDCELL->m_wVOBidn);
        printf("Cell ID             : %u\n", pDVDCELL->m_wCELLidn);

        printf("Number Of Cells     : %u\n", dvdGetUnitCount(pDvDetectHandle, wTitleSetNo, wProgramChainNo, wProgramNo, wCell));
        printf("Number Of VOB Ids   : %u\n", pDVDCELL->m_wNumberOfVOBIds);

        for (wUnit = 1; wUnit <= dvdGetUnitCount(pDvDetectHandle, wTitleSetNo, wProgramChainNo, wProgramNo, wCell); wUnit++)
        {
            LPCDVDUNIT pDVDUNIT = dvdGetDVDUNIT(pDvDetectHandle, wTitleSetNo, wProgramChainNo, wProgramNo, wCell, wUnit);

            printf("Unit                : %u\n", wUnit);
            printf("Start Sector        : %u\n", pDVDUNIT->m_dwStartSector);
            printf("End Sector          : %u\n", pDVDUNIT->m_dwEndSector);
        }
    }
}

static int showPhysicalStructure(LPDVDETECTHANDLE pDvDetectHandle)
{
    LPCDVDVMGM pDVDVMGM = dvdGetDVDVMGM(pDvDetectHandle);
    uint16_t wTitleSetNo = 1;
    int res = 0;

    showHeader(pDVDVMGM);

    printf("Number of Title Sets: %u\n", dvdGetTitleSetCount(pDvDetectHandle));

    for (wTitleSetNo = 1; wTitleSetNo <= dvdGetTitleSetCount(pDvDetectHandle); wTitleSetNo++)
    {
        LPCDVDVTS pDVDVTS = dvdGetDVDVTS(pDvDetectHandle, wTitleSetNo);
        uint16_t wProgramChainNo = 1;

        printf("******************************** Title %02u ********************************\n", wTitleSetNo);

        printf("Version             : %u.%u\n", pDVDVTS->m_wVersionNumberMajor, pDVDVTS->m_wVersionNumberMinor);
        printf("Program Chains (PGC): %u\n", dvdGetPgcCount(pDvDetectHandle, wTitleSetNo));

        for (wProgramChainNo = 1; wProgramChainNo <= dvdGetPgcCount(pDvDetectHandle, wTitleSetNo); wProgramChainNo++)
        {
            LPCDVDPGC pDVDPGC = dvdGetDVDPGC(pDvDetectHandle, wTitleSetNo, wProgramChainNo);
            uint16_t wProgramNo = 1;

            printf("---------------------------------- PGC -----------------------------------\n");
            printf("Entry PGC           : %s\n", (pDVDPGC->m_bEntryPGC ? "yes" : "no"));
            printf("Title Number        : %u\n", pDVDPGC->m_wTitleSetNo);
            printf("PGC Number          : %u\n", wProgramChainNo);
            printf("Programs            : %u\n", dvdGetProgramCount(pDvDetectHandle, wTitleSetNo, wProgramChainNo));

            for (wProgramNo = 1; wProgramNo <= dvdGetProgramCount(pDvDetectHandle, wTitleSetNo, wProgramChainNo); wProgramNo++)
            {
                LPCDVDPROGRAM pDVDPROGRAM = dvdGetDVDPROGRAM(pDvDetectHandle, wTitleSetNo, wProgramChainNo, wProgramNo);

                printf("\nProgram #           : %u\n", pDVDPROGRAM->m_wProgramNo);
                printf("Cells               : %u\n", dvdGetCellCount(pDvDetectHandle, wTitleSetNo, wProgramChainNo, wProgramNo));

                showCellsAndUnits(pDvDetectHandle, wTitleSetNo, wProgramChainNo, wProgramNo);
            }
        }
    }

    printf("\n");
    printf("Size                : %"PRIu64" MB\n", dvdGetSize(pDvDetectHandle) / (1024*1024));
    playTime(dvdGetPlayTime(pDvDetectHandle), -1);

    printf("**************************************************************************\n");

    return res;
}

static int showVirtualStructure(LPDVDETECTHANDLE pDvDetectHandle)
{
    LPCDVDVMGM pDVDVMGM = dvdGetDVDVMGM(pDvDetectHandle);
    uint16_t wTitleSetNo = 1;
    int res = 0;

    showHeader(pDVDVMGM);

    printf("Number of Title Sets: %u\n", dvdGetDvdPttVmgCount(pDvDetectHandle));

    for (wTitleSetNo = 1; wTitleSetNo <= dvdGetDvdPttVmgCount(pDvDetectHandle); wTitleSetNo++)
    {
        LPCDVDPTTVMG pDVDPTTVMG = dvdGetDVDPTTVMG(pDvDetectHandle, wTitleSetNo);
        LPCDVDVTS pDVDVTS = dvdGetDVDVTS(pDvDetectHandle, pDVDPTTVMG->m_wVideoTitleSetNo);
        uint16_t wPtt = 1;

        printf("******************************** Title %02u ********************************\n", wTitleSetNo);

        printf("Version             : %u.%u\n", pDVDVTS->m_wVersionNumberMajor, pDVDVTS->m_wVersionNumberMinor);
        printf("Chapters            : %u\n", dvdGetPttVtsCount(pDvDetectHandle, wTitleSetNo));
        printf("Angles              : %u\n", (unsigned)pDVDPTTVMG->m_wNumberOfVideoAngles);

        for (wPtt = 1; wPtt <= dvdGetPttVtsCount(pDvDetectHandle, wTitleSetNo); wPtt++)
        {
            LPCDVDPTTVTS pDVDPTTVTS = dvdGetDVDPTTVTS(pDvDetectHandle, wTitleSetNo, wPtt);

            printf("\n");
            printf("Program #           : %u\n", pDVDPTTVTS->m_wProgramNo);
            printf("Cells               : %u\n", dvdGetCellCount(pDvDetectHandle, pDVDPTTVTS->m_wTitleSetNo, pDVDPTTVTS->m_wProgramChainNo, pDVDPTTVTS->m_wProgramNo));

            showCellsAndUnits(pDvDetectHandle, pDVDPTTVTS->m_wTitleSetNo, pDVDPTTVTS->m_wProgramChainNo, pDVDPTTVTS->m_wProgramNo);
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
    LPDVDETECTHANDLE pDvDetectHandle = NULL;
    bool bPhysicalStructure = false;
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

    while ((c = getopt (argc, argv, "hxpd:")) != -1)
    {
        switch (c)
        {
        case 'h':
            usage();
            return 1;
            break;
        case 'p':
            bPhysicalStructure = true;
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
        case '?':
            if (optopt == 'd' || optopt == 'x')
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
            if (bPhysicalStructure)
            {
                showPhysicalStructure(pDvDetectHandle);
            }
            else
            {
                res = showVirtualStructure(pDvDetectHandle);
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

