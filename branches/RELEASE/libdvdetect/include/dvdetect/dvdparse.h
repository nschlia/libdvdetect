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

/** \file dvdparse.h
 *
 *  \brief dvdparse class
 */

#pragma once

#ifndef DVDPARSE_H
#define DVDPARSE_H

#include <iostream>

#include <dvdetect/dvdtitle.h>

/*!
 *  @brief dvdparse class
 *
 * This class is used to access and parse a DVD. Normally this is the only class
 * one needs to instanciate, all other class objects will be created while parsing
 * as required.
 *
 * dvdparse (Video Manager)
 * +--- dvdtitle (Title)
 *      +---- dvdpgc (Program Chain/PGC)
 *            +---- dvdptt (Chapter/PTT)
 *            +---- dvdprogram (Program)
 *                  +---- dvdcell (Cell)
 *                        +---- dvdunit (VOB Unit)
 */
class DLL_PUBLIC dvdparse : public dvdetectbase
{
public:
    explicit dvdparse();
    virtual ~dvdparse();

    int                 parse(const std::string & strPath);

    LPCDVDVMGM          getDVDVMGM() const;
    const dvdtitle *    getDvdTitle(uint16_t wTitleSetNo) const;
    uint16_t            getTitleCount() const;
    const dvdfile *     getDvdFile(uint16_t wFileNo) const;
    uint16_t            getFileCount() const;

    // Fast Access helpers
    const dvdpgc *      getDvdPgc(uint16_t wTitleSetNo, uint16_t wProgramChainNo) const;
    const dvdprogram *  getDvdProgram(uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram) const;
    const dvdptt *      getDvdPtt(uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wPtt) const;
    const dvdcell *     getDvdCell(uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram, uint16_t wCell) const;
    const dvdunit *     getDvdUnit(uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgram, uint16_t wCell, uint16_t wUnit) const;
    uint64_t            getPlayTime() const;

    const dvdpgc *      getStartPgc() const;

    std::string         getErrorString() const;
    std::string         getPath() const;
    std::string         getTitle() const;

    void                setFullScan(bool bFullScan);
    bool                getFullScan() const;

    virtual const std::type_info & classtype() const;

protected:
    // Parse Video Manager
    void                getVmgMain(const uint8_t *pData);
    void                getVmgAttributes(const uint8_t *pData);

    void                getVmgIfo(time_t ftime, uint64_t qwSizeOfVMGI);
    void                getVmgMenuVob(time_t ftime, uint32_t dwMenuVobSize);

    bool                parseVideoManager();

    // Parse Title Set
    void                addUnit(const uint8_t* pData, dvdcell & dvdCell);
    void                addCell(const uint8_t* pData, dvdprogram & dvdProgram, uint16_t wCell, uint16_t wPGCCellPlaybackOffset, uint32_t dwOffsetVTS_PGC, uint16_t wCellPositionOffset);
    void                addPrograms(const uint8_t* pData, dvdpgc & dvdPgc, uint16_t wTitleSetNo, uint16_t wProgramChainNo);
    void                getVtsAttributes(const uint8_t* pData, dvdtitle & dvdTitle);

    void                getVtsIfo(dvdtitle & dvdTitle, time_t ftime, uint64_t qwSizeOfVMGI, uint16_t wTitleSetNo);
    void                getVtsMenuVob(dvdtitle & dvdTitle, time_t ftime, uint32_t dwMenuVobSize, uint16_t wTitleSetNo);
    void                getVtsVob(dvdtitle & dvdTitle, time_t ftime, uint32_t dwSize, uint16_t wVobNo, uint16_t wTitleSetNo);

    bool                parseTitleSet(uint16_t wTitleSetNo);

    // Get Video/Audio/Subpicture details
    void                getVideoAttributes(DVDVIDEOATTRIBUTES & videoAttributes, const uint8_t byVideoAttributes[2]);
    void                getAudioAttributes(DVDAUDIOATTRIBUTESMENU & audioAttributes, const uint8_t byAudioAttributes[8], uint16_t wAudioStream);
    void                getAudioAttributes(DVDAUDIOATTRIBUTESTITLE & audioAttributes, const uint8_t byAudioAttributes[8], uint16_t wAudioStream);
    void                getSubpictureAttributes(DVDSUBPICTUREATTRIBUTES & subpictureAttributes, const uint8_t bySubpictureAttributes[6], uint16_t wSubpictureStream);
    void                getCoding(int nCoding, DVDAUDIOATTRIBUTESMENU& audioAttributes, uint16_t wAudioStream);

    // Debug: check presence of file
#ifndef NDEBUG
    void                checkFile(dvdfile dvdFile, std::string strFileName);
#endif

protected:
    std::string         m_strPath;
    DVDVMGM             m_DVDVMGM;
    dvdtitlelst         m_dvdTitleLst;
    dvdfilelst          m_dvdFileLst;
    std::string         m_strErrorString;
    bool                m_bFullScan;
};

#endif // DVDPARSE_H

