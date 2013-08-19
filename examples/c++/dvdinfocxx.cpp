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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#endif

#include <dvdetect/dvdetectc++.h>

const char *pszProgramName = NULL;

using namespace std;

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
        cerr << "WSAStartup failed with error: " << err << endl;
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
            cerr << "Could not find a usable version of Winsock.dll" << endl;
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
    cout << endl;
    cout << "Usage:" << endl << endl;
    cout << pszProgramName << " [-px] -d PATH-TO-DVD  list DVD structure" << endl;
    cout << pszProgramName << " -x proxy:port         enable proxy" << endl;
    cout << pszProgramName << " -h                    show help" << endl;
    cout << endl << "To list the physical structure, use \"-p\", otherwise the virtual structure will" << endl << "be shown." << endl;
    cout << endl;

    cout << "Example:" << endl << endl;

#ifdef _WIN32
    cout << pszProgramName << " -d F:" << endl;
#else
    cout << pszProgramName << " -d /mnt/cdrom1/" << endl;
#endif
}

static void showHeader(LPCDVDVMGM pDVDVMGM)
{
    cout << "**************************************************************************" << endl;
    cout << "Version             : " << pDVDVMGM->m_wVersionNumberMajor << "." << pDVDVMGM->m_wVersionNumberMinor << endl;
    //cout << "VMG Category        : " << pDVDVMGM->m_dwVMGCategory << endl;
    cout << "Volumes/Volume No.  : " << pDVDVMGM->m_wNumberOfVolumes << "/" << pDVDVMGM->m_wVolumeNumber << endl;
    cout << "Side ID             : " << pDVDVMGM->m_wSideID << endl;
    cout << "Provider ID         : " << pDVDVMGM->m_szProviderID << endl;
}

static void showCellsAndUnits(const dvdprogram *pDvdProgram)
{
    for (uint16_t wCell = 1; wCell <= pDvdProgram->getCellCount(); wCell++)
    {
        const dvdcell *pDvdCell = pDvdProgram->getDvdCell(wCell);
        LPCDVDCELL pDVDCELL = pDvdCell->getDVDCELL();

        cout << endl;
        cout << "Cell                : " << pDVDCELL->m_wCellNo << endl;
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

        cout << "Number Of Cells     : " << pDvdCell->getUnitCount() << endl;
        cout << "Number Of VOB Ids   : " << pDVDCELL->m_wNumberOfVOBIds << endl;

        for (uint16_t wUnit = 1; wUnit <= pDvdCell->getUnitCount(); wUnit++)
        {
            const dvdunit * pDvdUnit = pDvdCell->getDvdUnit(wUnit);
            LPCDVDUNIT pDVDUNIT = pDvdUnit->getDVDUNIT();

            cout << "Unit                : " << wUnit << endl;
            cout << "Start Sector        : " << pDVDUNIT->m_dwStartSector << endl;
            cout << "End Sector          : " << pDVDUNIT->m_dwEndSector << endl;
        }
    }
}

static int showPhysicalStructure(dvdparse & DVD)
{
    LPCDVDVMGM pDVDVMGM = DVD.getDVDVMGM();
    int res = 0;

    showHeader(pDVDVMGM);

    cout << "Number of Title Sets: " << DVD.getTitleSetCount() << endl;

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= DVD.getTitleSetCount(); wTitleSetNo++)
    {
        const dvdtitle *pDvdTitle = DVD.getDvdTitle(wTitleSetNo);
        LPCDVDVTS pDVDVTS = pDvdTitle->getDVDVTS();

        cout << "******************************** Title " << setfill('0')  << setw(2) << wTitleSetNo << " ********************************" << endl;

        cout << "Version             : " << pDVDVTS->m_wVersionNumberMajor << "." << pDVDVTS->m_wVersionNumberMinor << endl;
        cout << "Program Chains (PGC): " << pDvdTitle->getPgcCount() << endl;

        for (uint16_t wProgramChainNo = 1; wProgramChainNo <= pDvdTitle->getPgcCount();  wProgramChainNo++)
        {
            const dvdpgc *pDvdPgc = pDvdTitle->getDvdPgc(wProgramChainNo);
            LPCDVDPGC pDVDPGC = pDvdPgc->getDVDPGC();

            cout << "---------------------------------- PGC -----------------------------------" << endl;
            cout << "Entry PGC           : " << (pDVDPGC->m_bEntryPGC ? "yes" : "no") << endl;
            cout << "Title Number        : " << pDVDPGC->m_wTitleSetNo << endl;
            cout << "PGC Number          : " << wProgramChainNo << endl;
            cout << "Programs            : " << pDvdPgc->getProgramCount() << endl;

            for (uint16_t wProgramNo = 1; wProgramNo <= pDvdPgc->getProgramCount(); wProgramNo++)
            {
                const dvdprogram *pDvdProgram = pDvdPgc->getDvdProgram(wProgramNo);
                LPCDVDPROGRAM pDVDPROGRAM = pDvdProgram->getDVDPROGRAM();

                cout << endl;
                cout << "Program #           : " << pDVDPROGRAM->m_wProgramNo << endl;
                cout << "Cells               : " << pDvdProgram->getCellCount() << endl;

                showCellsAndUnits(pDvdProgram);
            }
        }
    }

    cout << endl;
    cout << "Size                : " << DVD.getSize() / (1024*1024) << " MB" << endl;
    playTime(DVD.getPlayTime());

    cout << "**************************************************************************" << endl;

    return res;
}

