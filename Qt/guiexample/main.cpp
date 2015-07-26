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

/*! \file main.cpp
 *
 *  \brief Qt GUI example main function
 */

#include <QApplication>
#include <QTextCodec>
#include "dvdexplorerdlg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // See: http://stackoverflow.com/questions/6520485/qt-unicode-and-umlauts
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0) // Qt 5.0.0
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    dvdexplorerdlg w;
    w.show();

    return a.exec();
}
