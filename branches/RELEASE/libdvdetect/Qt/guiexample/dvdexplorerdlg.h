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

/** \file dvdexplorerdlg.h
 *
 *  \brief Qt GUI example main window code
 */

#pragma once

#ifndef DVDEXPLORERDLG_H
#define DVDEXPLORERDLG_H

#include <stdint.h>

#include <dvdetect/dvdetectc++.h>

#include <QMainWindow>

namespace Ui {
class dvdexplorerdlg;
}

class QModelIndex;
class QStandardItemModel;
class QStandardItem;

/*!
 * \brief dvdexplorerdlg class
 *  Display and maintain application's main window.
 */
class dvdexplorerdlg : public QMainWindow
{
    Q_OBJECT

public:
    //! Constructor.
    /*!
     *  Construct a dvdexplorerdlg element.
     *
     * \param parent QWidget * Optional parent windows, may be NULL
     */
    explicit dvdexplorerdlg(QWidget *parent = 0);
    //! Destructor.
    /*!
     *  Destruct a dvdexplorerdlg element.
     */
    virtual ~dvdexplorerdlg();

protected:
    //! Access the DVDUNIT data structure.
    /*!
     * \param strPath const QString & Path to DVD files
     * \return Success:
     * \return Fail:
     */
    int                 parseDVD(const QString & strPath);

    void                showCells(const dvdprogram* pDvdProgram, QStandardItem* parent);
    void                showPhysicalView();
    void                showVirtualView();
    void                updateDialog();

    void                setDVDColumnSize();
    void                setDetailsColumnSize();
    void                setStreamColumneSize();
    void                setFilesColumnSize();

    virtual bool        showDetails(const dvdparse *pDvdParse);
    virtual bool        showDetails(const dvdtitle* pDvdTitle);
    virtual bool        showDetails(const dvdpgc* pDvdPgc);
    virtual bool        showDetails(const dvdpttvts* pDvdPtt);
    virtual bool        showDetails(const dvdprogram* pDvdProgram);
    virtual bool        showDetails(const dvdcell* pDvdCell);
    virtual bool        showDetails(const dvdunit* pDvdUnit);
    virtual bool        showStreams(const dvdtitle* pDvdTitle);
    virtual bool        showFiles(const dvdtitle* pDvdTitle);
    virtual bool        showFiles(const dvdparse *pDvdParse);

    QString         	getVideoCodingMode(DVDVIDEOCODINGMODE eDvdVideoCodingMode) const;
    QString         	getVideoStandard(DVDVIDEOTVSTANDARD eDvdVideoStandard) const;
    QString         	getVideoAspect(DVDVIDEOASPECT eDvdVideoAspect) const;
    QString           	getAudioCodingMode(DVDAUDIOCODINGMODE eAudioCodingMode) const;
    QString            	getLanguage(const char *pszLanguage) const;
    QString             getCellType(CELLTYPE eCellType) const;
    QString             getBlockType(BLOCKTYPE eBlockType) const;
    QString             getYesOrNo(bool bValue) const;
    QString             getFilmOrCamera(bool bValue) const;
    QString             getFormattedSize(uint64_t size, bool bInBytes = false) const;
    QString             getFormattedDate(time_t unixTime) const;
    QString             getPlayTime(uint64_t qwPlaytimems, uint16_t wFrameRate = (uint16_t)-1) const;

private slots:
    void                on_action_Open_triggered();
    void                on_actionE_xit_triggered();
    void                on_treeViewDVD_clicked(const QModelIndex &index);
    void                on_checkBoxPhysicalView_clicked();

protected:
    dvdparse            m_dvd;              //!< dvdparse parser object

private:
    Ui::dvdexplorerdlg *ui;
};

#endif // DVDEXPLORERDLG_H
