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
  along with this program.  If not, see <http:// www.gnu.org/licenses/>.
*/

/*! \file vts_ifo.h
 *
 *  \brief VTS info file structure
*/

#pragma once

#ifndef VTS_IFO_H
#define VTS_IFO_H

//! Video Title Set IFO file header
/*!
 * Video Title Set IFO file header
 */
typedef struct _tagVTS_IFO
{
#pragma pack(1)
/*@{*/
    uint8_t     m_header[12];                                       //!< Offset 0x000 Size 12	"DVDVIDEO-VTS"
    uint32_t	m_dwLastSectorVTS;                                  //!< Offset 0x00C Size 4	last sector of VMG set (last sector of BUP)
    uint8_t     m_byPadding01[12];                                  //!< Offset 0x010 Size 12
    uint32_t	m_dwLastSectorVMGI;                                 //!< Offset 0x01C Size 4	last sector of IFO
    uint16_t	m_wVersionNumber;                                   //!< Offset 0x020 Size 2	Version number
    uint8_t     m_byVTSCategory01;                                  //!< Offset 0x022 Size 1	VTS category   0 = unspecified, 1 = Karaoke
    uint8_t     m_byVTSCategory02;                                  //!< Offset 0x023 Size 1	VTS category
    uint8_t     m_byVTSCategory03;                                  //!< Offset 0x024 Size 1	VTS category
    uint8_t     m_byVTSCategory04;                                  //!< Offset 0x025 Size 1	VTS category
    int8_t      m_byPadding02[2];                                   //!< Offset 0x026 Size 2
    int8_t      m_byPadding03[2];                                   //!< Offset 0x028 Size 2
    int8_t      m_byPadding04;                                      //!< Offset 0x02A Size 1
    uint8_t     m_byPadding05[19];                                  //!< Offset 0x02B Size 19
    int8_t      m_byPadding06[2];                                   //!< Offset 0x03E Size 2
    int8_t      m_byPadding07[32];                                  //!< Offset 0x040 Size 32
    int8_t      m_byPadding08[8];                                   //!< Offset 0x060 Size 8
    uint8_t     m_byPadding09[24];                                  //!< Offset 0x068 Size 24
    uint32_t	m_dwEndVTS_MAT;                                     //!< Offset 0x080 Size 4	end byte address of VTSI_MAT
    int8_t      m_byPadding10[4];                                   //!< Offset 0x084 Size 4
    uint8_t     m_byPadding11[56];                                  //!< Offset 0x088 Size 56
    uint32_t	m_dwSectorPointerVTSM_VOBS;                         //!< Offset 0x0C0 Size 4	sector pointer to VTSM_VOBS (Menu VOB)
    uint32_t	m_dwSectorPointerVTST_VOBS;                         //!< Offset 0x0C4 Size 4	sector pointer to VTST_VOBS (Title VOB)
    uint32_t	m_dwSectorPointerVTS_PTT_SRPT;                      //!< Offset 0x0C8 Size 4	sector pointer to VTS_PTT_SRPT (table of Titles and Chapters)
    uint32_t	m_dwSectorPointerVTS_PGCI_UT;                       //!< Offset 0x0CC Size 4	sector pointer to VTS_PGCI_UT (Title Program Chain table)
    uint32_t	m_dwSectorPointerVTSM_PGCI_UT;                      //!< Offset 0x0D0 Size 4    Size sector pointer to VTSM_PGCI_UT (Menu Program Chain table)
    uint32_t	m_dwSectorPointerVTS_TMAPT;                         //!< Offset 0x0D4 Size 4    Size sector pointer to VTS_TMAPT (time map)
    uint32_t	m_dwSectorPointerVTSM_C_ADT;                        //!< Offset 0x0D8 Size 4    Size sector pointer to VTSM_C_ADT (menu cell address table)
    uint32_t	m_dwSectorPointerVTSM_VOBU_ADMAP;                   //!< Offset 0x0DC Size 4    Size sector pointer to VTSM_VOBU_ADMAP (menu VOBU address map)
    uint32_t	m_dwSectorPointerVTS_C_ADT;                         //!< Offset 0x0E0 Size 4    Size sector pointer to VTS_C_ADT (title set cell address table)
    uint32_t	m_dwSectorPointerVTS_VOBU_ADMAP;                    //!< Offset 0x0E4 Size 4    Size sector pointer to VTS_VOBU_ADMAP (title set VOBU address map)
    uint8_t     m_byPadding12[24];                                  //!< Offset 0x0E8 Size 24

    uint8_t     m_byVideoStreamVTSM[2];                             //!< Offset 0x100 Size 2	video attributes of VTSM_VOBS
    uint16_t    m_wAudioStreamCountVTSM;                            //!< Offset 0x102 Size 2	number of audio streams in VTSM_VOBS (0 or 1)
    uint8_t     m_byAudioStreamVTSM[8];                             //!< Offset 0x104 Size 8	audio attributes of VTSM_VOBS
    uint8_t     m_byPadding13[56];                                  //!< Offset 0x10C Size 56
    uint8_t     m_byPadding14[16];                                  //!< Offset 0x144 Size 16
    uint16_t	m_wSubpicStreamCountVTSM;                           //!< Offset 0x154 Size 2	number of subpicture streams in VTSM_VOBS (0 or 1)
    uint8_t     m_bySubpicStreamVTSM_VOBS[6];                       //!< Offset 0x156 Size 6	subpicture attributes of VTSM_VOBS
    uint8_t     m_byPadding15[164];                                 //!< Offset 0x15C Size 164

    uint8_t     m_byVideoStreamVTS_VOBS[2];                         //!< Offset 0x200 Size 2	video attributes of VTS_VOBS
    uint16_t    m_wAudioStreamCountVTS;                             //!< Offset 0x202 Size 2	number of audio streams in VTS_VOBS (0...8)
    uint8_t     m_byAudioStreamsVTS_VOBS[DVDVTS_MAXAUDIOSTREAM][8]; //!< Offset 0x204 Size 8*8	audio attributes of VTS_VOBS
    uint8_t     m_byPadding16[16];                                  //!< Offset 0x244 Size 16
    uint16_t    m_wSubpicStreamCountVTS;                            //!< Offset 0x254 Size 2	number of subpicture streams in VTS_VOBS
    uint8_t     m_bySubpicStreamVTS_VOBS[DVDVTS_MAXSUBPICSTREAM][6];//!< Offset 0x256 Size 32*6	subpicture attributes of VTS_VOBS (0...32)
    uint8_t     m_byPadding17[2];                                   //!< Offset 0x316 Size 2

    uint8_t     m_byMultichannelExt[DVDVTS_MAXAUDIOSTREAM][24];     //!< Offset 0x318 Size 8*24	multichannel extension
    uint8_t     m_byPadding18[40];                                  //!< Offset 0x3D8 Size 40
/*@}*/
} VTS_IFO, *LPVTS_IFO;

