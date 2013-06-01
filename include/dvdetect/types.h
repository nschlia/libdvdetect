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

/** \file types.h
 *
 *  \brief Types and defines
 *
 * Types and defines of structures and enumerations for the public interface
 * of libdvdetect. These structurse contain preprocessed data about the DVD
 * (mainly decoded bit field information and big endian values converted to
 * native format).
 */

#pragma once

#ifndef TYPES_H
#define TYPES_H

#if !defined(__cplusplus) && !defined(_MSC_VER)
//#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
//#else
//#define false 0
//#define true 1
//typedef int bool;
//#endif
#include <stdint.h>

#endif

#include <time.h>

#ifndef MAX_PATH
#include <limits.h>
#define MAX_PATH    PATH_MAX
#endif // MAX_PATH

#ifndef DVDETECT_COPYRIGHT
#define DVDETECT_COPYRIGHT "Copyright (C) 2013 Norbert Schlia <nschlia@dvdetect.de>"
#endif // DVDETECT_COPYRIGHT

// -------------------------------------------------------------------------------------------------------

/**
 * Video attributes
 * Coding mode: MPEG1 or MPEG2
 */
typedef enum _tagDVDVIDEOCODINGMODE
{
    DVDVIDEOCODINGMODE_INVALID,                                         /**< invalid */
    DVDVIDEOCODINGMODE_MPEG1,                                           /**< MPEG-1 */
    DVDVIDEOCODINGMODE_MPEG2                                            /**< MPEG-2 */

} DVDVIDEOCODINGMODE, *LPDVDVIDEOCODINGMODE;

typedef const DVDVIDEOCODINGMODE* LPCDVDVIDEOCODINGMODE;

/**
 * TV Standard: PAL or NTSC
 */
typedef enum _tagDVDVIDEOTVSTANDARD
{
    DVDVIDEOTVSTANDARD_INVALID,                                         /**< invalid */
    DVDVIDEOTVSTANDARD_NTSC,                                            /**< NTSC */
    DVDVIDEOTVSTANDARD_PAL                                              /**< PAL */

} DVDVIDEOTVSTANDARD, *LPDVDVIDEOTVSTANDARD;

typedef const DVDVIDEOTVSTANDARD* LPCDVDVIDEOTVSTANDARD;

/**
 * Aspect ratio
 */
typedef enum _tagDVDVIDEOASPECT
{
    DVDVIDEOASPECT_INVALID,                                             /**< invalid */
    DVDVIDEOASPECT_4_3,                                                 /**< 4:3 */
    DVDVIDEOASPECT_16_9                                                 /**< 16:9 */

} DVDVIDEOASPECT, *LPDVDVIDEOASPECT;

typedef const DVDVIDEOASPECT* LPCDVDVIDEOASPECT;

/**
 * Decoded video attributes
 */
typedef struct _tagDVDVIDEOATTRIBUTES
{
    uint8_t             m_byID;                                         /**<  */
    DVDVIDEOCODINGMODE  m_eDvdVideoCodingMode;                          /**<  */
    DVDVIDEOTVSTANDARD  m_eDvdVideoStandard;                            /**<  */
    DVDVIDEOASPECT      m_eDvdVideoAspect;                              /**<  */
    bool                m_bAutomaticPanScanDisallowed;                  /**< Automatic Pan/Scan */
    bool                m_bAutomaticLetterboxDisallowed;                /**< Automatic Letterbox */
    bool                m_bCCForLine21Field1InGOP;                      /**< CC for line 21 field 1 in GOP (NTSC only) */
    bool                m_bCCForLine21Field2InGOP;                      /**< CC for line 21 field 2 in GOP (NTSC only) */
    bool                m_bCBR;                                         /**<  Constant/variable bit rate */
    uint16_t            m_wResolutionX;                                 /**< Resolution NTSC (PAL) */
    uint16_t            m_wResolutionY;                                 /**< 720x480 (720x576), 704x480 (704x576), 352x480 (352x576), 352x240 (352x288) */
    bool                m_bLetterBoxed;                                 /**< false = Full Screen; true = top and bottom cropped (4:3 aspect only) */
    bool                m_bSourceFilm;                                  /**< false = camera, true = film */

} DVDVIDEOATTRIBUTES, *LPDVDVIDEOATTRIBUTES;

