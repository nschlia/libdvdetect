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

/** \file dvdexplorerdlg.h
 *
 *  \brief Qt GUI example main window declaration
 */

#pragma once

#ifndef DVDEXPLORERDLG_H
#define DVDEXPLORERDLG_H

#define PROGRAM_NAME "libDVDetect Explorer Example V" DVDETECT_PRODUCTVERSION_STR

#include <stdint.h>

#include <dvdetect/dvdetectc++.h>

#include <QMainWindow>

namespace Ui {
class dvdexplorerdlg;
}

class QModelIndex;
class QStandardItem;
class QStandardItemModel;

/*!
 * \brief dvdexplorerdlg class
 *  Display and maintain application's main window.
 */
class dvdexplorerdlg : public QMainWindow
{
    Q_OBJECT
    Q_CLASSINFO("author", "Norbert Schlia")
    Q_CLASSINFO("url", "http://www.dvdetect.de/")
    Q_CLASSINFO("project", "DVD Lookup Library")

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
    int                 parseDvd(const QString & strPath);

    void                showCells(const dvdprogram* pDvdProgram, QStandardItem* parent);
    void                showPhysicalView();
    void                showVirtualView();
    void                updateDialog();

    void                setDvdColumnSize();
    void                setDetailsColumnSize();
    void                setStreamColumnSize();
    void                setFilesColumnSize();

    virtual void        clearDetails();
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
    QString             getAudioQuantisation(DVDAUDIOQUANTISATION eQuantisation) const;
    QString            	getLanguage(const char *pszLanguage) const;
    QString             getCellType(CELLTYPE eCellType) const;
    QString             getBlockType(BLOCKTYPE eBlockType) const;
    QString             getYesOrNo(bool bValue) const;
    QString             getFilmOrCamera(bool bValue) const;
    QString             getFormattedSize(uint64_t size, bool bInBytes = false) const;
    QString             getFormattedDate(time_t unixTime) const;
    QString             getPlayTime(uint64_t qwPlaytimems, uint16_t wFrameRate = (uint16_t)-1) const;

    int                 queryDvd();
    int                 findDvd();
    int                 submitDvd();
    int                 editDetails();
    int                 editOptions();
    int                 exportXml();
    int                 importXml();

    void                loadSettings(dvddatabase * pDvdDatabase);
    void                updateData();

    bool                isPhysicalView() const;

private slots:
    void                on_actionOpen_triggered();
    void                on_actionExportXml_triggered();
    void                on_actionImportXml_triggered();
    void                on_actionOptions_triggered();
    void                on_actionOpenFromWeb_triggered();
    void                on_actionExit_triggered();
    void                on_actionQueryDvd_triggered();
    void                on_actionFindDvd_triggered();
    void                on_actionSubmitDvd_triggered();
    void                on_actionAbout_triggered();
    void                on_treeViewDvd_clicked(const QModelIndex &index);
    void                on_actionEditDetails_triggered();
    void                on_treeViewDvd_customContextMenuRequested(const QPoint &pos);
    void                handleContextMenuEditDetails();
    void                on_checkBoxPhysicalView_clicked();

protected:
    dvdparse            m_DvdParse;              //!< dvdparse parser object

private:
    Ui::dvdexplorerdlg *ui;
};

#endif // DVDEXPLORERDLG_H
