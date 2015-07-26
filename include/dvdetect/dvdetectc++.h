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

/*! \file dvdetectc++.h
 *
 *  \brief LIBDVDETECT C++ API
 */

#pragma once

#ifndef DVDETECTCPP_H
#define DVDETECTCPP_H

#ifndef __cplusplus
#error "Please include this file in C++ mode only"
#endif

#include <dvdetect/types.h>
#include <dvdetect/vector_ptr.h>
#include <dvdetect/version.h>
#include <dvdetect/dvdetectdll.h>
#include <dvdetect/dvdutils.h>

#include <dvdetect/dvdetectbase.h>
#include <dvdetect/dvdparse.h>
#include <dvdetect/dvddatabase.h>
#include <dvdetect/dvdfile.h>

#endif // DVDETECTCPP_H
