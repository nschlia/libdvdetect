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

/*! \file types.h
 *
 *  \brief Types and defines
 *
 * Types and defines of structures and enumerations for the public interface
 * of libdvdetect. These structurse contain preprocessed data about the DVD
 * (mainly decoded bit field information and big endian values converted to
 * native format).
 *
 */

#pragma once

#ifndef TYPES_H
#define TYPES_H

#if !defined(__cplusplus) && !defined(_MSC_VER)
#include <stdbool.h>
#include <stdint.h>
#endif

#include <time.h>

#ifndef MAX_PATH
#include <limits.h>
#define MAX_PATH    PATH_MAX
#endif // MAX_PATH

#ifndef LIBDVDETECT_COPYRIGHT
#define LIBDVDETECT_COPYRIGHT "Copyright (C) 2013-2015 Norbert Schlia <nschlia@dvdetect.de>"
#endif // LIBDVDETECT_COPYRIGHT

#ifndef LIBDVDETECT_XML_COMMENT
#define LIBDVDETECT_XML_COMMENT "Created with libdvdetect DVD analyser and lookup library"
#endif // LIBDVDETECT_XML_COMMENT

#ifndef LIBDVDETECT_LIBRARYNAME
#define LIBDVDETECT_LIBRARYNAME "libdvdetect library"
#endif // LIBDVDETECT_LIBRARYNAME

// -------------------------------------------------------------------------------------------------------

#define DVDVTS_MAXAUDIOSTREAM   8           //!< DVD titles can have up to 8 audio streams
#define DVDVTS_MAXSUBPICSTREAM  32          //!< DVD titles can have up to 32 subtitle (subpicture) streams

//#define DVD_DATEFORMAT          "%F %T"     //!< Date format like used in SQL: YYYY-MM-DD HH:MM:SS
#define DVD_DATEFORMAT          "%Y-%m-%d %H:%M:%S"     //!< Date format like used in SQL: YYYY-MM-DD HH:MM:SS
#define DVD_LANGCODELEN         2           //!< Language codes are 2 bytes

// -------------------------------------------------------------------------------------------------------

#define DVD_SECTOR_SIZE      	2048
#define DVD_MAX_VOB            	9
#define DVD_MAX_VOB_SIZE     	((1024*1024*1024) - DVD_SECTOR_SIZE)    // = 1.073.739.776 bytes

/*! If the submitter name is set to this value, it will never be written
 * into the database. In fact it will be treated like NULL. This can be
 * used to mark it in xml to be changed.
 */
#define DEFSUBMITTER         	"***ChangeMe!***"

// -------------------------------------------------------------------------------------------------------

//! DVD error code
/*!
 * DVD error codes that can be used for localised messages.
 */
typedef enum
{
    DVDERRORCODE_SUCCESS,                       //!< Success/no error
    DVDERRORCODE_DIROPEN,                       //!< Error opening directory
    DVDERRORCODE_FILEOPEN,                      //!< Error opening file
    DVDERRORCODE_FILESTAT,                      //!< Error getting file status
    DVDERRORCODE_FILEREAD,                      //!< Error reading file
    DVDERRORCODE_OPEN_DVD,                      //!< Error opening DVD
    DVDERRORCODE_VMG_IFO,                       //!< Error reading video manager IFO
    DVDERRORCODE_VTS_IFO,                       //!< Error reading video title set IFO
    DVDERRORCODE_EMPTY_PATH,                    //!< Path empty
    DVDERRORCODE_NOT_IMPLEMENTED,               //!< Function not implemented
    DVDERRORCODE_NOT_INITIALISED,               //!< Class not initialised
    DVDERRORCODE_INVALID_PARAMETER,             //!< Invalid parameter
    DVDERRORCODE_INTERNAL_ERROR,                //!< libdvdetect internal error
    DVDERRORCODE_HTML_ERROR,                    //!< HTML error (see text for details)
    DVDERRORCODE_XML_ERROR,                     //!< XML error (see text for details)
    DVDERRORCODE_XML_WRONG_MODE,                //!< Wrong XML mode
    DVDERRORCODE_XML_UNKNOWN_MODE,              //!< Unknown XML mode
    DVDERRORCODE_XML_INCONSISTENT_DATA,         //!< Inconsistent data in XML (see text for details)
    DVDERRORCODE_XML_RESPONSE,                  //!< Response (error text verbatim as send from server)
    DVDERRORCODE_XML_UNSUPPORTED_VERSION,       //!< XML protocol version not supported
    DVDERRORCODE_NOT_FOUND,                     //!< DVD not found
    DVDERRORCODE_SQL_ERROR,                     //!< SQL error
    DVDERRORCODE_DUPICLATE_SUBMISSION,          //!< Duplicate submission
    DVDERRORCODE_UNSUPPORTED_VERSION            //!< Unsupported protocol version

} DVDERRORCODE, *LPDVDERRORCODE;