typedef const VTS_IFO* LPCVTS_IFO;

//! VTS_PGCI_UT_HEADER
/*!
 * The VTS_PGCI contains a list of Program Chains (PGCs), and the PGCs themselves.
 *
 * VTS_PGCI
 * VTS_PGCI
 *      VTS_PGC
 *      :
 *      VTS_PGC
 */
typedef struct _tagVTS_PGCI_UT_HEADER
{
#pragma pack(1)
/*@{*/
    uint16_t	m_wNumberOfProgramChains;                   //!< Offset 0x000 Size 2		number of Program Chains
    uint16_t	m_byPadding01;                              //!< Offset 0x002 Size 2
    uint32_t	m_dwEndAddress;                             //!< Offset 0x004 Size 4		end address (last byte of last PGC) relative to VTS_PGCI
/*@}*/
} VTS_PGCI_UT_HEADER, *LPVTS_PGCI_UT_HEADER;

typedef const VTS_PGCI_UT_HEADER* LPCVTS_PGCI_UT_HEADER;

//! VTS_PGCI_UT
/*!
 *
 */
typedef struct _tagVTS_PGCI_UT
{
#pragma pack(1)
/*@{*/
    uint8_t     m_byTitleNo;                                //!< Offset 0x008 Size 1		bit 7: 1=entry PGC, rest: title number
    uint8_t     m_byPadding02;                              //!< Offset 0x009 Size 1
    uint8_t     m_byParentalManagementMask1;                //!< Offset 0x00A Size 1		parental management mask
    uint8_t     m_byParentalManagementMask2;                //!< Offset 0x00B Size 1		parental management mask
    uint32_t	m_dwOffsetVTS_PGC;                          //!< Offset 0x00C Size 4    	offset to VTS_PGC, relative to VTS_PGCI
/*@}*/
} VTS_PGCI_UT, *LPVTS_PGCI_UT;

typedef const VTS_PGCI_UT* LPCVTS_PGCI_UT;

//! VTS_PGCHEADER
/*!
 * PGC header
 */
