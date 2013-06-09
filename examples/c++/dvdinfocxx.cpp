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

/*! \file dvdinfocxx.cpp
 *
 *  \brief DVD info example (c++ version)
 *
 * This example reads the information of a DVD and displays them in a rather
 * confusing manner. The purpose of this example is to demonstrate how to
 * traverse through the DVD structure. If you want a more clearly arranged
 * look, better check out the Qt GUI example instead.
 */

#include <iostream>
#include <iomanip>
#include <stdint.h>

#include <dvdetect/dvdetectc++.h>

using namespace std;
/*!
 * Print playtime (and optionally frame rate)
 *
 *  @param qwPlaytimems uint32_t Play time in milliseconds
 *  @param wFrameRate uint16_t Frame rate (25/30) or -1 to not display
 *  @return Adress in bytes
 */
static void playTime(uint64_t qwPlaytimems, uint16_t wFrameRate = (uint16_t)-1)
{
    uint64_t qwPlayTimeS = qwPlaytimems / 1000;

    cout << "Playback Time       : " <<
            setfill('0') << setw(2) << (qwPlayTimeS) / (60 * 60) << ":" <<
            setfill('0') << setw(2) << ((qwPlayTimeS) / 60) % 60 << ":" <<
            setfill('0') << setw(2) << (qwPlayTimeS) % 60  << "." <<
            setfill('0') << setw(3) << (qwPlaytimems - (qwPlayTimeS * 1000));
    if (wFrameRate != (uint16_t)-1)
    {
        cout << " " << wFrameRate << " fps";
    }

    cout << endl;
}

/*!
 * Show usage info
 */
static void usage()
{
    cout << "Usage:" << endl << endl;
    cout << "dvdinfo PATH-TO-DVD" << endl;
    cout << endl;

    cout << "Example:" << endl << endl;

#ifdef _WIN32
    cout << "dvdinfo F:" << endl;
#else
    cout << "dvdinfo /mnt/cdrom1/" << endl;
#endif
}

