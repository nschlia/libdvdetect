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

#include <QFileDialog>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>

std::string name( const std::type_info& tinfo ) { return tinfo.name() ; }

using namespace std;

dvdexplorerdlg::dvdexplorerdlg(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::dvdexplorerdlg)
{
    ui->setupUi(this);

    ui->treeViewDVD->setModel(new QStandardItemModel(0, 3, (QObject*)parent));
    ui->treeViewDetails->setModel(new QStandardItemModel(0, 2, (QObject*)parent));
    ui->treeViewStreams->setModel(new QStandardItemModel(0, 5, (QObject*)parent));
    ui->treeViewFiles->setModel(new QStandardItemModel(0, 2, (QObject*)parent));

    setDVDColumnSize();
    setDetailsColumnSize();
    setStreamColumneSize();
    setFilesColumnSize();
}

dvdexplorerdlg::~dvdexplorerdlg()
{
    delete ui;
}

void dvdexplorerdlg::showCells(const dvdprogram* pDvdProgram, QStandardItem* parent)
{
    LPCDVDPROGRAM pDVDPROGRAM = pDvdProgram->getDVDPROGRAM();
    QList<QStandardItem*> aitems;

    for (uint16_t wCell = 1; wCell <= pDVDPROGRAM->m_wCells; wCell++)
    {
        const dvdcell *pDvdCell = pDvdProgram->getDvdCell(wCell);
        LPCDVDCELL pDVDCELL = pDvdCell->getDVDCELL();

        QStandardItem* child1 = new QStandardItem(tr("Cell %0").arg(wCell));

        aitems.clear();
        aitems.append(child1);
        aitems.append(new QStandardItem(getFormattedSize(pDvdCell->getSize())));
        aitems.append(new QStandardItem(getPlayTime(pDVDCELL->m_qwPlayTime)));
        parent->appendRow(aitems);

        child1->setData(QVariant((long long)pDvdCell));

        for (uint16_t wUnit = 1; wUnit <= pDVDCELL->m_wCellPosInfoCount; wUnit++)
        {
            const dvdunit *pDvdUnit = pDvdCell->getDvdUnit(wUnit);
            //LPDVDUNIT pDVDUNIT = pDvdUnit->getDVDUNIT();

            QStandardItem* child2 = new QStandardItem(tr("Unit %0").arg(wUnit));

            aitems.clear();
            aitems.append(child2);
            aitems.append(new QStandardItem(getFormattedSize(pDvdUnit->getSize())));
            aitems.append(new QStandardItem(""/*getPlayTime(pDvdCell->getPlayTime())*/));
            child1->appendRow(aitems);

            child2->setData(QVariant((long long)pDvdUnit));
        }
    }
}

void dvdexplorerdlg::showPhysicalView()
{
    LPCDVDVMGM pDVDVMGM = m_dvd.getDVDVMGM();
    QStandardItemModel *ptreeViewDVDModel = (QStandardItemModel *)ui->treeViewDVD->model();
    QList<QStandardItem*> aitems;

    qDebug() << "Building physical view";

    ptreeViewDVDModel->clear();

    setDVDColumnSize();

    QStandardItem* item = new QStandardItem(tr("DVD [%0]").arg(QString::fromUtf8(m_dvd.getPath().c_str())));

    item->setData(QVariant((long long)&m_dvd));

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= pDVDVMGM->m_wNumberOfTitleSets; wTitleSetNo++)
    {
        const dvdtitle *pDvdTitle = m_dvd.getDvdTitle(wTitleSetNo);
        LPCDVDVTS pDVDVTS = pDvdTitle->getDVDVTS();

        QStandardItem* child1 = new QStandardItem(tr("Title %0").arg(wTitleSetNo));

        aitems.clear();
        aitems.append(child1);
        aitems.append(new QStandardItem(tr("%0").arg(getFormattedSize(pDvdTitle->getSize()))));
        aitems.append(new QStandardItem(tr("%0").arg(getPlayTime(pDvdTitle->getPlayTime()))));

        item->appendRow(aitems);

        child1->setData(QVariant((long long)pDvdTitle));

        for (uint16_t wProgramChainNo = 1; wProgramChainNo <= pDVDVTS->m_wNumberOfProgramChains;  wProgramChainNo++)
        {
            const dvdpgc *pDvdPgc = pDvdTitle->getDvdPgc(wProgramChainNo);
            LPCDVDPGC pDVDPGC = pDvdPgc->getDVDPGC();

            QStandardItem* child2 = new QStandardItem(tr("PGC %0").arg(wProgramChainNo));

            aitems.clear();
            aitems.append(child2);
            aitems.append(new QStandardItem(getFormattedSize(pDvdPgc->getSize())));
            aitems.append(new QStandardItem(getPlayTime(pDVDPGC->m_qwPlayTime)));
            child1->appendRow(aitems);

            child2->setData(QVariant((long long)pDvdPgc));

            getPlayTime(pDVDPGC->m_qwPlayTime, pDVDPGC->m_wFrameRate);

            for (uint16_t wProgram = 1; wProgram <= pDVDPGC->m_wNumberOfPrograms; wProgram++)
            {
                const dvdprogram * pDvdProgram = pDvdPgc->getDvdProgram(wProgram);

                QStandardItem* child3 = new QStandardItem(tr("Program %0").arg(wProgram));

                aitems.clear();
                aitems.append(child3);
                aitems.append(new QStandardItem(getFormattedSize(pDvdProgram->getSize())));
                aitems.append(new QStandardItem(getPlayTime(pDvdProgram->getPlayTime())));
                child2->appendRow(aitems);

                child3->setData(QVariant((long long)pDvdProgram));
                //child3->setIcon(m_iconcamera);

                showCells(pDvdProgram, child3);
            }
        }
    }

    ptreeViewDVDModel->setItem(0, 0, item);
    ptreeViewDVDModel->setItem(0, 1, new QStandardItem(tr("%0").arg(getFormattedSize(m_dvd.getSize()))));
    ptreeViewDVDModel->setItem(0, 2, new QStandardItem(tr("%0").arg(getPlayTime(m_dvd.getPlayTime()))));

    QModelIndex index = ptreeViewDVDModel->indexFromItem(item);
    ui->treeViewDVD->setExpanded(index, true);
    QItemSelectionModel *selection = ui->treeViewDVD->selectionModel();
    selection->select(index, QItemSelectionModel::ClearAndSelect);
}