typedef const DVDVIDEOATTRIBUTES* LPCDVDVIDEOATTRIBUTES;

// -------------------------------------------------------------------------------------------------------

/**
 * Coding mode
 */
typedef enum _tagDVDAUDIOCODINGMODE
{
    DVDAUDIOCODINGMODE_INVALID,                                         /**< invalid */
    DVDAUDIOCODINGMODE_AC3,                                             /**< 0 AC3 */
    DVDAUDIOCODINGMODE_MPEG1,                                           /**< 2 Mpeg-1 */
    DVDAUDIOCODINGMODE_MPEG2_EXT,                                       /**< 3 Mpeg-2ext */
    DVDAUDIOCODINGMODE_LPCM,                                            /**< 4 LPCM */
    DVDAUDIOCODINGMODE_DTS                                              /**< 6 DTS */

} DVDAUDIOCODINGMODE, *LPDVDAUDIOCODINGMODE;

typedef const DVDAUDIOCODINGMODE* LPCDVDAUDIOCODINGMODE;

/**
 * Application mode
 */
typedef enum _tagDVDAUDIOAPPLICATIONMODE
{
    DVDAUDIOAPPLICATIONMODE_INVALID,                                    /**< invalid */
    DVDAUDIOAPPLICATIONMODE_UNSPECIFIED,                                /**< 0 = unspecified */
    DVDAUDIOAPPLICATIONMODE_KARAOKE,                                    /**< 1 = karaoke */
    DVDAUDIOAPPLICATIONMODE_SURROUND                                    /**< 2 = surround */

} DVDAUDIOAPPLICATIONMODE, *LPDVDAUDIOAPPLICATIONMODE;

typedef const DVDAUDIOAPPLICATIONMODE* LPCDVDAUDIOAPPLICATIONMODE;

/**
 * Quantization/DRC
 */
typedef enum _tagDVDAUDIOQUANTISATION
{
    DVDAUDIOQUANTISATION_INVALID,                                       /**< invalid */
    DVDAUDIOQUANTISATION_16BPS,                                         /**< 0 = 16 bits per sample */
    DVDAUDIOQUANTISATION_20BPS,                                         /**< 1 = 20 bits per sample */
    DVDAUDIOQUANTISATION_24BPS,                                         /**< 2 = 24 bits per sample */
    DVDAUDIOQUANTISATION_DRC                                            /**< 3 = DRC */

} DVDAUDIOQUANTISATION, *LPDVDAUDIOQUANTISATION;

typedef const DVDAUDIOQUANTISATION* LPCDVDAUDIOQUANTISATION;

typedef enum _tagDVDAUDIOCODEEXTENSION
{
    DVDAUDIOCODEEXTENSION_INVALID,                                      /**< invalid */
    DVDAUDIOCODEEXTENSION_UNSPECIFIED,                                  /**< 0 = unspecified */
    DVDAUDIOCODEEXTENSION_NORMAL,                                       /**< 1 = normal */
    DVDAUDIOCODEEXTENSION_VISUALLY_IMPAIRED,                            /**< 2 = for visually impaired */
    DVDAUDIOCODEEXTENSION_DIRECTORS_COMMENTS,                           /**< 3 = director's comments */
    DVDAUDIOCODEEXTENSION_ALT_DIRECTORS_COMMENTS                        /**< 4 = alternate director's comments */

} DVDAUDIOCODEEXTENSION, *LPDVDAUDIOCODEEXTENSION;

typedef const DVDAUDIOCODEEXTENSION* LPCDVDAUDIOCODEEXTENSION;

/**
 * Channel assignments (karaoke mode)
 */
