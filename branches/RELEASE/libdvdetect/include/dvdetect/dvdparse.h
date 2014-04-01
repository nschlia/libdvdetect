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

/*! \file dvdparse.h
 *
 *  \brief dvdparse class declaration
 */

#pragma once

#ifndef DVDPARSE_H
#define DVDPARSE_H

#include <iostream>

#include <dvdetect/dvdtitle.h>
#include <dvdetect/dvdpttvmg.h>

class xmldocbuilder;
class xmldocparser;

/*!
 *  @brief dvdparse class
 *
 * This class is used to access and parse a DVD. Normally this is the only class
 * one needs to instanciate, all other class objects will be created while parsing
 * as required.
 *
 * Virtual View:
 *
 * dvdparse (Video Manager)
 * +--- dvdpttvmg (PTT list for video manager)
 *      +---- dvdpttvts (Title)
 *            +---- dvdprogram (Program)
 *                  +---- dvdcell (Cell)
 *                        +---- dvdunit (VOB Unit)
 * Physical view
 *
 * dvdparse (Video Manager)
 * +--- dvdtitle (Title)
 *      +---- dvdpgc (Program Chain/PGC)
 *            +---- dvdprogram (Program)
 *                  +---- dvdcell (Cell)
 *                        +---- dvdunit (VOB Unit)
 */
class DLL_PUBLIC dvdparse : public dvdetectbase
{
    friend class xmldocbuilder;
    friend class xmldocparser;

public:
    //! Constructor.
    /*!
     *  Construct a dvdparse element.
     */
    explicit dvdparse(
            void *  (*openFile)(const char *filename, const char *mode, const char *proxy) = ::openFile,
            size_t  (*readFile)(void* buffer, size_t size, size_t count, void* stream) = ::readFile,
            int     (*writeFile)(const void* buffer, size_t size, size_t count, void* stream) = ::writeFile,
            int     (*closeFile)(void *stream) = ::closeFile,
            int     (*statFile)(const char *file, LPDVDFILESTAT pFileStat, const char *proxy) = ::statFile,
            int     (*fstatFile)(void* stream, LPDVDFILESTAT pFileStat) = ::fstatFile);

    //! Destructor.
    /*!
     *  Destruct a dvdparse element.
     */
    virtual ~dvdparse();

    //! Clear all data stored in this class.
    void                clear();

    //! Parse a directory with DVD files and fill in the DVD structure.
    /*!
     *  \param strPath std::string Path to files. VIDEO_TS will be automatically added if necessesary.
     *  \return Success: 0
     *  \return Error: non zero
     */
    int                 parse(const std::string & strPath);

    //! Access the DVDVMGM data structure.
    /*!
     *  \return Pointer to the underlying DVDVMGM (DVD Video Manager) structure.
     *  \return It is guaranteed that a valid structure is returned (never NULL).
     */
    LPCDVDVMGM          getDVDVMGM() const;

    //! Get a DVD title set on this DVD.
    /*!
     *  \param wTitleSetNo uint16_t Index of title set (1...n)
     *  \return Success: Pointer to dvdtitle class
     *  \return Fail: NULL if wTitleSetNo out of bounds.
     */
    dvdtitle *          getDvdTitle(uint16_t wTitleSetNo) const;

    //! Get the number of title sets in this on this DVD.
    /*!
     *  \return Number of number of title sets (1...n)
     */
    uint16_t            getTitleSetCount() const;

    //! Get a DVD file on this DVD.
    /*!
     *  \param wFileNo uint16_t Index of file (1...n)
     *  \return Success: Pointer to dvdfile class
     *  \return Fail: NULL if wFileNo out of bounds.
     */
    dvdfile *           getDvdFile(uint16_t wFileNo) const;

    //! Get the number of files in this on this DVD.
    /*!
     *  \return Number of number of files (1...n)
     */
    uint16_t            getFileCount() const;

    // Fast Access helpers

    //! Get a DVD program chain (PGC) on this DVD.
    /*!
     *  \param wTitleSetNo uint16_t Index of title set number (1...n)
     *  \param wProgramChainNo uint16_t Index of program chain (1...n)
     *  \return Success: Pointer to dvdpgc class
     *  \return Fail: NULL if parameter out of bounds.
     */
    dvdpgc *            getDvdPgc(uint16_t wTitleSetNo, uint16_t wProgramChainNo) const;

