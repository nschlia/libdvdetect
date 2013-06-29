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

/*! \file types.h
 *
 *  \brief Types and defines
 *
 * Types and defines of structures and enumerations for the public interface
 * of libdvdetect. These structurse contain preprocessed data about the DVD
 * (mainly decoded bit field information and big endian values converted to
 * native format).
 *
 * <table>
 * <tr><td>COUNTRY</td><td>LANG CODE</td><td>HEX</td><td>DECIMAL</td></tr>
 * <tr><td>Afar </td><td>aa</td><td>6161</td><td>24929</td></tr>
 * <tr><td>Abkhazian</td><td>ab</td><td>6162</td><td>24930</td></tr>
 * <tr><td>Afrikaans</td><td>af</td><td>6166</td><td>24934</td></tr>
 * <tr><td>Albanian</td><td>sq</td><td>7371</td><td>29553</td></tr>
 * <tr><td>Amharic (Ameharic)</td><td>am</td><td>616d</td><td>24941</td></tr>
 * <tr><td>Arabic</td><td>ar</td><td>6172</td><td>24946</td></tr>
 * <tr><td>Armenian</td><td>ia</td><td>6961</td><td>26977</td></tr>
 * <tr><td>Assamese</td><td>as</td><td>6173</td><td>24947</td></tr>
 * <tr><td>Aymara</td><td>ay</td><td>6179</td><td>24953</td></tr>
 * <tr><td>Azerbaijani</td><td>az</td><td>617a</td><td>24954</td></tr>
 * <tr><td>Bashkir</td><td>ba</td><td>6261</td><td>25185</td></tr>
 * <tr><td>Basque</td><td>eu</td><td>6575</td><td>25973</td></tr>
 * <tr><td>Bhutani</td><td>dz</td><td>647a</td><td>25722</td></tr>
 * <tr><td>Bihari</td><td>bh</td><td>6268</td><td>25192</td></tr>
 * <tr><td>Bislama</td><td>bi</td><td>6269</td><td>25193</td></tr>
 * <tr><td>Bengali;Bangla</td><td>bn</td><td>626e</td><td>25198</td></tr>
 * <tr><td>Breton</td><td>br</td><td>6272</td><td>25202</td></tr>
 * <tr><td>Bulgarian</td><td>bg</td><td>6267</td><td>25191</td></tr>
 * <tr><td>Burmese (Myanmar)</td><td>my</td><td>6d79</td><td>28025</td></tr>
 * <tr><td>Byelorussian</td><td>be</td><td>6265</td><td>25189</td></tr>
 * <tr><td>Cambodian</td><td>km</td><td>6b6d</td><td>27501</td></tr>
 * <tr><td>Català (Catalan)</td><td>ca</td><td>6361</td><td>25441</td></tr>
 * <tr><td>Chinese</td><td>zh</td><td>7a68</td><td>31336</td></tr>
 * <tr><td>Corsican</td><td>co</td><td>636f</td><td>25455</td></tr>
 * <tr><td>Czech(Ceske)</td><td>cs</td><td>6373</td><td>25459</td></tr>
 * <tr><td>Dansk (Danish)</td><td>da</td><td>6461</td><td>25697</td></tr>
 * <tr><td>Deutsch</td><td>de</td><td>6465</td><td>25701</td></tr>
 * <tr><td>English</td><td>en</td><td>656e</td><td>25966</td></tr>
 * <tr><td>Esperanto</td><td>eo</td><td>656f</td><td>25967</td></tr>
 * <tr><td>Español</td><td>es</td><td>6573</td><td>25971</td></tr>
 * <tr><td>Estonian</td><td>et</td><td>6574</td><td>25972</td></tr>
 * <tr><td>Finnish</td><td>fi</td><td>6669</td><td>26217</td></tr>
 * <tr><td>Fiji</td><td>fj</td><td>666a</td><td>26218</td></tr>
 * <tr><td>Faroese</td><td>fo</td><td>666f</td><td>26223</td></tr>
 * <tr><td>Français</td><td>fr</td><td>6672</td><td>26226</td></tr>
 * <tr><td>Frisian</td><td>fy</td><td>6679</td><td>26233</td></tr>
 * <tr><td>Galician</td><td>gl</td><td>676c</td><td>26476</td></tr>
 * <tr><td>Georgian</td><td>ka</td><td>6b61</td><td>27489</td></tr>
 * <tr><td>Greek</td><td>el</td><td>656c</td><td>25964</td></tr>
 * <tr><td>Greenlandic</td><td>kl</td><td>6b6c</td><td>27500</td></tr>
 * <tr><td>Guarani</td><td>gn</td><td>676e</td><td>26478</td></tr>
 * <tr><td>Gujarati</td><td>gu</td><td>6775</td><td>26485</td></tr>
 * <tr><td>Hausa</td><td>ha</td><td>6861</td><td>26721</td></tr>
 * <tr><td>Hebrew</td><td>he</td><td>6865</td><td>26725</td></tr>
 * <tr><td>Hebrew</td><td>iw</td><td>6977</td><td>26999</td></tr>
 * <tr><td>Hindi</td><td>hi</td><td>6869</td><td>26729</td></tr>
 * <tr><td>Hrvatski (Croatian)</td><td>hr</td><td>6872</td><td>26738</td></tr>
 * <tr><td>Indonesian</td><td>id</td><td>6964</td><td>26980</td></tr>
 * <tr><td>Indonesian</td><td>in</td><td>696e</td><td>26990</td></tr>
 * <tr><td>Interlingue</td><td>ie</td><td>6965</td><td>26981</td></tr>
 * <tr><td>Inupiak</td><td>ik</td><td>696b</td><td>26987</td></tr>
 * <tr><td>Irish</td><td>ga</td><td>6761</td><td>26465</td></tr>
 * <tr><td>Islenska(Icelandic)</td><td>is</td><td>6973</td><td>26995</td></tr>
 * <tr><td>Italiano (Italian)</td><td>it</td><td>6974</td><td>26996</td></tr>
 * <tr><td>Inuktitut</td><td>iu</td><td>6975</td><td>26997</td></tr>
 * <tr><td>Japanese</td><td>ja</td><td>6a61</td><td>27233</td></tr>
 * <tr><td>Javanese</td><td>jw</td><td>6a77</td><td>27255</td></tr>
 * <tr><td>Kannada</td><td>kn</td><td>6b6e</td><td>27502</td></tr>
 * <tr><td>Kashmiri</td><td>ks</td><td>6b73</td><td>27507</td></tr>
 * <tr><td>Kazakh</td><td>kk</td><td>6b6b</td><td>27499</td></tr>
 * <tr><td>Korean</td><td>ko</td><td>6b6f</td><td>27503</td></tr>
 * <tr><td>Kurdish</td><td>ku</td><td>6b75</td><td>27509</td></tr>
 * <tr><td>Kinyarwanda</td><td>rw</td><td>7277</td><td>29303</td></tr>
 * <tr><td>Kirghiz</td><td>ky</td><td>6b79</td><td>27513</td></tr>
 * <tr><td>Kirundi</td><td>rn</td><td>726e</td><td>29294</td></tr>
 * <tr><td>Latin</td><td>la</td><td>6c61</td><td>27745</td></tr>
 * <tr><td>Lingala</td><td>ln</td><td>6c6e</td><td>27758</td></tr>
 * <tr><td>Laothian</td><td>lo</td><td>6c6f</td><td>27759</td></tr>
 * <tr><td>Lithuanian</td><td>lt</td><td>6c74</td><td>27764</td></tr>
 * <tr><td>Latvian, Lettish</td><td>lv</td><td>6c76</td><td>27766</td></tr>
 * <tr><td>Macedonian</td><td>mk</td><td>6d6b</td><td>28011</td></tr>
 * <tr><td>Magyar</td><td>hu</td><td>6875</td><td>26741</td></tr>
 * <tr><td>Malagasy</td><td>mg</td><td>6d67</td><td>28007</td></tr>
 * <tr><td>Malay</td><td>ms</td><td>6d73</td><td>28019</td></tr>
 * <tr><td>Malayalam</td><td>ml</td><td>6d6c</td><td>28012</td></tr>
 * <tr><td>Maltese</td><td>mt</td><td>6d74</td><td>28020</td></tr>
 * <tr><td>Maori</td><td>mi</td><td>6d69</td><td>28009</td></tr>
 * <tr><td>Marathi</td><td>mr</td><td>6d72</td><td>28018</td></tr>
 * <tr><td>Moldavian</td><td>mo</td><td>6d6f</td><td>28015</td></tr>
 * <tr><td>Mongolian</td><td>mn</td><td>6d6e</td><td>28014</td></tr>
 * <tr><td>Nauru</td><td>na</td><td>6e61</td><td>28257</td></tr>
 * <tr><td>Nederlands</td><td>nl</td><td>6e6c</td><td>28268</td></tr>
 * <tr><td>Nepali</td><td>ne</td><td>6e65</td><td>28261</td></tr>
 * <tr><td>Norsk</td><td>no</td><td>6e6f</td><td>28271</td></tr>
 * <tr><td>Occitan</td><td>oc</td><td>6f63</td><td>28515</td></tr>
 * <tr><td>Oriya</td><td>or</td><td>6f72</td><td>28530</td></tr>
 * <tr><td>Oromo (Afan)</td><td>om</td><td>6f6d</td><td>28525</td></tr>
 * <tr><td>Pashto, Pushto</td><td>ps</td><td>7073</td><td>28787</td></tr>
 * <tr><td>Persian</td><td>fa</td><td>6661</td><td>26209</td></tr>
 * <tr><td>Polish</td><td>pl</td><td>706c</td><td>28780</td></tr>
 * <tr><td>Portugues</td><td>pt</td><td>7074</td><td>28788</td></tr>
 * <tr><td>Punjabi</td><td>pa</td><td>7061</td><td>28769</td></tr>
 * <tr><td>Quechua</td><td>qu</td><td>7175</td><td>29045</td></tr>
 * <tr><td>Rhaeto-Romance</td><td>rm</td><td>726d</td><td>29293</td></tr>
 * <tr><td>Romanian</td><td>ro</td><td>726f</td><td>29295</td></tr>
 * <tr><td>Russian</td><td>ru</td><td>7275</td><td>29301</td></tr>
 * <tr><td>Sangho</td><td>sg</td><td>7367</td><td>29543</td></tr>
 * <tr><td>Samoan</td><td>sm</td><td>736d</td><td>29549</td></tr>
 * <tr><td>Sanskrit</td><td>sa</td><td>7361</td><td>29537</td></tr>
 * <tr><td>Scots Gaelic</td><td>gd</td><td>6764</td><td>26468</td></tr>
 * <tr><td>Serbian</td><td>sr</td><td>7372</td><td>29554</td></tr>
 * <tr><td>Serbo-Croatian</td><td>sh</td><td>7368</td><td>29544</td></tr>
 * <tr><td>Sesotho</td><td>st</td><td>7374</td><td>29556</td></tr>
 * <tr><td>Setswana</td><td>tn</td><td>746e</td><td>29806</td></tr>
 * <tr><td>Shona</td><td>sn</td><td>736e</td><td>29550</td></tr>
 * <tr><td>Sinhalese</td><td>si</td><td>7369</td><td>29545</td></tr>
 * <tr><td>Sindhi</td><td>sd</td><td>7364</td><td>29540</td></tr>
 * <tr><td>Siswati</td><td>ss</td><td>7373</td><td>29555</td></tr>
 * <tr><td>Slovak</td><td>sk</td><td>736b</td><td>29547</td></tr>
 * <tr><td>Slovenian</td><td>sl</td><td>736c</td><td>29548</td></tr>
 * <tr><td>Somali</td><td>so</td><td>736f</td><td>29551</td></tr>
 * <tr><td>Sundanese</td><td>su</td><td>7375</td><td>29557</td></tr>
 * <tr><td>Svenska</td><td>sv</td><td>7376</td><td>29558</td></tr>
 * <tr><td>Swahili</td><td>sw</td><td>7377</td><td>29559</td></tr>
 * <tr><td>Tagalog</td><td>tl</td><td>746c</td><td>29804</td></tr>
 * <tr><td>Tajik</td><td>tg</td><td>7467</td><td>29799</td></tr>
 * <tr><td>Tamil</td><td>ta</td><td>7461</td><td>29793</td></tr>
 * <tr><td>Tatar</td><td>tt</td><td>7474</td><td>29812</td></tr>
 * <tr><td>Telugu</td><td>te</td><td>7465</td><td>29797</td></tr>
 * <tr><td>Thai</td><td>th</td><td>7468</td><td>29800</td></tr>
 * <tr><td>Tibetan</td><td>bo</td><td>626f</td><td>25199</td></tr>
 * <tr><td>Tigrinya</td><td>ti</td><td>7469</td><td>29801</td></tr>
 * <tr><td>Tonga</td><td>to</td><td>746f</td><td>29807</td></tr>
 * <tr><td>Tsonga</td><td>ts</td><td>7473</td><td>29811</td></tr>
 * <tr><td>Turkish</td><td>tr</td><td>7472</td><td>29810</td></tr>
 * <tr><td>Turkmen</td><td>tk</td><td>746b</td><td>29803</td></tr>
 * <tr><td>Twi</td><td>tw</td><td>7477</td><td>29815</td></tr>
 * <tr><td>Uighur</td><td>ug</td><td>7567</td><td>30055</td></tr>
 * <tr><td>Ukrainian</td><td>uk</td><td>756b</td><td>30059</td></tr>
 * <tr><td>Urdu</td><td>ur</td><td>7572</td><td>30066</td></tr>
 * <tr><td>Uzbek</td><td>uz</td><td>757a</td><td>30074</td></tr>
 * <tr><td>Vietnamese</td><td>ui</td><td>7569</td><td>30057</td></tr>
 * <tr><td>Volapuk</td><td>vo</td><td>766f</td><td>30319</td></tr>
 * <tr><td>Welsh</td><td>cy</td><td>6379</td><td>25465</td></tr>
 * <tr><td>Wolof</td><td>wo</td><td>776f</td><td>30575</td></tr>
 * <tr><td>Xhosa</td><td>xh</td><td>7868</td><td>30824</td></tr>
 * <tr><td>Yiddish</td><td>ji</td><td>6a69</td><td>27241</td></tr>
 * <tr><td>Yiddish</td><td>yi</td><td>7969</td><td>31081</td></tr>
 * <tr><td>Yoruba</td><td>yo</td><td>796f</td><td>31087</td></tr>
 * <tr><td>Zhuang</td><td>za</td><td>7a61</td><td>31329</td></tr>
 * <tr><td>Zulu</td><td>zu</td><td>7a75</td><td>31349</td></tr>
 * </table>
 *
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

#ifndef LIBDVDETECT_COPYRIGHT
#define LIBDVDETECT_COPYRIGHT "Copyright (C) 2013 Norbert Schlia <nschlia@dvdetect.de>"
#endif // LIBDVDETECT_COPYRIGHT

#ifndef LIBDVDETECT_LIBRARYNAME
#define LIBDVDETECT_LIBRARYNAME "libdvdetect library"
#endif // LIBDVDETECT_LIBRARYNAME

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

//! Video attributes
/*!
 * Decoded video attributes
 */
