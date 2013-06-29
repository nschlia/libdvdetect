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

/*! \file xmldoc.cpp
 *
 *  \brief
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>
#include <tinyxml.h>

#include "xmldoc.h"

#include "localutils.h"

std::string saveget(const char * psz)
{
    return (psz != NULL ? psz : "");
}

xmldoc::xmldoc(const std::string &strClientName)
    : m_strClientName(strClientName)
{
}

xmldoc::~xmldoc()
{
}

int xmldoc::getResponse(TiXmlHandle hRoot)
{
    TiXmlElement* pResponse = hRoot.FirstChild("response").Element();
    int nResult = 0;

    if (pResponse != NULL)
    {
        TiXmlElement* pResponseText = pResponse->FirstChildElement("ResponseText");
        if (pResponseText != NULL)
        {
            const char *pszResponseText = pResponseText->GetText();
            if (pszResponseText)
            {
                m_strErrorString = pszResponseText;
            }
        }

        pResponse->QueryIntAttribute("Result", &nResult);
    }

    return nResult;
}

int xmldoc::parseXmlResponse(const std::string & strXMLIn, XMLMODE & eXmlModeOut)
{
    TiXmlDocument	doc;
    int res = 0;

    try
    {
        doc.Parse(strXMLIn.c_str(), NULL, TIXML_ENCODING_UTF8);

        if (doc.Error())
        {
            m_strErrorString = doc.ErrorDesc();
            throw -1;
        }
        else
        {
            TiXmlHandle		hDoc(&doc);
            TiXmlElement*	pElem = NULL;
            TiXmlHandle		hRoot = 0;

            pElem = hDoc.FirstChildElement().Element();
            // should always have a valid root but handle gracefully if it does not
            if (pElem == NULL)
            {
                throw -1;
            }

            int n = 0;
            pElem->QueryIntAttribute("XmlMode", &n);
            eXmlModeOut = (XMLMODE)n;

            // const char *	pXmlVersion = NULL;
            // const char *	pszLibraryVersion = NULL;
            // const char *	pszLibraryName = NULL;
            // const char *	pszClientName = NULL;

            // pXmlVersion         = pElem->Attribute("XmlVersion");
            // pszLibraryVersion	= pElem->Attribute("LibraryVersion");
            // pszLibraryName      = pElem->Attribute("LibraryName");
            // pszClientName       = pElem->Attribute("ClientName");

            hRoot  =  TiXmlHandle(pElem);

            res = getResponse(hRoot);
        }
    }
    catch (int _res)
    {
        res = _res;
    }
    return res;
}

int xmldoc::parseXml(const std::string & strXMLIn, XMLMODE & eXmlModeOut, dvdparselst & dvdParseLstOut)
{
    TiXmlDocument	doc;
    int res = 0;

    dvdParseLstOut.clear();

    try
    {
        doc.Parse(strXMLIn.c_str(), NULL, TIXML_ENCODING_UTF8);

        if (doc.Error())
        {
            m_strErrorString = doc.ErrorDesc();
            throw -1;
        }
        else
        {
            TiXmlHandle		hDoc(&doc);
            TiXmlElement*	pElem = NULL;
            TiXmlHandle		hRoot = 0;

            pElem = hDoc.FirstChildElement().Element();
            // should always have a valid root but handle gracefully if it does not
            if (pElem == NULL)
            {
                throw -1;
            }

            int n = 0;
            pElem->QueryIntAttribute("XmlMode", &n);
            eXmlModeOut = (XMLMODE)n;

            // const char *	pXmlVersion = NULL;
            // const char *	pszLibraryVersion = NULL;
            // const char *	pszLibraryName = NULL;
            // const char *	pszClientName = NULL;

            // pXmlVersion          = pElem->Attribute("XmlVersion");
            // pszLibraryVersion    = pElem->Attribute("LibraryVersion");
            // pszLibraryName       = pElem->Attribute("LibraryName");
            // pszClientName        = pElem->Attribute("ClientName");

            // save this for later
            hRoot  =  TiXmlHandle(pElem);

            res = getResponse(hRoot);
            if (res)
            {
                throw res;
            }

            TiXmlElement* pDVD = hRoot.FirstChild("DVD").Element();

            for (; pDVD != NULL; pDVD = pDVD->NextSiblingElement())
            {
                dvdparse dvdParse;

                //dvdParse.setHash(saveget(pDVD->Attribute("Hash")));
                dvdParse.setAlbumArtist(saveget(pDVD->Attribute("AlbumArtist")));
                dvdParse.setAlbum(saveget(pDVD->Attribute("Album")));
                dvdParse.setGenre(saveget(pDVD->Attribute("Genre")));
                dvdParse.setCast(saveget(pDVD->Attribute("Cast")));
                dvdParse.setCrew(saveget(pDVD->Attribute("Crew")));
                dvdParse.setDirector(saveget(pDVD->Attribute("Director")));
                dvdParse.setCountry(saveget(pDVD->Attribute("Country")));
                dvdParse.setReleaseDate(saveget(pDVD->Attribute("ReleaseDate")));
                dvdParse.setSpecialFeatures(saveget(pDVD->Attribute("SpecialFeatures")));
                dvdParse.setEAN_UPC(saveget(pDVD->Attribute("EAN_UPC")));
                dvdParse.setStoryline(saveget(pDVD->Attribute("Storyline")));
                dvdParse.setRemarks(saveget(pDVD->Attribute("Remarks")));
                int nRevision = 0;
                pDVD->QueryIntAttribute("Revision", &nRevision);
                dvdParse.setRevision(nRevision);
                dvdParse.setSubmitter(saveget(pDVD->Attribute("Submitter")));
                dvdParse.setSubmitterIP(saveget(pDVD->Attribute("SubmitterIP")));
                dvdParse.setClient(saveget(pDVD->Attribute("Client")));
                dvdParse.setDateCreated(saveget(pDVD->Attribute("DateCreated")));
                dvdParse.setDateLastChanged(saveget(pDVD->Attribute("DateLastChanged")));
                dvdParse.setKeywords(saveget(pDVD->Attribute("Keywords")));

                //const char *	pText = NULL;
                // pText        = pDVD->GetText();

                TiXmlElement* pTitle = pDVD->FirstChildElement("title");
                for (; pTitle != NULL; pTitle = pTitle->NextSiblingElement())
                {
                    dvdpttvmg dvdTitle(NULL);

                    int             nTitleSetNo = 0;
                    int             nAngles = 0;

                    pTitle->QueryIntAttribute("TitleSetNo", &nTitleSetNo);
                    pTitle->QueryIntAttribute("Angles", &nAngles);

                    dvdTitle.setTitle(saveget(pTitle->Attribute("Title")));
                    dvdTitle.m_DVDPTTVMG.m_byVideoTitleSet  = nTitleSetNo;
                    //dvdTitle.m_DVDPTTVMG.m_wAngles     = nAngles;

                    TiXmlElement* pChapter = pTitle->FirstChildElement("chapter");
                    for (; pChapter != NULL; pChapter = pChapter->NextSiblingElement())
                    {
                        dvdpttvts dvdChapter(NULL);

                        int             nNumber = 0;
                        //uint64_t        qwPlayTime = 0;

                        pChapter->QueryIntAttribute("Number", &nNumber);
                        //pChapter->QueryIntAttribute("PlayTime", &nPlayTime);

                        dvdChapter.setArtist(saveget(pChapter->Attribute("Artist")));
                        dvdChapter.setTitle(saveget(pChapter->Attribute("Title")));

                        dvdChapter.m_DVDPTTVTS.m_wPttNo             = nNumber;
                        dvdChapter.m_DVDPTTVTS.m_wProgramChainNo    = 1;
                        //dvdChapter.m_DVDPTTVTS.m_qwPlayTime        = qwPlayTime;

                        dvdTitle.m_dvdPttVtsLst.push_back(dvdChapter);
                    }
                    dvdParse.m_dvdPttVmgLst.push_back(dvdTitle);
                }
                dvdParseLstOut.push_back(dvdParse);
            }
        }
    }
    catch (int _res)
    {
        res = _res;
    }
    return res;
}

int xmldoc::buildXml(std::string & strXMLOut, XMLMODE eXmlModeIn, const dvdparselst & dvdParseLstIn, const std::string & strSearchIn /*= ""*/)
{
    TiXmlDocument doc;
    int res = 0;

    strXMLOut.empty();

    try
    {
        TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
        doc.LinkEndChild( decl );

        TiXmlElement* root = new TiXmlElement("xml");
        doc.LinkEndChild(root);

        root->SetAttribute("XmlMode", (int)(eXmlModeIn));
        root->SetAttribute("XmlVersion", LIBDVDETECT_PROTOCOL_VERSION);
        root->SetAttribute("LibraryVersion", LIBDVDETECT_VERSION);
        root->SetAttribute("LibraryName", LIBDVDETECT_LIBRARYNAME);

        saveSetAttribute(root, "ClientName", m_strClientName);

        TiXmlComment* comment = new TiXmlComment();
        comment->SetValue(" Query for libdvdetect ");
        root->LinkEndChild(comment);

        if (strSearchIn.length())
        {
            TiXmlElement * search = new TiXmlElement("Search");

            search->LinkEndChild(new TiXmlText(strSearchIn));
            root->LinkEndChild(search);
        }

        {
            TiXmlElement * dvd = new TiXmlElement("DVD");

            dvd->SetAttribute("Hash", dvdParseLstIn[0].getHash());
            //dvd->SetAttribute("Code", dvdParseLst[0].getCode());

            saveSetAttribute(dvd, "Album", dvdParseLstIn[0].getAlbum());
            saveSetAttribute(dvd, "AlbumArtist", dvdParseLstIn[0].getAlbumArtist());
            saveSetAttribute(dvd, "Genre", dvdParseLstIn[0].getGenre());

            // if (!dvdParseLstIn[0].getCover())
            // {
            //  dvd->SetAttribute("Cover", dvdParseLstIn[0].getCover());
            // }

            saveSetAttribute(dvd, "Cast", dvdParseLstIn[0].getCast());
            saveSetAttribute(dvd, "Crew", dvdParseLstIn[0].getCrew());
            saveSetAttribute(dvd, "Director", dvdParseLstIn[0].getDirector());
            saveSetAttribute(dvd, "Country", dvdParseLstIn[0].getCountry());
            saveSetAttribute(dvd, "ReleaseDate", dvdParseLstIn[0].getReleaseDate());
            saveSetAttribute(dvd, "SpecialFeatures", dvdParseLstIn[0].getSpecialFeatures());
            saveSetAttribute(dvd, "EAN_UPC", dvdParseLstIn[0].getEAN_UPC());
            saveSetAttribute(dvd, "Storyline", dvdParseLstIn[0].getStoryline());
            saveSetAttribute(dvd, "Remarks", dvdParseLstIn[0].getRemarks());
            saveSetAttribute(dvd, "Submitter", dvdParseLstIn[0].getSubmitter());
            saveSetAttribute(dvd, "Keywords", dvdParseLstIn[0].getKeywords());
            saveSetAttribute(dvd, "Client", m_strClientName);

            {
                LPCDVDVMGM pDVDVMGM = dvdParseLstIn[0].getDVDVMGM();
                for (uint16_t wTitleSetNo = 1; wTitleSetNo <= pDVDVMGM->m_wPTTNumberOfTitles; wTitleSetNo++)
                {
                    const dvdpttvmg *pDvdPttVmg = dvdParseLstIn[0].getDvdPttVmg(wTitleSetNo);
                    LPCDVDPTTVMG pDVDPTTVMG = pDvdPttVmg->getDVDPTTVMG();

                    TiXmlElement * title = new TiXmlElement("title");

                    title->SetAttribute("TitleSetNo", wTitleSetNo);

                    saveSetAttribute(title,"Title", pDvdPttVmg->getTitle());

                    if (eXmlModeIn == XMLMODE_SUBMIT_VERBOSE)
                    {
                        const dvdtitle * pDvdTitle = pDvdPttVmg->getDvdTitle();
                        LPCDVDVTS pDVDVTS = pDvdTitle->getDVDVTS();

                        title->SetAttribute("Angles", (int)(pDVDPTTVMG->m_byNumberOfVideoAngles));

                        std::string strVideoCodingMode = getVideoCodingMode(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_eCodingMode);
                        std::string strVideoStandard = getVideoStandard(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_eStandard);
                        std::string strVideoAspect = getVideoAspect(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_eAspect);

                        saveSetAttribute(title, "VideoCodingMode", strVideoCodingMode);
                        saveSetAttribute(title, "VideoStandard", strVideoStandard);
                        saveSetAttribute(title, "VideoAspect", strVideoAspect);

                        title->SetAttribute("Resolution", ::toString(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_wResolutionX) + "x" + ::toString(pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_wResolutionY));
                        title->SetAttribute("LetterBoxed", pDVDVTS->m_VideoAttributesOfVTS_VOBS.m_bLetterBoxed);

                        for (uint16_t wAudioStream = 1; wAudioStream <= pDVDVTS->m_wNumberOfAudioStreamsInVTS_VOBS; wAudioStream++)
                        {
                            TiXmlElement * audiostream = new TiXmlElement("audiostream");

                            audiostream->SetAttribute("Number", wAudioStream);

                            audiostream->SetAttribute("Quantisation", (int)(pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_eQuantisation));
                            if (pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_bLanguageTypePresent)
                            {
                                audiostream->SetAttribute("Language", getLanguage(pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_szLanguageCode));
                            }
                            audiostream->SetAttribute("Coding", getAudioCodingMode(pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_eCodingMode));
                            audiostream->SetAttribute("Channels", pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_wChannels);
                            audiostream->SetAttribute("SampleRate", pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_dwSampleRate);
                            audiostream->SetAttribute("CodeExt", (int)(pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream  - 1].m_eCodeExt));
                            audiostream->SetAttribute("Quantisation", getAudioQuantisation(pDVDVTS->m_AudioAttributesOfVTS_VOBS[wAudioStream - 1].m_eQuantisation));

                            title->LinkEndChild(audiostream);
                        }

                        for (uint16_t wSubpictureStream = 1; wSubpictureStream <= pDVDVTS->m_wNumberOfSubpictureStreamsInVTS_VOBS; wSubpictureStream++)
                        {
                            TiXmlElement * subpicturestream = new TiXmlElement("subpicturestream");

                            subpicturestream->SetAttribute("Number", wSubpictureStream);
                            if (pDVDVTS->m_SubpictureAttributesOfVTS_VOBS[wSubpictureStream - 1].m_bLanguageTypePresent)
                            {
                                subpicturestream->SetAttribute("Language", getLanguage(pDVDVTS->m_SubpictureAttributesOfVTS_VOBS[wSubpictureStream - 1].m_szLanguageCode));
                            }
                            subpicturestream->SetAttribute("CodeExt", (int)(pDVDVTS->m_SubpictureAttributesOfVTS_VOBS[wSubpictureStream - 1].m_eCodeExt));

                            title->LinkEndChild(subpicturestream);
                        }
                    }

                    for (uint16_t wPtt = 1; wPtt <= pDvdPttVmg->getPttCount(); wPtt++)
                    {
                        const dvdpttvts * pDvdPttVts = pDvdPttVmg->getDvdPttVts(wPtt);

                        TiXmlElement* chapter = new TiXmlElement("chapter");

                        chapter->SetAttribute("Number", wPtt);
                        chapter->SetAttribute("PlayTime", pDvdPttVts->getPlayTime());
                        saveSetAttribute(chapter,"Artist", pDvdPttVts->getArtist());
                        saveSetAttribute(chapter,"Title", pDvdPttVts->getTitle());

                        title->LinkEndChild(chapter);
                    }
                    dvd->LinkEndChild(title);
                }
                root->LinkEndChild(dvd);
            }
        }

        TiXmlPrinter printer;
        doc.Accept(&printer);

        if (doc.Error())
        {
            m_strErrorString = doc.ErrorDesc();
            throw -1;
        }

        strXMLOut = printer.CStr();
    }
    catch (int _res)
    {
        res = _res;
    }
    return res;
}