void dvdexplorerdlg::showVirtualView()
{
    LPCDVDVMGM pDVDVMGM = m_dvd.getDVDVMGM();
    QStandardItemModel *ptreeViewDVDModel = (QStandardItemModel *)ui->treeViewDVD->model();
    QList<QStandardItem*> aitems;

    qDebug() << "Building virtual view";

    ptreeViewDVDModel->clear();

    setDVDColumnSize();

    QStandardItem* item = new QStandardItem(tr("DVD [%0]").arg(QString::fromUtf8(m_dvd.getPath().c_str())));

    item->setData(QVariant((long long)&m_dvd));

    for (uint16_t wTitleSetNo = 1; wTitleSetNo <= pDVDVMGM->m_wPTTNumberOfTitles; wTitleSetNo++)
    {
        const dvdpttvmg *pDvdPttVmg = m_dvd.getDvdPttVmg(wTitleSetNo);
        const dvdtitle *pDvdTitle = pDvdPttVmg->getDvdTitle();
        //LPCDVDVTS pDVDVTS = pDvdTitle->getDVDVTS();

        QStandardItem* child1 = new QStandardItem(tr("Title %0").arg(wTitleSetNo));

        aitems.clear();
        aitems.append(child1);
        aitems.append(new QStandardItem(tr("%0").arg(getFormattedSize(pDvdPttVmg->getSize()))));
        aitems.append(new QStandardItem(tr("%0").arg(getPlayTime(pDvdPttVmg->getPlayTime()))));
        item->appendRow(aitems);

        child1->setData(QVariant((long long)pDvdTitle));

        for (uint16_t wPtt = 1; wPtt <= pDvdPttVmg->getPttCount(); wPtt++)
        {
            const dvdpttvts * pDvdPttVts = pDvdPttVmg->getDvdPttVts(wPtt);
            LPCDVDPTTVTS pDVDPTTVTS = pDvdPttVts->getDVDPTTVTS();
            const dvdprogram * pDvdProgram = m_dvd.getDvdProgram(pDVDPTTVTS);

            QStandardItem* child2 = new QStandardItem(tr("Chapter %0").arg(pDVDPTTVTS->m_wProgram));

            aitems.clear();
            aitems.append(child2);
            aitems.append(new QStandardItem(getFormattedSize(pDvdPttVts->getSize())));
            aitems.append(new QStandardItem(getPlayTime(pDvdPttVts->getPlayTime())));
            child1->appendRow(aitems);

            child2->setData(QVariant((long long)pDvdProgram));
            //child2->setIcon(m_iconcamera);

            showCells(pDvdProgram, child2);
        }
    }

    ptreeViewDVDModel->setItem(0, 0, item);
    ptreeViewDVDModel->setItem(0, 1, new QStandardItem(tr("%0").arg(getFormattedSize(m_dvd.getVirtSize()))));
    ptreeViewDVDModel->setItem(0, 2, new QStandardItem(tr("%0").arg(getPlayTime(m_dvd.getVirtPlayTime()))));

    QModelIndex index = ptreeViewDVDModel->indexFromItem(item);
    ui->treeViewDVD->setExpanded(index, true);
    QItemSelectionModel *selection = ui->treeViewDVD->selectionModel();
    selection->select(index, QItemSelectionModel::ClearAndSelect);
}