typedef const DVDERRORCODE* LPCDVDERRORCODE;

// -------------------------------------------------------------------------------------------------------

typedef enum
{
    DVDSCANMODE_INVALID,                        //!< Invalid mode
    DVDSCANMODE_FULL,                           //!< Perform full scan
    DVDSCANMODE_FAST,                           //!< Perform fast scan
    DVDSCANMODE_AUTO                            //!< Automatically use best mode

} DVDSCANMODE, *LPDVDSCANMODE;

typedef const DVDSCANMODE* LPCDVDSCANMODE;

// -------------------------------------------------------------------------------------------------------

//! Video attributes
/*!
 * Coding mode: MPEG1 or MPEG2
 */
typedef enum
{
    DVDVIDEOCODINGMODE_INVALID,                                         //!< invalid
    DVDVIDEOCODINGMODE_MPEG1,                                           //!< MPEG-1
    DVDVIDEOCODINGMODE_MPEG2                                            //!< MPEG-2

} DVDVIDEOCODINGMODE, *LPDVDVIDEOCODINGMODE;

typedef const DVDVIDEOCODINGMODE* LPCDVDVIDEOCODINGMODE;                //!< constant version

//! TV Standard
/*!
 * PAL or NTSC
 */
typedef enum
{
    DVDVIDEOTVSTANDARD_INVALID,                                         //!< invalid
    DVDVIDEOTVSTANDARD_NTSC,                                            //!< NTSC
    DVDVIDEOTVSTANDARD_PAL                                              //!< PAL

} DVDVIDEOTVSTANDARD, *LPDVDVIDEOTVSTANDARD;

typedef const DVDVIDEOTVSTANDARD* LPCDVDVIDEOTVSTANDARD;

//! Aspect ratio
/*!
 * Aspect ratio
 */
typedef enum
{
    DVDVIDEOASPECT_INVALID,                                             //!< invalid
    DVDVIDEOASPECT_4_3,                                                 //!< 4:3
    DVDVIDEOASPECT_16_9                                                 //!< 16:9

} DVDVIDEOASPECT, *LPDVDVIDEOASPECT;

typedef const DVDVIDEOASPECT* LPCDVDVIDEOASPECT;

//! Video resolution
/*!
 * Video resolution
 * Possible values:
 * <table><tr>
 * <td>NTSC</td><td>PAL</td>
 * </tr><tr>
 * <td>720x480</td>720x576<td></td>
 * </tr><tr>
 * <td>704x480</td>704x576<td></td>
 * </tr><tr>
 * <td>352x480</td>352x576<td></td>
 * </tr><tr>
 * <td>352x240</td>352x288<td></td>
 * </tr></table>
 */
typedef struct _tagDVDVIDEORESOLUTION
{
    uint16_t            m_wX;                                           //!< Resolution (width)
    uint16_t            m_wY;                                           //!< Resolution (height)

} DVDVIDEORESOLUTION, *LPDVDVIDEORESOLUTION;

typedef const DVDVIDEORESOLUTION* LPCDVDVIDEORESOLUTION;

//! Video attributes
/*!
 * Decoded video attributes
 */
typedef struct _tagDVDVIDEOSTREAM
{
    uint16_t            m_wID;                                          //!< Video stream ID, fixed to 0xE0
    DVDVIDEOCODINGMODE  m_eCodingMode;                                  //!< Video coding mode (MPEG-1 or MPEG-2)
    DVDVIDEOTVSTANDARD  m_eStandard;                                    //!< Video standard (NTSC, PAL)
    DVDVIDEOASPECT      m_eAspect;                                      //!< Video aspect ratio (16:9, 4:3)
    bool                m_bAutomaticPanScanDisallowed;                  //!< Automatic Pan/Scan
    bool                m_bAutomaticLetterboxDisallowed;                //!< Automatic Letterbox
    bool                m_bCCForLine21Field1InGOP;                      //!< NTSC only: Closed Caption for line 21 field 1 in GOP (Group of Pictures)
    bool                m_bCCForLine21Field2InGOP;                      //!< NTSC only: Closed Caption for line 21 field 2 in GOP (Group of Pictures)
    bool                m_bCBR;                                         //!< Constant/variable bit rate
    DVDVIDEORESOLUTION  m_Resolution;                                   //!< Resolution (width/height)
    bool                m_bLetterBoxed;                                 //!< false = Full Screen; true = top and bottom cropped (4:3 aspect only)
    bool                m_bSourceFilm;                                  //!< false = camera, true = film

} DVDVIDEOSTREAM, *LPDVDVIDEOSTREAM;