typedef struct
{
    uint8_t             m_byID;                                         //!< Video stream ID, fixed to 0xE0
    DVDVIDEOCODINGMODE  m_eCodingMode;                                  //!< Video coding mode (MPEG-1 or MPEG-2)
    DVDVIDEOTVSTANDARD  m_eStandard;                                    //!< Video standard (NTSC, PAL)
    DVDVIDEOASPECT      m_eAspect;                                      //!< Video aspect ratio (16:9, 4:3)
    bool                m_bAutomaticPanScanDisallowed;                  //!< Automatic Pan/Scan
    bool                m_bAutomaticLetterboxDisallowed;                //!< Automatic Letterbox
    bool                m_bCCForLine21Field1InGOP;                      //!< CC for line 21 field 1 in GOP (NTSC only)
    bool                m_bCCForLine21Field2InGOP;                      //!< CC for line 21 field 2 in GOP (NTSC only)
    bool                m_bCBR;                                         //!< Constant/variable bit rate
    uint16_t            m_wResolutionX;                                 //!< Resolution (width): possible values are NTSC (PAL) 720x480 (720x576), 704x480 (704x576), 352x480 (352x576), 352x240 (352x288)
    uint16_t            m_wResolutionY;                                 //!< Resolution (height)
    bool                m_bLetterBoxed;                                 //!< false = Full Screen; true = top and bottom cropped (4:3 aspect only)
    bool                m_bSourceFilm;                                  //!< false = camera, true = film

} DVDVIDEOATTRIBUTES, *LPDVDVIDEOATTRIBUTES;

