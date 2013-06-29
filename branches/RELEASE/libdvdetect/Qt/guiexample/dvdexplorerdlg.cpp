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

/*! \file dvdexplorerdlg.cpp
 *
 *  \brief Qt GUI example main window code
 */

#include <math.h>

#include "dvdexplorerdlg.h"
#include "ui_dvdexplorerdlg.h"
#include "searchtextdlg.h"
#include "aboutdlg.h"
#include "editdetailsdlg.h"
#include "explorerutils.h"

#ifdef _WIN32
#include <winsock2.h>
#endif

#include <QFileDialog>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>

#define PROGRAM_NAME "libDVDetect Explorer Example"

std::string name( const std::type_info& tinfo ) { return tinfo.name() ; }

using namespace std;

dvdexplorerdlg::dvdexplorerdlg(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::dvdexplorerdlg)
{
    ui->setupUi(this);

#ifdef _WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        QMessageBox::critical(this, tr("Error"), tr("WSAStartup failed with error: %1").arg(err));
    }
    else
    {
        /* Confirm that the WinSock DLL supports 2.2.*/
        /* Note that if the DLL supports versions greater    */
        /* than 2.2 in addition to 2.2, it will still return */
        /* 2.2 in wVersion since that is the version we      */
        /* requested.                                        */

        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        {
            /* Tell the user that we could not find a usable */
            /* WinSock DLL.                                  */
            QMessageBox::critical(this, tr("Error"), tr("Could not find a usable version of Winsock.dll"));
            WSACleanup();
        }
    }
#endif

    clearDetails();
}

dvdexplorerdlg::~dvdexplorerdlg()
{
    delete ui;

#ifdef _WIN32
    WSACleanup();
#endif
}

void dvdexplorerdlg::showCells(const dvdprogram* pDvdProgram, QStandardItem* parent)
{
    LPCDVDPROGRAM pDVDPROGRAM = pDvdProgram->getDVDPROGRAM();
    QList<QStandardItem*> aitems;

    for (uint16_t wCell = 1; wCell <= pDVDPROGRAM->m_wCells; wCell++)
    {
        const dvdcell *pDvdCell = pDvdProgram->getDvdCell(wCell);
        LPCDVDCELL pDVDCELL = pDvdCell->getDVDCELL();

        QStandardItem* child1 = getItem(tr("Cell %0").arg(wCell));

        aitems.clear();
        aitems.append(child1);
        aitems.append(getItem(tr("%0").arg(getFormattedSize(pDvdCell->getSize()))));
        aitems.append(getItem(tr("%0").arg(getPlayTime(pDVDCELL->m_qwPlayTime))));
        aitems.append(getItem(""));
        aitems.append(getItem(""));
        aitems.append(getItem(""));
        parent->appendRow(aitems);

        child1->setData(QVariant((long long)pDvdCell));

        for (uint16_t wUnit = 1; wUnit <= pDVDCELL->m_wCellPosInfoCount; wUnit++)
        {
            const dvdunit *pDvdUnit = pDvdCell->getDvdUnit(wUnit);
            //LPDVDUNIT pDVDUNIT = pDvdUnit->getDVDUNIT();

            QStandardItem* child2 = getItem(tr("Unit %0").arg(wUnit));

            aitems.clear();
            aitems.append(child2);
            aitems.append(getItem(tr("%0").arg(getFormattedSize(pDvdUnit->getSize()))));
            aitems.append(getItem(tr("%0").arg(""/*getPlayTime(pDvdCell->getPlayTime())*/)));
            aitems.append(getItem(""));
            aitems.append(getItem(""));
            aitems.append(getItem(""));
            child1->appendRow(aitems);

            child2->setData(QVariant((long long)pDvdUnit));
        }
    }
}

void dvdexplorerdlg::showPhysicalView()
{
    LPCDVDVMGM pDVDVMGM = m_DVD.getDVDVMGM();
    QStandardItemModel *ptreeViewDVDModel = (QStandardItemModel *)ui->treeViewDVD->model();
    QList<QStandardItem*> aitems;

    qDebug() << "Building physical view";

    ui->treeViewDVD->setEditTriggers(QTreeView::NoEditTriggers);   // Cannot edit in physical mode

    setDVDColumnSize();

    QStandardItem* item = getItem(tr("DVD [%0]").arg(QString::fromUtf8(m_DVD.getPath().c_str())));

    item->setData(QVariant((long long)&m_DVD));

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= pDVDVMGM->m_wNumberOfTitleSets; wTitleSetNo++)
    {
        const dvdtitle *pDvdTitle = m_DVD.getDvdTitle(wTitleSetNo);
        LPCDVDVTS pDVDVTS = pDvdTitle->getDVDVTS();

        QStandardItem* child1 = getItem(tr("Title %0").arg(wTitleSetNo));

        aitems.clear();
        aitems.append(child1);
        aitems.append(getItem(tr("%0").arg(getFormattedSize(pDvdTitle->getSize()))));
        aitems.append(getItem(tr("%0").arg(getPlayTime(pDvdTitle->getPlayTime()))));
        aitems.append(getItem(""));
        aitems.append(getItem(""));
        aitems.append(getItem(""));
        item->appendRow(aitems);

        child1->setData(QVariant((long long)pDvdTitle));

        for (uint16_t wProgramChainNo = 1; wProgramChainNo <= pDVDVTS->m_wNumberOfProgramChains;  wProgramChainNo++)
        {
            const dvdpgc *pDvdPgc = pDvdTitle->getDvdPgc(wProgramChainNo);
            LPCDVDPGC pDVDPGC = pDvdPgc->getDVDPGC();

            QStandardItem* child2 = getItem(tr("PGC %0").arg(wProgramChainNo));

            aitems.clear();
            aitems.append(child2);
            aitems.append(getItem(tr("%0").arg(getFormattedSize(pDvdPgc->getSize()))));
            aitems.append(getItem(tr("%0").arg(getPlayTime(pDVDPGC->m_qwPlayTime))));
            aitems.append(getItem(""));
            aitems.append(getItem(""));
            aitems.append(getItem(""));
            child1->appendRow(aitems);

            child2->setData(QVariant((long long)pDvdPgc));

            getPlayTime(pDVDPGC->m_qwPlayTime, pDVDPGC->m_wFrameRate);

            for (uint16_t wProgram = 1; wProgram <= pDVDPGC->m_wNumberOfPrograms; wProgram++)
            {
                const dvdprogram * pDvdProgram = pDvdPgc->getDvdProgram(wProgram);

                QStandardItem* child3 = getItem(tr("Program %0").arg(wProgram));

                aitems.clear();
                aitems.append(child3);
                aitems.append(getItem(tr("%0").arg(getFormattedSize(pDvdProgram->getSize()))));
                aitems.append(getItem(tr("%0").arg(getPlayTime(pDvdProgram->getPlayTime()))));
                aitems.append(getItem(""));
                aitems.append(getItem(""));
                aitems.append(getItem(""));
                child2->appendRow(aitems);

                child3->setData(QVariant((long long)pDvdProgram));
                //child3->setIcon(m_iconcamera);

                showCells(pDvdProgram, child3);
            }
        }
    }

    ptreeViewDVDModel->setItem(0, 0, item);
    ptreeViewDVDModel->setItem(0, 1, getItem(tr("%0").arg(getFormattedSize(m_DVD.getSize()))));
    ptreeViewDVDModel->setItem(0, 2, getItem(tr("%0").arg(getPlayTime(m_DVD.getPlayTime()))));
    ptreeViewDVDModel->setItem(0, 3, getItem(""));
    ptreeViewDVDModel->setItem(0, 4, getItem(""));
    ptreeViewDVDModel->setItem(0, 5, getItem(""));

    QModelIndex index = ptreeViewDVDModel->indexFromItem(item);
    ui->treeViewDVD->setExpanded(index, true);
    QItemSelectionModel *selection = ui->treeViewDVD->selectionModel();
    selection->select(index, QItemSelectionModel::ClearAndSelect);
}

