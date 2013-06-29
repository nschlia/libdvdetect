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
    printf("\nTo list the physical structure, use \"-p\", otherwise the virtual structure will\nbe shown.\n");
    printf("\n");

    printf("Example:\n\n");

#ifdef _WIN32
    printf("dvdinfo -d F:\n");
#else
    printf("dvdinfo -d /mnt/cdrom1/\n");
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

static void showCellsAndUnits(LPDVDETECTHANDLE pDvDetectHandle, LPCDVDPROGRAM pDVDPROGRAM, uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram)
{
    uint16_t wCell = 1;

    for (wCell = 1; wCell <= pDVDPROGRAM->m_wCells; wCell++)
    {
        LPCDVDCELL pDVDCELL = dvdGetDVDCELL(pDvDetectHandle, wTitleSetNo, wProgramChainNo, wProgram, wCell);
        uint16_t wUnit = 1;

        printf("\nCell                : %u\n", pDVDCELL->m_wCell);
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

        printf("Cell Pos Info Count : %u\n", pDVDCELL->m_wCellPosInfoCount);
        printf("Number Of VOB Ids   : %u\n", pDVDCELL->m_wNumberOfVOBIds);

        for (wUnit = 1; wUnit <= pDVDCELL->m_wCellPosInfoCount; wUnit++)
        {
            LPCDVDUNIT pDVDUNIT = dvdGetDVDUNIT(pDvDetectHandle, wTitleSetNo, wProgramChainNo, wProgram, wCell, wUnit);

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

    printf("Number of Title Sets: %u\n", pDVDVMGM->m_wNumberOfTitleSets);

    for (wTitleSetNo = 1; wTitleSetNo <= pDVDVMGM->m_wNumberOfTitleSets; wTitleSetNo++)
    {
        LPCDVDVTS pDVDVTS = dvdGetDVDVTS(pDvDetectHandle, wTitleSetNo);
        uint16_t wProgramChainNo = 1;

        printf("******************************** Title %02u ********************************\n", wTitleSetNo);

        printf("Version             : %u.%u\n", pDVDVTS->m_wVersionNumberMajor, pDVDVTS->m_wVersionNumberMinor);
        printf("Program Chains (PGC): %u\n", pDVDVTS->m_wNumberOfProgramChains);

        for (wProgramChainNo = 1; wProgramChainNo <= pDVDVTS->m_wNumberOfProgramChains;  wProgramChainNo++)
        {
            LPCDVDPGC pDVDPGC =dvdGetDVDPGC(pDvDetectHandle, wTitleSetNo, wProgramChainNo);
            uint16_t wProgram = 1;

            printf("---------------------------------- PGC -----------------------------------\n");
            printf("Entry PGC           : %s\n", (pDVDPGC->m_bEntryPGC ? "yes" : "no"));
            printf("Title Number        : %u\n", pDVDPGC->m_wTitleSetNo);
            printf("PGC Number          : %u\n", wProgramChainNo);
            printf("Programs/Cells      : %u/%u\n", pDVDPGC->m_wNumberOfPrograms, pDVDPGC->m_wNumberOfCells);

            playTime(pDVDPGC->m_qwPlayTime, pDVDPGC->m_wFrameRate);

            for (wProgram = 1; wProgram <= pDVDPGC->m_wNumberOfPrograms; wProgram++)
            {
                LPCDVDPROGRAM pDVDPROGRAM = dvdGetDVDPROGRAM(pDvDetectHandle, wTitleSetNo, wProgramChainNo, wProgram);

                printf("\nProgram #           : %u\n", pDVDPROGRAM->m_wProgramNo);
                printf("Cells               : %u\n", pDVDPROGRAM->m_wCells);

                showCellsAndUnits(pDvDetectHandle, pDVDPROGRAM, wTitleSetNo, wProgramChainNo, wProgram);
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

    printf("Number of Title Sets: %u\n", pDVDVMGM->m_wPTTNumberOfTitles);

    for (wTitleSetNo = 1; wTitleSetNo <= pDVDVMGM->m_wPTTNumberOfTitles; wTitleSetNo++)
    {
        LPCDVDPTTVMG pDVDPTTVMG = dvdGetDVDPTTVMG(pDvDetectHandle, wTitleSetNo);
        LPCDVDVTS pDVDVTS = dvdGetDVDVTS(pDvDetectHandle, wTitleSetNo);
        uint16_t wPtt = 1;

        printf("******************************** Title %02u ********************************\n", wTitleSetNo);

        printf("Version             : %u.%u\n", pDVDVTS->m_wVersionNumberMajor, pDVDVTS->m_wVersionNumberMinor);
        printf("Chapters            : %u\n", pDVDPTTVMG->m_wNumberOfChapters);
        printf("Angles              : %u\n", (unsigned)pDVDPTTVMG->m_byNumberOfVideoAngles);                                   //!< Number of video angles (1...9)

        for (wPtt = 1; wPtt <= pDVDPTTVMG->m_wNumberOfChapters; wPtt++)
        {
            LPCDVDPTTVTS pDVDPTTVTS = dvdGetDVDPTTVTS(pDvDetectHandle, wTitleSetNo, wPtt);
            LPCDVDPROGRAM pDVDPROGRAM = dvdGetDVDPROGRAM(pDvDetectHandle, pDVDPTTVTS->m_wTitleSetNo, pDVDPTTVTS->m_wProgramChainNo, pDVDPTTVTS->m_wProgram);

            printf("\n");
            printf("Program #           : %u\n", pDVDPTTVTS->m_wProgram);
            printf("Cells               : %u\n", pDVDPROGRAM->m_wCells);

            showCellsAndUnits(pDvDetectHandle, pDVDPROGRAM, pDVDPTTVTS->m_wTitleSetNo, pDVDPTTVTS->m_wProgramChainNo, pDVDPTTVTS->m_wProgram);
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
    bool bPhysicalStructure = false;
    bool bMissingParameter = true;
    int res = 0;
    int c;

    pszProgramName = getFileName(argv[0]);

    printf("%s V%s\n", pszProgramName, LIBDVDETECT_VERSION);
    printf("%s\n\n", LIBDVDETECT_COPYRIGHT);

    opterr = 0;

    while ((c = getopt (argc, argv, "hpd:")) != -1)
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

    printf("TESTPATH: %s\n\n", pszPath);

    pDvDetectHandle = dvdOpenLib();
    if (pDvDetectHandle != NULL)
    {
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

        dvdCloseLib(pDvDetectHandle);
    }
    else
    {
        perror("Error opening dvdetect library");
        res = 1;
    }

    return res;
}