typedef const DVDVIDEOSTREAM* LPCDVDVIDEOSTREAM;

// -------------------------------------------------------------------------------------------------------

//! Coding mode
/*!
 * Coding mode
 */
typedef enum _enumDVDAUDIOCODINGMODE
{
    DVDAUDIOCODINGMODE_INVALID,                                         //!< invalid
    DVDAUDIOCODINGMODE_AC3,                                             //!< 0 AC3
    DVDAUDIOCODINGMODE_MPEG1,                                           //!< 2 Mpeg-1
    DVDAUDIOCODINGMODE_MPEG2,                                       //!< 3 Mpeg-2ext
    DVDAUDIOCODINGMODE_LPCM,                                            //!< 4 LPCM
    DVDAUDIOCODINGMODE_DTS                                              //!< 6 DTS

} DVDAUDIOCODINGMODE, *LPDVDAUDIOCODINGMODE;

typedef const DVDAUDIOCODINGMODE* LPCDVDAUDIOCODINGMODE;

//! Application mode
/*!
 * Application mode
 */
typedef enum _enumDVDAUDIOAPPLICATIONMODE
{
    DVDAUDIOAPPLICATIONMODE_INVALID,                                    //!< invalid
    DVDAUDIOAPPLICATIONMODE_UNSPECIFIED,                                //!< 0 = unspecified
    DVDAUDIOAPPLICATIONMODE_KARAOKE,                                    //!< 1 = karaoke
    DVDAUDIOAPPLICATIONMODE_SURROUND                                    //!< 2 = surround

} DVDAUDIOAPPLICATIONMODE, *LPDVDAUDIOAPPLICATIONMODE;

typedef const DVDAUDIOAPPLICATIONMODE* LPCDVDAUDIOAPPLICATIONMODE;

//! Quantization/DRC
/*!
 * Quantization/DRC
 */
typedef enum _enumDVDAUDIOQUANTISATION
{
    DVDAUDIOQUANTISATION_INVALID,                                       //!< invalid
    DVDAUDIOQUANTISATION_16BPS,                                         //!< 0 = 16 bits per sample
    DVDAUDIOQUANTISATION_20BPS,                                         //!< 1 = 20 bits per sample
    DVDAUDIOQUANTISATION_24BPS,                                         //!< 2 = 24 bits per sample
    DVDAUDIOQUANTISATION_DRC                                            //!< 3 = DRC

} DVDAUDIOQUANTISATION, *LPDVDAUDIOQUANTISATION;

typedef const DVDAUDIOQUANTISATION* LPCDVDAUDIOQUANTISATION;

typedef enum _enumDVDAUDIOCODEEXT
{
    DVDAUDIOCODEEXT_INVALID,                                            //!< invalid
    DVDAUDIOCODEEXT_UNSPECIFIED,                                        //!< 0 = unspecified
    DVDAUDIOCODEEXT_NORMAL,                                             //!< 1 = normal
    DVDAUDIOCODEEXT_VISUALLY_IMPAIRED,                                  //!< 2 = for visually impaired
    DVDAUDIOCODEEXT_DIRECTORS_COMMENTS,                                 //!< 3 = director's comments
    DVDAUDIOCODEEXT_ALT_DIRECTORS_COMMENTS                              //!< 4 = alternate director's comments

} DVDAUDIOCODEEXT, *LPDVDAUDIOCODEEXT;

typedef const DVDAUDIOCODEEXT* LPCDVDAUDIOCODEEXT;

//! Channel assignments (karaoke mode)
/*!
 * Channel assignments (karaoke mode)
 */
typedef enum _enumDVDAUDIOCHANNELASSIGNMENT
{
    DVDAUDIOCHANNELASSIGNMENT_INVALID,                                  //!< invalid
    DVDAUDIOCHANNELASSIGNMENT_2_0,                                      //!< 2 = 2/0 L,R
    DVDAUDIOCHANNELASSIGNMENT_3_0,                                      //!< 3 = 3/0 L,M,R
    DVDAUDIOCHANNELASSIGNMENT_2_1,                                      //!< 4 = 2/1 L,R,V1
    DVDAUDIOCHANNELASSIGNMENT_3_1,                                      //!< 5 = 3/1 L,M,R,V1
    DVDAUDIOCHANNELASSIGNMENT_2_2,                                      //!< 6 = 2/2 L,R,V1,V2
    DVDAUDIOCHANNELASSIGNMENT_3_2                                       //!< 7 = 3/2 L,M,R,V1,V2

} DVDAUDIOCHANNELASSIGNMENT, *LPDVDAUDIOCHANNELASSIGNMENT;