    //! Get a DVD program chain (PGC) on this DVD.
    /*!
     *  \param wTitleSetNo uint16_t Index of title set number (1...n)
     *  \param wProgramChainNo uint16_t Index of program chain (1...n)
     *  \param wProgramNo uint16_t Index of program (1...n)
     *  \return Success: Pointer to dvdprogram class
     *  \return Fail: NULL if parameter out of bounds.
     */
    dvdprogram *        getDvdProgram(uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo) const;

    //! Get a DVD program chain (PGC) on this DVD.
    /*!
     *  \param pDVDPTTVTS LPCDVDPTTVTS Pointer to DVDPTT class to get virtual chapter
     *  \return Success: Pointer to dvdprogram class
     *  \return Fail: NULL if not found.
     */
    dvdprogram *        getDvdProgram(LPCDVDPTTVTS pDVDPTTVTS) const;

    //! Get a DVD part-of-program (PTT) for a video title set on this DVD.
    /*!
     *  \param wTitleSetNo uint16_t Index of title set number (1...n)
     *  \return Success: Pointer to dvdpttvmg class
     *  \return Fail: NULL if wTitleSetNo out of bounds.
     */
    dvdpttvmg *         getDvdPttVmg(uint16_t wTitleSetNo) const;

    //! Get the PTT count.
    /*!
     *  \return Number of PTTs
     */
    uint16_t            getDvdPttVmgCount() const;

    //! Get a DVD PTT on this DVD.
    /*!
     *  \param wTitleSetNo uint16_t Index of title set number (1...n)
     *  \param wPttNo uint16_t Index of part of title (1...n)
     *  \return Success: Pointer to dvdpttvts class
     *  \return Fail: NULL if parameter out of bounds.
     */
    dvdpttvts *         getDvdPttVts(uint16_t wTitleSetNo, uint16_t wPttNo) const;

    //! Get a DVD PTT on this DVD.
    /*!
     *  \param pDvdProgram dvdprogram * dvdprogram object to search for
     *  \return Success: Pointer to dvdpttvts class
     *  \return Fail: NULL if parameter out of bounds.
     */
    dvdpttvts *         getDvdPttVts(const dvdprogram *pDvdProgram) const;

    //! Get a DVD cell on this DVD.
    /*!
     *  \param wTitleSetNo uint16_t Index of title set number (1...n)
     *  \param wProgramChainNo uint16_t Index of program chain (1...n)
     *  \param wProgramNo uint16_t Index of program (1...n)
     *  \param wCellNo uint16_t Index of cell (1...n)
     *  \return Success: Pointer to dvdcell class
     *  \return Fail: NULL if parameter out of bounds.
     */
    dvdcell *           getDvdCell(uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo, uint16_t wCellNo) const;

    //! Get a DVD unit on this DVD.
    /*!
     *  \param wTitleSetNo uint16_t Index of title set number (1...n)
     *  \param wProgramChainNo uint16_t Index of program chain (1...n)
     *  \param wProgramNo uint16_t Index of program (1...n)
     *  \param wCellNo uint16_t Index of cell (1...n)
     *  \param wUnitNo uint16_t Index of unit (1...n)
     *  \return Success: Pointer to dvdunit class
     *  \return Fail: NULL if parameter out of bounds.
     */
    dvdunit *           getDvdUnit(uint16_t wTitleSetNo, uint16_t wProgramChainNo, uint16_t wProgramNo, uint16_t wCellNo, uint16_t wUnitNo) const;

    //! Get the size (in bytes) on this DVD.
    /*! Get the size (in bytes) on this DVD.
     * This is not the combined size of all VOBS. This is calculated
     * by the size of all cells in this program, so this may (and will)
     * differ from the file size.
     *
     *  \return Size (in bytes) on this DVD.
     */
    uint64_t            getSize() const;

    //! Get the playtime (in ms) on this DVD.
    /*!
     *  \return Playtime (in ms) on this DVD.
     */
    uint64_t            getPlayTime() const;