typedef enum _tagDVDAUDIOCHANNELASSIGNMENT
{
    DVDAUDIOCHANNELASSIGNMENT_INVALID,                                  /**< invalid */
    DVDAUDIOCHANNELASSIGNMENT_2_0,                                      /**< 2 = 2/0 L,R */
    DVDAUDIOCHANNELASSIGNMENT_3_0,                                      /**< 3 = 3/0 L,M,R */
    DVDAUDIOCHANNELASSIGNMENT_2_1,                                      /**< 4 = 2/1 L,R,V1 */
    DVDAUDIOCHANNELASSIGNMENT_3_1,                                      /**< 5 = 3/1 L,M,R,V1 */
    DVDAUDIOCHANNELASSIGNMENT_2_2,                                      /**< 6 = 2/2 L,R,V1,V2 */
    DVDAUDIOCHANNELASSIGNMENT_3_2                                       /**< 7 = 3/2 L,M,R,V1,V2 */

} DVDAUDIOCHANNELASSIGNMENT, *LPDVDAUDIOCHANNELASSIGNMENT;

typedef const DVDAUDIOCHANNELASSIGNMENT* LPCDVDAUDIOCHANNELASSIGNMENT;

/**
 * Channel assignments (karaoke mode)
 */
typedef enum _tagDVDAUDIOKARAOKEMODE
{
    DVDKARAOKEMODE_INVALID,                                             /**< invalid */
    DVDKARAOKEMODE_SOLO,
    DVDKARAOKEMODE_DUET

} DVDAKARAOKEMODE, *LPDVDAKARAOKEMODE;

typedef const DVDAKARAOKEMODE* LPCDVDAKARAOKEMODE;

/**
 * Decoded audio attributes
 */
typedef struct _tagDVDAUDIOATTRIBUTESMENU
{
    uint8_t                     m_byID;
    DVDAUDIOCODINGMODE          m_eAudioCodingMode;
    bool                        m_bMultichannelExtensionPresent;
    DVDAUDIOAPPLICATIONMODE     m_eAudioApplicationMode;
    DVDAUDIOQUANTISATION        m_eAudioQuantisation;
    uint32_t                    m_dwSampleRate;
    uint16_t                    m_wChannels;

} DVDAUDIOATTRIBUTESMENU, *LPDVDAUDIOATTRIBUTESMENU;

typedef const DVDAUDIOATTRIBUTESMENU* LPCDVDAUDIOATTRIBUTESMENU;

typedef struct _tagDVDAUDIOATTRIBUTESTITLE
{
    uint8_t                     m_byID;
    DVDAUDIOCODINGMODE          m_eAudioCodingMode;
    bool                        m_bMultichannelExtensionPresent;
    bool                        m_bLanguageTypePresent;
    DVDAUDIOAPPLICATIONMODE     m_eAudioApplicationMode;
    DVDAUDIOQUANTISATION        m_eAudioQuantisation;
    uint32_t                    m_dwSampleRate;
    uint16_t                    m_wChannels;
    char                        m_szLanguageCode[3];
    bool                        m_bLanguageCodeExtension;
    DVDAUDIOCODEEXTENSION       m_eAudioCodeExtension;
    // Application information
    // karaoke mode
    DVDAUDIOCHANNELASSIGNMENT   m_eAudioChannelAssignment;
    uint16_t                    m_wKaraokeVersion;
    bool                        m_bMCIntroPresent;
    DVDAKARAOKEMODE             m_eKaraokeMode;
    // Surround mode
    bool                        m_bSuitableForDolbySurroundDecoding;

} DVDAUDIOATTRIBUTESTITLE, *LPDVDAUDIOATTRIBUTESTITLE;

typedef const DVDAUDIOATTRIBUTESTITLE* LPCDVDAUDIOATTRIBUTESTITLE;

// -------------------------------------------------------------------------------------------------------

/**
 * Sub-picture language code extension
 */