typedef const DVDAUDIOCHANNELASSIGNMENT* LPCDVDAUDIOCHANNELASSIGNMENT;

//! Karaoke mode
/*!
 * Karaoke mode: solo, duet
 */
typedef enum _enumDVDAKARAOKEMODE
{
    DVDKARAOKEMODE_INVALID,                                             //!< invalid
    DVDKARAOKEMODE_SOLO,                                                //!< solo
    DVDKARAOKEMODE_DUET                                                 //!< duet

} DVDAKARAOKEMODE, *LPDVDAKARAOKEMODE;

typedef const DVDAKARAOKEMODE* LPCDVDAKARAOKEMODE;

//! Audio attributes (Menus/Video Titles)
/*!
 * Decoded audio attributes (Menus/Video Titles)
 */
typedef struct _tagDVDAUDIOSTREAM
{
    /*@{*/
    //! Audio Stream Id
    /*!
     * <table><tr>
     * <td>AC3:         </td><td>0x80 + (Stream # - 1)</td>
     * </tr><tr>
     * <td>MPEG1:       </td><td>0xC0 + (Stream # - 1)</td>
     * </tr><tr>
     * <td>MPEG2_EXT:   </td><td>0xC8 + (Stream # - 1)</td>
     * </tr><tr>
     * <td>LPCM:        </td><td>0xA0 + (Stream # - 1)</td>
     * </tr><tr>
     * <td>DTS:         </td><td>0x88 + (Stream # - 1)</td>
     * </tr></table>
     */
    uint16_t                    m_wID;
    DVDAUDIOCODINGMODE          m_eCodingMode;                          //!< Audio coding mode (AC3, MPEG1, MPEG2, LPCM, DTS)
    bool                        m_bMultichannelExtPresent;              //!< True if multichannel extension present
    DVDAUDIOQUANTISATION        m_eQuantisation;                        //!< Quantisation: 16/20/24 bits per sample or DRC
    uint32_t                    m_dwSampleRate;                         //!< Sample rate: 48 Kbps or 96 Kbps
    uint16_t                    m_wChannels;                            //!< Number of channels (1...6)
    /*@}*/
} DVDAUDIOSTREAM, *LPDVDAUDIOSTREAM;

typedef const DVDAUDIOSTREAM* LPCDVDAUDIOSTREAM;

//! Audio attributes (Video Titles only)
/*!
 * Decoded audio attributes (Video Titles only)
 */
typedef struct _tagDVDAUDIOSTREAMEX
{
    //!< If true, language code present
    /*!
     * Not really reliable: even if true, m_szLanguageCode can be empty.
     */
    bool                        m_bLanguageCodePresent;
    char                        m_szLanguageCode[DVD_LANGCODELEN + 1];  //!< 2 byte language code, e.g. de, en
    bool                        m_bCodeExtPresent;                      //!< Code extension present
    //!< Audio Code Extension
    /*!
     * unspecified
     * normal
     * for visually impaired
     * director's comments
     * alternate director's comments
     */
    DVDAUDIOCODEEXT             m_eCodeExt;
    //!< Audio Channel Assignment
    /*!
     * 2/0 L,R
     * 3/0 L,M,R
     * 2/1 L,R,V1
     * 3/1 L,M,R,V1
     * 2/2 L,R,V1,V2
     * 3/2 L,M,R,V1,V2
     */
    DVDAUDIOCHANNELASSIGNMENT   m_eChannelAssignment;
    bool                        m_bMCIntroPresent;                      //!< MC intro present
    uint16_t                    m_wKaraokeVersion;                      //!< Karaoke version
    DVDAUDIOAPPLICATIONMODE     m_eApplicationMode;                     //!< Audio application mode: undefined, karaoke, surround
    DVDAKARAOKEMODE             m_eKaraokeMode;                         //!< Karaoke mode: solo or duet
    // Surround mode
    bool                        m_bSuitableForDolbySurroundDecoding;    //!< Suitable for Dolby surround decoding

} DVDAUDIOSTREAMEX, *LPDVDAUDIOSTREAMEX;

typedef const DVDAUDIOSTREAMEX* LPCDVDAUDIOSTREAMEX;

// -------------------------------------------------------------------------------------------------------

//! Sub-picture language code extension
/*!
 * Sub-picture language code extension
 */