typedef struct _tagVTS_PGCHEADER
{
#pragma pack(1)
/*@{*/
    uint8_t     m_byPadding01[2];                           //!< Offset 0x000 Size 2
    uint8_t     m_byNumberOfPrograms;                       //!< Offset 0x002 Size 1		number of programs
    uint8_t     m_byNumberOfCells;                          //!< Offset 0x003 Size 1 		number of cells
    uint8_t     m_byPlaytime[4];                            //!< Offset 0x004 Size 4		playback time, BCD, hh:mm:ss:ff with bits 7&6 of frame (last) byte indicating frame rate 11 = 30 fps, 10 = illegal, 01 = 25 fps, 00 = illegal
    uint8_t     m_UserOps[4];                               //!< Offset 0x008 Size 4		prohibited user ops
    uint16_t	m_wPGC_AST_CTL[8];                          //!< Offset 0x00C Size 8*2		Audio Stream Control
    uint32_t	m_dwPGC_SPST_CTL[32];                       //!< Offset 0x01C Size 32*4		Subpicture Stream Control
    uint16_t	m_wNextPGCN;                                //!< Offset 0x09C Size 2		next PGCN
    uint16_t	m_wPrevPGCN;                                //!< Offset 0x09E Size 2		prev PGCN
    uint16_t	m_wGroupPGCN;                               //!< Offset 0x0A0 Size 2		group PGCN
    uint8_t     m_PCPlaybackMode;                           //!< Offset 0x0A2 Size 1		PG playback mode, 0=sequential, otherwise bit7 indicates random (0) or shuffle (1), and the program count-1 is in bits6-0 (minimum program count is 2)
    uint8_t     m_PFCStillTime;                             //!< Offset 0x0A3 Size 1		PGC still time, 255=infinite
    uint8_t     m_ColorLookupTable[16*4];                   //!< Offset 0x0A4 Size 16*4		Color LookUp Table (CLUT) (0, Y, Cr, Cb)
    uint16_t	m_wPGCCommandsOffset;                       //!< Offset 0x0E4 Size 2		offset within PGC to commands
    uint16_t	m_wPGCProgramMapOffset;                     //!< Offset 0x0E6 Size 2		offset within PGC to program map
    uint16_t	m_wPGCCellPlaybackOffset;                   //!< Offset 0x0E8 Size 2		offset within PGC to cell playback information table
    uint16_t	m_wCellPositionOffset;                      //!< Offset 0x0EA Size 2		offset within PGC to cell position information table
/*@}*/
} VTS_PGCHEADER, *LPVTS_PGCHEADER;

typedef const VTS_PGCHEADER* LPCVTS_PGCHEADER;

//! VTS_PTT_SRPT_HEADER
/*!
 * The VTS_PTT_SRPT contains pointers to the tables for each title,
 * and is indexed by VTS_TTN (title number within VTS)
 */
typedef struct _tagVTS_PTT_SRPT_HEADER
{
#pragma pack(1)
/*@{*/
    uint16_t	m_wNumberOfTitles;                          //!< Offset 0x0		number of titles
    uint8_t     m_byPadding01[2];                           //!< Offset 0x2
    uint32_t	m_dwEndAdress;                              //!< Offset 0x4		end address (last byte of last VTS_PTT)
/*@}*/
} VTS_PTT_SRPT_HEADER, *LPVTS_PTT_SRPT_HEADER;

typedef const VTS_PTT_SRPT_HEADER* LPCVTS_PTT_SRPT_HEADER;

//! VTS_PTT_SRPT
/*!
 *
 */
typedef struct _tagVTS_PTT_SRPT
{
#pragma pack(1)
/*@{*/
    uint32_t	m_dwOffsetToPTT;                            //!< Offset 0x8		offset to PTT for title 1
/*@}*/
} VTS_PTT_SRPT, *LPVTS_PTT_SRPT;

typedef const VTS_PTT_SRPT* LPCVTS_PTT_SRPT;

//! VTS_PTT
/*!
 * The VTS_PTT contains pointers to the Program Chain (PGCN) and Program (PGN)
 * for each Part-of-Title (PTT or Chapter), and is indexed by TT_PTTN (PTT number within title)
 */
typedef struct _tagVTS_PTT
{
#pragma pack(1)
/*@{*/
    uint16_t	m_wProgramChainNo;                          //!< Program Chain (PGCN)
    uint16_t	m_wProgramNo;                               //!< Program (PGN)
/*@}*/
} VTS_PTT, *LPVTS_PTT;

typedef const VTS_PTT* LPCVTS_PTT;

//! VTS_PROGRAMMAP
/*!
 * program map entry (one per program, pad to word boundary with zero)
 */
