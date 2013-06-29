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

/*! \file explorerutils.cpp
 *
 *  \brief Internal utility functions definitions
 */

#include "explorerutils.h"

#include <QStandardItem>

QStandardItem * getItem(const QString & text, bool bEditable /*= false*/)
{
    QStandardItem *item = new QStandardItem(text);
    if (!bEditable)
        item->setEditable(false);
    return item;
}

QStandardItem * getItemFromUtf8(const std::string & text, bool bEditable /*= false*/)
{
    return getItem(QString::fromUtf8(text.c_str()), bEditable);
}