void dvdexplorerdlg::showVirtualView()
{
    LPCDVDVMGM pDVDVMGM = m_DVD.getDVDVMGM();
    QStandardItemModel *ptreeViewDVDModel = (QStandardItemModel *)ui->treeViewDVD->model();
    QList<QStandardItem*> aitems;

    qDebug() << "Building virtual view";

    ui->treeViewDVD->setEditTriggers(QTreeView::AllEditTriggers);

    setDVDColumnSize();

    QStandardItem* item = getItem(tr("DVD [%0]").arg(QString::fromUtf8(m_DVD.getPath().c_str())));

    item->setData(QVariant((long long)&m_DVD));

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= pDVDVMGM->m_wPTTNumberOfTitles; wTitleSetNo++)
    {
        const dvdpttvmg *pDvdPttVmg = m_DVD.getDvdPttVmg(wTitleSetNo);
        const dvdtitle *pDvdTitle = pDvdPttVmg->getDvdTitle();
        //LPCDVDVTS pDVDVTS = pDvdTitle->getDVDVTS();

        QStandardItem* child1 = getItem(tr("Title %0").arg(wTitleSetNo));

        aitems.clear();
        aitems.append(child1);
        aitems.append(getItem(tr("%0").arg(getFormattedSize(pDvdPttVmg->getSize()))));
        aitems.append(getItem(tr("%0").arg(getPlayTime(pDvdPttVmg->getPlayTime()))));
        aitems.append(getItem(""));
        aitems.append(getItemFromUtf8(pDvdPttVmg->getTitle(), true));
        aitems.append(getItem(""));
        item->appendRow(aitems);

        child1->setData(QVariant((long long)pDvdTitle));

        for (uint16_t wPtt = 1; wPtt <= pDvdPttVmg->getPttCount(); wPtt++)
        {
            const dvdpttvts * pDvdPttVts = pDvdPttVmg->getDvdPttVts(wPtt);
            LPCDVDPTTVTS pDVDPTTVTS = pDvdPttVts->getDVDPTTVTS();
            const dvdprogram * pDvdProgram = m_DVD.getDvdProgram(pDVDPTTVTS);

            QStandardItem* child2 = getItem(tr("Chapter %0").arg(pDVDPTTVTS->m_wProgram));

            aitems.clear();
            aitems.append(child2);
            aitems.append(getItem(tr("%0").arg(getFormattedSize(pDvdPttVts->getSize()))));
            aitems.append(getItem(tr("%0").arg(getPlayTime(pDvdPttVts->getPlayTime()))));
            aitems.append(getItemFromUtf8(pDvdPttVts->getArtist(), true));
            aitems.append(getItemFromUtf8(pDvdPttVts->getTitle(), true));
            aitems.append(getItem(""));
            child1->appendRow(aitems);

            child2->setData(QVariant((long long)pDvdProgram));
            //child2->setIcon(m_iconcamera);

            showCells(pDvdProgram, child2);
        }
    }

    ptreeViewDVDModel->setItem(0, 0, item);
    ptreeViewDVDModel->setItem(0, 1, getItem(tr("%0").arg(getFormattedSize(m_DVD.getVirtSize()))));
    ptreeViewDVDModel->setItem(0, 2, getItem(tr("%0").arg(getPlayTime(m_DVD.getVirtPlayTime()))));
    ptreeViewDVDModel->setItem(0, 3, getItemFromUtf8(m_DVD.getAlbumArtist(), true));
    ptreeViewDVDModel->setItem(0, 4, getItemFromUtf8(m_DVD.getAlbum(), true));
    ptreeViewDVDModel->setItem(0, 5, getItemFromUtf8(m_DVD.getGenre(), true));

    QModelIndex index = ptreeViewDVDModel->indexFromItem(item);
    ui->treeViewDVD->setExpanded(index, true);
    QItemSelectionModel *selection = ui->treeViewDVD->selectionModel();
    selection->select(index, QItemSelectionModel::ClearAndSelect);
}

void dvdexplorerdlg::updateDialog()
{
    if (m_DVD.isLoaded())
    {
        ui->actionQuery_DVD->setEnabled(true);
        ui->actionSubmit_DVD->setEnabled(true);
        ui->action_Edit_View_Details->setEnabled(true);

        if (ui->checkBoxPhysicalView->isChecked())
        {
            showPhysicalView();
        }
        else
        {
            showVirtualView();
        }

        showDetails(&m_DVD);
    }
    else
    {
        ui->actionQuery_DVD->setEnabled(false);
        ui->actionSubmit_DVD->setEnabled(false);
        ui->action_Edit_View_Details->setEnabled(false);

        clearDetails();
    }
}

int dvdexplorerdlg::parseDVD(const QString & strPath)
{
    int res = 0;

    showStreams(NULL);
    showFiles((dvdparse*)NULL);

    res = m_DVD.parse((const char *)strPath.toUtf8());

    updateDialog();

    if (res != 0)
    {
        QMessageBox::critical(this, tr("Error"), QString::fromUtf8(m_DVD.getErrorString().c_str()));
    }

    return res;
}

int dvdexplorerdlg::queryDVD()
{
    dvddatabase dvdDatabase(PROGRAM_NAME);
    dvdparselst dvdParseLst;
    int res = 0;

    ui->statusBar->showMessage(tr("Querying DVD..."));

    //dvdDatabase.setProxy("proxyserver:3128");

    res = dvdDatabase.query(dvdParseLst, m_DVD);

    if (res != 0)
    {
        ui->statusBar->showMessage(tr("DVD query error"));
        QMessageBox::critical(this, tr("Error"), QString::fromUtf8(dvdDatabase.getErrorString().c_str()));
    }
    else if (dvdParseLst.size())
    {
        m_DVD.update(dvdParseLst[0]);
        ui->checkBoxPhysicalView->setChecked(false);
        updateDialog();
        ui->statusBar->showMessage(tr("DVD found"));
    }
    else
    {
        ui->statusBar->showMessage(tr("DVD not found"));
        QMessageBox::information(this, tr("Info"), tr("DVD not found in database"));
    }

    return res;
}