typedef const DVDVIDEOATTRIBUTES* LPCDVDVIDEOATTRIBUTES;

// -------------------------------------------------------------------------------------------------------

//! Coding mode
/*!
 * Coding mode
 */
typedef enum
{
    DVDAUDIOCODINGMODE_INVALID,                                         //!< invalid
    DVDAUDIOCODINGMODE_AC3,                                             //!< 0 AC3
    DVDAUDIOCODINGMODE_MPEG1,                                           //!< 2 Mpeg-1
    DVDAUDIOCODINGMODE_MPEG2_EXT,                                       //!< 3 Mpeg-2ext
    DVDAUDIOCODINGMODE_LPCM,                                            //!< 4 LPCM
    DVDAUDIOCODINGMODE_DTS                                              //!< 6 DTS

} DVDAUDIOCODINGMODE, *LPDVDAUDIOCODINGMODE;

typedef const DVDAUDIOCODINGMODE* LPCDVDAUDIOCODINGMODE;

//! Application mode
/*!
 * Application mode
 */
typedef enum
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
typedef enum
{
    DVDAUDIOQUANTISATION_INVALID,                                       //!< invalid
    DVDAUDIOQUANTISATION_16BPS,                                         //!< 0 = 16 bits per sample
    DVDAUDIOQUANTISATION_20BPS,                                         //!< 1 = 20 bits per sample
    DVDAUDIOQUANTISATION_24BPS,                                         //!< 2 = 24 bits per sample
    DVDAUDIOQUANTISATION_DRC                                            //!< 3 = DRC

} DVDAUDIOQUANTISATION, *LPDVDAUDIOQUANTISATION;