typedef enum
{
    DVDSUBPICCODEEXT_INVALID,                                       //!< invalid
    DVDSUBPICCODEEXT_NOT_SPECIFIED,                                 //!< 0 = not specified
    DVDSUBPICCODEEXT_NORMAL,                                        //!< 1 = normal
    DVDSUBPICCODEEXT_LARGE,                                         //!< 2 = large
    DVDSUBPICCODEEXT_CHILDREN,                                      //!< 3 = children
    DVDSUBPICCODEEXT_NORMAL_CAPTIONS,                               //!< 5 = normal captions
    DVDSUBPICCODEEXT_LARGE_CAPTIONS,                                //!< 6 = large captions
    DVDSUBPICCODEEXT_CHILDRENS_CAPTION,                             //!< 7 = children’s captions
    DVDSUBPICCODEEXT_FORCED,                                        //!< 9 = forced
    DVDSUBPICCODEEXT_DIRECTORS_COMMENTS,                            //!< 13 = director comments
    DVDSUBPICCODEEXT_LARGE_DIRECTORS_COMMENTS,                      //!< 14 = large director comments
    DVDSUBPICCODEEXT_DIRECTORS_COMMENTS_FOR_CHILDREN                //!< 15 = director comments for children

} DVDSUBPICCODEEXT, *LPDVDSUBPICCODEEXT;

typedef const DVDSUBPICCODEEXT* LPCDVDSUBPICCODEEXT;

//! Subpicture attributes
/*!
 * Decoded subpicture attributes
 */
typedef struct _tagDVDSUBPICSTREAM
{
    uint16_t                    m_wID;                                  //!< Subpicture steam ID, 0x20 + (Stream # - 1)
    uint16_t                    m_wCodingMode;                          //!< Subtitle coding mode
    //!< If true, language code present
    /*!
     * Not really reliable: even if true, m_szLanguageCode can be empty.
     */
    bool                        m_bLanguageCodePresent;
    char                        m_szLanguageCode[DVD_LANGCODELEN + 1];  //!< 2 byte language code, e.g. de, en
    bool                        m_bCodeExtPresent;                      //!< Code extension present
    //!< Language code extension
    /*!
     * not specified
     * normal
     * large
     * children
     * normal captions
     * large captions
     * children’s captions
     * forced
     * director comments
     * large director comments
     * director comments for children
     */
    DVDSUBPICCODEEXT            m_eCodeExt;

} DVDSUBPICSTREAM, *LPDVDSUBPICSTREAM;

typedef const DVDSUBPICSTREAM* LPCDVDSUBPICSTREAM;

// -------------------------------------------------------------------------------------------------------

//! Video Manager Header Data
/*!
 * Video Manager Header Data
 */
typedef struct _tagDVDVMGM
{
    uint16_t                    m_wVersionNumberMajor;                      //!< Major version number
    uint16_t                    m_wVersionNumberMinor;                      //!< Minor version number
    bool                        m_bRegionProhibited[8];                     //!< Prohibited regions
    uint16_t                    m_wNumberOfVolumes;                         //!< Number of volumes
    uint16_t                    m_wVolumeNumber;                            //!< Volume number
    uint16_t                    m_wSideID;                                  //!< Side ID
    char                        m_szProviderID[33];                         //!< Provider ID

    DVDVIDEOSTREAM              m_VideoStream;                              //!< Video attributes of VMGM_VOBS
    uint16_t                    m_wAudioStreamCount;                        //!< Number of audio streams in VMGM_VOBS (0 or 1)
    DVDAUDIOSTREAM              m_AudioStream;                              //!< Audio attributes of VMGM_VOBS
    uint16_t                    m_wSubpicStreamCount;                       //!< Number of subpicture streams in VMGM_VOBS (0 or 1)
    DVDSUBPICSTREAM             m_SubpicStream;                             //!< Subpicture attributes of VMGM_VOBS

} DVDVMGM, *LPDVDVMGM;

typedef const DVDVMGM* LPCDVDVMGM;

// -------------------------------------------------------------------------------------------------------

//! Video Title Set Header Data
/*!
 * Video Title Set Header Data
 */