int xmldoc::query(std::string & strXML, const dvdparse & dvdParseSearch)
{
    dvdparselst dvdParseLst;

    dvdParseLst.push_back(dvdParseSearch);

    return buildXml(strXML, XMLMODE_QUERY, dvdParseLst);
}

int xmldoc::search(std::string & strXML, dvdparselst & dvdParseLstResponse, const std::string & strSearch)
{
    return buildXml(strXML, XMLMODE_SEARCH, dvdParseLstResponse, strSearch);
}

int xmldoc::submit(std::string & strXML, const dvdparselst & dvdParseLst, bool bVerbose)
{
    return buildXml(strXML, bVerbose ? XMLMODE_SUBMIT_VERBOSE : XMLMODE_SUBMIT_CONDENSED, dvdParseLst);
}

std::string xmldoc::getErrorString() const
{
    return m_strErrorString;
}

void xmldoc::setClientName(const std::string & strClientName)
{
    m_strClientName = strClientName;
}

std::string xmldoc::getClientName() const
{
    return m_strClientName;
}

std::string xmldoc::getVideoCodingMode(DVDVIDEOCODINGMODE eCodingMode) const
{
    std::string strVideoCodingMode;

    switch (eCodingMode)
    {
    case DVDVIDEOCODINGMODE_MPEG1:
        strVideoCodingMode = "MPEG-1";
        break;
    case DVDVIDEOCODINGMODE_MPEG2:
        strVideoCodingMode = "MPEG-2";
        break;
    default:
        break;
    }

    return strVideoCodingMode;
}