typedef enum _tagDVDSUBPICTURECODEEXT
{
    DVDSUBPICTURECODEEXT_INVALID,                                       /**< invalid */
    DVDSUBPICTURECODEEXT_NOT_SPECIFIED,                                 /**< 0 = not specified */
    DVDSUBPICTURECODEEXT_NORMAL,                                        /**< 1 = normal */
    DVDSUBPICTURECODEEXT_LARGE,                                         /**< 2 = large */
    DVDSUBPICTURECODEEXT_CHILDREN,                                      /**< 3 = children */
    DVDSUBPICTURECODEEXT_NORMAL_CAPTIONS,                               /**< 5 = normal captions */
    DVDSUBPICTURECODEEXT_LARGE_CAPTIONS,                                /**< 6 = large captions */
    DVDSUBPICTURECODEEXT_CHILDRENS_CAPTION,                             /**< 7 = children’s captions */
    DVDSUBPICTURECODEEXT_FORCED,                                        /**< 9 = forced */
    DVDSUBPICTURECODEEXT_DIRECTORS_COMMENTS,                            /**< 13 = director comments */
    DVDSUBPICTURECODEEXT_LARGE_DIRECTORS_COMMENTS,                      /**< 14 = large director comments */
    DVDSUBPICTURECODEEXT_DIRECTORS_COMMENTS_FOR_CHILDREN                /**< 15 = director comments for children */

} DVDSUBPICTURECODEEXT, *LPDVDSUBPICTURECODEEXT;

typedef const DVDSUBPICTURECODEEXT* LPCDVDSUBPICTURECODEEXT;

/**
 * Decoded subpicture attributes
 */
typedef struct _tagDVDSUBPICTUREATTRIBUTES
{
    uint8_t                     m_byID;
    uint16_t                    m_wCodingMode;
    bool                        m_bLanguageTypePresent;
    char                        m_szLanguageCode[3];
    bool                        m_bLanguageCodeExtension;
    DVDSUBPICTURECODEEXT        m_eSubpictureCodeExtension;

} DVDSUBPICTUREATTRIBUTES, *LPDVDSUBPICTUREATTRIBUTES;

typedef const DVDSUBPICTUREATTRIBUTES* LPCDVDSUBPICTUREATTRIBUTES;

// -------------------------------------------------------------------------------------------------------

/**
 * Video Manager Header Data
 */
typedef struct _tagDVDVMGM
{
    uint16_t                    m_wVersionNumberMajor;                      /**< Major version number */
    uint16_t                    m_wVersionNumberMinor;                      /**< Minor version number */
    bool                        m_bRegionProhibited[8];                     /**< Prohibited regions */
    uint16_t                    m_wNumberOfVolumes;                         /**< Number of volumes */
    uint16_t                    m_wVolumeNumber;                            /**< Volume number */
    uint16_t                    m_wSideID;                                  /**< Side ID */
    uint16_t                    m_wNumberOfTitleSets;                       /**< Number of title sets */
    char                        m_szProviderID[33];                         /**< Provider ID */
    // uint8_t                  m_byVMG_POS[8];                             /**< VMG POS ??? */

    DVDVIDEOATTRIBUTES          m_VideoAttributesOfVMGM_VOBS;               /**< Video attributes of VMGM_VOBS */
    // audio attributes of VMGM_VOBS
    uint16_t                    m_wNumberOfAudioStreamsInVMGM_VOBS;         /**< Number of audio streams in VMGM_VOBS (0 or 1) */
    DVDAUDIOATTRIBUTESMENU      m_AudioAttributesOfVMGM_VOBS;               /**< Audio attributes of VMGM_VOBS */
    // subpicture attributes of VMGM_VOBS
    uint16_t                    m_wNumberOfSubpictureStreamsInVMGM_VOBS;    /**< Number of subpicture streams in VMGM_VOBS (0 or 1) */
    DVDSUBPICTUREATTRIBUTES     m_SubpictureAttributesOfVMGM_VOBS;          /**< Subpicture attributes of VMGM_VOBS */

} DVDVMGM, *LPDVDVMGM;