typedef struct _tagDVDVTS
{
    uint16_t                    m_wTitleSetNo;                              //!< Title number
    uint16_t                    m_wVersionNumberMajor;                      //!< Major version number
    uint16_t                    m_wVersionNumberMinor;                      //!< Minor version number
    // uint8_t     m_byVTSCategory[4];                                      //!< TODO VTS category   0=unspecified, 1=Karaoke

    DVDVIDEOSTREAM              m_VideoStreamVTSM;                          //!< Video attributes of VTSM_VOBS
    uint16_t                    m_wAudioStreamCountVTSM;                    //!< Number of audio streams in VTSM_VOBS (0 or 1)
    DVDAUDIOSTREAM              m_AudioStreamVTSM;                          //!< Audio attributes of VTSM_VOBS
    uint16_t                    m_wSubpicStreamCountVTSM;                   //!< Number of subpicture streams in VTSM_VOBS (0 or 1)
    DVDSUBPICSTREAM             m_SubpicStreamVTSM;                         //!< Subpicture attributes of VTSM_VOBS

    DVDVIDEOSTREAM              m_VideoStreamVTS;                           //!< Video attributes of VTS_VOBS
    uint16_t                    m_wAudioStreamCountVTS;                     //!< Number of audio streams in VTS_VOBS (0...8)
    DVDAUDIOSTREAM              m_AudioStreamVTS[DVDVTS_MAXAUDIOSTREAM];    //!< Audio attributes of VTS_VOBS
    DVDAUDIOSTREAMEX            m_AudioStreamExVTS[DVDVTS_MAXAUDIOSTREAM];  //!< Audio attributes extension of VTS_VOBS
    uint16_t                    m_wSubpicStreamCountVTS;                    //!< Number of subpicture streams in VTS_VOBS (0...32)
    DVDSUBPICSTREAM             m_SubpicStreamVTS[DVDVTS_MAXSUBPICSTREAM];  //!< Subpicture attributes of VTS_VOBS

} DVDVTS, *LPDVDVTS;

typedef const DVDVTS* LPCDVDVTS;

// -------------------------------------------------------------------------------------------------------

//! DVD Program Chain (PGC)
/*!
 * A DVD Program Chain (PGC)
 */
typedef struct _tagDVDPGC
{
    uint16_t    m_wTitleSetNo;                                              //!< Owner title number
    uint16_t    m_wProgramChainNo;                                          //!< PGC number

    bool        m_bEntryPGC;                                                //!< entry PGC

    //    uint8_t     m_byParentalManagementMask1;                          //!< 00A 	1		parental management mask
    //    uint8_t     m_byParentalManagementMask2;                          //!< 00B 	1		parental management mask
    //    uint8_t   m_UserOps[4];                                           //!< 008	4		prohibited user ops
    //    uint16_t	m_wPGC_AST_CTL[8];                                      //!< 00C	8*2		Audio Stream Control
    //    uint32_t	m_dwPGC_SPST_CTL[32];                                   //!< 01C	32*4	Subpicture Stream Control
    //    uint16_t	m_wNextPGCN;                                            //!< 09C	2		next PGCN
    //    uint16_t	m_wPrevPGCN;                                            //!< 09E	2		prev PGCN
    //    uint16_t	m_wGroupPGCN;                                           //!< 0A0	2		group PGCN
    //    uint8_t   m_PCPlaybackMode;                                       //!< 0A2	1		PG playback mode, 0=sequential, otherwise bit7 indicates random (0) or shuffle (1), and the program count-1 is in bits6-0 (minimum program count is 2)
    //    uint8_t   m_PFCStillTime;                                         //!< 0A3	1		PGC still time, 255=infinite
    //    uint8_t   m_ColorLookupTable[16*4];                               //!< 0A4	16*4	Color LookUp Table (CLUT) (0, Y, Cr, Cb)
    //    uint16_t	m_wPGCCommandsOffset;                                   //!< 0E4	2		offset within PGC to commands
    //    uint16_t	m_wPGCProgramMapOffset;                                 //!< 0E6	2		offset within PGC to program map
    //    uint16_t	m_wPGCCellPlaybackOffset;                               //!< 0E8	2		offset within PGC to cell playback information table
    //    uint16_t	m_wCellPositionOffset;                                  //!< 0EA	2		offset within PGC to cell position information table

} DVDPGC, *LPDVDPGC;

typedef const DVDPGC* LPCDVDPGC;

// -------------------------------------------------------------------------------------------------------

//! DVD Part of Title (PTT)
/*!
 * A DVD Part of Title (PTT) in VMG
 */
typedef struct _tagDVDPTTVMG
{
    uint16_t    m_wPlaybackType;                                            //!< Playback type
    uint16_t    m_wNumberOfVideoAngles;                                     //!< Number of video angles (1...9)
    uint8_t     m_byParentalMgmMaskVMG;                                     //!< Parental management mask for VMG
    uint8_t     m_byParentalMgmMaskVTS;                                     //!< Parental management mask for VTS
    uint16_t    m_wVideoTitleSetNo;                                         //!< Video title set having this video title
    uint16_t    m_wTitleNo;                                                 //!< Title number within its video title set (VTS)

    uint16_t    m_wRealTitleNo;                                             //!<

} DVDPTTVMG, *LPDVDPTTVMG;

