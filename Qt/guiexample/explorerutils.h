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

/** \file explorerutils.h
 *
 *  \brief Internal utility functions declarations
 */

#pragma once

#ifndef EXPLORERUTILS_H
#define EXPLORERUTILS_H

#include <string>

class QStandardItem;
class QString;
class QVariant;

QStandardItem *     getItem(const QString & text, bool bEditable = false);
QStandardItem *     getItemAndValue(const QString & text, const QVariant & value, bool bEditable = false);
QStandardItem *     getItemFromStdString(const std::string & text, bool bEditable = false);

#endif // EXPLORERUTILS_H
