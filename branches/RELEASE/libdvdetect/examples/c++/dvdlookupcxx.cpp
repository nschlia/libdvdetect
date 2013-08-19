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
 *  \brief DVD online db lookup example (c++ version)
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
    cout << pszProgramName << " [-x] -d PATH-TO-DVD   list DVD structure" << endl;
    cout << pszProgramName << " -x proxy:port         enable proxy" << endl;
    cout << pszProgramName << " -o filename.xml       export structure to xml" << endl;
    cout << pszProgramName << " -h                    show help" << endl;
    cout << endl;

    cout << "Example:" << endl << endl;

#ifdef _WIN32
    cout << pszProgramName << " -d F:" << endl;
#else
    cout << pszProgramName << " -d /mnt/cdrom1/" << endl;
#endif
}

static int showStructure(dvdparse & DVD)
{
    int res = 0;

    cout << "Album Artist        : " << DVD.getAlbumArtist() << endl;
    cout << "Album               : " << DVD.getAlbum() << endl;
    cout << "Genre               : " << DVD.getGenre() << endl;
    cout << "Keywords            : " << DVD.getKeywords() << endl;
    cout << "Storyline/Synopsis  : " << DVD.getStoryline() << endl;
    cout << "Revision            : " << DVD.getRevision() << endl;
    cout << "Submitter           : " << DVD.getSubmitter() << endl;
    cout << "Submitter IP        : " << DVD.getSubmitterIP() << endl;
    cout << "Submission Client   : " << DVD.getClient() << endl;
    cout << "Hash                : " << DVD.getHash() << endl;

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= DVD.getDvdPttVmgCount(); wTitleSetNo++)
    {
        const dvdpttvmg *pDvdPttVmg = DVD.getDvdPttVmg(wTitleSetNo);

        cout << "******************************** Title " << setfill('0')  << setw(2) << wTitleSetNo << " ********************************" << endl;

        cout << "Chapters            : " << pDvdPttVmg->getPttVtsCount() << endl;
        cout << "Title               : " << pDvdPttVmg->getTitle() << endl;

        for (uint16_t wPtt = 1; wPtt <= pDvdPttVmg->getPttVtsCount(); wPtt++)
        {
            const dvdpttvts * pDvdPttVts = pDvdPttVmg->getDvdPttVts(wPtt);
            LPCDVDPTTVTS pDVDPTTVTS = pDvdPttVts->getDVDPTTVTS();

            cout << "Chapter #           : " << pDVDPTTVTS->m_wProgramNo << endl;
            cout << "Artist              : " << pDvdPttVts->getArtist() << endl;
            cout << "Title               : " << pDvdPttVts->getTitle() << endl;
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
    string strProxy;
    string strOutFile;
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

    while ((c = getopt (argc, argv, "hx:d:o:")) != -1)
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
        case 'x':
            strProxy = optarg;
            bUseProxy = true;
            bMissingParameter = false;
            break;
        case 'o':
            strOutFile = optarg;
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
        dvddatabase dvdDatabase(pszProgramName);
        dvdparselst lstDvdParse;

        cout << "Querying DVD..." << endl;

        if (bUseProxy)
        {
            dvdDatabase.setProxy(strProxy);
        }

        res = dvdDatabase.query(&lstDvdParse, &DVD);

        if (res != 0)
        {
            cerr << "DVD query error. " << dvdDatabase.getErrorString() << endl;
        }
        else if (lstDvdParse.size())
        {
            DVD.update(lstDvdParse[0]);

            if (strOutFile.size())
            {
                res = dvdDatabase.write(&DVD, strOutFile);
                if (!res)
                {
                    cout << "Result saved to " << strOutFile << "." << endl;
                }
                else
                {
                    cerr << "Error writing output file. " << dvdDatabase.getErrorString() << endl;
                }

                if (res == 0)
                {
                    cout << "Album Artist        : " << DVD.getAlbumArtist() << endl;
                    cout << "Album               : " << DVD.getAlbum() << endl;
                    cout << "Revision            : " << DVD.getRevision() << endl;
                }
            }
            else
            {
                res = showStructure(DVD);

                cout << "Revision            : " << DVD.getRevision() << endl;
                cout << "Protocol Version    : " << DVD.getProtocolVersion() << endl;
                cout << "Library Versiom     : " << DVD.getLibraryVersion() << endl;
                cout << "Library Name        : " << DVD.getLibraryName() << endl;
                cout << "Client Name         : " << DVD.getClientName() << endl;
            }
        }
        else
        {
            cerr << "DVD not found in database." << endl;;
            if (strOutFile.size())
            {
                cout << "Result saved to " << strOutFile << "." << endl;
            }
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
