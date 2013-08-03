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

/*! \file dvddatabase.cpp
 *
 *  \brief dvddatabase class implementation
 */

#include "compat.h"

#include <dvdetect/dvdetectc++.h>
#include <stdio.h>
#include <fstream>

#include "localutils.h"
#include "xmldocbuilder.h"
#include "xmldocparser.h"
#include "http.h"

#define SERVER_URL      "http://db.dvdetect.de/"

dvddatabase::dvddatabase(const std::string &strClientName) :
    m_eErrorCode(DVDERRORCODE_NOERROR),
    m_strServerUrl(SERVER_URL),
    m_strClientName(strClientName)
{
}

dvddatabase::~dvddatabase()
{
}

void dvddatabase::setError(const std::string & strErrorString, DVDERRORCODE eErrorCode)
{
    m_strErrorString    = strErrorString;
    m_eErrorCode        = eErrorCode;
}

void dvddatabase::setError(const xmldocbuilder & xmlDoc)
{
    m_strErrorString    = xmlDoc.getErrorString();
    m_eErrorCode        = xmlDoc.getErrorCode();
}

void dvddatabase::setError(const xmldocparser & xmlDoc)
{
    m_strErrorString    = xmlDoc.getErrorString();
    m_eErrorCode        = xmlDoc.getErrorCode();
}

void dvddatabase::setError(const http & httpServer)
{
    m_strErrorString    = httpServer.getErrorString();
    m_eErrorCode        = DVDERRORCODE_HTML_ERROR;
}

int dvddatabase::query(dvdparselst *pLstDvdParse, const dvdparse * pDvdParse)
{
    std::string strXML;
    xmldocbuilder xmlDoc(m_strClientName);

    int res = xmlDoc.buildXml(strXML, XMLMODE_QUERY, pDvdParse);

    if (!res)
    {
#ifdef DEBUG
        FILE *fpo = fopen("query.out.xml", "wt");
        if (fpo != NULL)
        {
            fprintf(fpo, "%s", strXML.c_str());
            fclose(fpo);
        }
#endif

        http httpServer;

        if (!m_strProxy.empty())
        {
            httpServer.setProxy(m_strProxy);
        }

        res = httpServer.request(http::POST, m_strServerUrl + "/query.php", "xml=" + uriEncode(strXML));

        if (res == 200)
        {
#ifdef DEBUG
            FILE *fpo = fopen("query.xml", "wt");
            if (fpo != NULL)
            {
                fprintf(fpo, "%s", httpServer.getContent().c_str());
                fclose(fpo);
            }
#endif
            res = 0;

            xmldocparser xmlDoc(m_strClientName);
            XMLMODE eXmlMode = XMLMODE_INVALID;

            pLstDvdParse->clear();

            res  = xmlDoc.parseXml(httpServer.getContent(), eXmlMode, pLstDvdParse);

            if (res)
            {
                setError(xmlDoc);
            }
        }
        else
        {
            setError(httpServer);
        }
    }
    else
    {
        setError(xmlDoc);
    }

    return res;
}

int dvddatabase::search(dvdparselst *pLstDvdParse, const std::string & strSearch)
{
    std::string strXML;
    xmldocbuilder xmlDoc(m_strClientName);

    int res = xmlDoc.buildXml(strXML, XMLMODE_SEARCH, pLstDvdParse, strSearch);

    if (!res)
    {
#ifdef DEBUG
        FILE *fpo = fopen("search.out.xml", "wt");
        if (fpo != NULL)
        {
            fprintf(fpo, "%s", strXML.c_str());
            fclose(fpo);
        }
#endif
        http httpServer;

        if (!m_strProxy.empty())
        {
            httpServer.setProxy(m_strProxy);
        }

        res = httpServer.request(http::POST, m_strServerUrl + "/search.php", "xml=" + uriEncode(strXML));

        if (res == 200)
        {
#ifdef DEBUG
            FILE *fpo = fopen("search.xml", "wt");
            if (fpo != NULL)
            {
                fprintf(fpo, "%s", httpServer.getContent().c_str());
                fclose(fpo);
            }
#endif
            res = 0;

            xmldocparser xmlDoc(m_strClientName);
            XMLMODE eXmlMode = XMLMODE_INVALID;

            pLstDvdParse->clear();

            res  = xmlDoc.parseXml(httpServer.getContent(), eXmlMode, pLstDvdParse);

            if (res)
            {
                setError(xmlDoc);
            }
        }
        else
        {
            setError(httpServer);
        }
    }
    else
    {
        setError(xmlDoc);
    }

    return res;
}

int dvddatabase::submit(const dvdparse *pDvdParse)
{
    dvdparselst lstDvdParse;
    dvdparse *pDvdParseTmp = new dvdparse; // No leak: gets deleted by dvdparselst destructor

    *pDvdParseTmp = *pDvdParse;

    lstDvdParse.push_back(pDvdParseTmp);

    return submit(&lstDvdParse);
}

