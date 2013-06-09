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

#include <stdio.h>

/*!
 * Print playtime (and optionally frame rate)
 *
 *  @param qwPlaytimems uint32_t Play time in milliseconds
 *  @param wFrameRate uint16_t Frame rate (25/30) or 0 to not display
 *  @return Adress in bytes
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
    printf("Usage:\n\n");
    printf("dvdinfo PATH-TO-DVD\n");
    printf("\n");

    printf("Example:\n\n");

#ifdef _WIN32
    printf("dvdinfo F:\n");
#else
    printf("dvdinfo /mnt/cdrom1/\n");
#endif
}

int main(int argc, char *argv[])
{
    const char *pszPath = "";
    LPDVDETECTHANDLE pDvDetectHandle = NULL;
    int res = 0;

    printf("dvdinfo V%s\n", LIBDVDETECT_VERSION);
    printf("%s\n\n", DVDETECT_COPYRIGHT);

    if (argc != 2)
    {
        usage();
        return 1;
    }

    pszPath = argv[1];

    printf("TESTPATH: %s\n\n", pszPath);

    pDvDetectHandle = dvdOpenLib();
    if (pDvDetectHandle != NULL)
    {
        res = dvdParse(pDvDetectHandle, pszPath);
        if (res == 0)
        {
            const DVDVMGM *pDVDVMGM = dvdGetDVDVMGM(pDvDetectHandle);
            uint16_t wTitleSetNo = 1;

            printf("**************************************************************************\n");
            printf("Version             : %u.%u\n", pDVDVMGM->m_wVersionNumberMajor, pDVDVMGM->m_wVersionNumberMinor);
            printf("Volumes/Volume No.  : %u/%u\n", pDVDVMGM->m_wNumberOfVolumes, pDVDVMGM->m_wVolumeNumber);
            printf("Side ID             : %u\n", pDVDVMGM->m_wSideID);
            printf("Provider ID         : %s\n", pDVDVMGM->m_szProviderID);
            printf("Number of Title Sets: %u\n", pDVDVMGM->m_wNumberOfTitleSets);

            for (wTitleSetNo = 1; wTitleSetNo <= pDVDVMGM->m_wNumberOfTitleSets; wTitleSetNo++)
            {
                const DVDVTS *pDVDVTS = dvdGetDVDVTS(pDvDetectHandle, wTitleSetNo);
                uint16_t wProgramChainNo = 1;

                printf("******************************** Title %02u ********************************\n", wTitleSetNo);

                printf("Version             : %u.%u\n", pDVDVTS->m_wVersionNumberMajor, pDVDVTS->m_wVersionNumberMinor);
                printf("Program Chains (PGC): %u\n", pDVDVTS->m_wNumberOfProgramChains);

                for (wProgramChainNo = 1; wProgramChainNo <= pDVDVTS->m_wNumberOfProgramChains;  wProgramChainNo++)
                {
                    const DVDPGC *pDVDPGC =dvdGetDVDPGC(pDvDetectHandle, wTitleSetNo, wProgramChainNo);
                    uint16_t wProgram = 1;

                    printf("---------------------------------- PGC -----------------------------------\n");
                    printf("Entry PGC           : %s\n", (pDVDPGC->m_bEntryPGC ? "yes" : "no"));
                    printf("Title Number        : %u\n", pDVDPGC->m_wTitleSetNo);
                    printf("PGC Number          : %u\n", wProgramChainNo);
                    printf("Programs/Cells      : %u/%u\n", pDVDPGC->m_wNumberOfPrograms, pDVDPGC->m_wNumberOfCells);

                    playTime(pDVDPGC->m_qwPlayTime, pDVDPGC->m_wFrameRate);

                    for (wProgram = 1; wProgram <= pDVDPGC->m_wNumberOfPrograms; wProgram++)
                    {
                        const DVDPROGRAM *pDVDPROGRAM = dvdGetDVDPROGRAM(pDvDetectHandle, wTitleSetNo, wProgramChainNo, wProgram);
                        uint16_t wCell = 1;

                        printf("\nProgram #           : %u\n", pDVDPROGRAM->m_wProgramNo);
                        printf("Cells               : %u\n", pDVDPROGRAM->m_wCells);

                        for (wCell = 1; wCell <= pDVDPROGRAM->m_wCells; wCell++)
                        {
                            const DVDCELL *pDVDCELL = dvdGetDVDCELL(pDvDetectHandle, wTitleSetNo, wProgramChainNo, wProgram, wCell);
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
                                const DVDUNIT *pDVDUNIT = dvdGetDVDUNIT(pDvDetectHandle, wTitleSetNo, wProgramChainNo, wProgram, wCell, wUnit);

                                printf("Unit                : %u\n", wUnit);
                                printf("Start Sector        : %u\n", pDVDUNIT->m_dwStartSector);
                                printf("End Sector          : %u\n", pDVDUNIT->m_dwEndSector);
                            }
                        }
                    }
                }
            }

            printf("**************************************************************************\n");
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



