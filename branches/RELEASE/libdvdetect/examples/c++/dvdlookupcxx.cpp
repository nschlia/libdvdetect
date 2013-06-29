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

/*! \file dvdlookupcxx.cpp
 *
 *  \brief DVD online db lookup example (c version)
 * This example reads the information of a DVD, tries to look it up on the
 * server and displays the information. The purpose of this example is to
 * demonstrate how to get the information from the server and then traverse
 * through the DVD structure. If you want a more clearly arranged look,
 * better check out the Qt GUI example instead.
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
    cout << pszProgramName << " -d PATH-TO-DVD        list DVD structure" << endl;
    cout << pszProgramName << " -h                    show help" << endl;
    cout << endl;

    cout << "Example:" << endl << endl;

#ifdef _WIN32
    cout << pszProgramName << " -d F:" << endl;
#else
    cout << pszProgramName << " -d /mnt/cdrom1/" << endl;
#endif
}

static int showStructure(dvdparse & dvd)
{
    LPCDVDVMGM pDVDVMGM = dvd.getDVDVMGM();
    int res = 0;

    cout << "Album Artist        : " << dvd.getAlbumArtist() << endl;
    cout << "Album               : " << dvd.getAlbum() << endl;
    cout << "Genre               : " << dvd.getGenre() << endl;
    cout << "Keywords            : " << dvd.getKeywords() << endl;
    cout << "Storyline/Synopsis  : " << dvd.getStoryline() << endl;
    cout << "Revision            : " << dvd.getRevision() << endl;
    cout << "Submitter           : " << dvd.getSubmitter() << endl;
    cout << "Submitter IP        : " << dvd.getSubmitterIP() << endl;
    cout << "Submission Client   : " << dvd.getClient() << endl;
    cout << "Hash                : " << dvd.getHash() << endl;

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= pDVDVMGM->m_wPTTNumberOfTitles; wTitleSetNo++)
    {
        const dvdpttvmg *pDvdPttVmg = dvd.getDvdPttVmg(wTitleSetNo);

        cout << "******************************** Title " << setfill('0')  << setw(2) << wTitleSetNo << " ********************************" << endl;

        cout << "Chapters            : " << pDvdPttVmg->getPttCount() << endl;
        cout << "Title               : " << pDvdPttVmg->getTitle() << endl;

        for (uint16_t wPtt = 1; wPtt <= pDvdPttVmg->getPttCount(); wPtt++)
        {
            const dvdpttvts * pDvdPttVts = pDvdPttVmg->getDvdPttVts(wPtt);
            LPCDVDPTTVTS pDVDPTTVTS = pDvdPttVts->getDVDPTTVTS();

            cout << "Chapter #           : " << pDVDPTTVTS->m_wProgram << endl;
            cout << "Artist              : " << pDvdPttVts->getArtist() << endl;
            cout << "Title               : " << pDvdPttVts->getTitle() << endl;
        }
    }

    cout << endl;
    cout << "Size                : " << dvd.getVirtSize() / (1024*1024) << " MB" << endl;
    playTime(dvd.getVirtPlayTime());

    cout << "**************************************************************************" << endl;

    return res;
}

int main(int argc, char *argv[])
{
    string strPath;
    bool bMissingParameter = true;
    int res = 0;
    int c;

    pszProgramName = getFileName(argv[0]);

    cout << pszProgramName << "/c++ V" << LIBDVDETECT_VERSION << endl;
    cout << LIBDVDETECT_COPYRIGHT << endl << endl;

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
            strPath = optarg;
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
        cerr << "Missing parameter." << endl;
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
        cerr << "WSAStartup failed with error: " << err << endl;
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
        }
    }
#endif

    cout << "TESTPATH: "<< strPath << endl << endl;

    dvdparse dvd;
    res = dvd.parse(strPath);

    if (res == 0)
    {
        dvddatabase dvdDatabase(pszProgramName);
        dvdparselst dvdParseLst;

        cout << "Querying DVD..." << endl;

        // dvdDatabase.setProxy("proxyserver:3128");

        res = dvdDatabase.query(dvdParseLst, dvd);

        if (res != 0)
        {
            cerr << "DVD query error: " << dvdDatabase.getErrorString() << endl;
        }
        else if (dvdParseLst.size())
        {
            dvd.update(dvdParseLst[0]);
            res = showStructure(dvd);
        }
        else
        {
            cerr << "DVD not found in database" << endl;;
        }
    }
    else
    {
        cerr << dvd.getErrorString() << endl;
    }

#ifdef _WIN32
    WSACleanup();
#endif

    return res;
}