typedef const DVDAUDIOQUANTISATION* LPCDVDAUDIOQUANTISATION;

typedef enum
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
typedef enum
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
typedef enum
{
    DVDKARAOKEMODE_INVALID,                                             //!< invalid
    DVDKARAOKEMODE_SOLO,                                                //!< solo
    DVDKARAOKEMODE_DUET                                                 //!< duet

} DVDAKARAOKEMODE, *LPDVDAKARAOKEMODE;

typedef const DVDAKARAOKEMODE* LPCDVDAKARAOKEMODE;

//! Audio attributes (Menus)
/*!
 * Decoded audio attributes (Menus)
 */
typedef struct
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
    uint8_t                     m_byID;
    DVDAUDIOCODINGMODE          m_eCodingMode;                          //!< Audio coding mode (AC3, MPEG1, MPEG2, LPCM, DTS)
    bool                        m_bMultichannelExtPresent;              //!< True if multichannel extension present
    DVDAUDIOAPPLICATIONMODE     m_eApplicationMode;                     //!< Audio application mode: undefined, karaoke, surround
    DVDAUDIOQUANTISATION        m_eQuantisation;                        //!< Quantisation: 16/20/24 bits per sample or DRC
    uint32_t                    m_dwSampleRate;                         //!< Sample rate: 48 Kbps or 96 Kbps
    uint16_t                    m_wChannels;                            //!< Number of channels (1...6)
    /*@}*/
} DVDAUDIOATTRIBUTESMENU, *LPDVDAUDIOATTRIBUTESMENU;