typedef const DVDPTTVMG* LPCDVDPTTVMG;


//! DVD Part of Title (PTT)
/*!
 * A DVD Part of Title (PTT) in VTS. This structure contains the link between
 * the "virtual" chapter and the real "physical" program. For example if a DVD
 * contains one title with two program chains that have one program each, it
 * could have one virtual title with two chapters.
 */
typedef struct _tagDVDPTTVTS
{
    uint16_t    m_wTitleSetNo;                                              //!< Owner title number
    uint16_t    m_wProgramChainNo;                                          //!< (Real) Program Chain (PGCN)
    uint16_t	m_wProgramNo;                                               //!< (Real) Program (PGN)

    uint16_t    m_wPttTitleSetNo;                                           //!< PTT (virtual) title number
    uint16_t    m_wPttChapterNo;                                            //!< PTT (virtual) chapter number

} DVDPTTVTS, *LPDVDPTTVTS;

typedef const DVDPTTVTS* LPCDVDPTTVTS;

// -------------------------------------------------------------------------------------------------------

//! DVD Program Data
/*!
 * DVD Program Data
 */
typedef struct _tagDVDPROGRAM
{
    uint16_t    m_wTitleSetNo;                                              //!< Owner title number
    uint16_t    m_wProgramChainNo;                                          //!< Owner program chain (PGC) number
    uint16_t    m_wProgramNo;                                               //!< Program number

} DVDPROGRAM, *LPDVDPROGRAM;

typedef const DVDPROGRAM* LPCDVDPROGRAM;

// -------------------------------------------------------------------------------------------------------

//! DVD Cell Type
/*!
 * DVD Cell Type
 */
typedef enum
{
    CELLTYPE_INVALID = 0,                                                   //!< invalid
    CELLTYPE_NORMAL = 1,                                                    //!< normal cell
    CELLTYPE_FIRST = 2,                                                     //!< first cell of angle
    CELLTYPE_MIDDLE = 3,                                                    //!< middle cell of angle
    CELLTYPE_LAST = 4                                                       //!< last cell of angle

} CELLTYPE, *LPCELLTYPE;

typedef const CELLTYPE* LPCCELLTYPE;

//! DVD Cell Block Type
/*!
 * DVD Cell Block Type
 */
typedef enum
{
    BLOCKTYPE_INVALID = 0,                                                  //!< invalid
    BLOCKTYPE_NORMAL = 1,                                                   //!< normal cell
    BLOCKTYPE_ANGLE = 2                                                     //!< angle cell

} BLOCKTYPE, *LPBLOCKTYPE;

typedef const BLOCKTYPE* LPCBLOCKTYPE;

//! DVD Cell
/*!
 * DVD Cell
 */
typedef struct _tagDVDCELL
{
    uint16_t    m_wTitleSetNo;                                              //!< Owner title number
    uint16_t    m_wProgramChainNo;                                          //!< Owner PGC number
    uint16_t    m_wProgramNo;                                               //!< Owner program number
    uint16_t    m_wCellNo;                                                  //!< Cell number

    CELLTYPE    m_eCellType;                                                //!< Cell type: normal, first, middle, last
    BLOCKTYPE   m_eBlockType;                                               //!< normal/angle block
    bool        m_bSeamlessMultiplex;                                       //!< seamless playback linked in PCI
    bool        m_bInterleaved;                                             //!< interleaved
    bool        m_bSCRdiscontinuity;                                        //!< STC discontinuity
    bool        m_bSeamlessAngleLinkedInDSI;                                //!< seamless angle linked in DSI
    bool        m_bVOBStillMode;
    bool        m_bStopsTrickPlay;                                          //!< restricted (stops trick play)
    uint16_t    m_wCellStillTime;                                           //!< Cell still time
    uint16_t    m_wCellCommand;                                             //!< Cell command #
    uint64_t    m_qwPlayTime;                                               //!< Cell playback time in ms
    uint16_t    m_wFrameRate;                                               //!< Frame rate 30 fps/25 fps
    uint32_t	m_dwFirstVOBUStartSector;                                   //!< First VOBU Start Sector
    uint32_t	m_dwFirstILVUEndSector;                                     //!< First ILVU End Sector
    uint32_t	m_dwLastVOBUStartSector;                                    //!< Last VOBU Start Sector
    uint32_t	m_dwLastVOBUEndSector;                                      //!< Last VOBU End Sector

    uint16_t    m_wVOBidn;                                                  //!< VOB (Video Object Unit) id
    uint16_t    m_wCELLidn;                                                 //!< Cell id

    uint16_t    m_wNumberOfVOBIds;                                          //!< number of VOB IDs

} DVDCELL, *LPDVDCELL;

