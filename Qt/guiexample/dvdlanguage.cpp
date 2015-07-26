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

/*! \file 
 *
 *  \brief 
 */
#include "dvdlanguage.h"

#include <QApplication>

const dvdlanguage::LANGUAGETABLE dvdlanguage::m_LanguageTable[] =
{
    { "aa", QT_TR_NOOP("Afar") },
    { "ab", QT_TR_NOOP("Abkhazian") },
    { "ae", QT_TR_NOOP("Avestan") },
    { "af", QT_TR_NOOP("Afrikaans") },
    { "ak", QT_TR_NOOP("Akan") },
    { "am", QT_TR_NOOP("Amharic") },
    { "an", QT_TR_NOOP("Aragonese") },
    { "ar", QT_TR_NOOP("Arabic") },
    { "as", QT_TR_NOOP("Assamese") },
    { "av", QT_TR_NOOP("Avaric") },
    { "ay", QT_TR_NOOP("Aymara") },
    { "az", QT_TR_NOOP("Azerbaijani") },
    { "ba", QT_TR_NOOP("Bashkir") },
    { "be", QT_TR_NOOP("Belarusian") },
    { "bg", QT_TR_NOOP("Bulgarian") },
    { "bh", QT_TR_NOOP("Bihari languages") },
    { "bi", QT_TR_NOOP("Bislama") },
    { "bm", QT_TR_NOOP("Bambara") },
    { "bn", QT_TR_NOOP("Bengali") },
    { "bo", QT_TR_NOOP("Tibetan") },
    { "br", QT_TR_NOOP("Breton") },
    { "bs", QT_TR_NOOP("Bosnian") },
    { "ca", QT_TR_NOOP("Catalan; Valencian") },
    { "ce", QT_TR_NOOP("Chechen") },
    { "ch", QT_TR_NOOP("Chamorro") },
    { "co", QT_TR_NOOP("Corsican") },
    { "cr", QT_TR_NOOP("Cree") },
    { "cs", QT_TR_NOOP("Czech") },
    { "cu", QT_TR_NOOP("Church Slavic; Old Slavonic; Church Slavonic; Old Bulgarian; Old Church Slavonic") },
    { "cv", QT_TR_NOOP("Chuvash") },
    { "cy", QT_TR_NOOP("Welsh") },
    { "da", QT_TR_NOOP("Danish") },
    { "de", QT_TR_NOOP("German") },
    { "dv", QT_TR_NOOP("Divehi; Dhivehi; Maldivian") },
    { "dz", QT_TR_NOOP("Dzongkha") },
    { "ee", QT_TR_NOOP("Ewe") },
    { "el", QT_TR_NOOP("Greek, Modern (1453-)") },
    { "en", QT_TR_NOOP("English") },
    { "eo", QT_TR_NOOP("Esperanto") },
    { "es", QT_TR_NOOP("Spanish; Castilian") },
    { "et", QT_TR_NOOP("Estonian") },
    { "eu", QT_TR_NOOP("Basque") },
    { "fa", QT_TR_NOOP("Persian") },
    { "ff", QT_TR_NOOP("Fulah") },
    { "fi", QT_TR_NOOP("Finnish") },
    { "fj", QT_TR_NOOP("Fijian") },
    { "fo", QT_TR_NOOP("Faroese") },
    { "fr", QT_TR_NOOP("French") },
    { "fy", QT_TR_NOOP("Western Frisian") },
    { "ga", QT_TR_NOOP("Irish") },
    { "gd", QT_TR_NOOP("Gaelic; Scottish Gaelic") },
    { "gl", QT_TR_NOOP("Galician") },
    { "gn", QT_TR_NOOP("Guarani") },
    { "gu", QT_TR_NOOP("Gujarati") },
    { "gv", QT_TR_NOOP("Manx") },
    { "ha", QT_TR_NOOP("Hausa") },
    { "he", QT_TR_NOOP("Hebrew") },
    { "hi", QT_TR_NOOP("Hindi") },
    { "ho", QT_TR_NOOP("Hiri Motu") },
    { "hr", QT_TR_NOOP("Croatian") },
    { "ht", QT_TR_NOOP("Haitian; Haitian Creole") },
    { "hu", QT_TR_NOOP("Hungarian") },
    { "hy", QT_TR_NOOP("Armenian") },
    { "hz", QT_TR_NOOP("Herero") },
    { "ia", QT_TR_NOOP("Interlingua (International Auxiliary Language Association)") },
    { "id", QT_TR_NOOP("Indonesian") },
    { "ie", QT_TR_NOOP("Interlingue; Occidental") },
    { "ig", QT_TR_NOOP("Igbo") },
    { "ii", QT_TR_NOOP("Sichuan Yi; Nuosu") },
    { "ik", QT_TR_NOOP("Inupiaq") },
    { "io", QT_TR_NOOP("Ido") },
    { "is", QT_TR_NOOP("Icelandic") },
    { "it", QT_TR_NOOP("Italian") },
    { "iu", QT_TR_NOOP("Inuktitut") },
    { "ja", QT_TR_NOOP("Japanese") },
    { "jv", QT_TR_NOOP("Javanese") },
    { "ka", QT_TR_NOOP("Georgian") },
    { "kg", QT_TR_NOOP("Kongo") },
    { "ki", QT_TR_NOOP("Kikuyu; Gikuyu") },
    { "kj", QT_TR_NOOP("Kuanyama; Kwanyama") },
    { "kk", QT_TR_NOOP("Kazakh") },
    { "kl", QT_TR_NOOP("Kalaallisut; Greenlandic") },
    { "km", QT_TR_NOOP("Central Khmer") },
    { "kn", QT_TR_NOOP("Kannada") },
    { "ko", QT_TR_NOOP("Korean") },
    { "kr", QT_TR_NOOP("Kanuri") },
    { "ks", QT_TR_NOOP("Kashmiri") },
    { "ku", QT_TR_NOOP("Kurdish") },
    { "kv", QT_TR_NOOP("Komi") },
    { "kw", QT_TR_NOOP("Cornish") },
    { "ky", QT_TR_NOOP("Kirghiz; Kyrgyz") },
    { "la", QT_TR_NOOP("Latin") },
    { "lb", QT_TR_NOOP("Luxembourgish; Letzeburgesch") },
    { "lg", QT_TR_NOOP("Ganda") },
    { "li", QT_TR_NOOP("Limburgan; Limburger; Limburgish") },
    { "ln", QT_TR_NOOP("Lingala") },
    { "lo", QT_TR_NOOP("Lao") },
    { "lt", QT_TR_NOOP("Lithuanian") },
    { "lu", QT_TR_NOOP("Luba-Katanga") },
    { "lv", QT_TR_NOOP("Latvian") },
    { "mg", QT_TR_NOOP("Malagasy") },
    { "mh", QT_TR_NOOP("Marshallese") },
    { "mi", QT_TR_NOOP("Maori") },
    { "mk", QT_TR_NOOP("Macedonian") },
    { "ml", QT_TR_NOOP("Malayalam") },
    { "mn", QT_TR_NOOP("Mongolian") },
    { "mr", QT_TR_NOOP("Marathi") },
    { "ms", QT_TR_NOOP("Malay") },
    { "mt", QT_TR_NOOP("Maltese") },
    { "my", QT_TR_NOOP("Burmese") },
    { "na", QT_TR_NOOP("Nauru") },
    { "nb", QT_TR_NOOP("Bokmål, Norwegian; Norwegian Bokmål") },
    { "nd", QT_TR_NOOP("Ndebele, North; North Ndebele") },
    { "ne", QT_TR_NOOP("Nepali") },
    { "ng", QT_TR_NOOP("Ndonga") },
    { "nl", QT_TR_NOOP("Dutch; Flemish") },
    { "nn", QT_TR_NOOP("Norwegian Nynorsk; Nynorsk, Norwegian") },
    { "no", QT_TR_NOOP("Norwegian") },
    { "nr", QT_TR_NOOP("Ndebele, South; South Ndebele") },
    { "nv", QT_TR_NOOP("Navajo; Navaho") },
    { "ny", QT_TR_NOOP("Chichewa; Chewa; Nyanja") },
    { "oc", QT_TR_NOOP("Occitan (post 1500)") },
    { "oj", QT_TR_NOOP("Ojibwa") },
    { "om", QT_TR_NOOP("Oromo") },
    { "or", QT_TR_NOOP("Oriya") },
    { "os", QT_TR_NOOP("Ossetian; Ossetic") },
    { "pa", QT_TR_NOOP("Panjabi; Punjabi") },
    { "pi", QT_TR_NOOP("Pali") },
    { "pl", QT_TR_NOOP("Polish") },
    { "ps", QT_TR_NOOP("Pushto; Pashto") },
    { "pt", QT_TR_NOOP("Portuguese") },
    { "qu", QT_TR_NOOP("Quechua") },
    { "rm", QT_TR_NOOP("Romansh") },
    { "rn", QT_TR_NOOP("Rundi") },
    { "ro", QT_TR_NOOP("Romanian; Moldavian; Moldovan") },
    { "ru", QT_TR_NOOP("Russian") },
    { "rw", QT_TR_NOOP("Kinyarwanda") },
    { "sa", QT_TR_NOOP("Sanskrit") },
    { "sc", QT_TR_NOOP("Sardinian") },
    { "sd", QT_TR_NOOP("Sindhi") },
    { "se", QT_TR_NOOP("Northern Sami") },
    { "sg", QT_TR_NOOP("Sango") },
    { "si", QT_TR_NOOP("Sinhala; Sinhalese") },
    { "sk", QT_TR_NOOP("Slovak") },
    { "sl", QT_TR_NOOP("Slovenian") },
    { "sm", QT_TR_NOOP("Samoan") },
    { "sn", QT_TR_NOOP("Shona") },
    { "so", QT_TR_NOOP("Somali") },
    { "sq", QT_TR_NOOP("Albanian") },
    { "sr", QT_TR_NOOP("Serbian") },
    { "ss", QT_TR_NOOP("Swati") },
    { "st", QT_TR_NOOP("Sotho, Southern") },
    { "su", QT_TR_NOOP("Sundanese") },
    { "sv", QT_TR_NOOP("Swedish") },
    { "sw", QT_TR_NOOP("Swahili") },
    { "ta", QT_TR_NOOP("Tamil") },
    { "te", QT_TR_NOOP("Telugu") },
    { "tg", QT_TR_NOOP("Tajik") },
    { "th", QT_TR_NOOP("Thai") },
    { "ti", QT_TR_NOOP("Tigrinya") },
    { "tk", QT_TR_NOOP("Turkmen") },
    { "tl", QT_TR_NOOP("Tagalog") },
    { "tn", QT_TR_NOOP("Tswana") },
    { "to", QT_TR_NOOP("Tonga (Tonga Islands)") },
    { "tr", QT_TR_NOOP("Turkish") },
    { "ts", QT_TR_NOOP("Tsonga") },
    { "tt", QT_TR_NOOP("Tatar") },
    { "tw", QT_TR_NOOP("Twi") },
    { "ty", QT_TR_NOOP("Tahitian") },
    { "ug", QT_TR_NOOP("Uighur; Uyghur") },
    { "uk", QT_TR_NOOP("Ukrainian") },
    { "ur", QT_TR_NOOP("Urdu") },
    { "uz", QT_TR_NOOP("Uzbek") },
    { "ve", QT_TR_NOOP("Venda") },
    { "vi", QT_TR_NOOP("Vietnamese") },
    { "vo", QT_TR_NOOP("Volapük") },
    { "wa", QT_TR_NOOP("Walloon") },
    { "wo", QT_TR_NOOP("Wolof") },
    { "xh", QT_TR_NOOP("Xhosa") },
    { "yi", QT_TR_NOOP("Yiddish") },
    { "yo", QT_TR_NOOP("Yoruba") },
    { "za", QT_TR_NOOP("Zhuang; Chuang") },
    { "zh", QT_TR_NOOP("Chinese") },
    { "zu", QT_TR_NOOP("Zulu") }
};

dvdlanguage::dvdlanguage(QObject *parent) :
    QObject(parent)
{
}

QString dvdlanguage::convertISOtoLanguage(const char *pszISO639Code)
{
    for (int i = 0; i < getLanguageCount(); i++)
    {
        if (!strcasecmp(pszISO639Code, m_LanguageTable[i].m_pszISO639Code))
        {
            return getLanguage(i);
        }
    }

    return pszISO639Code;
}

int dvdlanguage::getLanguageCount()
{
    return DVDLANGUAGECOUNT;
}

QString dvdlanguage::getLanguage(int n)
{
    if (n < 0 || n >= DVDLANGUAGECOUNT)
    {
        return "";
    }

    return tr(m_LanguageTable[n].m_pszLanguageName);
}

QString dvdlanguage::getISO639Code(int n)
{
    if (n < 0 || n >= DVDLANGUAGECOUNT)
    {
        return "";
    }

    return tr(m_LanguageTable[n].m_pszISO639Code);
}

