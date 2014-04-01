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

/*! \file openfromwebdlg.cpp
 *
 *  \brief Qt GUI example open from web dialog
 */

#include "openfromwebdlg.h"
#include "ui_openfromwebdlg.h"

#include <QSettings>

openfromwebdlg::openfromwebdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::openfromwebdlg)
{
    ui->setupUi(this);

    QSettings settings("guiexample.conf");

    settings.beginGroup("libdvdetect");

    ui->lineEditWebUrl->setText(settings.value("weburl").toString());

    settings.endGroup();
}

openfromwebdlg::~openfromwebdlg()
{
    delete ui;
}

QString openfromwebdlg::getWebUrl() const
{
    return ui->lineEditWebUrl->text();
}

void openfromwebdlg::on_buttonBox_accepted()
{
    QSettings settings("guiexample.conf");

    settings.beginGroup("libdvdetect");

    settings.setValue("weburl", ui->lineEditWebUrl->text());

    settings.endGroup();
}