typedef struct _tagVTS_PROGRAMMAP
{
#pragma pack(1)
/*@{*/
    uint8_t     m_byEntryCell;
/*@}*/
} VTS_PROGRAMMAP, *LPVTS_PROGRAMMAP;

typedef const VTS_PROGRAMMAP* LPCVTS_PROGRAMMAP;

//! VTS_CELLPOS
/*!
 * program map entry (one per program, pad to word boundary with zero)
 */
typedef struct _tagVTS_CELLPOS
{
#pragma pack(1)
/*@{*/
    uint16_t    m_wVOBidn;                                  //!< Offset 0x000 Size 2    VOB (Video Object Unit) id
    uint8_t     m_byPadding01;                              //!< Offset 0x002 Size 2
    uint8_t     m_byCELLidn;                                //!< Offset 0x004 Size 4    Cell id
/*@}*/
} VTS_CELLPOS, *LPVTS_CELLPOS;

typedef const VTS_CELLPOS* LPCVTS_CELLPOS;

//! VTS_PGC
/*!
 * cell playback information table
 */
typedef struct _tagVTS_PGC
{
#pragma pack(1)
/*@{*/
    uint8_t     m_byCellCategory1;                          //!< Offset 0x000 Size 1	cell type 00=normal, 01=first of angle block, 10=middle of angle block, 11=last of angle block 	Block Type 00 = normal, 01 = angle block 	1 = seamless multiplex 	1 = interleaved 	1 = SCR discontinuity 	1 = seamless angle linked in DSI
    uint8_t     m_byCellCategory2;                          //!< Offset 0x001 Size 1	1 = VOBU still mode 	1 = restricted (stops trick play)	Application Cell Type, see below for karaoke meanings
    uint8_t     m_byCellStillTime;                          //!< Offset 0x002 Size 1	cell still time
    uint8_t     m_byCellCommand;                            //!< Offset 0x003 Size 1	cell command #
    uint8_t     m_byPlaytime[4];                            //!< Offset 0x004 Size 4	cell playback time, BCD, hh:mm:ss:ff with bits 7&6 of frame (last) byte indicating frame rate 11 = 30 fps, 10 = illegal, 01 = 25 fps, 00 = illegal
    uint32_t	m_dwFirstVOBUStartSector;                   //!< Offset 0x008 Size 4	first VOBU start sector
    uint32_t	m_dwFirstILVUEndSector;                     //!< Offset 0x00C Size 4	first ILVU end sector
    uint32_t	m_dwLastVOBUStartSector;                    //!< Offset 0x010 Size 4	last VOBU start sector
    uint32_t	m_dwLastVOBUEndSector;                      //!< Offset 0x014 Size 4	last VOBU end sector
/*@}*/
} VTS_PGC, *LPVTS_PGC;

typedef const VTS_PGC* LPCVTS_PGC;

//! VTS_C_ADT_HEADER
/*!
 * VMGM_C_ADT
 * VTSM_C_ADT
 * VTS_C_ADT
 *
 * This table contains pointers to all the cells in the menu or title set.
 */
typedef struct _tagVTS_C_ADT_HEADER
{
#pragma pack(1)
/*@{*/
    uint16_t    m_wNumberOfVOBIds;                          //!< Offset 0x000 Size 2    number of VOB IDs
    uint16_t    m_wReserverd01;                             //!< Offset 0x002 Size 2
    uint32_t    m_dwEndAdress;                              //!< Offset 0x004 Size 4    end address (last byte of last entry)
/*@}*/
} VTS_C_ADT_HEADER, *LPVTS_C_ADT_HEADER;

typedef const VTS_C_ADT_HEADER* LPCVTS_C_ADT_HEADER;

//! VTS_C_ADT
/*!
 *
 */
typedef struct _tagVTS_C_ADT
{
#pragma pack(1)
/*@{*/
    uint16_t    m_wVOBidn;                                  //!< Offset 0x008 Size 2    VOB (Video Object Unit) id
    uint8_t     m_byCELLidn;                                //!< Offset 0x00A Size 1    Cell id
    uint8_t     m_byPadding01;                              //!< Offset 0x00B Size 1
    uint32_t    m_dwStartSector;                            //!< Offset 0x00C Size 4    Starting sector within VOB
    uint32_t    m_dwEndSector;                              //!< Offset 0x010 Size 4    Ending sector within VOB
 /*@}*/
} VTS_C_ADT, *LPVTS_C_ADT;

typedef const VTS_C_ADT* LPCVTS_C_ADT;

#endif // VTS_IFO_H

