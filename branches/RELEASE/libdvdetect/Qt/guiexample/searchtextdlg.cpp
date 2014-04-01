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

/*! \file searchtextdlg.cpp
 *
 *  \brief Qt GUI example search dialog code
 */

#include "searchtextdlg.h"
#include "ui_searchtextdlg.h"

#include <QSettings>
#include <QPushButton>

searchtextdlg::searchtextdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::searchtextdlg)
{
    ui->setupUi(this);

    // Localisation
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));

    QSettings settings("guiexample.conf");

    settings.beginGroup("searchdlg");
    ui->lineEditSearch->setText(settings.value("search").toString());
    settings.endGroup();
}

searchtextdlg::~searchtextdlg()
{
    delete ui;
}

QString searchtextdlg::getSearch() const
{
    return ui->lineEditSearch->text();
}

void searchtextdlg::on_buttonBox_accepted()
{
    QSettings settings("guiexample.conf");

    settings.beginGroup("searchdlg");
    settings.setValue("search", ui->lineEditSearch->text());
    settings.endGroup();
}