    //! Get the virtual size (in bytes) on this DVD.
    /*! Get the virtual size (in bytes) on this DVD.
     * This is not the combined size of all VOBS. This is calculated
     * by the size of all cells in this program, so this may (and will)
     * differ from the file size.
     *
     *  \return Size (in bytes) on this DVD.
     */
    uint64_t            getVirtSize() const;

    //! Get the virtual playtime (in ms) on this DVD.
    /*!
     *  \return Virtual playtime (in ms) on this DVD.
     */
    uint64_t            getVirtPlayTime() const;

    //! Gets the staring program chain (PGC)
    /*!
     * Gets the staring program chain (PGC) TODO: NOT IMPLEMENTED YET. ALWAYS RETURNS NULL!
     *
     *  \return Success: Pointer to dvdpgc class
     *  \return Fail: NULL of no start PGC defined.
     */
    const dvdpgc *      getStartPgc() const;

    //! If an error occurred, this string will return (English) details.
    /*!
     *  \return Error text.
     */
    const std::string & getErrorString() const;

    //! If an error occurred, this will return the code.
    /*!
     *  \return Error code.
     */
    DVDERRORCODE        getErrorCode() const;

    //! After parse() was called, the path used can be retrieved with this fúnction. The trailing separator will always be included.
    /*!
     *  \return Current path including trailing separator.
     */
    const std::string & getPath() const;

    //! Enables/disables full scan.
    /*! Enables/disables full scan. If full scan is disabled, no extra disk access takes place:
     * File sizes are calculated using information gathered from IFO files, all file dates are
     * set to the file date of the VIDEO_TS.IFO. This minimizes file operations (especially
     * useful if scanning files on webservers).
     *
     *  \param eScanMode DVDSCANMODE full scan, fast scan or auto mode
     */
    void                setScanMode(DVDSCANMODE eScanMode);

    //! Check if full scan mode is enabled
    /*!
     *  \return true = full scan enabled, false = full scan disabled
     */
    bool                getScanMode() const;

    //! Check if full DVD has been parsed ("loaded")
    /*!
     *  \return true = disk loaded, false = disk not loaded
     */
    bool                isLoaded() const;

    //! Generate the disk id code
    /*!
     * Calculate the DVD code to generate the hash from. The code is built from
     * the title sets, chapters and playtimes of the virtual layout (PTT):
     *
     * Offset   Size    Contents    Remark
     * 0        3       "DVD"       For DVDs, the string "DVD" is prepended. If this library
     *                              will someday be extended to e.g. bluray, this might be "BD" etc.
     * 4        4       TTTT        Number of title sets
     *
     * for each title followed by:
     *
     * Offset   Size    Contents    Remark
     * 0        4       CCCC        Number of chapters
     * 4        8       PPPPPPPP    Playtime of chapter 1 in ms
     * 12       8       PPPPPPPP    Playtime of chapter 2 in ms
     * ...
     * 4*CCCC   8       PPPPPPPP    Playtime of chapter n in ms
     *
     *  \return Disk id code
     */
    std::string         getCode() const;

    //! Calculate the disk hash (md5 code)
    /*!
     *  \return Disk hash
     */
    std::string         getHash() const;

    //! Get disk album artist
    /*!
     * If DVD was successfully looked up, return the album artist
     *
     *  \return album artist or empty string if unknown
     */
    const std::string & getAlbumArtist() const;

    //! Set disk album artist
    /*!
     * Sets the album artist
     *
     *  \param strAlbumArtist const std::string & Album artist
     */
    void                setAlbumArtist(const std::string & strAlbumArtist);

    //! Get disk album name
    /*!
     * If DVD was successfully looked up, return the album
     *
     *  \return album or empty string if unknown
     */
    const std::string & getAlbum() const;

    //! Set disk album name
    /*!
     * Sets the album name
     *
     *  \param strAlbum const std::string & Album name
     */
    void                setAlbum(const std::string & strAlbum);

    //! Get original language disk album name
    /*!
     * If DVD was successfully looked up, return the original album name
     *
     *  \return album or empty string if unknown
     */
    const std::string & getOriginalAlbum() const;

    //! Set original language disk album name
    /*!
     * Sets the original language album name
     *
     *  \param strOriginalAlbum const std::string & Original language album nam
     */
    void                setOriginalAlbum(const std::string & strOriginalAlbum);

