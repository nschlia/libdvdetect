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

/*! \file editdetailsdlg.cpp
 *
 *  \brief Qt GUI example details editor
 */

#include "editdetailsdlg.h"
#include "ui_editdetailsdlg.h"

#include "dvdparse.h"
#include "explorerutils.h"

#include <QStandardItemModel>

editdetailsdlg::editdetailsdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editdetailsdlg),
    m_pDVD(NULL)
{
    ui->setupUi(this);
}

editdetailsdlg::~editdetailsdlg()
{
    delete ui;
}

void editdetailsdlg::setParser(dvdparse *pDVD)
{
    m_pDVD = pDVD;

    delete ui->treeViewDVDDetails->model();
    ui->treeViewDVDDetails->setModel(new QStandardItemModel(0, 2, this));

    setDVDDetailsColumnSize();

    if (m_pDVD == NULL)
    {
        return;
    }

    QStandardItemModel *model = (QStandardItemModel *)ui->treeViewDVDDetails->model();
    QList<QStandardItem*> aitems;

    aitems.clear();
    aitems.append(getItem(tr("Revision:")));
    aitems.append(getItem(QString::number(m_pDVD->getRevision())));
    model->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Keywords:")));
    aitems.append(getItemFromUtf8(m_pDVD->getKeywords(), true));
    model->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Cast:")));
    aitems.append(getItemFromUtf8(m_pDVD->getCast(), true));
    model->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Crew:")));
    aitems.append(getItemFromUtf8(m_pDVD->getCrew(), true));
    model->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Director:")));
    aitems.append(getItemFromUtf8(m_pDVD->getDirector(), true));
    model->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Country:")));
    aitems.append(getItemFromUtf8(m_pDVD->getCountry(), true));
    model->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("ReleaseDate:")));
    aitems.append(getItemFromUtf8(m_pDVD->getReleaseDate(), true));
    model->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Special Features:")));
    aitems.append(getItemFromUtf8(m_pDVD->getSpecialFeatures(), true));
    model->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("EAN/UPC:")));
    aitems.append(getItemFromUtf8(m_pDVD->getEAN_UPC(), true));
    model->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Storyline/Synopsis:")));
    aitems.append(getItemFromUtf8(m_pDVD->getStoryline(), true));
    model->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Remarks:")));
    aitems.append(getItemFromUtf8(m_pDVD->getRemarks(), true));
    model->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Submitter:")));
    aitems.append(getItemFromUtf8(m_pDVD->getSubmitter(), true));
    model->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Submitter IP:")));
    aitems.append(getItemFromUtf8(m_pDVD->getSubmitterIP()));
    model->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Submission Client:")));
    aitems.append(getItemFromUtf8(m_pDVD->getClient()));
    model->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Date Created:")));
    aitems.append(getItemFromUtf8(m_pDVD->getDateCreated()));
    model->appendRow(aitems);

    aitems.clear();
    aitems.append(getItem(tr("Date Last Changed:")));
    aitems.append(getItemFromUtf8(m_pDVD->getDateLastChanged()));
    model->appendRow(aitems);
}

void editdetailsdlg::setDVDDetailsColumnSize()
{
    QStandardItemModel *pModel = (QStandardItemModel *)ui->treeViewDVDDetails->model();

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
    QStandardItemModel *pModel = (QStandardItemModel *)ui->treeViewDVDDetails->model();

    if (m_pDVD == NULL || pModel == NULL)
    {
        return;
    }

    m_pDVD->setKeywords((const char*)pModel->item(1, 1)->text().toUtf8());
    m_pDVD->setCast((const char*)pModel->item(2, 1)->text().toUtf8());
    m_pDVD->setCrew((const char*)pModel->item(3, 1)->text().toUtf8());
    m_pDVD->setDirector((const char*)pModel->item(4, 1)->text().toUtf8());
    m_pDVD->setCountry((const char*)pModel->item(5, 1)->text().toUtf8());
    m_pDVD->setReleaseDate((const char*)pModel->item(6, 1)->text().toUtf8());
    m_pDVD->setSpecialFeatures((const char*)pModel->item(7, 1)->text().toUtf8());
    m_pDVD->setEAN_UPC((const char*)pModel->item(8, 1)->text().toUtf8());
    m_pDVD->setStoryline((const char*)pModel->item(9, 1)->text().toUtf8());
    m_pDVD->setRemarks((const char*)pModel->item(10, 1)->text().toUtf8());
    m_pDVD->setSubmitter((const char*)pModel->item(11, 1)->text().toUtf8());
}
