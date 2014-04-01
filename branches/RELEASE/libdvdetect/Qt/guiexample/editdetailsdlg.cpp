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

/*! \file editdetailsdlg.cpp
 *
 *  \brief Qt GUI example details editor
 */

#include "editdetailsdlg.h"
#include "ui_editdetailsdlg.h"

#include "dvdetect/dvdparse.h"
#include "explorerutils.h"

#include <QStandardItemModel>
#include <QPushButton>

editdetailsdlg::editdetailsdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editdetailsdlg),
    m_pDvdParse(NULL)
{
    ui->setupUi(this);

    // Localisation
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
}

editdetailsdlg::~editdetailsdlg()
{
    delete ui;
}

void editdetailsdlg::setParser(dvdparse *pDvdParse)
{
    m_pDvdParse = pDvdParse;

    delete ui->treeViewDVDDetails->model();
    ui->treeViewDVDDetails->setModel(new QStandardItemModel(0, 2, this));

    setDvdDetailsColumnSize();

    if (m_pDvdParse == NULL)
    {
        return;
    }

    QStandardItemModel *pModel = qobject_cast<QStandardItemModel *>(ui->treeViewDVDDetails->model());
    QList<QStandardItem*> aitems;

    Q_ASSERT(pModel != NULL);

    aitems.clear();
    aitems.append(getItem(tr("Revision:")));
    aitems.append(getItem(QString::number(m_pDvdParse->getRevision())));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Original Album:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getOriginalAlbum(), true));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Keywords:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getKeywords(), true));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Cast:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getCast(), true));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Crew:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getCrew(), true));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Director:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getDirector(), true));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Screenplay:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getScreenplay(), true));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Producer:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getProducer(), true));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Editor:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getEditing(), true));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Cinematographer:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getCinematography(), true));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Country:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getCountry(), true));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Original Language:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getOriginalLanguage(), true));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Release Date:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getReleaseDate(), true));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Special Features:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getSpecialFeatures(), true));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("EAN/UPC:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getEAN_UPC(), true));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Storyline/Synopsis:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getStoryline(), true));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Remarks:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getRemarks(), true));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Submitter:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getSubmitter()));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Submitter IP:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getSubmitterIP()));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Submission Client:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getClient()));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Protocol Version:")));
    aitems.append(getItem(QString::number(m_pDvdParse->getProtocolVersion())));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Library Version:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getLibraryVersion()));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Library Name:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getLibraryName()));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Client Name:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getClientName()));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Date Created:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getDateCreated()));
    pModel->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Date Last Changed:")));
    aitems.append(getItemFromStdString(m_pDvdParse->getDateLastChanged()));
    pModel->appendRow(aitems);
}

void editdetailsdlg::setDvdDetailsColumnSize()
{
    QStandardItemModel *pModel = qobject_cast<QStandardItemModel *>(ui->treeViewDVDDetails->model());

    Q_ASSERT(pModel != NULL);

    pModel->clear();

    pModel->setColumnCount(2);
    ui->treeViewDVDDetails->setColumnWidth(0, 150);

    QStringList labels;

    labels.append(tr("Field"));
    labels.append(tr("Data"));

    pModel->setHorizontalHeaderLabels(labels);
}

void editdetailsdlg::on_buttonBox_accepted()
{
    QStandardItemModel *pModel = qobject_cast<QStandardItemModel *>(ui->treeViewDVDDetails->model());

    if (m_pDvdParse == NULL || pModel == NULL)
    {
        return;
    }

    m_pDvdParse->setOriginalAlbum(pModel->item(1, 1)->text().toStdString());
    m_pDvdParse->setKeywords(pModel->item(2, 1)->text().toStdString());
    m_pDvdParse->setCast(pModel->item(3, 1)->text().toStdString());
    m_pDvdParse->setCrew(pModel->item(4, 1)->text().toStdString());
    m_pDvdParse->setDirector(pModel->item(5, 1)->text().toStdString());
    m_pDvdParse->setScreenplay(pModel->item(6, 1)->text().toStdString());
    m_pDvdParse->setProducer(pModel->item(7, 1)->text().toStdString());
    m_pDvdParse->setEditing(pModel->item(8, 1)->text().toStdString());
    m_pDvdParse->setCinematography(pModel->item(9, 1)->text().toStdString());
    m_pDvdParse->setCountry(pModel->item(10, 1)->text().toStdString());
    m_pDvdParse->setOriginalLanguage(pModel->item(11, 1)->text().toStdString());
    m_pDvdParse->setReleaseDate(pModel->item(12, 1)->text().toStdString());
    m_pDvdParse->setSpecialFeatures(pModel->item(13, 1)->text().toStdString());
    m_pDvdParse->setEAN_UPC(pModel->item(14, 1)->text().toStdString());
    m_pDvdParse->setStoryline(pModel->item(15, 1)->text().toStdString());
    m_pDvdParse->setRemarks(pModel->item(16, 1)->text().toStdString());
    m_pDvdParse->setSubmitter(pModel->item(17, 1)->text().toStdString());
}