typedef const DVDAUDIOATTRIBUTESMENU* LPCDVDAUDIOATTRIBUTESMENU;

//! Audio attributes (Video Title)
/*!
 * Decoded audio attributes (Video Title)
 */
typedef struct
{
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
    uint8_t                     m_byID;
    DVDAUDIOCODINGMODE          m_eCodingMode;                          //!< Audio coding mode (AC3, MPEG1, MPEG2, LPCM, DTS)
    bool                        m_bMultichannelExtPresent;              //!< True if multichannel extension present
    //!< If true, language code present
    /*!
     * Not really reliable: even if true, m_szLanguageCode can be empty.
     */
    bool                        m_bLanguageTypePresent;
    DVDAUDIOAPPLICATIONMODE     m_eApplicationMode;                     //!< Audio application mode: undefined, karaoke, surround
    DVDAUDIOQUANTISATION        m_eQuantisation;                        //!< Quantisation: 16/20/24 bits per sample or DRC
    uint32_t                    m_dwSampleRate;                         //!< Sample rate: 48 Kbps or 96 Kbps
    uint16_t                    m_wChannels;                            //!< Number of channels (1...6)
    char                        m_szLanguageCode[3];                    //!< 2 byte language code, e.g. de, en
    bool                        m_bLanguageCodeExt;                     //!< Language code extension present
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
    uint16_t                    m_wKaraokeVersion;                      //!< Karaoke version
    bool                        m_bMCIntroPresent;                      //!< MC intro present
    DVDAKARAOKEMODE             m_eKaraokeMode;                         //!< Karaoke mode: solo or duet
    // Surround mode
    bool                        m_bSuitableForDolbySurroundDecoding;    //!< Suitable for Dolby surround decoding

} DVDAUDIOATTRIBUTESTITLE, *LPDVDAUDIOATTRIBUTESTITLE;

