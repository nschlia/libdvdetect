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

/*! \file explorerutils.cpp
 *
 *  \brief Internal utility functions definitions
 */

#include "explorerutils.h"

#include <QStandardItem>

QStandardItem * getItem(const QString & text, bool bEditable /*= false*/)
{
    QStandardItem *pItem = new QStandardItem(text);
    if (!bEditable)
    {
        pItem->setEditable(false);
    }
    return pItem;
}

QStandardItem * getItemAndValue(const QString & text, const QVariant & value, bool bEditable /*= false*/)
{
    QStandardItem *pItem = new QStandardItem(text);
    pItem->setData(value);
    if (!bEditable)
    {
        pItem->setEditable(false);
    }
    return pItem;
}

QStandardItem * getItemFromStdString(const std::string & text, bool bEditable /*= false*/)
{
    QStandardItem *pItem = new QStandardItem(text.c_str());
    if (!bEditable)
    {
        pItem->setEditable(false);
    }
    return pItem;
}
