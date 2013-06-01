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

/** \file mainwindow.h
 *
 *  \brief Qt GUI example main window code
 */

#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdint.h>

#include <dvdetectc++.h>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QModelIndex;
class QStandardItemModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int         parseDVD(const QString & strPath);

protected:
    void        updateDialog();

    void        setDetailsColumnSize(QStandardItemModel *ptreeViewDetailsModel);
    void        setStreamColumneSize(QStandardItemModel *treeViewStreamsModel);
    void        setFilesColumnSize(QStandardItemModel *treeViewViewFiles);

    bool        showDetails(const dvdparse *pDvdParse);
    bool        showDetails(const dvdtitle* pDvdTitle);
    bool        showDetails(const dvdpgc* pDvdPgc);
    bool        showDetails(const dvdptt* pDvdPtt);
    bool        showDetails(const dvdprogram* pDvdProgram);
    bool        showDetails(const dvdcell* pDvdCell);
    bool        showDetails(const dvdunit* pDvdUnit);
    bool        showStreams(const dvdtitle* pDvdTitle);
    bool        showFiles(const dvdtitle* pDvdTitle);
    bool        showFiles(const dvdparse *pDvdParse);

    QString     getVideoCodingMode(DVDVIDEOCODINGMODE eDvdVideoCodingMode) const;
    QString     getVideoStandard(DVDVIDEOTVSTANDARD eDvdVideoStandard) const;
    QString     getVideoAspect(DVDVIDEOASPECT eDvdVideoAspect) const;
    QString     getAudioCodingMode(DVDAUDIOCODINGMODE eAudioCodingMode) const;
    QString     getLanguage(const char *pszLanguage) const;
    QString     getCellType(CELLTYPE eCellType) const;
    QString     getBlockType(BLOCKTYPE eBlockType) const;
    QString     getYesOrNo(bool bValue) const;
    QString     getFilmOrCamera(bool bValue) const;
    QString     getFormattedSize(uint64_t size, bool bInBytes = false) const;
    QString     getFormattedDate(time_t unixTime) const;
    QString     getPlayTime(uint64_t qwPlaytimems, uint16_t wFrameRate = (uint16_t)-1) const;

private slots:
    void        on_action_Open_triggered();
    void        on_actionE_xit_triggered();
    void        on_treeViewDVD_clicked(const QModelIndex &index);

protected:
    dvdparse    m_dvd;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