typedef const DVDVMGM* LPCDVDVMGM;

// -------------------------------------------------------------------------------------------------------

/**
 * Video Title Set Header Data
 */
typedef struct _tagDVDVTS
{
    uint16_t                    m_wTitleSetNo;                                 /**< Title number */
    uint16_t                    m_wVersionNumberMajor;                      /**< Major version number */
    uint16_t                    m_wVersionNumberMinor;                      /**< Minor version number */
    // uint8_t     m_byVTSCategory[4];                                      /**< VTS category   0=unspecified, 1=Karaoke */

    DVDVIDEOATTRIBUTES          m_VideoAttributesOfVTSM_VOBS;               /**< Video attributes of VTSM_VOBS */
    // audio attributes of VTSM_VOBS */
    uint16_t                    m_wNumberOfAudioStreamsInVTSM_VOBS;         /**< Number of audio streams in VTSM_VOBS (0 or 1) */
    DVDAUDIOATTRIBUTESMENU      m_AudioAttributesOfVTSM_VOBS;               /**< Audio attributes of VTSM_VOBS */
    // subpicture attributes of VTSM_VOBS
    uint16_t                    m_wNumberOfSubpictureStreamsInVTSM_VOBS;    /**< Number of subpicture streams in VTSM_VOBS (0 or 1) */
    DVDSUBPICTUREATTRIBUTES     m_SubpictureAttributesOfVTSM_VOBS;          /**< Subpicture attributes of VTSM_VOBS */

    DVDVIDEOATTRIBUTES          m_VideoAttributesOfVTS_VOBS;                /**< Video attributes of VTS_VOBS */
    // audio attributes of VTS_VOBS
    uint16_t                    m_wNumberOfAudioStreamsInVTS_VOBS;          /**< Number of audio streams in VTS_VOBS (0...8) */
    DVDAUDIOATTRIBUTESTITLE     m_AudioAttributesOfVTS_VOBS[8];             /**< Audio attributes of VTS_VOBS */
    // subpicture attributes of VTS_VOBS
    uint16_t                    m_wNumberOfSubpictureStreamsInVTS_VOBS;     /**< Number of subpicture streams in VTS_VOBS (0...32) */
    DVDSUBPICTUREATTRIBUTES     m_SubpictureAttributesOfVTS_VOBS[32];       /**< Subpicture attributes of VTS_VOBS */

    uint16_t                    m_wNumberOfProgramChains;                   /**< number of Program Chains */

    // Info from PTT
    uint16_t                    m_wNumberOfTitles;                          /**< number of titles */

} DVDVTS, *LPDVDVTS;

typedef const DVDVTS* LPCDVDVTS;

// -------------------------------------------------------------------------------------------------------

/**
 * DVD Program Chain (PGC)
 */
