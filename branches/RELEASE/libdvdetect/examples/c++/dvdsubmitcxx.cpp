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

/*! \file dvdsubmitcxx.cpp
 *
 *  \brief DVD online db submit example (c++ version)
 *
 * This example reads the information of a DVD from an xml file, and
 * submits it to the server. The purpose of this example is to
 * demonstrate how to submit information to the server.
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
#include <errno.h>
#include <sys/stat.h>
#include <fstream>

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
 * Show usage info
 */
static void usage()
{
    cout << endl;
    cout << "Usage:" << endl << endl;
    cout << pszProgramName << " [-x] -i input.xml     xml to send" << endl;
    cout << pszProgramName << " -x proxy:port         enable proxy" << endl;
    cout << pszProgramName << " -h                    show help" << endl;
    cout << endl;

    cout << "Example:" << endl << endl;

    cout << pszProgramName << " -i mydvd.xml" << endl;
}


int main(int argc, char *argv[])
{
    string strInputFile;
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

    while ((c = getopt (argc, argv, "hi:")) != -1)
    {
        switch (c)
        {
        case 'h':
            usage();
            return 1;
            break;
        case 'i':
            strInputFile = optarg;
            bMissingParameter = false;
            break;
        case 'x':
            strProxy = optarg;
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
        cerr << "Missing parameter." << endl;
        usage();
        return 1;
    }

    dvddatabase dvdDatabase(LIBDVDETECT_VERSION);
    dvdparselst lstDvdParseOut;

    if (bUseProxy)
    {
        cout << "Using proxy server: " << strProxy << endl;
        dvdDatabase.setProxy(strProxy);
    }

    res = dvdDatabase.read(&lstDvdParseOut, strInputFile);

    if (res != 0)
    {
        cerr << "DVD read error. " << dvdDatabase.getErrorString() << endl;
    }
    else
    {
        dvdparselst::const_iterator itDvdParse;
        for (itDvdParse = lstDvdParseOut.begin(); itDvdParse != lstDvdParseOut.end(); ++itDvdParse)
        {
            cout << "Submit Hash         : " << (*itDvdParse)->getHash() << endl;
            cout << "Revision            : " << (*itDvdParse)->getRevision() << endl << endl;
        }

        cout << "Submitting DVD..." << endl << endl;

        res = dvdDatabase.submit(&lstDvdParseOut);

        if (res != 0)
        {
            cerr << "DVD submit error. " << dvdDatabase.getErrorString() << endl;
        }
    }

    if (res == 0)
    {
        dvdparselst::const_iterator itDvdParseIn;
        for (itDvdParseIn = lstDvdParseOut.begin(); itDvdParseIn != lstDvdParseOut.end() && !res; ++itDvdParseIn)
        {
            dvdparselst lstDvdParse;

            res = dvdDatabase.query(&lstDvdParse, *itDvdParseIn);

            if (res)
            {
                cerr << "DVD query error. " << dvdDatabase.getErrorString() << endl;
                break;
            }

            dvdparselst::const_iterator itDvdParse;
            for (itDvdParse = lstDvdParse.begin(); itDvdParse != lstDvdParse.end(); ++itDvdParse)
            {
                cout << "Submit Hash         : " << (*itDvdParse)->getHash() << endl;
                cout << "Album Artist        : " << (*itDvdParse)->getAlbumArtist() << endl;
                cout << "Album               : " << (*itDvdParse)->getAlbum() << endl;
                cout << "New Revision at     : " << (*itDvdParse)->getRevision() << endl << endl;
            }
            cout << "Submit successful." << endl;
        }
    }

#ifdef _WIN32
    WSACleanup();
#endif

    return res;
}