static int showVirtualStructure(dvdparse & DVD)
{
    LPCDVDVMGM pDVDVMGM = DVD.getDVDVMGM();
    int res = 0;

    showHeader(pDVDVMGM);

    cout << "Number of Title Sets: " << DVD.getDvdPttVmgCount() << endl;

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= DVD.getDvdPttVmgCount(); wTitleSetNo++)
    {
        const dvdpttvmg *pDvdPttVmg = DVD.getDvdPttVmg(wTitleSetNo);
        const dvdtitle *pDvdTitle = pDvdPttVmg->getDvdTitle();
        LPCDVDVTS pDVDVTS = pDvdTitle->getDVDVTS();
        LPCDVDPTTVMG pDVDPTTVMG = pDvdPttVmg->getDVDPTTVMG();

        cout << "******************************** Title " << setfill('0')  << setw(2) << wTitleSetNo << " ********************************" << endl;

        cout << "Version             : " << pDVDVTS->m_wVersionNumberMajor << "." << pDVDVTS->m_wVersionNumberMinor << endl;
        cout << "Chapters            : " << pDvdPttVmg->getPttVtsCount() << endl;
        cout << "Angles              : " << pDVDPTTVMG->m_wNumberOfVideoAngles << endl;

        for (uint16_t wPtt = 1; wPtt <= pDvdPttVmg->getPttVtsCount(); wPtt++)
        {
            const dvdpttvts * pDvdPttVts = pDvdPttVmg->getDvdPttVts(wPtt);
            LPCDVDPTTVTS pDVDPTTVTS = pDvdPttVts->getDVDPTTVTS();
            const dvdprogram * pDvdProgram = DVD.getDvdProgram(pDVDPTTVTS->m_wTitleSetNo, pDVDPTTVTS->m_wProgramChainNo, pDVDPTTVTS->m_wProgramNo);

            cout << endl;
            cout << "Program #           : " << pDVDPTTVTS->m_wProgramNo << endl;
            cout << "Cells               : " << pDvdProgram->getCellCount() << endl;

            showCellsAndUnits(pDvdProgram);
        }
    }

    cout << endl;
    cout << "Size                : " << DVD.getVirtSize() / (1024*1024) << " MB" << endl;
    playTime(DVD.getVirtPlayTime());

    cout << "**************************************************************************" << endl;

    return res;
}

int main(int argc, char *argv[])
{
    string strPath;
    bool bPhysicalStructure = false;
    string strProxy;
    bool bMissingParameter = true;
    bool bUseProxy = false;
    int res = 0;
    int c;

    pszProgramName = getFileName(argv[0]);

    cout << pszProgramName << "/c++ V" << LIBDVDETECT_VERSION << endl;
    cout << LIBDVDETECT_COPYRIGHT << endl << endl;

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
            strPath = optarg;
            bMissingParameter = false;
            break;
        case 'x':
            strProxy = optarg;
            bUseProxy = true;
            bMissingParameter = false;
            break;
        case '?':
            if (optopt == 'd' || optopt == 'x'|| optopt == 'o')
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
        cerr << "Missing parameter." << endl;
        usage();
        return 1;
    }

    dvdparse DVD;

    if (bUseProxy)
    {
        cout << "Using proxy server: " << strProxy << endl;
        DVD.setProxy(strProxy);
    }

    res = DVD.parse(strPath);

    if (res == 0)
    {
        if (bPhysicalStructure)
        {
            res = showPhysicalStructure(DVD);
        }
        else
        {
            res = showVirtualStructure(DVD);
        }
    }
    else
    {
        cerr << DVD.getErrorString() << endl;
        res = 1;
    }

#ifdef _WIN32
    WSACleanup();
#endif

    return res;
}