typedef struct _tagDVDPGC
{
    uint16_t    m_wTitleSetNo;                                                 /**< Owner title number */
    uint16_t    m_wProgramChainNo;                                          /**< PGC number */

    bool        m_bEntryPGC;                                                /**< entry PGC */
    //    uint8_t     m_byParentalManagementMask1;                          /**< 00A 	1		parental management mask */
    //    uint8_t     m_byParentalManagementMask2;                          /**< 00B 	1		parental management mask */
    uint16_t    m_wNumberOfPrograms;                                        /**< number of programs */
    uint16_t    m_wNumberOfCells;                                           /**< number of cells */
    uint64_t    m_qwPlayTime;                                               /**< playback time in ms */
    uint16_t    m_wFrameRate;                                               /**< frame rate 30 fps/25 fps */
    //    uint8_t   m_UserOps[4];                                           /**< 008	4		prohibited user ops */
    //    uint16_t	m_wPGC_AST_CTL[8];                                      /**< 00C	8*2		Audio Stream Control */
    //    uint32_t	m_dwPGC_SPST_CTL[32];                                   /**< 01C	32*4	Subpicture Stream Control */
    //    uint16_t	m_wNextPGCN;                                            /**< 09C	2		next PGCN */
    //    uint16_t	m_wPrevPGCN;                                            /**< 09E	2		prev PGCN */
    //    uint16_t	m_wGroupPGCN;                                           /**< 0A0	2		group PGCN */
    //    uint8_t   m_PCPlaybackMode;                                       /**< 0A2	1		PG playback mode, 0=sequential, otherwise bit7 indicates random (0) or shuffle (1), and the program count-1 is in bits6-0 (minimum program count is 2) */
    //    uint8_t   m_PFCStillTime;                                         /**< 0A3	1		PGC still time, 255=infinite */
    //    uint8_t   m_ColorLookupTable[16*4];                               /**< 0A4	16*4	Color LookUp Table (CLUT) (0, Y, Cr, Cb) */
    //    uint16_t	m_wPGCCommandsOffset;                                   /**< 0E4	2		offset within PGC to commands */
    //    uint16_t	m_wPGCProgramMapOffset;                                 /**< 0E6	2		offset within PGC to program map */
    //    uint16_t	m_wPGCCellPlaybackOffset;                               /**< 0E8	2		offset within PGC to cell playback information table */
    //    uint16_t	m_wCellPositionOffset;                                  /**< 0EA	2		offset within PGC to cell position information table */

} DVDPGC, *LPDVDPGC;

typedef const DVDPGC* LPCDVDPGC;

// -------------------------------------------------------------------------------------------------------

/**
 * DVD Part of Title (PTT)
 */
typedef struct _tagDVDPTT
{
    uint16_t    m_wTitleSetNo;                                                 /**< Owner title number */
    uint16_t    m_wProgramChainNo;                                          /**< Owner PGC number */
    uint16_t    m_wPttNo;                                                   /**< PTT number */

    uint16_t	m_wProgramChain;                                            /**< Program Chain (PGCN) */
    uint16_t	m_wProgram;                                                 /**< Program (PGN) */

} DVDPTT, *LPDVDPTT;

typedef const DVDPTT* LPCDVDPTT;

// -------------------------------------------------------------------------------------------------------

/**
 * DVD Program Data
 */
typedef struct _tagDVDPROGRAM
{
    uint16_t    m_wTitleSetNo;                                                 /**< Owner title number */
    uint16_t    m_wProgramChainNo;                                          /**< Owner PGC number */
    uint16_t    m_wProgramNo;                                               /**< Program number */

    uint16_t    m_wCells;                                                   /**< Number of cells in program */

} DVDPROGRAM, *LPDVDPROGRAM;

typedef const DVDPROGRAM* LPCDVDPROGRAM;

// -------------------------------------------------------------------------------------------------------

typedef enum _tagCELLTYPE
{
    CELLTYPE_INVALID = 0,                                                   /**< invalid */
    CELLTYPE_NORMAL = 1,
    CELLTYPE_FIRST = 2,
    CELLTYPE_MIDDLE = 3,
    CELLTYPE_LAST = 4

} CELLTYPE, *LPCELLTYPE;

typedef const CELLTYPE* LPCCELLTYPE;

typedef enum _tagBLOCKTYPE
{
    BLOCKTYPE_INVALID = 0,                                                  /**< invalid */
    BLOCKTYPE_NORMAL = 1,
    BLOCKTYPE_ANGLE = 2

} BLOCKTYPE, *LPBLOCKTYPE;

typedef const BLOCKTYPE* LPCBLOCKTYPE;

/**
 * DVD Cell
 */