    //! Get the DVD genre
    /*!
     * If DVD was successfully looked up, return DVD genre
     *
     *  \return DVD genre or empty string if unknown
     */
    const std::string & getGenre() const;

    //! Set the DVD genre
    /*!
     * Sets the DVD genre
     *
     *  \param strGenre const std::string & DVD genre
     */
    void                setGenre(const std::string & strGenre);

    //! Get DVD movie cast
    /*!
     * If DVD was successfully looked up, return DVD movie cast
     *
     *  \return DVD movie cast or empty string if unknown
     */
    const std::string & getCast() const;

    //! Set the DVD movie cast
    /*!
     * Sets the DVD movie cast
     *
     *  \param strCast const std::string & DVD movie cast
     */
    void                setCast(const std::string & strCast);

    //! Get DVD movie crew
    /*!
     * If DVD was successfully looked up, return DVD movie crew
     *
     *  \return DVD movie crew or empty string if unknown
     */
    const std::string & getCrew() const;

    //! Set the DVD movie crew
    /*!
     * Sets the DVD movie crew
     *
     *  \param strCrew const std::string & DVD movie crew
     */
    void                setCrew(const std::string & strCrew);

    //! Get the DVD movie director
    /*!
     * If DVD was successfully looked up, return DVD movie director
     *
     *  \return DVD movie director or empty string if unknown
     */
    const std::string & getDirector() const;

    //! Set the DVD movie director
    /*!
     * Sets the DVD movie director
     *
     *  \param strDirector const std::string & DVD movie director
     */
    void                setDirector(const std::string & strDirector);

    //! Get storyline writer
    /*!
     * If DVD was successfully looked up, return storyline writer
     *
     *  \return Storyline writer or empty string if unknown
     */
    const std::string & getScreenplay() const;

    //! Set the storyline writer
    /*!
     * Sets the storyline writer
     *
     *  \param strScreenplay const std::string & Storyline writer
     */
    void                setScreenplay(const std::string & strScreenplay);

    //! Get producer
    /*!
     * If DVD was successfully looked up, return producer
     *
     *  \return Producer or empty string if unknown
     */
    const std::string & getProducer() const;

    //! Set the producer
    /*!
     * Sets the producer
     *
     *  \param strProducer const std::string & Producer
     */
    void                setProducer(const std::string & strProducer);

    //! Get editor
    /*!
     * If DVD was successfully looked up, return editor
     *
     *  \return Editor or empty string if unknown
     */
    const std::string & getEditing() const;

    //! Set the editor
    /*!
     * Sets the editor
     *
     *  \param strEditing const std::string & Editor
     */
    void                setEditing(const std::string & strEditing);

    //! Get cinematographer
    /*!
     * If DVD was successfully looked up, return cinematographer
     *
     *  \return Cinematographer empty string if unknown
     */
    const std::string & getCinematography() const;

    //! Set the cinematographer
    /*!
     * Sets the cinematographer
     *
     *  \param strCinematography const std::string & Cinematographer
     */
    void                setCinematography(const std::string & strCinematography);

    //! Get country of origin
    /*!
     * If DVD was successfully looked up, return country of origin
     *
     *  \return country of origin or empty string if unknown
     */
    const std::string & getCountry() const;

    //! Set the country of origin
    /*!
     * Sets the country of origin
     *
     *  \param strCountry const std::string & Country of origin
     */
    void                setCountry(const std::string & strCountry);

    //! Get original language
    /*!
     * If DVD was successfully looked up, return original language
     *
     *  \return original language or empty string if unknown
     */
    const std::string & getOriginalLanguage() const;

    //! Set the original language
    /*!
     * Sets the original language
     *
     *  \param strOriginalLanguage const std::string & Original language
     */
    void                setOriginalLanguage(const std::string & strOriginalLanguage);

    //! Get release date
    /*!
     * If DVD was successfully looked up, return release date
     * Returned in ISO format (YYYY-MM-DD HH:MM:SS).
     *
     *  \return release date or empty string if unknown
     */
    const std::string & getReleaseDate() const;

