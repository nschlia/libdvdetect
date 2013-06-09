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
 *
 * TO BE IMPLEMENTED YET...
 */

#include <dvdetect/dvdetect.h>

#include <stdio.h>

int main(__attribute__((unused))int argc, __attribute__((unused))char *argv[])
{
    printf("dvdlookup V%s\n", LIBDVDETECT_VERSION);
    printf("%s\n\n", DVDETECT_COPYRIGHT);

    printf("DVD online lookup not implemented in this version.\n");
    printf("Support will be added with next release.\n");
    printf("Please be patient.\n");

    return 0;
}