int dvddatabase::submit(const dvdparselst *pLstDvdParse)
{
    std::string strXML;
    xmldocbuilder xmlDoc(m_strClientName);

    int res = xmlDoc.buildXml(strXML, XMLMODE_SUBMIT, pLstDvdParse);

    if (!res)
    {
        http httpServer;

        if (!m_strProxy.empty())
        {
            httpServer.setProxy(m_strProxy);
        }


#ifdef DEBUG
        FILE *fpo = fopen("submit.out.xml", "wt");
        if (fpo != NULL)
        {
            fprintf(fpo, "%s\n", strXML.c_str());
            fclose(fpo);
        }
#endif

        res = httpServer.request(http::POST, m_strServerUrl + "/submit.php", "xml=" + uriEncode(strXML));

        if (res == 200)
        {
#ifdef DEBUG
            fpo = fopen("submit.xml", "wt");
            if (fpo != NULL)
            {
                fprintf(fpo, "%s", httpServer.getContent().c_str());
                fclose(fpo);
            }
#endif
            res = 0;

            xmldocparser xmlDoc(m_strClientName);
            XMLMODE eXmlMode = XMLMODE_INVALID;

            res  = xmlDoc.parseXmlResponse(httpServer.getContent(), eXmlMode);

            if (res)
            {
                setError(xmlDoc);
            }
        }
    }
    else
    {
        setError(xmlDoc);
    }

    return res;
}

int dvddatabase::read(dvdparselst *pLstDvdParse, const std::string & strInFile)
{
    string strShortFilePath(getWin32ShortFilePath(strInFile));
    int res = 0;

    struct stat buf;

    std::FILE *fpi = std::fopen(strShortFilePath.c_str(), "rb");
    if (fpi == NULL)
    {
        std::string strError = "Error opening file: " + strInFile + "\n";
        strError += strerror(errno);
        setError(strError, DVDERRORCODE_FILEOPEN);
        res = -1;
    }
    else if (stat(strShortFilePath.c_str(), &buf) == -1) // fstat(fileno(fpi), &buf) == -1)
    {
        std::string strError = "Cannot stat file: " + strInFile + "\n";
        strError += strerror(errno);
        setError(strError, DVDERRORCODE_FILESTAT);
        res = -1;
    }
    else
    {
        char *pszData = new char [buf.st_size];
        if (pszData != NULL)
        {
            memset(pszData, 0, buf.st_size);
            if (std::fread(pszData, 1, buf.st_size, fpi) != (size_t)buf.st_size)
            {
                std::string strError = "Error reading file: " + strInFile + "\n";
                strError += strerror(errno);
                setError(strError, DVDERRORCODE_FILEREAD);
                res = -1;
            }
            else
            {
                xmldocparser xmlDoc(m_strClientName);
                XMLMODE eXmlMode = XMLMODE_EXPORT;

                pLstDvdParse->clear();

                res  = xmlDoc.parseXml(pszData, eXmlMode, pLstDvdParse);

                if (res)
                {
                    setError(xmlDoc);
                }
            }

            delete [] pszData;
            pszData = NULL;
        }
    }

    if (fpi != NULL)
    {
        std::fclose(fpi);
    }

    return res;
}

int dvddatabase::write(const dvdparse *pDvdParse, const std::string & strOutFile)
{
    dvdparselst lstDvdParse;
    dvdparse *pDvdParseTmp = new dvdparse; // No leak: gets deleted by dvdparselst destructor

    *pDvdParseTmp = *pDvdParse;

    lstDvdParse.push_back(pDvdParseTmp);

    return write(&lstDvdParse, strOutFile);
}

int dvddatabase::write(const dvdparselst *pLstDvdParse, const std::string & strOutFile)
{
    std::string strXML;
    xmldocbuilder xmlDoc(m_strClientName);

    int res = xmlDoc.buildXml(strXML, XMLMODE_EXPORT, pLstDvdParse);

    if (!res)
    {
        std::FILE *fpo = NULL;
        fpo = std::fopen(getWin32ShortFilePath(strOutFile).c_str(), "wb");
        if (fpo == NULL)
        {
            std::string strError = "Error opening file: " + strOutFile + "\n";
            strError += strerror(errno);
            setError(strError, DVDERRORCODE_FILEOPEN);
            res = -1;
        }
        else
        {
            std::fwrite(strXML.c_str(), 1, strXML.size(), fpo);
            std::fclose(fpo);
        }
    }

    return res;
}

const std::string & dvddatabase::getErrorString() const
{
    return m_strErrorString;
}

DVDERRORCODE dvddatabase::getErrorCode() const
{
    return m_eErrorCode;
}

dvddatabase& dvddatabase::operator= (dvddatabase const & source)
{
    if (this != &source)
    {
        m_strClientName     = source.m_strClientName;
        m_eErrorCode        = source.m_eErrorCode;
        m_strErrorString    = source.m_strErrorString;
        m_strProxy          = source.m_strProxy;
    }
    return *this;
}

void dvddatabase::setClientName(const std::string & strClientName)
{
    m_strClientName = strClientName;
}

const std::string & dvddatabase::getClientName() const
{
    return m_strClientName;
}

void dvddatabase::setProxy(const std::string & strProxy)
{
    m_strProxy = strProxy;
}

const std::string & dvddatabase::getServerUrl() const
{
    return m_strServerUrl;
}

void dvddatabase::setServerUrl(const std::string & strServerUrl)
{
    m_strServerUrl = strServerUrl;
}


