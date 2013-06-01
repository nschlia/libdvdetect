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

/** \file vmg_ifo.h
 *
 *  \brief Video manager info file structure
 */

#pragma once

/**
 * Video Manager IFO file header
 */
typedef struct _tagVMG_IFO
{
#pragma pack(1)
/*@{*/
    uint8_t     m_header[12];                               /**< Offset 0x000 Size 12		"DVDVIDEO-VMG" */
    uint32_t	m_dwLastSectorVMG;                          /**< Offset 0x00C Size 4		last sector of VMG set (last sector of BUP) */
    uint8_t     m_byPadding01[12];                          /**< Offset 0x010 Size 12 */
    uint32_t	m_dwLastSectorVMGI;                         /**< Offset 0x01C Size 4		last sector of IFO */
    uint16_t	m_wVersionNumber;                           /**< Offset 0x020 Size 2		Version number */
    uint8_t     m_byVMGCategory01;                          /**< Offset 0x022 Size 1		VMG category */
    uint8_t     m_byRegionMask;                             /**< Offset 0x023 Size 1		prohibited region mask */
    uint8_t     m_byVMGCategory03;                          /**< Offset 0x024 Size 1		VMG category */
    uint8_t     m_byVMGCategory04;                          /**< Offset 0x025 Size 1		VMG category */
    uint16_t	m_wNumberOfVolumes;                         /**< Offset 0x026 Size 2		Number of volumes */
    uint16_t	m_wVolumeNumber;                            /**< Offset 0x028 Size 2		Volume number */
    uint8_t     m_bySideID;                                 /**< Offset 0x02A Size 1		Side ID */
    uint8_t     m_byPadding02[19];                          /**< Offset 0x02B Size 19 */
    uint16_t	m_wNumberOfTitleSets;                       /**< Offset 0x03E Size 2		Number of title sets */
    uint8_t     m_ProviderID[32];                           /**< Offset 0x040 Size 32		Provider ID */
    uint8_t     m_byVMG_POS[8];                             /**< Offset 0x060 Size 8 		VMG POS */
    uint8_t     m_byPadding03[24];                          /**< Offset 0x068 Size 24 */
    uint32_t	m_dwEndByteVMGI_MAT;                        /**< Offset 0x080 Size 4		end byte address of VMGI_MAT */
    uint32_t	m_dwStartAddressFP_PGC;                     /**< Offset 0x084 Size 4		start address of FP_PGC (First Play program chain) */
    uint8_t     m_byPadding04[56];                          /**< Offset 0x088 Size 56 */
    uint32_t	m_dwSectorPointerVMGM_VOBS;                 /**< Offset 0x0C0 Size 4		start sector of VMGM_VOBS (Menu VOB) */
    uint32_t	m_dwSectorPointerVMG_TT_SRPT;               /**< Offset 0x0C4 Size 4		sector pointer to VMG_TT_SRPT (table of titles) */
    uint32_t	m_dwSectorPointerVMGM_PGCI_UT;              /**< Offset 0x0C8 Size 4		sector pointer to VMGM_PGCI_UT (Menu Program Chain table) */
    uint32_t	m_dwSectorPointerVMG_PTL_MAIT;              /**< Offset 0x0CC Size 4		sector pointer to VMG_PTL_MAIT (Parental Management masks) */
    uint32_t	m_dwSectorPointerVMG_VTS_ATRT;              /**< Offset 0x0D0 Size 4		sector pointer to VMG_VTS_ATRT (copies of VTS audio/sub-picture attributes) */
    uint32_t	m_dwSectorPointerVMG_TXTDT_MG;              /**< Offset 0x0D4 Size 4		sector pointer to VMG_TXTDT_MG (text data) */
    uint32_t	m_dwSectorPointerVMGM_C_ADT;                /**< Offset 0x0D8 Size 4		sector pointer to VMGM_C_ADT (menu cell address table) */
    uint32_t	m_dwSectorPointerVMGM_VOBU_ADMAP;           /**< Offset 0x0DC Size 4		sector pointer to VMGM_VOBU_ADMAP (menu VOBU address map) */
    uint32_t	m_byPadding05;                              /**< Offset 0x0E0 Size 4 */
    uint32_t	m_byPadding06;                              /**< Offset 0x0E4 Size 4 */
    uint8_t     m_byPadding07[24];                          /**< Offset 0x0E8 Size 24 */

    uint8_t     m_byVideoAttributesOfVMGM_VOBS[2];          /**< Offset 0x100 Size 2		video attributes of VMGM_VOBS */
    uint16_t    m_wNumberOfAudioStreamsInVMGM_VOBS;         /**< Offset 0x102 Size 2		number of audio streams in VMGM_VOBS (0 or 1) */
    uint8_t     m_byAudioAttributesOfVMGM_VOBS[8];          /**< Offset 0x104 Size 8		audio attributes of VMGM_VOBS */
    uint8_t     m_byPadding08[56];                          /**< Offset 0x10C Size 56 */
    uint8_t     m_byPadding09[16];                          /**< Offset 0x144 Size 16 */
    uint16_t	m_wNumberOfSubpictureStreamsInVMGM_VOBS;    /**< Offset 0x154 Size 2		number of subpicture streams in VMGM_VOBS (0 or 1) */
    uint8_t     m_bySubpictureAttributesOfVMGM_VOBS[6];     /**< Offset 0x156 Size 6		subpicture attributes of VMGM_VOBS */
    uint8_t     m_byPadding10[164];                         /**< Offset 0x15C Size 164 */

    uint8_t     m_byPadding11[2];                           /**< Offset 0x200 Size 2 */
    uint8_t     m_byPadding12[2];                           /**< Offset 0x202 Size 2 */
    uint8_t     m_byPadding13[8][8];                        /**< Offset 0x204 Size 8*8 */
    uint8_t     m_byPadding14[16];                          /**< Offset 0x244 Size 16 */
    uint8_t     m_byPadding15[2];                           /**< Offset 0x254 Size 2 */
    uint8_t     m_byPadding16[32][6];                       /**< Offset 0x256 Size 32*6 */
    uint8_t     m_byPadding17[2];                           /**< Offset 0x316 Size 2 */
    uint8_t     m_byPadding18[8][24];                       /**< Offset 0x318 Size 8*24 */
    uint8_t     m_byPadding19[40];                          /**< Offset 0x3D8 Size 40 */
 /*@}*/
} VMG_IFO, *LPVMG_IFO;

typedef const VMG_IFO* LPCVMG_IFO;