std::string xmldoc::getVideoStandard(DVDVIDEOTVSTANDARD eStandard) const
{
    std::string strVideoStandard;

    switch (eStandard)
    {
    case DVDVIDEOTVSTANDARD_NTSC:
        strVideoStandard = "NTSC";
        break;
    case DVDVIDEOTVSTANDARD_PAL:
        strVideoStandard = "PAL";
        break;
    default:
        break;
    }

    return strVideoStandard;
}

std::string xmldoc::getVideoAspect(DVDVIDEOASPECT eAspect) const
{
    std::string strVideoAspect;

    switch (eAspect)
    {
    case DVDVIDEOASPECT_4_3:
        strVideoAspect = "4:3";
        break;
    case DVDVIDEOASPECT_16_9:
        strVideoAspect = "16:9";
        break;
    default:
        break;
    }

    return strVideoAspect;
}

std::string xmldoc::getAudioCodingMode(DVDAUDIOCODINGMODE eAudioCodingMode) const
{
    std::string strCodingMode;

    switch (eAudioCodingMode)
    {
    case DVDAUDIOCODINGMODE_AC3:
        strCodingMode = "AC3";
        break;
    case DVDAUDIOCODINGMODE_MPEG1:
        strCodingMode = "MPEG-1";
        break;
    case DVDAUDIOCODINGMODE_MPEG2_EXT:
        strCodingMode = "MPEG-2";
        break;
    case DVDAUDIOCODINGMODE_LPCM:
        strCodingMode = "LPCM";
        break;
    case DVDAUDIOCODINGMODE_DTS:
        strCodingMode = "DTS";
        break;
    default:
        break;
    }

    return strCodingMode;
}

std::string xmldoc::getAudioQuantisation(DVDAUDIOQUANTISATION eQuantisation) const
{
    std::string strQuantisation;

    switch (eQuantisation)
    {
    case DVDAUDIOQUANTISATION_16BPS:
        strQuantisation = "16 bps";
        break;
    case DVDAUDIOQUANTISATION_20BPS:
        strQuantisation = "20 bps";
        break;
    case DVDAUDIOQUANTISATION_24BPS:
        strQuantisation = "24 bps";
        break;
    case DVDAUDIOQUANTISATION_DRC:
        strQuantisation = "DRC";
        break;
    default:
        break;
    }
    return strQuantisation;
}

std::string xmldoc::getLanguage(const char *pszLanguage) const
{
    return pszLanguage;
}

void xmldoc::saveSetAttribute(TiXmlElement* element, const char *name, const std::string & strValue) const
{
    if (!strValue.empty())
    {
        element->SetAttribute(name, strValue);
    }
}