    //! Set release date
    /*!
     * Sets the release date
     * Must be in ISO format (YYYY-MM-DD HH:MM:SS). Time can be left empty.
     *
     *  \param strReleaseDate const std::string & release date
     */
    void                setReleaseDate(const std::string & strReleaseDate);

    //! Get special feature list
    /*!
     * If DVD was successfully looked up, return special feature list
     *
     *  \return Special feature list or empty string if unknown
     */
    const std::string & getSpecialFeatures() const;

    //! Set the special feature list
    /*!
     * Sets the special feature list
     *
     *  \param strFeatures const std::string & special feature list
     */
    void                setSpecialFeatures(const std::string & strFeatures);

    //! Get EAN/UPC
    /*!
     * If DVD was successfully looked up, return EAN/UPC
     *
     *  \return EAN/UPC or empty string if unknown
     */
    const std::string & getEAN_UPC() const;

    //! Set EAN/UPC
    /*!
     * Sets the EAN/UPC
     *
     *  \param strEAN_UPC const std::string & EAN/UPC
     */
    void                setEAN_UPC(const std::string & strEAN_UPC);

    //! Get storyline/synopsis
    /*!
     * If DVD was successfully looked up, return storyline/synopsis
     *
     *  \return storyline/synopsis or empty string if unknown
     */
    const std::string & getStoryline() const;

    //! Set storyline/synopsis
    /*!
     * Sets the storyline/synopsis
     *
     *  \param strStoryline const std::string & Storyline/synopsis
     */
    void                setStoryline(const std::string & strStoryline);

    //! Get remarks
    /*!
     * If DVD was successfully looked up, return storyline/synopsis
     *
     *  \return remarks or empty string if unknown
     */
    const std::string & getRemarks() const;

    //! Set remarks
    /*!
     * Sets the remarks
     *
     *  \param strRemarks const std::string & Remarks
     */
    void                setRemarks(const std::string & strRemarks);

    //! Get revision
    /*!
     * If DVD was successfully looked up, return revision
     *
     *  \return revision or 0 if unknown
     */
    int                 getRevision() const;

    //! Get submitter
    /*!
     * If DVD was successfully looked up, return submitter
     *
     *  \return submitter or empty string if unknown
     */
    const std::string & getSubmitter() const;

    //! Set submitter
    /*!
     * Sets the submitter
     *
     *  \param strSubmitter const std::string & Submitter
     */
    void                setSubmitter(const std::string & strSubmitter);

    //! Get keywords
    /*!
     * If DVD was successfully looked up, return keywords
     *
     *  \return keywords or empty string if unknown
     */
    const std::string & getKeywords() const;

    //! Set keywords
    /*!
     * Sets the keywords
     *
     *  \param strKeywords const std::string & Keywords
     */
    void                setKeywords(const std::string & strKeywords);

    //! Get protocol version
    /*!
     * If DVD was successfully looked up, return protocol version
     *
     *  \return Protocol version or 0 if unknown
     */
    int                 getProtocolVersion() const;

    //! Get library version
    /*!
     * If DVD was successfully looked up, return library version
     *
     *  \return Library version or empty string if unknown
     */
    const std::string & getLibraryVersion() const;

    //! Get library name
    /*!
     * If DVD was successfully looked up, return library name
     *
     *  \return Library name or empty string if unknown
     */
    const std::string & getLibraryName() const;

    //! Client name
    /*!
     * If DVD was successfully looked up, return client name
     *
     *  \return Client name or empty string if unknown
     */
    const std::string & getClientName() const;

    //! Get submitter IP
    /*!
     * If DVD was successfully looked up, return submitter IP
     *
     *  \return submitter IP or empty string if unknown
     */
    const std::string & getSubmitterIP() const;

    //! Get submission client
    /*!
     * If DVD was successfully looked up, return submission client
     *
     *  \return submission client or empty string if unknown
     */
    const std::string & getClient() const;

    //! Set the client program
    /*!
     * Sets the client program name
     *
     *  \param strClient const std::string & Client program name
     */
    void                setClient(const std::string & strClient);

    //! Get date of creation
    /*!
     * If DVD was successfully looked up, return date of creation
     *
     *  \return Date of creation or empty string if unknown
     */
    const std::string & getDateCreated() const;