void dvdexplorerdlg::updateDialog()
{
    if (ui->checkBoxPhysicalView->isChecked())
    {
        showPhysicalView();
    }
    else
    {
        showVirtualView();
    }

    showDetails(&m_dvd);
}

int dvdexplorerdlg::parseDVD(const QString & strPath)
{
    int res = 0;

    showStreams(NULL);
    showFiles((dvdparse*)NULL);

    res = m_dvd.parse((const char *)strPath.toUtf8());

    if (res == 0)
    {
        updateDialog();
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), QString::fromUtf8(m_dvd.getErrorString().c_str()));
    }

    return res;
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

    pModel->setColumnCount(3);
    ui->treeViewDVD->setColumnWidth(0, 250);
    ui->treeViewDVD->setColumnWidth(1, 60);
    ui->treeViewDVD->setColumnWidth(2, 50);

    QStringList labels;

    labels.append(tr(""));
    labels.append(tr("Size"));
    labels.append(tr("Playtime"));

    pModel->setHorizontalHeaderLabels(labels);
}

void dvdexplorerdlg::setDetailsColumnSize()
{
    QStandardItemModel *pModel = (QStandardItemModel *)ui->treeViewDetails->model();

    pModel->setColumnCount(2);
    ui->treeViewDetails->setColumnWidth(0, 190);
}

void dvdexplorerdlg::setStreamColumneSize()
{
    QStandardItemModel *pModel = (QStandardItemModel *)ui->treeViewStreams->model();

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

    pModel->setColumnCount(4);
    ui->treeViewFiles->setColumnWidth(0, 70);
    ui->treeViewFiles->setColumnWidth(1, 100);
    ui->treeViewFiles->setColumnWidth(2, 80);
}

