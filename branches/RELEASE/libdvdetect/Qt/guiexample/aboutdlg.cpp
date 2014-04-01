/*
  dvdetect DVD detection, analysis & DVDETECT lookup library

  Copyright (C) 2013-2014 Norbert Schlia <nschlia@dvdetect.de>

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

/*! \file aboutdlg.cpp
 *
 *  \brief Qt GUI example about dialog code
 */

#include "aboutdlg.h"
#include "ui_aboutdlg.h"

#include "config.h"

aboutdlg::aboutdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutdlg)
{
    ui->setupUi(this);
    ui->textEditAbout->setText(tr(
                                   "Qt GUI Example for <b>%0</b> Qt %1<br /><br />"
                                   "This program demonstrates how to send query, search requests and submit data to the dvdetect server.<br />"
                                   "<br />"
                                   "Copyright (C) 2013-2014 Norbert Schlia<br />"
                                   "<a href=\"nschlia@dvdetect.de\">nschlia@dvdetect.de</a><br />"
                                   "<a href=\"http://www.dvdetect.de/\">http://www.dvdetect.de/</a><br />"
                                   "<br />"
                                   "This program is free software: you can redistribute it and/or modify "
                                   "it under the terms of the GNU General Public License as published by "
                                   "the Free Software Foundation, either version 3 of the License, or "
                                   "(at your option) any later version.<br />"
                                   "<br />"
                                   "This program is distributed in the hope that it will be useful, "
                                   "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                                   "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
                                   "GNU General Public License for more details.<br />"
                                   "<br />"
                                   "You should have received a copy of the GNU General Public License "
                                   "along with this program.  If not, see <a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>."
                                   ).arg(PACKAGE_STRING).arg(QT_VERSION_STR));
}

aboutdlg::~aboutdlg()
{
    delete ui;
}

void aboutdlg::on_pushButtonOK_clicked()
{
    close();
}