typedef const DVDAUDIOATTRIBUTESTITLE* LPCDVDAUDIOATTRIBUTESTITLE;

// -------------------------------------------------------------------------------------------------------

//! Sub-picture language code extension
/*!
 * Sub-picture language code extension
 */
typedef enum
{
    DVDSUBPICTURECODEEXT_INVALID,                                       //!< invalid
    DVDSUBPICTURECODEEXT_NOT_SPECIFIED,                                 //!< 0 = not specified
    DVDSUBPICTURECODEEXT_NORMAL,                                        //!< 1 = normal
    DVDSUBPICTURECODEEXT_LARGE,                                         //!< 2 = large
    DVDSUBPICTURECODEEXT_CHILDREN,                                      //!< 3 = children
    DVDSUBPICTURECODEEXT_NORMAL_CAPTIONS,                               //!< 5 = normal captions
    DVDSUBPICTURECODEEXT_LARGE_CAPTIONS,                                //!< 6 = large captions
    DVDSUBPICTURECODEEXT_CHILDRENS_CAPTION,                             //!< 7 = children’s captions
    DVDSUBPICTURECODEEXT_FORCED,                                        //!< 9 = forced
    DVDSUBPICTURECODEEXT_DIRECTORS_COMMENTS,                            //!< 13 = director comments
    DVDSUBPICTURECODEEXT_LARGE_DIRECTORS_COMMENTS,                      //!< 14 = large director comments
    DVDSUBPICTURECODEEXT_DIRECTORS_COMMENTS_FOR_CHILDREN                //!< 15 = director comments for children

} DVDSUBPICTURECODEEXT, *LPDVDSUBPICTURECODEEXT;

typedef const DVDSUBPICTURECODEEXT* LPCDVDSUBPICTURECODEEXT;

//! Subpicture attributes
/*!
 * Decoded subpicture attributes
 */
typedef struct
{
    uint8_t                     m_byID;                                 //!< Subpicture steam ID, 0x20 + (Stream # - 1)
    uint16_t                    m_wCodingMode;                          //!< Coding mode
    //!< If true, language code present
    /*!
     * Not really reliable: even if true, m_szLanguageCode can be empty.
     */
    bool                        m_bLanguageTypePresent;
    char                        m_szLanguageCode[3];                    //!< 2 byte language code, e.g. de, en
    bool                        m_bLanguageCodeExt;                     //!< Language code extension present
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
    DVDSUBPICTURECODEEXT        m_eCodeExt;

} DVDSUBPICTUREATTRIBUTES, *LPDVDSUBPICTUREATTRIBUTES;

typedef const DVDSUBPICTUREATTRIBUTES* LPCDVDSUBPICTUREATTRIBUTES;

// -------------------------------------------------------------------------------------------------------

//! Video Manager Header Data
/*!
 * Video Manager Header Data
 */