int dvdexplorerdlg::searchDVD()
{
    searchtextdlg searchTextDlg(this);
    int res = -1;

    if (searchTextDlg.exec() == QDialog::Accepted)
    {
        dvddatabase dvdDatabase(PROGRAM_NAME);
        dvdparselst dvdParseLst;

        ui->statusBar->showMessage(tr("Searching DVD..."));

        dvdParseLst.push_back(m_DVD);

        //dvdDatabase.setProxy("proxyserver:3128");

        res = dvdDatabase.search(dvdParseLst, (const char *)searchTextDlg.getSearch().toUtf8());

        if (res != 0)
        {
            ui->statusBar->showMessage(tr("DVD search failed"));
            QMessageBox::critical(this, tr("Error"), QString::fromUtf8(dvdDatabase.getErrorString().c_str()));
        }
        else
        {
            ui->statusBar->showMessage(tr("DVD search complete"));
        }
    }

    return res;
}

int dvdexplorerdlg::submitDVD()
{
    dvddatabase dvdDatabase(PROGRAM_NAME);
    dvdparselst dvdParseLst;
    int res = 0;

    if (ui->checkBoxPhysicalView->isChecked())
    {
        QMessageBox::information(this, tr("Information"), tr("Cannot submit DVD in physical mode."));
        return -1;
    }

    ui->statusBar->showMessage(tr("Submitting DVD..."));

    QStandardItemModel *ptreeViewDVDModel = (QStandardItemModel *)ui->treeViewDVD->model();
    QStandardItem *item = NULL;

    item = ptreeViewDVDModel->item(0, 3);
    m_DVD.setAlbumArtist((const char*)item->text().toUtf8());
    item = ptreeViewDVDModel->item(0, 4);
    m_DVD.setAlbum((const char*)item->text().toUtf8());
    item = ptreeViewDVDModel->item(0, 5);
    m_DVD.setGenre((const char*)item->text().toUtf8());

    QStandardItem* child1 = ptreeViewDVDModel->item(0, 0);

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= child1->rowCount(); wTitleSetNo++)
    {
        item = child1->child(wTitleSetNo - 1, 4);

        dvdpttvmg * pDvdPttVmg = (dvdpttvmg * )m_DVD.getDvdPttVmg(wTitleSetNo);

        pDvdPttVmg->setTitle((const char*)item->text().toUtf8());

        QStandardItem* child2 = child1->child(wTitleSetNo - 1, 0);

        for (uint16_t wPtt = 1; wPtt <= child2->rowCount(); wPtt++)
        {
            dvdpttvts * pDvdPttVts = (dvdpttvts *)m_DVD.getDvdPttVts(wTitleSetNo, wPtt);

            item = child2->child(wPtt - 1, 3);
            pDvdPttVts->setArtist((const char*)item->text().toUtf8());
            item = child2->child(wPtt - 1, 4);
            pDvdPttVts->setTitle((const char*)item->text().toUtf8());
        }
    }

    dvdParseLst.push_back(m_DVD);

    //dvdDatabase.setProxy("proxyserver:3128");

    res = dvdDatabase.submit(dvdParseLst);

    if (res != 0)
    {
        ui->statusBar->showMessage(tr("DVD submit failed"));
        QMessageBox::critical(this, tr("Error"), QString::fromUtf8(dvdDatabase.getErrorString().c_str()));
    }
    else
    {
        ui->statusBar->showMessage(tr("DVD submitted"));
    }

    return res;
}

int dvdexplorerdlg::editDetails()
{
    editdetailsdlg editDetailsDlg(this);
    editDetailsDlg.setParser(&m_DVD);
    editDetailsDlg.exec();
    return -1;
}

void dvdexplorerdlg::on_treeViewDVD_clicked(const QModelIndex & index)
{
    if (!index.isValid())
    {
        return;
    }

    QStandardItemModel *ptreeViewDVDModel = (QStandardItemModel *)ui->treeViewDVD->model();
    QStandardItem *child1 = ptreeViewDVDModel->itemFromIndex(index);

    if (child1 != NULL)
    {
        const dvdetectbase *pDvDetectBase = (const dvdetectbase *)child1->data().toLongLong();

        if (pDvDetectBase != NULL)
        {
            showStreams(NULL);
            showFiles((const dvdparse*)NULL);

            if (pDvDetectBase->classtype() == typeid(dvdparse))
            {
                showDetails((const dvdparse*)pDvDetectBase);
            }
            else if (pDvDetectBase->classtype() == typeid(dvdtitle))
            {
                showDetails((const dvdtitle*)pDvDetectBase);
            }
            else if (pDvDetectBase->classtype() == typeid(dvdpgc))
            {
                showDetails((const dvdpgc*)pDvDetectBase);
            }
            else if (pDvDetectBase->classtype() == typeid(dvdpttvts))
            {
                showDetails((const dvdpttvts*)pDvDetectBase);
            }
            else if (pDvDetectBase->classtype() == typeid(dvdprogram))
            {
                showDetails((const dvdprogram*)pDvDetectBase);
            }
            else if (pDvDetectBase->classtype() == typeid(dvdcell))
            {
                showDetails((const dvdcell*)pDvDetectBase);
            }
            else if (pDvDetectBase->classtype() == typeid(dvdunit))
            {
                showDetails((const dvdunit*)pDvDetectBase);
            }
            else
            {
                qDebug() << "Unknown class: " << name(pDvDetectBase->classtype()).c_str();
                Q_ASSERT(false);
            }
        }
    }
}

void dvdexplorerdlg::setDVDColumnSize()
{
    QStandardItemModel *pModel = (QStandardItemModel *)ui->treeViewDVD->model();

    pModel->clear();

    pModel->setColumnCount(6);
    ui->treeViewDVD->setColumnWidth(0, 250);
    ui->treeViewDVD->setColumnWidth(1, 60);
    ui->treeViewDVD->setColumnWidth(2, 90);
    ui->treeViewDVD->setColumnWidth(3, 150);
    ui->treeViewDVD->setColumnWidth(4, 150);
    ui->treeViewDVD->setColumnWidth(6, 150);

    QStringList labels;

    labels.append(tr(""));
    labels.append(tr("Size"));
    labels.append(tr("Playtime"));
    labels.append(tr("Artist"));
    labels.append(tr("Title"));
    labels.append(tr("Genre"));

    pModel->setHorizontalHeaderLabels(labels);
}