    //! Get date of last change
    /*!
     * If DVD was successfully looked up, return date of last change
     *
     *  \return date of last change or empty string if unknown/not changed
     */
    const std::string & getDateLastChanged() const;

    //! Update album name etc.
    /*!
     * Updates the album name, artist etc. from the parse data received.
     *
     *  \param dvdParse const dvdparse & Data received from server
     */
    void                update(const dvdparse *dvdParse);

    //! Set the proxy name
    /*!
     * Set the proxy name (and optionally port), e.g. proxy:3128.
     *
     *  \param strProxy const std::string & Proxy name and port
     */
    void                setProxy(const std::string & strProxy);

    //! Get the proxy name
    /*!
     * Get the proxy name.
     *
     *  \return Returns the proxy name
     */
    std::string         getProxy() const;

    dvdparse& operator= (dvdparse const & source);

protected:

    // Parse Video Manager
    void                getVmgPtt(const uint8_t* pData);
    uint16_t            getVmgMain(const uint8_t *pData);
    void                getVmgAttributes(const uint8_t *pData);
    int                 getVmgIfo();
    int                 getVmgMenuVob();
    void                getVmgIfo(time_t ftime, uint64_t qwSizeOfVMGI);
    void                getVmgMenuVob(time_t ftime, uint32_t dwMenuVobSize);
    uint16_t            parseVideoManager();

    // Parse Title Set
    void                addUnit(const uint8_t* pData, dvdcell *dvdCell);
    void                addCell(const uint8_t* pData, dvdprogram * dvdProgram, uint16_t wCellNo, uint16_t wPGCCellPlaybackOffset, uint32_t dwOffsetVTS_PGC, uint16_t wCellPositionOffset);
    void                addPrograms(const uint8_t* pData, dvdpgc * dvdPgc, uint16_t wTitleSetNo, uint16_t wProgramChainNo);
    void                getVtsAttributes(const uint8_t* pData, dvdtitle *dvdTitle);
    int                 getVtsIfo(dvdtitle * dvdTitle, uint16_t wTitleSetNo);
    int                 getVtsMenuVob(dvdtitle * dvdTitle, uint16_t wTitleSetNo);
    int                 getVtsVob(dvdtitle * dvdTitle, uint16_t wVobNo, uint16_t wTitleSetNo);
    void                getVtsIfo(dvdtitle * dvdTitle, time_t ftime, uint64_t qwSizeOfVMGI, uint16_t wTitleSetNo);
    void                getVtsMenuVob(dvdtitle * dvdTitle, time_t ftime, uint32_t dwMenuVobSize, uint16_t wTitleSetNo);
    void                getVtsVob(dvdtitle * dvdTitle, time_t ftime, uint32_t dwSize, uint16_t wVobNo, uint16_t wTitleSetNo);
    bool                parseTitleSet(uint16_t wTitleSetNo);

    // Get Video/Audio/Subpicture details
    void                getVideoAttributes(DVDVIDEOSTREAM & videoAttributes, const uint8_t byVideoAttributes[2]);
    void                getAudioAttributes(DVDAUDIOSTREAM & audioAttributes, const uint8_t byAudioAttributes[8], uint16_t wAudioStreamNo);
    void                getAudioAttributesEx(DVDAUDIOSTREAMEX & audioAttributes, const uint8_t byAudioAttributes[8]);
    void                getSubpictureAttributes(DVDSUBPICSTREAM & subpictureAttributes, const uint8_t bySubpictureAttributes[6], uint16_t wSubpicStreamNo);
    void                getCoding(int nCoding, DVDAUDIOSTREAM& audioAttributes, uint16_t wAudioStreamNo);

    bool				locateDVD();

    //! Read binary file into memory
    /*!
     * IFO files are never too large, so they can always be read into memory as a whole.
     *
     * \param strFilePath const string & Full filename including path
     * \param ftime time_t & If file successfully read, contains file creation time
     * \return Success: Pointer to memory area with file contents. Call delete when done with it.
     * \return Fail: NULL
     */
    const uint8_t*      readIFO(const std::string & strFilePath, time_t & ftime);

    //! Set the error.
    void                setError(const std::string & strErrorString, DVDERRORCODE eErrorCode);