typedef struct
{
    uint16_t                    m_wVersionNumberMajor;                      //!< Major version number
    uint16_t                    m_wVersionNumberMinor;                      //!< Minor version number
    bool                        m_bRegionProhibited[8];                     //!< Prohibited regions
    uint16_t                    m_wNumberOfVolumes;                         //!< Number of volumes
    uint16_t                    m_wVolumeNumber;                            //!< Volume number
    uint16_t                    m_wSideID;                                  //!< Side ID
    uint16_t                    m_wNumberOfTitleSets;                       //!< Number of title sets
    char                        m_szProviderID[33];                         //!< Provider ID
    // uint8_t                  m_byVMG_POS[8];                             //!< VMG POS ???

    DVDVIDEOATTRIBUTES          m_VideoAttributesOfVMGM_VOBS;               //!< Video attributes of VMGM_VOBS
    // audio attributes of VMGM_VOBS
    uint16_t                    m_wNumberOfAudioStreamsInVMGM_VOBS;         //!< Number of audio streams in VMGM_VOBS (0 or 1)
    DVDAUDIOATTRIBUTESMENU      m_AudioAttributesOfVMGM_VOBS;               //!< Audio attributes of VMGM_VOBS
    // subpicture attributes of VMGM_VOBS
    uint16_t                    m_wNumberOfSubpictureStreamsInVMGM_VOBS;    //!< Number of subpicture streams in VMGM_VOBS (0 or 1)
    DVDSUBPICTUREATTRIBUTES     m_SubpictureAttributesOfVMGM_VOBS;          //!< Subpicture attributes of VMGM_VOBS

    uint16_t                    m_wPTTNumberOfTitles;                       //!< Number of PTT titles

} DVDVMGM, *LPDVDVMGM;

typedef const DVDVMGM* LPCDVDVMGM;

// -------------------------------------------------------------------------------------------------------

//! Video Title Set Header Data
/*!
 * Video Title Set Header Data
 */
typedef struct
{
    uint16_t                    m_wTitleSetNo;                              //!< Title number
    uint16_t                    m_wVersionNumberMajor;                      //!< Major version number
    uint16_t                    m_wVersionNumberMinor;                      //!< Minor version number
    // uint8_t     m_byVTSCategory[4];                                      //!< VTS category   0=unspecified, 1=Karaoke

    DVDVIDEOATTRIBUTES          m_VideoAttributesOfVTSM_VOBS;               //!< Video attributes of VTSM_VOBS
    // audio attributes of VTSM_VOBS
    uint16_t                    m_wNumberOfAudioStreamsInVTSM_VOBS;         //!< Number of audio streams in VTSM_VOBS (0 or 1)
    DVDAUDIOATTRIBUTESMENU      m_AudioAttributesOfVTSM_VOBS;               //!< Audio attributes of VTSM_VOBS
    // subpicture attributes of VTSM_VOBS
    uint16_t                    m_wNumberOfSubpictureStreamsInVTSM_VOBS;    //!< Number of subpicture streams in VTSM_VOBS (0 or 1)
    DVDSUBPICTUREATTRIBUTES     m_SubpictureAttributesOfVTSM_VOBS;          //!< Subpicture attributes of VTSM_VOBS

    DVDVIDEOATTRIBUTES          m_VideoAttributesOfVTS_VOBS;                //!< Video attributes of VTS_VOBS
    // audio attributes of VTS_VOBS
    uint16_t                    m_wNumberOfAudioStreamsInVTS_VOBS;          //!< Number of audio streams in VTS_VOBS (0...8)
    DVDAUDIOATTRIBUTESTITLE     m_AudioAttributesOfVTS_VOBS[8];             //!< Audio attributes of VTS_VOBS
    // subpicture attributes of VTS_VOBS
    uint16_t                    m_wNumberOfSubpictureStreamsInVTS_VOBS;     //!< Number of subpicture streams in VTS_VOBS (0...32)
    DVDSUBPICTUREATTRIBUTES     m_SubpictureAttributesOfVTS_VOBS[32];       //!< Subpicture attributes of VTS_VOBS

    uint16_t                    m_wNumberOfProgramChains;                   //!< number of Program Chains

} DVDVTS, *LPDVDVTS;

typedef const DVDVTS* LPCDVDVTS;

// -------------------------------------------------------------------------------------------------------

//! DVD Program Chain (PGC)
/*!
 * A DVD Program Chain (PGC)
 */