void dvdexplorerdlg::setDetailsColumnSize()
{
    QStandardItemModel *pModel = (QStandardItemModel *)ui->treeViewDetails->model();

    pModel->clear();

    pModel->setColumnCount(2);
    ui->treeViewDetails->setColumnWidth(0, 190);
}

void dvdexplorerdlg::setStreamColumnSize()
{
    QStandardItemModel *pModel = (QStandardItemModel *)ui->treeViewStreams->model();

    pModel->clear();

    pModel->setColumnCount(6);
    ui->treeViewStreams->setColumnWidth(0, 80);
    ui->treeViewStreams->setColumnWidth(1, 50);
    ui->treeViewStreams->setColumnWidth(2, 80);
    ui->treeViewStreams->setColumnWidth(3, 50);
    ui->treeViewStreams->setColumnWidth(4, 50);
    ui->treeViewStreams->setColumnWidth(5, 50);
    ui->treeViewStreams->setColumnWidth(6, 50);
}

void dvdexplorerdlg::setFilesColumnSize()
{
    QStandardItemModel *pModel = (QStandardItemModel *)ui->treeViewFiles->model();

    pModel->clear();

    pModel->setColumnCount(4);
    ui->treeViewFiles->setColumnWidth(0, 70);
    ui->treeViewFiles->setColumnWidth(1, 100);
    ui->treeViewFiles->setColumnWidth(2, 80);
}

void dvdexplorerdlg::clearDetails()
{
    delete ui->treeViewDVD->model();
    delete ui->treeViewDetails->model();
    delete ui->treeViewStreams->model();
    delete ui->treeViewFiles->model();

    ui->treeViewDVD->setModel(new QStandardItemModel(0, 5, this));
    ui->treeViewDetails->setModel(new QStandardItemModel(0, 2, this));
    ui->treeViewStreams->setModel(new QStandardItemModel(0, 5, this));
    ui->treeViewFiles->setModel(new QStandardItemModel(0, 2, this));

    setDVDColumnSize();
    setDetailsColumnSize();
    setStreamColumnSize();
    setFilesColumnSize();
}