    // Debug: check presence of file
#if !defined(NDEBUG) && defined(HAVE_ASSERT_H)
    void                checkFile(const dvdfile * dvdFile);
#endif

    void                setSubmitterIP(const std::string & strSubmitterIP);
    void                setDateCreated(const std::string & strDateCreated);
    void                setDateLastChanged(const std::string & strDateLastChanged);
    void                setHash(const std::string & strHash);
    void                setRevision(int nRevision);
    void 				setProtocolVersion(int nProtocolVersion);
    void 				setLibraryVersion(const std::string & strLibraryVersion);
    void 				setLibraryName(const std::string & strLibraryName);
    void 				setClientName(const std::string & strClientName);

    bool                getFullScan() const;

protected:
    std::string         m_strPath;					//!< Path to DVD (including trailing separator)
    DVDVMGM             m_DVDVMGM;					//!< DVDVMGM structure (DVD part of title)
    dvdtitlelst         m_lstDvdTitle;				//!< list of DVD program chains (PGC) in this title set
    dvdpttvmglst        m_lstDvdPttVmg;             //!< PTT list for video manager
    dvdfilelst          m_lstDvdFile;				//!< list of DVD files in this video manager
    std::string         m_strErrorString;			//!< Last error as clear text (English)
    DVDERRORCODE        m_eErrorCode;               //!< Error code
    DVDSCANMODE         m_eScanMode;				//!< Scan mode: full/fast/auto scan
    int                 m_nVirtTitleCount;          //!< Number of virtual titles
    bool                m_bIsLoaded;                //!< true if DVD successfully loaded

    // database data
    std::string         m_strHash;                  //!< md5 hash
    std::string         m_strAlbumArtist;           //!< Album artist
    std::string         m_strAlbum;                 //!< Album name
    std::string         m_strOriginalAlbum;         //!< Original language title/album name
    std::string         m_strGenre;                 //!< DVD genre
    std::string         m_strCast;                  //!< Cast details
    std::string         m_strCrew;                  //!< Crew details
    std::string         m_strDirector;              //!< Director
    std::string         m_strScreenplay;            //!< Storyline writer
    std::string         m_strProducer;              //!< Produced by
    std::string         m_strEditing;               //!< Editing by
    std::string         m_strCinematography;        //!< Cinematography/camera by
    std::string         m_strCountry;               //!< Country of origin
    std::string         m_strOriginalLanguage;      //!< Original Language
    std::string         m_strReleaseDate;           //!< Release date
    std::string         m_strSpecialFeatures;       //!< Special feature list
    std::string         m_strEAN_UPC;               //!< DVD EAN/UPC
    std::string         m_strStoryline;             //!< Storyline/synopsis
    std::string         m_strRemarks;               //!< Remarks
    int                 m_nRevision;                //!< Revision
    std::string         m_strSubmitter;             //!< Email address of submitter
    std::string         m_strSubmitterIP;           //!< IP of submitter
    std::string         m_strClient;                //!< Submission client
    std::string         m_strDateCreated;           //!< Date of data set creation
    std::string         m_strDateLastChanged;       //!< Data data set was last changed
    std::string         m_strKeywords;              //!< Keywords
    int                 m_nProtocolVersion;         //!< For received queries: protocol version
    std::string         m_strLibraryVersion;        //!< For received queries: remote library version
    std::string         m_strLibraryName;           //!< For received queries: remote library name
    std::string         m_strClientName;            //!< For received queries: remote client
    std::string         m_strProxy;                 //!< Proxy server address/port

    // I/O functions
    void *              (*m_pOpenFile)(const char *filename, const char *mode, const char *proxy);
    size_t              (*m_pReadFile)(void* buffer, size_t size, size_t count, void* stream);
    int                 (*m_pWriteFile)(const void* buffer, size_t size, size_t count, void* stream);
    int                 (*m_pCloseFile)(void *stream);
    int                 (*m_pStatFile)(const char *file, LPDVDFILESTAT pFileStat, const char *proxy);
    int                 (*m_pFstatFile)(void* stream, LPDVDFILESTAT pFileStat);
};

typedef vector_ptr<dvdparse> dvdparselst;           //!< shortcut for a list of dvdfiles

#endif // DVDPARSE_H