typedef struct
{
    uint16_t    m_wTitleSetNo;                                              //!< Owner title number
    uint16_t    m_wProgramChainNo;                                          //!< PGC number

    bool        m_bEntryPGC;                                                //!< entry PGC
    //    uint8_t     m_byParentalManagementMask1;                          //!< 00A 	1		parental management mask
    //    uint8_t     m_byParentalManagementMask2;                          //!< 00B 	1		parental management mask
    uint16_t    m_wNumberOfPrograms;                                        //!< number of programs
    uint16_t    m_wNumberOfCells;                                           //!< number of cells
    uint64_t    m_qwPlayTime;                                               //!< playback time in ms
    uint16_t    m_wFrameRate;                                               //!< frame rate 30 fps/25 fps
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
typedef struct
{
    uint8_t     m_byPlaybackType;                                           //!< Playback type
    uint8_t     m_byNumberOfVideoAngles;                                    //!< Number of video angles (1...9)
    uint16_t    m_wNumberOfChapters;                                        //!< Number of chapters
    uint8_t     m_byParentalMgmMaskVMG;                                     //!< Parental management mask for VMG
    uint8_t     m_byParentalMgmMaskVTS;                                     //!< Parental management mask for VTS
    uint8_t     m_byVideoTitleSet;                                          //!< Video title set having this video title
    uint8_t     m_byTitleNumber;                                            //!< Title number within its video title set (VTS)
    uint64_t    m_dwVtsAddress;                                             //!< Video title set address

} DVDPTTVMG, *LPDVDPTTVMG;

typedef const DVDPTTVMG* LPCDVDPTTVMG;


//! DVD Part of Title (PTT)
/*!
 * A DVD Part of Title (PTT) in VTS
 */
typedef struct
{
    uint16_t    m_wTitleSetNo;                                              //!< Owner title number
    uint16_t    m_wPttTitleSetNo;                                           //!< PTT (virtual) title number
    uint16_t    m_wPttNo;                                                   //!< PTT (virtual) chapter number

    uint16_t    m_wProgramChainNo;                                          //!< Program Chain (PGCN)
    uint16_t	m_wProgram;                                                 //!< Program (PGN)

} DVDPTTVTS, *LPDVDPTTVTS;

typedef const DVDPTTVTS* LPCDVDPTTVTS;

// -------------------------------------------------------------------------------------------------------

//! DVD Program Data
/*!
 * DVD Program Data
 */
typedef struct
{
    uint16_t    m_wTitleSetNo;                                              //!< Owner title number
    uint16_t    m_wProgramChainNo;                                          //!< Owner program chain (PGC) number
    uint16_t    m_wProgramNo;                                               //!< Program number

    uint16_t    m_wPTTTitleSetNo;                                           //!< PTT (virtual) title number
    uint16_t    m_wPTTProgramNo;                                            //!< PTT (virtual) program/chapter number

    uint16_t    m_wCells;                                                   //!< Number of cells in program

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
typedef struct
{
    uint16_t    m_wTitleSetNo;                                              //!< Owner title number
    uint16_t    m_wProgramChainNo;                                          //!< Owner PGC number
    uint16_t    m_wProgramNo;                                               //!< Owner program number
    uint16_t    m_wCell;                                                    //!< Cell number

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

    uint16_t    m_wCellPosInfoCount;                                        //!< ??? == number of VOB IDs ???
    uint16_t    m_wNumberOfVOBIds;                                          //!< number of VOB IDs

} DVDCELL, *LPDVDCELL;

typedef const DVDCELL* LPCDVDCELL;

// -------------------------------------------------------------------------------------------------------

//! DVD Unit
/*!
 * DVD Unit
 */
typedef struct
{
    uint16_t    m_wTitleSetNo;                                              //!< Owner title number
    uint16_t    m_wProgramChainNo;                                          //!< Owner PGC number
    uint16_t    m_wProgramNo;                                               //!< Owner program number
    uint16_t    m_wCell;                                                    //!< Owner cell number
    uint16_t    m_wUnit;                                                    //!< Cell number

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
typedef struct
{
    uint16_t    m_wTitleSetNo;                                              //!< Number of owner title set (0...99)

    DVDFILETYPE m_eDvdFileType;                                             //!< File type (IFO, VOB, etc.)
    uint16_t    m_wVobNo;                                                   //!< For VOB files: number (0...9)
    char        m_szFileName[MAX_PATH];                                     //!< Filename without path
    uint32_t    m_dwSize;                                                   //!< Size of file
    time_t      m_Date;                                                     //!< File date

} DVDFILE, *LPDVDFILE;

typedef const DVDFILE* LPCDVDFILE;

// -------------------------------------------------------------------------------------------------------

#endif // TYPES_H