typedef const DVDCELL* LPCDVDCELL;

// -------------------------------------------------------------------------------------------------------

//! DVD Unit
/*!
 * DVD Unit
 */
typedef struct _tagDVDUNIT
{
    uint16_t    m_wTitleSetNo;                                              //!< Owner title number
    uint16_t    m_wProgramChainNo;                                          //!< Owner PGC number
    uint16_t    m_wProgramNo;                                               //!< Owner program number
    uint16_t    m_wCellNo;                                                  //!< Owner cell number
    uint16_t    m_wUnitNo;                                                  //!< Cell number

    uint32_t    m_dwStartSector;                                            //!< Unit start sector
    uint32_t    m_dwEndSector;                                              //!< Unit end sector

} DVDUNIT, *LPDVDUNIT;

typedef const DVDUNIT* LPCDVDUNIT;

// -------------------------------------------------------------------------------------------------------

//! DVD file type
/*!
 * DVD file type
 */
typedef enum
{
    DVDFILETYPE_INVALID,                                                    //!< invalid

    DVDFILETYPE_VMG_IFO,                                                    //!< VIDEO_TS.IFO
    DVDFILETYPE_VMG_BUP,                                                    //!< VIDEO_TS.BUP
    DVDFILETYPE_VMG_VOB,                                                    //!< VIDEO_TS.VOB

    DVDFILETYPE_VTS_IFO,                                                    //!< VTS_XX_0.IFO
    DVDFILETYPE_VTS_BUP,                                                    //!< VTS_XX_0.BUP
    DVDFILETYPE_MENU_VOB,                                                   //!< VTS_XX_0.VOB

    DVDFILETYPE_VTS_VOB                                                     //!< VTS_XX_1.VOB to VTS_XX_9.VOB

} DVDFILETYPE, *LPDVDFILETYPE;

typedef const DVDFILETYPE* LPCDVDFILETYPE;

//! DVD file
/*!
 * DVD file
 */
typedef struct _tagDVDFILE
{
    uint16_t    m_wTitleSetNo;                                              //!< Number of owner title set (0...99)

    DVDFILETYPE m_eDvdFileType;                                             //!< File type (IFO, VOB, etc.)
    uint16_t    m_wVobNo;                                                   //!< For VOB files: number (0...9)
    uint32_t    m_dwSize;                                                   //!< Size of file
    time_t      m_Date;                                                     //!< File date

} DVDFILE, *LPDVDFILE;

typedef const DVDFILE* LPCDVDFILE;

// -------------------------------------------------------------------------------------------------------

//! File status
/*!
 * File status information
 */
typedef struct _tagDVDFILESTAT
{
    uint64_t    m_qwFileSize;           //!< total size, in bytes
    time_t      m_FileTime;             //!< time of last modification
    bool        m_bIsDirectory;         //!< is a directory

} DVDFILESTAT, *LPDVDFILESTAT;

typedef const DVDFILESTAT* LPCDVDFILESTAT;

// -------------------------------------------------------------------------------------------------------

typedef enum _enumXMLMODE
{
    XMLMODE_INVALID,                    //!< Invalid query
    XMLMODE_QUERY,                      //!< Query DVD
    XMLMODE_QUERY_RESPONSE,             //!< Response
    XMLMODE_SEARCH,                     //!< Search DVD
    XMLMODE_SEARCH_RESPONSE,            //!< Response
    XMLMODE_SUBMIT,                     //!< Submit DVD
    XMLMODE_SUBMIT_RESPONSE,            //!< Response
    XMLMODE_EXPORT                      //!< Export DVD

} XMLMODE, *LPXMLMODE;

typedef const XMLMODE* LPCXMLMODE;      //!< constant version

typedef enum _enumXMLRESULT
{
    XMLRESULT_SUCCESS,                  //!< Success
    XMLRESULT_NOT_FOUND,                 //!< Query not successful
    XMLRESULT_NOT_IMPLEMENTED,          //!< Not implemented
    XMLRESULT_SQL_ERROR,                //!< SQL error, description see error string
    XMLRESULT_DUPLICATE_SUBMISSION,     //!< DBD already in database
    XMLRESULT_XML_ERROR,                //!< XML error, description see error string
    XMLRESULT_UNSUPPORTED_VERSION       //!< Unsupported XML version

} XMLRESULT, *LPXMLRESULT;

typedef const XMLRESULT* LPCXMLRESULT;  //!< constant version

// -------------------------------------------------------------------------------------------------------

#endif // TYPES_H
