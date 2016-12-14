/*
  dvdetect DVD detection, analysis & DVDETECT lookup library

  Copyright (C) 2013-2015 Norbert Schlia <nschlia@dvdetect.de>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU LESSER GENERAL PUBLIC LICENSE for more details.

  You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*! \file dvdetectdll.h
 *
 *  \brief Special defines for function visibility in Windoze DLL and libraries
 */

#pragma once

#ifndef DVDETECTDLL_H
#define DVDETECTDLL_H

#ifndef BUILDING_STATIC
#if defined _WIN32 || defined __CYGWIN__
#define DLL_PUBLIC 
#define DLL_LOCAL
#else // !defined _WIN32 && !defined __CYGWIN__
#if  __GNUC__ >= 4
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else   // __GNUC__ < 4
#define DLL_PUBLIC
#define DLL_LOCAL
#endif  // __GNUC__ < 4
#endif  // !defined _WIN32 && !defined __CYGWIN__
#else   // !BUILDING_STATIC
#define DLL_PUBLIC
#define DLL_LOCAL
#endif  // !BUILDING_STATIC

#endif // DVDETECTDLL_H