int main(int argc, char *argv[])
{
    string strPath;
    int res = 0;

    cout << "dvdinfo/c++ V" << LIBDVDETECT_VERSION << endl;
    cout << DVDETECT_COPYRIGHT << endl << endl;

    if (argc != 2)
    {
        cerr << "Invalid parameter" << endl << endl;
        usage();
        return 1;
    }

    strPath = argv[1];

    cout << "TESTPATH: "<< strPath << endl << endl;

    dvdparse dvd;
    res = dvd.parse(strPath);

    //res = 1;
    if (res == 0)
    {
        const DVDVMGM *pDVDVMGM = dvd.getDVDVMGM();

        cout << "**************************************************************************" << endl;
        cout << "Version             : " << pDVDVMGM->m_wVersionNumberMajor << "." << pDVDVMGM->m_wVersionNumberMinor << endl;
        //cout << "VMG Category        : " << pDVDVMGM->m_dwVMGCategory << endl;
        cout << "Volumes/Volume No.  : " << pDVDVMGM->m_wNumberOfVolumes << "/" << pDVDVMGM->m_wVolumeNumber << endl;
        cout << "Side ID             : " << pDVDVMGM->m_wSideID << endl;
        cout << "Provider ID         : " << pDVDVMGM->m_szProviderID << endl;
        cout << "Number of Title Sets: " << pDVDVMGM->m_wNumberOfTitleSets << endl;

        for (uint16_t wTitleSetNo = 1; wTitleSetNo <= pDVDVMGM->m_wNumberOfTitleSets; wTitleSetNo++)
        {
            const dvdtitle *pDvdTitle = dvd.getDvdTitle(wTitleSetNo);
            const DVDVTS *pDVDVTS = pDvdTitle->getDVDVTS();

            cout << "******************************** Title " << setfill('0')  << setw(2) << wTitleSetNo << " ********************************" << endl;

            cout << "Version             : " << pDVDVTS->m_wVersionNumberMajor << "." << pDVDVTS->m_wVersionNumberMinor << endl;
            cout << "Program Chains (PGC): " << pDVDVTS->m_wNumberOfProgramChains << endl;

            for (uint16_t wProgramChainNo = 1; wProgramChainNo <= pDVDVTS->m_wNumberOfProgramChains;  wProgramChainNo++)
            {
                const dvdpgc *pDvdPgc = pDvdTitle->getDvdPgc(wProgramChainNo);
                const DVDPGC *pDVDPGC = pDvdPgc->getDVDPGC();

                cout << "---------------------------------- PGC -----------------------------------" << endl;
                cout << "Entry PGC           : " << (pDVDPGC->m_bEntryPGC ? "yes" : "no") << endl;
                cout << "Title Number        : " << pDVDPGC->m_wTitleSetNo << endl;
                cout << "PGC Number          : " << wProgramChainNo << endl;
                cout << "Programs/Cells      : " << pDVDPGC->m_wNumberOfPrograms << "/" << pDVDPGC->m_wNumberOfCells << endl;

                playTime(pDVDPGC->m_qwPlayTime, pDVDPGC->m_wFrameRate);

                for (uint16_t wProgram = 1; wProgram <= pDVDPGC->m_wNumberOfPrograms; wProgram++)
                {
                    const dvdprogram *pDvdProgram = pDvdPgc->getDvdProgram(wProgram);
                    const DVDPROGRAM *pDVDPROGRAM = pDvdProgram->getDVDPROGRAM();

                    cout << endl;
                    cout << "Program #           : " << pDVDPROGRAM->m_wProgramNo << endl;
                    cout << "Cells               : " << pDVDPROGRAM->m_wCells << endl;

                    for (uint16_t wCell = 1; wCell <= pDVDPROGRAM->m_wCells; wCell++)
                    {
                        const dvdcell *pDvdCell = pDvdProgram->getDvdCell(wCell);
                        const DVDCELL *pDVDCELL = pDvdCell->getDVDCELL();

                        cout << endl;
                        cout << "Cell                : " << pDVDCELL->m_wCell << endl;
                        cout << "Seamless Multiplex  : " << pDVDCELL->m_bSeamlessMultiplex << endl;
                        cout << "Interleaved         : " << pDVDCELL->m_bInterleaved << endl;
                        cout << "STC Discontinuity   : " << pDVDCELL->m_bSCRdiscontinuity << endl;
                        cout << "Seamless Angle      : " << pDVDCELL->m_bSeamlessAngleLinkedInDSI << endl;
                        cout << "VOB Still Mode      : " << pDVDCELL->m_bVOBStillMode << endl;
                        cout << "Stops Trick Play    : " << pDVDCELL->m_bStopsTrickPlay << endl;
                        cout << "Cell Still Time     : " << pDVDCELL->m_wCellStillTime << endl;
                        cout << "Cell Command        : " << pDVDCELL->m_wCellCommand << endl;

                        cout << "Cell Type           : ";
                        switch (pDVDCELL->m_eCellType)
                        {
                        case CELLTYPE_NORMAL:
                            cout << "normal";
                            break;
                        case CELLTYPE_FIRST:
                            cout << "first of angle block";
                            break;
                        case CELLTYPE_MIDDLE:
                            cout << "middle of angle block";
                            break;
                        case CELLTYPE_LAST:
                            cout << "last of angle block";
                            break;
                        default:
                            cout << "invalid";
                            break;
                        }
                        cout << endl;

                        cout << "Block Type          : ";
                        switch (pDVDCELL->m_eBlockType)
                        {
                        case BLOCKTYPE_NORMAL:
                            cout << "normal";
                            break;
                        case BLOCKTYPE_ANGLE:
                            cout << "angle block";
                            break;
                        default:
                            cout << "invalid";
                            break;
                        }
                        cout << endl;

                        playTime(pDVDCELL->m_qwPlayTime, pDVDCELL->m_wFrameRate);

                        cout << "1st VOBU Start Sec. : " << pDVDCELL->m_dwFirstVOBUStartSector << endl;
                        cout << "1st ILVU End Sector : " << pDVDCELL->m_dwFirstILVUEndSector << endl;
                        cout << "Last VOBU Start Sec.: " << pDVDCELL->m_dwLastVOBUStartSector << endl;
                        cout << "Last VOBU End Sector: " << pDVDCELL->m_dwLastVOBUEndSector << endl;

                        cout << "VOB ID              : " << pDVDCELL->m_wVOBidn << endl;
                        cout << "Cell ID             : " << pDVDCELL->m_wCELLidn << endl;

                        cout << "Cell Pos Info Count : " << pDVDCELL->m_wCellPosInfoCount << endl;
                        cout << "Number Of VOB Ids   : " << pDVDCELL->m_wNumberOfVOBIds << endl;

                        for (uint16_t wUnit = 1; wUnit <= pDVDCELL->m_wCellPosInfoCount; wUnit++)
                        {
                            const dvdunit * pDvdUnit = pDvdCell->getDvdUnit(wUnit);
                            const DVDUNIT *pDVDUNIT = pDvdUnit->getDVDUNIT();

                            cout << "Unit                : " << wUnit << endl;
                            cout << "Start Sector        : " << pDVDUNIT->m_dwStartSector << endl;
                            cout << "End Sector          : " << pDVDUNIT->m_dwEndSector << endl;
                        }
                    }
                }
            }
        }

        cout << "**************************************************************************" << endl;
    }
    else
    {
        cerr << dvd.getErrorString() << endl;
    }

    return res;
}