bool dvdexplorerdlg::showDetails(const dvdparse *pDvdParse)
{

    setDetailsColumnSize();

    if (pDvdParse == NULL)
    {
        return false;
    }

    QStandardItemModel *ptreeViewDetailsModel = (QStandardItemModel *)ui->treeViewDetails->model();

    const DVDVMGM *pDVDVMGM = pDvdParse->getDVDVMGM();
    QList<QStandardItem *> items;

    items.clear();
    items.append(getItem("Disk"));
    items.append(getItem(tr("%0").arg(QString::fromUtf8(m_DVD.getPath().c_str()))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Version"));
    items.append(getItem(tr("%0.%1").arg(pDVDVMGM->m_wVersionNumberMajor).arg(pDVDVMGM->m_wVersionNumberMinor)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Volume"));
    items.append(getItem(tr("%0/%1").arg(pDVDVMGM->m_wVolumeNumber).arg(pDVDVMGM->m_wNumberOfVolumes)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Side"));
    items.append(getItem(tr("%0").arg(pDVDVMGM->m_wSideID)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Title Sets"));
    items.append(getItem(tr("%0").arg(pDVDVMGM->m_wNumberOfTitleSets)));
    ptreeViewDetailsModel->appendRow(items);

    if (pDVDVMGM->m_szProviderID[0])
    {
        items.clear();
        items.append(getItem("Provider ID"));
        items.append(getItem(tr("%0").arg(pDVDVMGM->m_szProviderID)));
        ptreeViewDetailsModel->appendRow(items);
    }

    QString strRegions;
    for (int i = 0; i < 8; i++)
    {
        if (!pDVDVMGM->m_bRegionProhibited[i])
        {
            if (!strRegions.isEmpty())
            {
                strRegions += "-";
            }
            strRegions += QString::number(i + 1);
        }
    }

    items.clear();
    items.append(getItem("Regions"));
    items.append(getItem(strRegions));
    ptreeViewDetailsModel->appendRow(items);

    // TODO
    // uint8_t  m_byVMG_POS[8]; // VMG POS

    items.clear();
    items.append(getItem(tr("Menu Video")));
    items.append(getItem(tr("%0 / %1 / %2 / %3x%4")
                                   .arg(getVideoCodingMode(pDVDVMGM->m_VideoAttributesOfVMGM_VOBS.m_eCodingMode))
                                   .arg(getVideoStandard(pDVDVMGM->m_VideoAttributesOfVMGM_VOBS.m_eStandard))
                                   .arg(getVideoAspect(pDVDVMGM->m_VideoAttributesOfVMGM_VOBS.m_eAspect))
                                   .arg(pDVDVMGM->m_VideoAttributesOfVMGM_VOBS.m_wResolutionX).arg(pDVDVMGM->m_VideoAttributesOfVMGM_VOBS.m_wResolutionY)));
    ptreeViewDetailsModel->appendRow(items);

    if (pDVDVMGM->m_wNumberOfAudioStreamsInVMGM_VOBS)
    {
        items.clear();
        items.append(getItem(tr("Menu Audio")));
        items.append(getItem(tr("%0 / %1 / %2 kHz / %3")
                                       .arg(getAudioCodingMode(pDVDVMGM->m_AudioAttributesOfVMGM_VOBS.m_eCodingMode))
                                       .arg(pDVDVMGM->m_AudioAttributesOfVMGM_VOBS.m_wChannels)
                                       .arg(pDVDVMGM->m_AudioAttributesOfVMGM_VOBS.m_dwSampleRate / 1000)
                                       .arg(getAudioQuantisation(pDVDVMGM->m_AudioAttributesOfVMGM_VOBS.m_eQuantisation))));
        ptreeViewDetailsModel->appendRow(items);
    }

    if (pDVDVMGM->m_wNumberOfSubpictureStreamsInVMGM_VOBS)
    {
        items.clear();
        items.append(getItem(tr("Menu Subpicture")));
        if (pDVDVMGM->m_SubpictureAttributesOfVMGM_VOBS.m_bLanguageTypePresent)
        {
            items.append(getItem(tr("%0")
                                           .arg(getLanguage(pDVDVMGM->m_SubpictureAttributesOfVMGM_VOBS.m_szLanguageCode))));
        }
        else
        {
            items.append(getItem(tr("unknown")));
        }

        ptreeViewDetailsModel->appendRow(items);
    }

    items.clear();
    items.append(getItem("DVDetect Hash"));
    items.append(getItemFromUtf8(pDvdParse->getHash()));
    ptreeViewDetailsModel->appendRow(items);

    showFiles(pDvdParse);

    return true;
}

bool dvdexplorerdlg::showDetails(const dvdtitle *pDvdTitle)
{
    setDetailsColumnSize();

    if (pDvdTitle == NULL)
    {
        return false;
    }

    QStandardItemModel *ptreeViewDetailsModel = (QStandardItemModel *)ui->treeViewDetails->model();

    const DVDVTS *pDVDVTS = pDvdTitle->getDVDVTS();
    QList<QStandardItem *> items;

    items.clear();
    items.append(getItem("Version"));
    items.append(getItem(tr("%0.%1").arg(pDVDVTS->m_wVersionNumberMajor).arg(pDVDVTS->m_wVersionNumberMinor)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("PGC"));
    items.append(getItem(tr("%0").arg(pDVDVTS->m_wNumberOfProgramChains)));
    ptreeViewDetailsModel->appendRow(items);

    const DVDVIDEOATTRIBUTES & videoAttributes = pDVDVTS->m_VideoAttributesOfVTS_VOBS;

    items.clear();
    items.append(getItem(tr("Automatic PanScan Disallowed")));
    items.append(getItem(tr("%0").arg(getYesOrNo(videoAttributes.m_bAutomaticPanScanDisallowed))));
    ptreeViewDetailsModel->appendRow(items);

    if (pDVDVTS->m_VideoAttributesOfVTSM_VOBS.m_eStandard == DVDVIDEOTVSTANDARD_NTSC)
    {
        items.clear();
        items.append(getItem(tr("CC for line 21 field 2 in GOP")));
        items.append(getItem(tr("%0").arg(getYesOrNo(videoAttributes.m_bCCForLine21Field1InGOP))));
        ptreeViewDetailsModel->appendRow(items);

        items.clear();
        items.append(getItem(tr("CC for line 21 field 2 in GOP")));
        items.append(getItem(tr("%0").arg(getYesOrNo(videoAttributes.m_bCCForLine21Field2InGOP))));
        ptreeViewDetailsModel->appendRow(items);
    }

    items.clear();
    items.append(getItem(tr("CBR")));
    items.append(getItem(tr("%0").arg(getYesOrNo(videoAttributes.m_bCBR))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem(tr("Letterbox Disallowed")));
    items.append(getItem(tr("%0").arg(getYesOrNo(videoAttributes.m_bAutomaticLetterboxDisallowed))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem(tr("Letterboxed")));
    items.append(getItem(tr("%0").arg(getYesOrNo(videoAttributes.m_bLetterBoxed))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem(tr("Source")));
    items.append(getItem(tr("%0").arg(getFilmOrCamera(videoAttributes.m_bSourceFilm))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem(tr("Menu Video")));
    items.append(getItem(tr("%0 / %1 / %2 / %3x%4")
                                   .arg(getVideoCodingMode(pDVDVTS->m_VideoAttributesOfVTSM_VOBS.m_eCodingMode))
                                   .arg(getVideoStandard(pDVDVTS->m_VideoAttributesOfVTSM_VOBS.m_eStandard))
                                   .arg(getVideoAspect(pDVDVTS->m_VideoAttributesOfVTSM_VOBS.m_eAspect))
                                   .arg(pDVDVTS->m_VideoAttributesOfVTSM_VOBS.m_wResolutionX).arg(pDVDVTS->m_VideoAttributesOfVTSM_VOBS.m_wResolutionY)));
    ptreeViewDetailsModel->appendRow(items);

    if (pDVDVTS->m_wNumberOfAudioStreamsInVTSM_VOBS)
    {
        items.clear();
        items.append(getItem(tr("Menu Audio")));
        items.append(getItem(tr("%0 / %1 / %2 kHz / %3")
                                       .arg(getAudioCodingMode(pDVDVTS->m_AudioAttributesOfVTSM_VOBS.m_eCodingMode))
                                       .arg(pDVDVTS->m_AudioAttributesOfVTSM_VOBS.m_wChannels)
                                       .arg(pDVDVTS->m_AudioAttributesOfVTSM_VOBS.m_dwSampleRate / 1000)
                                       .arg(getAudioQuantisation(pDVDVTS->m_AudioAttributesOfVTSM_VOBS.m_eQuantisation))));
        ptreeViewDetailsModel->appendRow(items);
    }

    if (pDVDVTS->m_wNumberOfSubpictureStreamsInVTSM_VOBS)
    {
        items.clear();
        items.append(getItem(tr("Menu Subpicture")));
        if (pDVDVTS->m_SubpictureAttributesOfVTSM_VOBS.m_bLanguageTypePresent)
        {
            items.append(getItem(tr("%0").arg(getLanguage(pDVDVTS->m_SubpictureAttributesOfVTSM_VOBS.m_szLanguageCode))));
        }
        else
        {
            items.append(getItem(tr("unknown")));
        }

        ptreeViewDetailsModel->appendRow(items);
    }

    showStreams(pDvdTitle);
    showFiles(pDvdTitle);

    return true;
}

bool dvdexplorerdlg::showDetails(const dvdpgc* pDvdPgc)
{
    setDetailsColumnSize();

    if (pDvdPgc == NULL)
    {
        return false;
    }

    QStandardItemModel *ptreeViewDetailsModel = (QStandardItemModel *)ui->treeViewDetails->model();

    const DVDPGC *pDVDPGC = pDvdPgc->getDVDPGC();
    QList<QStandardItem *> items;

    items.clear();
    items.append(getItem("EntryPGC"));
    items.append(getItem(tr("%0").arg(getYesOrNo(pDVDPGC->m_bEntryPGC))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("PGC Number"));
    items.append(getItem(tr("%0").arg(pDVDPGC->m_wProgramChainNo)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Programs/Cells"));
    items.append(getItem(tr("%0/%1").arg(pDVDPGC->m_wNumberOfPrograms).arg(pDVDPGC->m_wNumberOfCells)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("PlayTime"));
    items.append(getItem(tr("%0").arg(getPlayTime(pDVDPGC->m_qwPlayTime, pDVDPGC->m_wFrameRate))));
    ptreeViewDetailsModel->appendRow(items);

    const dvdtitle * pDvdTitle = m_DVD.getDvdTitle(pDVDPGC->m_wTitleSetNo);
    showStreams(pDvdTitle);
    showFiles(pDvdTitle);

    return true;
}

bool dvdexplorerdlg::showDetails(const dvdpttvts* pDvdPtt)
{
    setDetailsColumnSize();

    if (pDvdPtt == NULL)
    {
        return false;
    }

    QStandardItemModel *ptreeViewDetailsModel = (QStandardItemModel *)ui->treeViewDetails->model();

    LPCDVDPTTVTS pDVDPTTVTS = pDvdPtt->getDVDPTTVTS();
    QList<QStandardItem *> items;

    items.clear();
    items.append(getItem("Program Chain (PGCN)"));
    items.append(getItem(tr("%0").arg(pDVDPTTVTS->m_wProgramChainNo)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Program (PGN)"));
    items.append(getItem(tr("%0").arg(pDVDPTTVTS->m_wProgram)));
    ptreeViewDetailsModel->appendRow(items);

    const dvdtitle * pDvdTitle = m_DVD.getDvdTitle(pDVDPTTVTS->m_wTitleSetNo);
    showStreams(pDvdTitle);
    showFiles(pDvdTitle);

    return true;
}

bool dvdexplorerdlg::showDetails(const dvdprogram *pDvdProgram)
{
    setDetailsColumnSize();

    if (pDvdProgram == NULL)
    {
        return false;
    }

    QStandardItemModel *ptreeViewDetailsModel = (QStandardItemModel *)ui->treeViewDetails->model();

    LPCDVDPROGRAM pDVDPROGRAM = pDvdProgram->getDVDPROGRAM();
    QList<QStandardItem *> items;

    items.clear();
    items.append(getItem("Program"));
    items.append(getItem(tr("%0").arg(pDVDPROGRAM->m_wProgramNo)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Cells"));
    items.append(getItem(tr("%0").arg(pDVDPROGRAM->m_wCells)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Start Sector"));
    items.append(getItem(tr("%0").arg(pDvdProgram->getStartSector())));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("End Sector"));
    items.append(getItem(tr("%0").arg(pDvdProgram->getEndSector())));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("PlayTime"));
    items.append(getItem(tr("%0").arg(getPlayTime(pDvdProgram->getPlayTime()))));
    ptreeViewDetailsModel->appendRow(items);

    const dvdtitle * pDvdTitle = m_DVD.getDvdTitle(pDVDPROGRAM->m_wTitleSetNo);
    showStreams(pDvdTitle);
    showFiles(pDvdTitle);

    return true;
}

bool dvdexplorerdlg::showDetails(const dvdcell *pDvdCell)
{
    setDetailsColumnSize();

    if (pDvdCell == NULL)
    {
        return false;
    }

    QStandardItemModel *ptreeViewDetailsModel = (QStandardItemModel *)ui->treeViewDetails->model();

    LPCDVDCELL pDVDCELL = pDvdCell->getDVDCELL();
    QList<QStandardItem *> items;

    items.clear();
    items.append(getItem("Cell"));
    items.append(getItem(tr("%0").arg(pDVDCELL->m_wCell)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Cell Type"));
    items.append(getItem(tr("%0").arg(getCellType(pDVDCELL->m_eCellType))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Block Type"));
    items.append(getItem(tr("%0").arg(getBlockType(pDVDCELL->m_eBlockType))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Cell playback time"));
    items.append(getItem(tr("%0").arg(getPlayTime(pDVDCELL->m_qwPlayTime, pDVDCELL->m_wFrameRate))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("First VOBU Start Sector"));
    items.append(getItem(tr("%0").arg(pDVDCELL->m_dwFirstVOBUStartSector)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("First ILVU End Sector"));
    items.append(getItem(tr("%0").arg(pDVDCELL->m_dwFirstILVUEndSector)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Last VOBU Start Sector"));
    items.append(getItem(tr("%0").arg(pDVDCELL->m_dwLastVOBUStartSector)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Last VOBU End Sector"));
    items.append(getItem(tr("%0").arg(pDVDCELL->m_dwLastVOBUEndSector)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Seamless Multiplex"));
    items.append(getItem(tr("%0").arg(getYesOrNo(pDVDCELL->m_bSeamlessMultiplex))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Interleaved"));
    items.append(getItem(tr("%0").arg(getYesOrNo(pDVDCELL->m_bInterleaved))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("SCR Discontinuity"));
    items.append(getItem(tr("%0").arg(getYesOrNo(pDVDCELL->m_bSCRdiscontinuity))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Seamless Angle Linked In DSI"));
    items.append(getItem(tr("%0").arg(getYesOrNo(pDVDCELL->m_bSeamlessAngleLinkedInDSI))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("VOB Still Mode"));
    items.append(getItem(tr("%0").arg(getYesOrNo(pDVDCELL->m_bVOBStillMode))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Stops Trick Play"));
    items.append(getItem(tr("%0").arg(getYesOrNo(pDVDCELL->m_bStopsTrickPlay))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Cell Still Time"));
    items.append(getItem(tr("%0").arg(pDVDCELL->m_wCellStillTime)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("Cell Command #"));
    items.append(getItem(tr("%0").arg(pDVDCELL->m_wCellCommand)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("VOBU/Cell ID"));
    items.append(getItem(tr("%0/%1").arg(pDVDCELL->m_wVOBidn).arg(pDVDCELL->m_wCELLidn)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("??? VOB IDs"));
    items.append(getItem(tr("%0").arg(pDVDCELL->m_wCellPosInfoCount)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("VOB IDs"));
    items.append(getItem(tr("%0").arg(pDVDCELL->m_wNumberOfVOBIds)));
    ptreeViewDetailsModel->appendRow(items);

    const dvdtitle * pDvdTitle = m_DVD.getDvdTitle(pDVDCELL->m_wTitleSetNo);
    showStreams(pDvdTitle);
    showFiles(pDvdTitle);

    return true;
}

bool dvdexplorerdlg::showDetails(const dvdunit *pDvdUnit)
{
    setDetailsColumnSize();

    if (pDvdUnit == NULL)
    {
        return false;
    }

    QStandardItemModel *ptreeViewDetailsModel = (QStandardItemModel *)ui->treeViewDetails->model();

    LPCDVDUNIT pDVDUNIT = pDvdUnit->getDVDUNIT();
    QList<QStandardItem *> items;

    items.clear();
    items.append(getItem("Start Sector"));
    items.append(getItem(tr("%0").arg(pDVDUNIT->m_dwStartSector)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(getItem("End Sector"));
    items.append(getItem(tr("%0").arg(pDVDUNIT->m_dwEndSector)));
    ptreeViewDetailsModel->appendRow(items);

    const dvdtitle * pDvdTitle = m_DVD.getDvdTitle(pDVDUNIT->m_wTitleSetNo);
    showStreams(pDvdTitle);
    showFiles(pDvdTitle);

    return true;
}

bool dvdexplorerdlg::showStreams(const dvdtitle *pDvdTitle)
{
    setStreamColumnSize();

    if (pDvdTitle == NULL)
    {
        return false;
    }

    QStandardItemModel *treeViewStreamsModel = (QStandardItemModel *)ui->treeViewStreams->model();

    LPCDVDVTS pDVDVTS = pDvdTitle->getDVDVTS();
    QList<QStandardItem *> items;
    QStandardItem *item;

    items.clear();
    item = getItem(tr("%0").arg(tr("Video")));
    item->setData((int)pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_byID);
    items.append(item);
    items.append(getItem(tr("[0x%0]").arg(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_byID, 0, 16)));
    items.append(getItem(tr("%0").arg(getVideoCodingMode(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_eCodingMode))));
    items.append(getItem(tr("%0").arg(getVideoStandard(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_eStandard))));
    items.append(getItem(tr("%0").arg(getVideoAspect(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_eAspect))));
    items.append(getItem(tr("%0x%1").arg(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_wResolutionX).arg(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_wResolutionY)));
    treeViewStreamsModel->appendRow(items);

    for (uint16_t wAudioStream = 1; wAudioStream <= pDVDVTS->m_wNumberOfAudioStreamsInVTS_VOBS; wAudioStream++)
    {
        items.clear();
        item = getItem(tr("%0").arg(tr("Audio")));
        item->setData((int)pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_byID);
        items.append(item);
        items.append(getItem(tr("[0x%0]").arg(pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_byID, 0, 16)));
        if (pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_bLanguageTypePresent)
        {
            items.append(getItem(tr("%0").arg(getLanguage(pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_szLanguageCode))));
        }
        else
        {
            items.append(getItem(tr("%0").arg(tr("unk."))));
        }

        items.append(getItem(tr("%0").arg(getAudioCodingMode(pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_eCodingMode))));
        items.append(getItem(tr("%0 Ch").arg(pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_wChannels)));
        items.append(getItem(tr("%0 kHz").arg(pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_dwSampleRate / 1000)));
        //items.append(getItem(tr("%0").arg(getAudioQuantisation(pDVDVTS->m_AudioAttributesOfVTSM_VOBS.m_eQuantisation))));

        treeViewStreamsModel->appendRow(items);
    }

    for (uint16_t wSubpictureStream = 1; wSubpictureStream <= pDVDVTS->m_wNumberOfSubpictureStreamsInVTS_VOBS; wSubpictureStream++)
    {
        items.clear();
        item = getItem(tr("%0").arg(tr("Subpicture")));
        item->setData((int)pDVDVTS->m_SubpictureAttributesOfVTS_VOBS[wSubpictureStream - 1].m_byID);
        items.append(item);
        items.append(getItem(tr("[0x%0]").arg(pDVDVTS->m_SubpictureAttributesOfVTS_VOBS[wSubpictureStream - 1].m_byID, 0, 16)));
        if (pDVDVTS->m_SubpictureAttributesOfVTS_VOBS[wSubpictureStream - 1].m_bLanguageTypePresent)
        {
            items.append(getItem(tr("%0").arg(getLanguage(pDVDVTS->m_SubpictureAttributesOfVTS_VOBS[wSubpictureStream - 1].m_szLanguageCode))));
        }
        else
        {
            items.append(getItem(tr("%0").arg(tr("unknown"))));
        }

        treeViewStreamsModel->appendRow(items);
    }

    return true;
}

bool dvdexplorerdlg::showFiles(const dvdtitle *pDvdTitle)
{
    setFilesColumnSize();

    if (pDvdTitle == NULL)
    {
        return false;
    }

    QStandardItemModel *treeViewViewFiles = (QStandardItemModel *)ui->treeViewFiles->model();

    for (uint16_t wFileNo = 0; wFileNo < pDvdTitle->getFileCount(); wFileNo++)
    {
        const dvdfile *pDvdVoB = pDvdTitle->getDvdFile(wFileNo);
        LPCDVDFILE pDVDFILE = pDvdVoB->getDVDFILE();
        QList<QStandardItem *> items;

        items.clear();
        items.append(getItem(tr("%0").arg(dvdGetFileType(pDVDFILE->m_eDvdFileType))));
        items.append(getItem(tr("%0").arg(pDVDFILE->m_szFileName)));
        items.append(getItem(tr("%0").arg(getFormattedSize(pDVDFILE->m_dwSize))));
        items.append(getItem(tr("%0").arg(getFormattedDate(pDVDFILE->m_Date))));
        treeViewViewFiles->appendRow(items);
    }

    return true;
}

bool dvdexplorerdlg::showFiles(const dvdparse *pDvdParse)
{
    setFilesColumnSize();

    if (pDvdParse == NULL)
    {
        return false;
    }

    QStandardItemModel *treeViewViewFiles = (QStandardItemModel *)ui->treeViewFiles->model();

    for (uint16_t wFileNo = 0; wFileNo < pDvdParse->getFileCount(); wFileNo++)
    {
        const dvdfile *pDvdVoB = pDvdParse->getDvdFile(wFileNo);
        LPCDVDFILE pDVDFILE = pDvdVoB->getDVDFILE();
        QList<QStandardItem *> items;

        items.clear();
        items.append(getItem(tr("%0").arg(dvdGetFileType(pDVDFILE->m_eDvdFileType))));
        items.append(getItem(tr("%0").arg(pDVDFILE->m_szFileName)));
        items.append(getItem(tr("%0").arg(getFormattedSize(pDVDFILE->m_dwSize))));
        items.append(getItem(tr("%0").arg(getFormattedDate(pDVDFILE->m_Date))));
        treeViewViewFiles->appendRow(items);
    }

    return true;
}

QString dvdexplorerdlg::getVideoCodingMode(DVDVIDEOCODINGMODE eCodingMode) const
{
    QString strVideoCodingMode;

    switch (eCodingMode)
    {
    case DVDVIDEOCODINGMODE_MPEG1:
        strVideoCodingMode = tr("MPEG-1");
        break;
    case DVDVIDEOCODINGMODE_MPEG2:
        strVideoCodingMode = tr("MPEG-2");
        break;
    default:
        strVideoCodingMode = tr("invalid");
        break;
    }

    return strVideoCodingMode;
}

QString dvdexplorerdlg::getVideoStandard(DVDVIDEOTVSTANDARD eStandard) const
{
    QString strVideoStandard;

    switch (eStandard)
    {
    case DVDVIDEOTVSTANDARD_NTSC:
        strVideoStandard = tr("NTSC");
        break;
    case DVDVIDEOTVSTANDARD_PAL:
        strVideoStandard = tr("PAL");
        break;
    default:
        strVideoStandard = tr("invalid");
        break;
    }

    return strVideoStandard;
}

QString dvdexplorerdlg::getVideoAspect(DVDVIDEOASPECT eAspect) const
{
    QString strVideoAspect;

    switch (eAspect)
    {
    case DVDVIDEOASPECT_4_3:
        strVideoAspect = tr("4:3");
        break;
    case DVDVIDEOASPECT_16_9:
        strVideoAspect = tr("16:9");
        break;
    default:
        strVideoAspect = tr("unknown");
        break;
    }

    return strVideoAspect;
}

QString dvdexplorerdlg::getAudioCodingMode(DVDAUDIOCODINGMODE eAudioCodingMode) const
{
    QString strCodingMode;

    switch (eAudioCodingMode)
    {
    case DVDAUDIOCODINGMODE_AC3:
        strCodingMode = tr("AC3");
        break;
    case DVDAUDIOCODINGMODE_MPEG1:
        strCodingMode = tr("MPEG-1");
        break;
    case DVDAUDIOCODINGMODE_MPEG2_EXT:
        strCodingMode = tr("MPEG-2");
        break;
    case DVDAUDIOCODINGMODE_LPCM:
        strCodingMode = tr("LPCM");
        break;
    case DVDAUDIOCODINGMODE_DTS:
        strCodingMode = tr("DTS");
        break;
    default:
        strCodingMode = tr("invalid");
        break;
    }

    return strCodingMode;
}

QString dvdexplorerdlg::getAudioQuantisation(DVDAUDIOQUANTISATION eQuantisation) const
{
    QString strQuantisation;

    switch (eQuantisation)
    {
    case DVDAUDIOQUANTISATION_16BPS:
        strQuantisation = tr("16 bps");
        break;
    case DVDAUDIOQUANTISATION_20BPS:
        strQuantisation = tr("20 bps");
        break;
    case DVDAUDIOQUANTISATION_24BPS:
        strQuantisation = tr("24 bps");
        break;
    case DVDAUDIOQUANTISATION_DRC:
        strQuantisation = tr("DRC");
        break;
    default:
        strQuantisation = tr("invalid");
        break;
    }
    return strQuantisation;
}

QString dvdexplorerdlg::getLanguage(const char *pszLanguage) const
{
    return pszLanguage;
}

QString dvdexplorerdlg::getCellType(CELLTYPE eCellType) const
{
    QString strCellType;

    switch (eCellType)
    {
    case CELLTYPE_NORMAL:
        strCellType = tr("normal");
        break;
    case CELLTYPE_FIRST:
        strCellType = tr("first of angle block");
        break;
    case CELLTYPE_MIDDLE:
        strCellType = tr("middle of angle block");
        break;
    case CELLTYPE_LAST:
        strCellType = tr("last of angle block");
        break;
    default:
        strCellType = tr("invalid");
        break;
    }

    return strCellType;
}

QString dvdexplorerdlg::getBlockType(BLOCKTYPE eBlockType) const
{
    QString strBlockType;

    switch (eBlockType)
    {
    case BLOCKTYPE_NORMAL:
        strBlockType = tr("normal");
        break;
    case BLOCKTYPE_ANGLE:
        strBlockType = tr("angle block");
        break;
    default:
        strBlockType = tr("invalid");
        break;
    }

    return strBlockType;
}

QString dvdexplorerdlg::getYesOrNo(bool bValue) const
{
    return (bValue ? tr("yes") : tr("no"));
}

QString dvdexplorerdlg::getFilmOrCamera(bool bValue) const
{
    return (bValue ? tr("Film") : tr("Camera"));
}

QString dvdexplorerdlg::getFormattedSize(uint64_t size, bool bInBytes) const
{
    const QLocale & cLocale = QLocale::system();
    QString strSize;

    if (size < 1024 || bInBytes)
    {   // Byte
        strSize = cLocale.toString((qlonglong)size) + " " + tr("B");
    }
    else if (size < (1024*1024))
    {   // KB
        strSize = cLocale.toString(round((double)size / 1024)) + " " + tr("KB");
    }
    else if (size < (1024*1024*1024))
    {   // MB
        strSize = cLocale.toString(round((double)size / (1024*1024))) + " " + tr("MB");
    }
    else if (size < (1024ULL*1024*1024*1024))
    {   // GB
        strSize = cLocale.toString(round((double)size / (1024ULL*1024*1024))) + " " + tr("GB");
    }
    else
    {   // TB
        strSize = cLocale.toString(round((double)size / (1024ULL*1024*1024*1024))) + " " + tr("TB");
    }
    return strSize;
}

QString dvdexplorerdlg::getFormattedDate(time_t unixTime) const
{
    if (unixTime <= 0)
    {
        return "";
    }

    QDateTime timestamp;
    timestamp.setTime_t(unixTime);

    return timestamp.toString("dd.MM.yy hh:mm:ss");
}

QString dvdexplorerdlg::getPlayTime(uint64_t qwPlaytimems, uint16_t wFrameRate /*= (uint16_t)-1*/) const
{
    uint64_t qwPlayTimeS = qwPlaytimems / 1000;
    int hours = (qwPlayTimeS) / (60 * 60);
    QTime time(hours, ((qwPlayTimeS) / 60) % 60, (qwPlayTimeS) % 60, (qwPlaytimems - (qwPlayTimeS * 1000)));

    if (wFrameRate != (uint16_t)-1)
    {
        if (hours)
        {
            return time.toString(tr("hh:mm:ss.zzz")) + tr(" %0 fps").arg(wFrameRate);
        }
        else
        {
            return time.toString(tr("mm:ss.zzz")) + tr(" %0 fps").arg(wFrameRate);
        }
    }
    else
    {
        if (hours)
        {
            return time.toString(tr("hh:mm:ss.zzz"));
        }
        else
        {
            return time.toString(tr("mm:ss.zzz"));
        }
    }
}

void dvdexplorerdlg::on_checkBoxPhysicalView_clicked()
{
    updateDialog();
}

void dvdexplorerdlg::on_action_Open_triggered()
{
    QString dir;

    dir = QFileDialog::getExistingDirectory(this, tr("Open DVD drive or directory with DVD files"),
                                            dir,
                                            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty())
    {
        return;
    }

    parseDVD(dir);
}

void dvdexplorerdlg::on_actionE_xit_triggered()
{
    close();
}

void dvdexplorerdlg::on_actionQuery_DVD_triggered()
{
    queryDVD();
}

void dvdexplorerdlg::on_actionSearch_DVD_triggered()
{
    searchDVD();
}

void dvdexplorerdlg::on_actionSubmit_DVD_triggered()
{
    submitDVD();
}

void dvdexplorerdlg::on_action_Edit_View_Details_triggered()
{
    editDetails();
}

void dvdexplorerdlg::on_actionAbout_triggered()
{
    aboutdlg aboutDlg(this);

    aboutDlg.exec();
}

void dvdexplorerdlg::on_treeViewDVD_customContextMenuRequested(const QPoint & /*pos*/)
{
    QAction *actionenqueue                  = new QAction(tr("Edit details"), this);

    QMenu menu(tr("Search"), this);

    menu.addAction(actionenqueue);
    //menu.addSeparator();

    menu.setDefaultAction(actionenqueue);
    menu.setEnabled(m_DVD.isLoaded());

    connect(actionenqueue, SIGNAL(triggered()), this, SLOT(handleContextMenuEditDetails()));

    menu.exec(QCursor::pos(), 0);
}

void dvdexplorerdlg::handleContextMenuEditDetails()
{
    editDetails();
}