bool dvdexplorerdlg::showDetails(const dvdparse *pDvdParse)
{
    QStandardItemModel *ptreeViewDetailsModel = (QStandardItemModel *)ui->treeViewDetails->model();

    ptreeViewDetailsModel->clear();

    if (pDvdParse == NULL)
    {
        return false;
    }

    const DVDVMGM *pDVDVMGM = pDvdParse->getDVDVMGM();
    QList<QStandardItem *> items;

    setDetailsColumnSize();

    items.clear();
    items.append(new QStandardItem("Disk"));
    items.append(new QStandardItem(tr("%0").arg(QString::fromUtf8(m_dvd.getPath().c_str()))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Version"));
    items.append(new QStandardItem(tr("%0.%1").arg(pDVDVMGM->m_wVersionNumberMajor).arg(pDVDVMGM->m_wVersionNumberMinor)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Volume"));
    items.append(new QStandardItem(tr("%0/%1").arg(pDVDVMGM->m_wVolumeNumber).arg(pDVDVMGM->m_wNumberOfVolumes)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Side"));
    items.append(new QStandardItem(tr("%0").arg(pDVDVMGM->m_wSideID)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Title Sets"));
    items.append(new QStandardItem(tr("%0").arg(pDVDVMGM->m_wNumberOfTitleSets)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Provider ID"));
    items.append(new QStandardItem(tr("%0").arg(pDVDVMGM->m_szProviderID)));
    ptreeViewDetailsModel->appendRow(items);

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
    items.append(new QStandardItem("Regions"));
    items.append(new QStandardItem(strRegions));
    ptreeViewDetailsModel->appendRow(items);

    // TODO
    // uint8_t  m_byVMG_POS[8]; // VMG POS

    items.clear();
    items.append(new QStandardItem(tr("Menu Video")));
    items.append(new QStandardItem(tr("%0 / %1 / %2 / %3x%4")
                                   .arg(getVideoCodingMode(pDVDVMGM->m_VideoAttributesOfVMGM_VOBS.m_eDvdVideoCodingMode))
                                   .arg(getVideoStandard(pDVDVMGM->m_VideoAttributesOfVMGM_VOBS.m_eDvdVideoStandard))
                                   .arg(getVideoAspect(pDVDVMGM->m_VideoAttributesOfVMGM_VOBS.m_eDvdVideoAspect))
                                   .arg(pDVDVMGM->m_VideoAttributesOfVMGM_VOBS.m_wResolutionX).arg(pDVDVMGM->m_VideoAttributesOfVMGM_VOBS.m_wResolutionY)));
    ptreeViewDetailsModel->appendRow(items);

    if (pDVDVMGM->m_wNumberOfAudioStreamsInVMGM_VOBS)
    {
        items.clear();
        items.append(new QStandardItem(tr("Menu Audio")));
        items.append(new QStandardItem(tr("%0 / %1 / %2 kHz")
                                       .arg(getAudioCodingMode(pDVDVMGM->m_AudioAttributesOfVMGM_VOBS.m_eAudioCodingMode))
                                       .arg(pDVDVMGM->m_AudioAttributesOfVMGM_VOBS.m_wChannels)
                                       .arg(pDVDVMGM->m_AudioAttributesOfVMGM_VOBS.m_dwSampleRate / 1000)));
        ptreeViewDetailsModel->appendRow(items);
    }

    if (pDVDVMGM->m_wNumberOfSubpictureStreamsInVMGM_VOBS)
    {
        items.clear();
        items.append(new QStandardItem(tr("Menu Subpicture")));
        if (pDVDVMGM->m_SubpictureAttributesOfVMGM_VOBS.m_bLanguageTypePresent)
        {
            items.append(new QStandardItem(tr("%0")
                                           .arg(getLanguage(pDVDVMGM->m_SubpictureAttributesOfVMGM_VOBS.m_szLanguageCode))));
        }
        else
        {
            items.append(new QStandardItem(tr("unknown")));
        }

        ptreeViewDetailsModel->appendRow(items);
    }

    showFiles(pDvdParse);

    return true;
}

bool dvdexplorerdlg::showDetails(const dvdtitle *pDvdTitle)
{
    QStandardItemModel *ptreeViewDetailsModel = (QStandardItemModel *)ui->treeViewDetails->model();

    ptreeViewDetailsModel->clear();

    if (pDvdTitle == NULL)
    {
        return false;
    }

    const DVDVTS *pDVDVTS = pDvdTitle->getDVDVTS();
    QList<QStandardItem *> items;

    setDetailsColumnSize();

    items.clear();
    items.append(new QStandardItem("Version"));
    items.append(new QStandardItem(tr("%0.%1").arg(pDVDVTS->m_wVersionNumberMajor).arg(pDVDVTS->m_wVersionNumberMinor)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("PGC"));
    items.append(new QStandardItem(tr("%0").arg(pDVDVTS->m_wNumberOfProgramChains)));
    ptreeViewDetailsModel->appendRow(items);

    const DVDVIDEOATTRIBUTES & videoAttributes = pDVDVTS->m_VideoAttributesOfVTS_VOBS;

    items.clear();
    items.append(new QStandardItem(tr("Automatic PanScan Disallowed")));
    items.append(new QStandardItem(tr("%0").arg(getYesOrNo(videoAttributes.m_bAutomaticPanScanDisallowed))));
    ptreeViewDetailsModel->appendRow(items);

    if (pDVDVTS->m_VideoAttributesOfVTSM_VOBS.m_eDvdVideoStandard == DVDVIDEOTVSTANDARD_NTSC)
    {
        items.clear();
        items.append(new QStandardItem(tr("CC for line 21 field 2 in GOP")));
        items.append(new QStandardItem(tr("%0").arg(getYesOrNo(videoAttributes.m_bCCForLine21Field1InGOP))));
        ptreeViewDetailsModel->appendRow(items);

        items.clear();
        items.append(new QStandardItem(tr("CC for line 21 field 2 in GOP")));
        items.append(new QStandardItem(tr("%0").arg(getYesOrNo(videoAttributes.m_bCCForLine21Field2InGOP))));
        ptreeViewDetailsModel->appendRow(items);
    }

    items.clear();
    items.append(new QStandardItem(tr("CBR")));
    items.append(new QStandardItem(tr("%0").arg(getYesOrNo(videoAttributes.m_bCBR))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem(tr("Letterbox Disallowed")));
    items.append(new QStandardItem(tr("%0").arg(getYesOrNo(videoAttributes.m_bAutomaticLetterboxDisallowed))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem(tr("Letterboxed")));
    items.append(new QStandardItem(tr("%0").arg(getYesOrNo(videoAttributes.m_bLetterBoxed))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem(tr("Source")));
    items.append(new QStandardItem(tr("%0").arg(getFilmOrCamera(videoAttributes.m_bSourceFilm))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem(tr("Menu Video")));
    items.append(new QStandardItem(tr("%0 / %1 / %2 / %3x%4")
                                   .arg(getVideoCodingMode(pDVDVTS->m_VideoAttributesOfVTSM_VOBS.m_eDvdVideoCodingMode))
                                   .arg(getVideoStandard(pDVDVTS->m_VideoAttributesOfVTSM_VOBS.m_eDvdVideoStandard))
                                   .arg(getVideoAspect(pDVDVTS->m_VideoAttributesOfVTSM_VOBS.m_eDvdVideoAspect))
                                   .arg(pDVDVTS->m_VideoAttributesOfVTSM_VOBS.m_wResolutionX).arg(pDVDVTS->m_VideoAttributesOfVTSM_VOBS.m_wResolutionY)));
    ptreeViewDetailsModel->appendRow(items);

    if (pDVDVTS->m_wNumberOfAudioStreamsInVTSM_VOBS)
    {
        items.clear();
        items.append(new QStandardItem(tr("Menu Audio")));
        items.append(new QStandardItem(tr("%0 / %1 / %2 kHz")
                                       .arg(getAudioCodingMode(pDVDVTS->m_AudioAttributesOfVTSM_VOBS.m_eAudioCodingMode))
                                       .arg(pDVDVTS->m_AudioAttributesOfVTSM_VOBS.m_wChannels)
                                       .arg(pDVDVTS->m_AudioAttributesOfVTSM_VOBS.m_dwSampleRate / 1000)));
        ptreeViewDetailsModel->appendRow(items);
    }

    if (pDVDVTS->m_wNumberOfSubpictureStreamsInVTSM_VOBS)
    {
        items.clear();
        items.append(new QStandardItem(tr("Menu Subpicture")));
        if (pDVDVTS->m_SubpictureAttributesOfVTSM_VOBS.m_bLanguageTypePresent)
        {
            items.append(new QStandardItem(tr("%0")
                                           .arg(getLanguage(pDVDVTS->m_SubpictureAttributesOfVTSM_VOBS.m_szLanguageCode))));
        }
        else
        {
            items.append(new QStandardItem(tr("unknown")));
        }

        ptreeViewDetailsModel->appendRow(items);
    }

    showStreams(pDvdTitle);
    showFiles(pDvdTitle);

    return true;
}

bool dvdexplorerdlg::showDetails(const dvdpgc* pDvdPgc)
{
    QStandardItemModel *ptreeViewDetailsModel = (QStandardItemModel *)ui->treeViewDetails->model();

    ptreeViewDetailsModel->clear();

    if (pDvdPgc == NULL)
    {
        return false;
    }

    const DVDPGC *pDVDPGC = pDvdPgc->getDVDPGC();
    QList<QStandardItem *> items;

    setDetailsColumnSize();

    items.clear();
    items.append(new QStandardItem("EntryPGC"));
    items.append(new QStandardItem(tr("%0").arg(getYesOrNo(pDVDPGC->m_bEntryPGC))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("PGC Number"));
    items.append(new QStandardItem(tr("%0").arg(pDVDPGC->m_wProgramChainNo)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Programs/Cells"));
    items.append(new QStandardItem(tr("%0/%1").arg(pDVDPGC->m_wNumberOfPrograms).arg(pDVDPGC->m_wNumberOfCells)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("PlayTime"));
    items.append(new QStandardItem(tr("%0").arg(getPlayTime(pDVDPGC->m_qwPlayTime, pDVDPGC->m_wFrameRate))));
    ptreeViewDetailsModel->appendRow(items);

    const dvdtitle * pDvdTitle = m_dvd.getDvdTitle(pDVDPGC->m_wTitleSetNo);
    showStreams(pDvdTitle);
    showFiles(pDvdTitle);

    return true;
}

bool dvdexplorerdlg::showDetails(const dvdpttvts* pDvdPtt)
{
    QStandardItemModel *ptreeViewDetailsModel = (QStandardItemModel *)ui->treeViewDetails->model();

    ptreeViewDetailsModel->clear();

    if (pDvdPtt == NULL)
    {
        return false;
    }

    LPCDVDPTTVTS pDVDPTTVTS = pDvdPtt->getDVDPTTVTS();
    QList<QStandardItem *> items;

    setDetailsColumnSize();

    items.clear();
    items.append(new QStandardItem("Program Chain (PGCN)"));
    items.append(new QStandardItem(tr("%0").arg(pDVDPTTVTS->m_wProgramChainNo)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Program (PGN)"));
    items.append(new QStandardItem(tr("%0").arg(pDVDPTTVTS->m_wProgram)));
    ptreeViewDetailsModel->appendRow(items);

    const dvdtitle * pDvdTitle = m_dvd.getDvdTitle(pDVDPTTVTS->m_wTitleSetNo);
    showStreams(pDvdTitle);
    showFiles(pDvdTitle);

    return true;
}

bool dvdexplorerdlg::showDetails(const dvdprogram *pDvdProgram)
{
    QStandardItemModel *ptreeViewDetailsModel = (QStandardItemModel *)ui->treeViewDetails->model();

    ptreeViewDetailsModel->clear();

    if (pDvdProgram == NULL)
    {
        return false;
    }

    LPCDVDPROGRAM pDVDPROGRAM = pDvdProgram->getDVDPROGRAM();
    QList<QStandardItem *> items;

    setDetailsColumnSize();

    items.clear();
    items.append(new QStandardItem("Program"));
    items.append(new QStandardItem(tr("%0").arg(pDVDPROGRAM->m_wProgramNo)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Cells"));
    items.append(new QStandardItem(tr("%0").arg(pDVDPROGRAM->m_wCells)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Start Sector"));
    items.append(new QStandardItem(tr("%0").arg(pDvdProgram->getStartSector())));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("End Sector"));
    items.append(new QStandardItem(tr("%0").arg(pDvdProgram->getEndSector())));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("PlayTime"));
    items.append(new QStandardItem(tr("%0").arg(getPlayTime(pDvdProgram->getPlayTime()))));
    ptreeViewDetailsModel->appendRow(items);

    const dvdtitle * pDvdTitle = m_dvd.getDvdTitle(pDVDPROGRAM->m_wTitleSetNo);
    showStreams(pDvdTitle);
    showFiles(pDvdTitle);

    return true;
}

bool dvdexplorerdlg::showDetails(const dvdcell *pDvdCell)
{
    QStandardItemModel *ptreeViewDetailsModel = (QStandardItemModel *)ui->treeViewDetails->model();

    ptreeViewDetailsModel->clear();

    if (pDvdCell == NULL)
    {
        return false;
    }

    LPCDVDCELL pDVDCELL = pDvdCell->getDVDCELL();
    QList<QStandardItem *> items;

    setDetailsColumnSize();

    items.clear();
    items.append(new QStandardItem("Cell"));
    items.append(new QStandardItem(tr("%0").arg(pDVDCELL->m_wCell)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Cell Type"));
    items.append(new QStandardItem(tr("%0").arg(getCellType(pDVDCELL->m_eCellType))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Block Type"));
    items.append(new QStandardItem(tr("%0").arg(getBlockType(pDVDCELL->m_eBlockType))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Cell playback time"));
    items.append(new QStandardItem(tr("%0").arg(getPlayTime(pDVDCELL->m_qwPlayTime, pDVDCELL->m_wFrameRate))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("First VOBU Start Sector"));
    items.append(new QStandardItem(tr("%0").arg(pDVDCELL->m_dwFirstVOBUStartSector)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("First ILVU End Sector"));
    items.append(new QStandardItem(tr("%0").arg(pDVDCELL->m_dwFirstILVUEndSector)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Last VOBU Start Sector"));
    items.append(new QStandardItem(tr("%0").arg(pDVDCELL->m_dwLastVOBUStartSector)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Last VOBU End Sector"));
    items.append(new QStandardItem(tr("%0").arg(pDVDCELL->m_dwLastVOBUEndSector)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Seamless Multiplex"));
    items.append(new QStandardItem(tr("%0").arg(getYesOrNo(pDVDCELL->m_bSeamlessMultiplex))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Interleaved"));
    items.append(new QStandardItem(tr("%0").arg(getYesOrNo(pDVDCELL->m_bInterleaved))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("SCR Discontinuity"));
    items.append(new QStandardItem(tr("%0").arg(getYesOrNo(pDVDCELL->m_bSCRdiscontinuity))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Seamless Angle Linked In DSI"));
    items.append(new QStandardItem(tr("%0").arg(getYesOrNo(pDVDCELL->m_bSeamlessAngleLinkedInDSI))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("VOB Still Mode"));
    items.append(new QStandardItem(tr("%0").arg(getYesOrNo(pDVDCELL->m_bVOBStillMode))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Stops Trick Play"));
    items.append(new QStandardItem(tr("%0").arg(getYesOrNo(pDVDCELL->m_bStopsTrickPlay))));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Cell Still Time"));
    items.append(new QStandardItem(tr("%0").arg(pDVDCELL->m_wCellStillTime)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("Cell Command #"));
    items.append(new QStandardItem(tr("%0").arg(pDVDCELL->m_wCellCommand)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("VOBU/Cell ID"));
    items.append(new QStandardItem(tr("%0/%1").arg(pDVDCELL->m_wVOBidn).arg(pDVDCELL->m_wCELLidn)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("??? VOB IDs"));
    items.append(new QStandardItem(tr("%0").arg(pDVDCELL->m_wCellPosInfoCount)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("VOB IDs"));
    items.append(new QStandardItem(tr("%0").arg(pDVDCELL->m_wNumberOfVOBIds)));
    ptreeViewDetailsModel->appendRow(items);

    const dvdtitle * pDvdTitle = m_dvd.getDvdTitle(pDVDCELL->m_wTitleSetNo);
    showStreams(pDvdTitle);
    showFiles(pDvdTitle);
    return true;
}

bool dvdexplorerdlg::showDetails(const dvdunit *pDvdUnit)
{
    QStandardItemModel *ptreeViewDetailsModel = (QStandardItemModel *)ui->treeViewDetails->model();

    ptreeViewDetailsModel->clear();

    if (pDvdUnit == NULL)
    {
        return false;
    }

    LPCDVDUNIT pDVDUNIT = pDvdUnit->getDVDUNIT();
    QList<QStandardItem *> items;

    setDetailsColumnSize();

    items.clear();
    items.append(new QStandardItem("Start Sector"));
    items.append(new QStandardItem(tr("%0").arg(pDVDUNIT->m_dwStartSector)));
    ptreeViewDetailsModel->appendRow(items);

    items.clear();
    items.append(new QStandardItem("End Sector"));
    items.append(new QStandardItem(tr("%0").arg(pDVDUNIT->m_dwEndSector)));
    ptreeViewDetailsModel->appendRow(items);

    const dvdtitle * pDvdTitle = m_dvd.getDvdTitle(pDVDUNIT->m_wTitleSetNo);
    showStreams(pDvdTitle);
    showFiles(pDvdTitle);
    return true;
}

bool dvdexplorerdlg::showStreams(const dvdtitle *pDvdTitle)
{
    QStandardItemModel *treeViewStreamsModel = (QStandardItemModel *)ui->treeViewStreams->model();

    treeViewStreamsModel->clear();

    if (pDvdTitle == NULL)
    {
        return false;
    }

    LPCDVDVTS pDVDVTS = pDvdTitle->getDVDVTS();
    QList<QStandardItem *> items;
    QStandardItem *item;

    setStreamColumneSize();

    items.clear();
    item = new QStandardItem(tr("%0").arg(tr("Video")));
    item->setData((int)pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_byID);
    items.append(item);
    items.append(new QStandardItem(tr("[0x%0]").arg(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_byID, 0, 16)));
    items.append(new QStandardItem(tr("%0").arg(getVideoCodingMode(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_eDvdVideoCodingMode))));
    items.append(new QStandardItem(tr("%0").arg(getVideoStandard(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_eDvdVideoStandard))));
    items.append(new QStandardItem(tr("%0").arg(getVideoAspect(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_eDvdVideoAspect))));
    items.append(new QStandardItem(tr("%0x%1").arg(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_wResolutionX).arg(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_wResolutionY)));
    treeViewStreamsModel->appendRow(items);

    for (uint16_t wAudioStream = 1; wAudioStream <= pDVDVTS->m_wNumberOfAudioStreamsInVTS_VOBS; wAudioStream++)
    {
        items.clear();
        item = new QStandardItem(tr("%0").arg(tr("Audio")));
        item->setData((int)pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_byID);
        items.append(item);
        items.append(new QStandardItem(tr("[0x%0]").arg(pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_byID, 0, 16)));
        if (pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_bLanguageTypePresent)
        {
            items.append(new QStandardItem(tr("%0").arg(getLanguage(pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_szLanguageCode))));
        }
        else
        {
            items.append(new QStandardItem(tr("%0").arg(tr("unk."))));
        }

        items.append(new QStandardItem(tr("%0").arg(getAudioCodingMode(pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_eAudioCodingMode))));
        items.append(new QStandardItem(tr("%0 Ch").arg(pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_wChannels)));
        items.append(new QStandardItem(tr("%0 kHz").arg(pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_dwSampleRate / 1000)));

        treeViewStreamsModel->appendRow(items);
    }

    for (uint16_t wSubpictureStream = 1; wSubpictureStream <= pDVDVTS->m_wNumberOfSubpictureStreamsInVTS_VOBS; wSubpictureStream++)
    {
        items.clear();
        item = new QStandardItem(tr("%0").arg(tr("Subpicture")));
        item->setData((int)pDVDVTS->m_SubpictureAttributesOfVTS_VOBS[wSubpictureStream - 1].m_byID);
        items.append(item);
        items.append(new QStandardItem(tr("[0x%0]").arg(pDVDVTS->m_SubpictureAttributesOfVTS_VOBS[wSubpictureStream - 1].m_byID, 0, 16)));
        if (pDVDVTS->m_SubpictureAttributesOfVTS_VOBS[wSubpictureStream - 1].m_bLanguageTypePresent)
        {
            items.append(new QStandardItem(tr("%0").arg(getLanguage(pDVDVTS->m_SubpictureAttributesOfVTS_VOBS[wSubpictureStream - 1].m_szLanguageCode))));
        }
        else
        {
            items.append(new QStandardItem(tr("%0").arg(tr("unknown"))));
        }

        treeViewStreamsModel->appendRow(items);
    }

    return true;
}

bool dvdexplorerdlg::showFiles(const dvdtitle *pDvdTitle)
{
    QStandardItemModel *treeViewViewFiles = (QStandardItemModel *)ui->treeViewFiles->model();

    treeViewViewFiles->clear();

    if (pDvdTitle == NULL)
    {
        return false;
    }

    setFilesColumnSize();

    for (uint16_t wFileNo = 0; wFileNo < pDvdTitle->getFileCount(); wFileNo++)
    {
        const dvdfile *pDvdVoB = pDvdTitle->getDvdFile(wFileNo);
        LPCDVDFILE pDVDFILE = pDvdVoB->getDVDFILE();
        QList<QStandardItem *> items;

        items.clear();
        items.append(new QStandardItem(tr("%0").arg(dvdGetFileType(pDVDFILE->m_eDvdFileType))));
        items.append(new QStandardItem(tr("%0").arg(pDVDFILE->m_szFileName)));
        items.append(new QStandardItem(tr("%0").arg(getFormattedSize(pDVDFILE->m_dwSize))));
        items.append(new QStandardItem(tr("%0").arg(getFormattedDate(pDVDFILE->m_Date))));
        treeViewViewFiles->appendRow(items);
    }

    return true;
}

bool dvdexplorerdlg::showFiles(const dvdparse *pDvdParse)
{
    QStandardItemModel *treeViewViewFiles = (QStandardItemModel *)ui->treeViewFiles->model();

    treeViewViewFiles->clear();

    if (pDvdParse == NULL)
    {
        return false;
    }

    setFilesColumnSize();

    for (uint16_t wFileNo = 0; wFileNo < pDvdParse->getFileCount(); wFileNo++)
    {
        const dvdfile *pDvdVoB = pDvdParse->getDvdFile(wFileNo);
        LPCDVDFILE pDVDFILE = pDvdVoB->getDVDFILE();
        QList<QStandardItem *> items;

        items.clear();
        items.append(new QStandardItem(tr("%0").arg(dvdGetFileType(pDVDFILE->m_eDvdFileType))));
        items.append(new QStandardItem(tr("%0").arg(pDVDFILE->m_szFileName)));
        items.append(new QStandardItem(tr("%0").arg(getFormattedSize(pDVDFILE->m_dwSize))));
        items.append(new QStandardItem(tr("%0").arg(getFormattedDate(pDVDFILE->m_Date))));
        treeViewViewFiles->appendRow(items);
    }

    return true;
}

QString dvdexplorerdlg::getVideoCodingMode(DVDVIDEOCODINGMODE eDvdVideoCodingMode) const
{
    QString strVideoCodingMode;

    switch (eDvdVideoCodingMode)
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

QString dvdexplorerdlg::getVideoStandard(DVDVIDEOTVSTANDARD eDvdVideoStandard) const
{
    QString strVideoStandard;

    switch (eDvdVideoStandard)
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

QString dvdexplorerdlg::getVideoAspect(DVDVIDEOASPECT eDvdVideoAspect) const
{
    QString strVideoAspect;

    switch (eDvdVideoAspect)
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
        strCodingMode = tr("Mpeg-1");
        break;
    case DVDAUDIOCODINGMODE_MPEG2_EXT:
        strCodingMode = tr("Mpeg-2ext");
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

    //QString dir("/files/mp3base2/DVD/Musik/Arch Enemy/Tyrants of the Rising Sun - Live in Japan/DVD1/VIDEO_TS");
    //QString dir("/files/mp3base2/DVD/Musik/Metallica/Live Shit Binge & Purge DVD 1 - San Diego/DVD1/VIDEO_TS/");
    //QString dir("/files/mp3base2/DVD/Musik/Metallica/S+M DVD 1/DVD1/VIDEO_TS/");
    //QString dir("D:/temp/DVD/Arch Enemy/Tyrants of the Rising Sun - Live in Japan/DVD1/VIDEO_TS");
    //QString dir("T:/DVD/Musik/Arch Enemy/Tyrants of the Rising Sun - Live in Japan/DVD1/VIDEO_TS");
    //QString dir("T:/DVD/Musik/Metallica/S+M DVD 1/DVD1/VIDEO_TS");
    QString dir("/files/mp3base2/DVD/Serien/King of Queens/Season 5 3");
    //QString dir;

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