typedef struct _tagDVDCELL
{
    uint16_t    m_wTitleSetNo;                                                 /**< Owner title number */
    uint16_t    m_wProgramChainNo;                                          /**< Owner PGC number */
    uint16_t    m_wProgramNo;                                               /**< Owner program number */
    uint16_t    m_wCell;                                                    /**< Cell number */

    CELLTYPE    m_eCellType;
    BLOCKTYPE   m_eBlockType;
    bool        m_bSeamlessMultiplex;                                       /**< seamless playback linked in PCI */
    bool        m_bInterleaved;                                             /**< interleaved */
    bool        m_bSCRdiscontinuity;                                        /**< STC discontinuity */
    bool        m_bSeamlessAngleLinkedInDSI;                                /**< seamless angle linked in DSI */
    bool        m_bVOBStillMode;
    bool        m_bStopsTrickPlay;                                          /**< restricted (stops trick play) */
    uint16_t    m_wCellStillTime;                                           /**< Cell still time */
    uint16_t    m_wCellCommand;                                             /**< Cell command # */
    uint64_t    m_qwPlayTime;                                               /**< Cell playback time in ms */
    uint16_t    m_wFrameRate;                                               /**< Frame rate 30 fps/25 fps */
    uint32_t	m_dwFirstVOBUStartSector;                                   /**< First VOBU Start Sector */
    uint32_t	m_dwFirstILVUEndSector;                                     /**< First ILVU End Sector */
    uint32_t	m_dwLastVOBUStartSector;                                    /**< Last VOBU Start Sector */
    uint32_t	m_dwLastVOBUEndSector;                                      /**< Last VOBU End Sector */

    uint16_t    m_wVOBidn;                                                  /**< VOB (Video Object Unit) id */
    uint16_t    m_wCELLidn;                                                 /**< Cell id */

    uint16_t    m_wCellPosInfoCount;                                        /**< ??? == number of VOB IDs ??? */
    uint16_t    m_wNumberOfVOBIds;                                          /**< number of VOB IDs */

} DVDCELL, *LPDVDCELL;

typedef const DVDCELL* LPCDVDCELL;

// -------------------------------------------------------------------------------------------------------

/**
 * DVD Unit
 */
typedef struct _tagDVDUNIT
{
    uint16_t    m_wTitleSetNo;                                                 /**< Owner title number */
    uint16_t    m_wProgramChainNo;                                          /**< Owner PGC number */
    uint16_t    m_wProgramNo;                                               /**< Owner program number */
    uint16_t    m_wCell;                                                    /**< Owner cell number */
    uint16_t    m_wUnit;                                                    /**< Cell number */

    uint32_t    m_dwStartSector;
    uint32_t    m_dwEndSector;

} DVDUNIT, *LPDVDUNIT;

typedef const DVDUNIT* LPCDVDUNIT;

// -------------------------------------------------------------------------------------------------------

typedef enum _tagDVDFILETYPE
{
    DVDFILETYPE_INVALID,                                                    /**< invalid */

    DVDFILETYPE_VMG_IFO,                                                    /**< VIDEO_TS.IFO */
    DVDFILETYPE_VMG_BUP,                                                    /**< VIDEO_TS.BUP */
    DVDFILETYPE_VMG_VOB,                                                    /**< VIDEO_TS.VOB */

    DVDFILETYPE_VTS_IFO,                                                    /**< VTS_XX_0.IFO */
    DVDFILETYPE_VTS_BUP,                                                    /**< VTS_XX_0.BUP */
    DVDFILETYPE_MENU_VOB,                                                   /**< VTS_XX_0.VOB */

    DVDFILETYPE_VTS_VOB                                                     /**< VTS_XX_1.VOB to VTS_XX_9.VOB */

} DVDFILETYPE, *LPDVDFILETYPE;

typedef const DVDFILETYPE* LPCDVDFILETYPE;

/**
 * DVD Unit
 */
typedef struct _tagDVDFILE
{
    uint16_t    m_wTitleSetNo;

    DVDFILETYPE m_eDvdFileType;
    uint16_t    m_wVobNo;
    char        m_szFileName[MAX_PATH];
    uint32_t    m_dwSize;
    time_t      m_Date;

} DVDFILE, *LPDVDFILE;

typedef const DVDFILE* LPCDVDFILE;

// -------------------------------------------------------------------------------------------------------

#endif // TYPES_H
