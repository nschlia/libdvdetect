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

/*! \file dvddatabase.cpp
 *
 *  \brief Qt GUI example options editor
 */
#include "compat.h"

#include "dvdexplorerdlg.h"
#include "editoptionsdlg.h"
#include "ui_editoptionsdlg.h"

#include <dvddatabase.h>
#include <QSettings>

editoptionsdlg::editoptionsdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editoptionsdlg)
{
    ui->setupUi(this);

    ui->comboBoxScanMode->addItem(tr("Full Scan"), DVDSCANMODE_FULL);
    ui->comboBoxScanMode->addItem(tr("Fast Scan"), DVDSCANMODE_FAST);
    ui->comboBoxScanMode->addItem(tr("Automatic Scan Mode"), DVDSCANMODE_AUTO);

    QSettings settings("guiexample.conf");

    settings.beginGroup("libdvdetect");

    dvddatabase dvdDatabase(PROGRAM_NAME);

    ui->comboBoxScanMode->setCurrentIndex(ui->comboBoxScanMode->findData(settings.value("scanmode", DVDSCANMODE_AUTO)));
    ui->lineEditDVDetectSubmitter->setText(settings.value("submitter", DEFSUBMITTER).toString());
    ui->lineEditDVDetectServerName->setText(settings.value("serverurl", dvdDatabase.getServerUrl().c_str()).toString());
    ui->lineEditDVDetectProxyServer->setText(settings.value("proxyserver").toString());

    settings.endGroup();
}

editoptionsdlg::~editoptionsdlg()
{
    delete ui;
}

void editoptionsdlg::on_buttonBox_accepted()
{
    QSettings settings("guiexample.conf");

    settings.beginGroup("libdvdetect");

    settings.setValue("scanmode", ui->comboBoxScanMode->itemData(ui->comboBoxScanMode->currentIndex()));
    settings.setValue("submitter", ui->lineEditDVDetectSubmitter->text());
    settings.setValue("serverurl", ui->lineEditDVDetectServerName->text());
    settings.setValue("proxyserver", ui->lineEditDVDetectProxyServer->text());

    settings.endGroup();
}
