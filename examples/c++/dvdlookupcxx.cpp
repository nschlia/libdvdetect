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
 */

#include <iostream>
#include <stdint.h>

#include <dvdetect/dvdetectc++.h>

using namespace std;

int main(__attribute__((unused))int argc, __attribute__((unused))char *argv[])
{
    cout << "dvdlookup/c++ V" << LIBDVDETECT_VERSION << endl;
    cout << DVDETECT_COPYRIGHT << endl << endl;

    cout << "DVD online lookup not implemented in this version." << endl;
    cout << "Support will be added with next release." << endl;
    cout << "Please be patient." << endl;

    return 0;
}

