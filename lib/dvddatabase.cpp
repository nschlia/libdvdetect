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

dvddatabase::dvddatabase(const std::string &strClientName,
                         void *  (*openFile)(const char *, const char *, const char *),
                         size_t  (*readFile)(void*, size_t, size_t, void*),
                         int     (*writeFile)(const void*, size_t, size_t, void*),
                         int     (*closeFile)(void *),
                         int     (*statFile)(const char *, LPDVDFILESTAT, const char *),
                         int     (*fstatFile)(void*, LPDVDFILESTAT)) :
    m_eErrorCode(DVDERRORCODE_SUCCESS),
    m_strServerUrl(SERVER_URL),
    m_strClientName(strClientName),
    m_pOpenFile(openFile),
    m_pReadFile(readFile),
    m_pWriteFile(writeFile),
    m_pCloseFile(closeFile),
    m_pStatFile(statFile),
    m_pFstatFile(fstatFile)
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
        void *fpo = m_pOpenFile("query.out.xml", "wt", m_strProxy.c_str());
        if (fpo != NULL)
        {
            m_pWriteFile(strXML.c_str(), 1, strXML.size(), fpo);
            m_pCloseFile(fpo);
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
            void *fpo = m_pOpenFile("query.xml", "wt", m_strProxy.c_str());
            if (fpo != NULL)
            {
                m_pWriteFile(httpServer.getContent().c_str(), 1, httpServer.getContent().size(), fpo);
                m_pCloseFile(fpo);
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
        void *fpo = m_pOpenFile("search.out.xml", "wt", m_strProxy.c_str());
        if (fpo != NULL)
        {
            m_pWriteFile(strXML.c_str(), 1, strXML.size(), fpo);
            m_pCloseFile(fpo);
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
            void *fpo = m_pOpenFile("search.xml", "wt", m_strProxy.c_str());
            if (fpo != NULL)
            {
                m_pWriteFile(httpServer.getContent().c_str(), 1, httpServer.getContent().size(), fpo);
                m_pCloseFile(fpo);
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
        void *fpo = m_pOpenFile("submit.out.xml", "wt", m_strProxy.c_str());
        if (fpo != NULL)
        {
            m_pWriteFile(strXML.c_str(), 1, strXML.size(), fpo);
            m_pCloseFile(fpo);
        }
#endif

        res = httpServer.request(http::POST, m_strServerUrl + "/submit.php", "xml=" + uriEncode(strXML));

        if (res == 200)
        {
#ifdef DEBUG
            fpo = m_pOpenFile("submit.xml", "wt", m_strProxy.c_str());
            if (fpo != NULL)
            {
                m_pWriteFile(httpServer.getContent().c_str(), 1, httpServer.getContent().size(), fpo);
                m_pCloseFile(fpo);
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
    int res = 0;
    DVDFILESTAT filestat;

    void *fpi = m_pOpenFile(strInFile.c_str(), "rb", m_strProxy.c_str());
    if (fpi == NULL)
    {
        std::string strError = "Error opening file: " + strInFile + "\n";
        strError += strerror(errno);
        setError(strError, DVDERRORCODE_FILEOPEN);
        res = -1;
    }
    else if (m_pStatFile(strInFile.c_str(), &filestat, m_strProxy.c_str()) == -1)
    {
        std::string strError = "Cannot stat file: " + strInFile + "\n";
        strError += strerror(errno);
        setError(strError, DVDERRORCODE_FILESTAT);
        res = -1;
    }
    else
    {
        char *pszData = new char [filestat.m_qwFileSize];
        if (pszData != NULL)
        {
            memset(pszData, 0, filestat.m_qwFileSize);
            if (m_pReadFile(pszData, 1, filestat.m_qwFileSize, fpi) != (size_t)filestat.m_qwFileSize)
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
        m_pCloseFile(fpi);
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
        void *fpo = m_pOpenFile(strOutFile.c_str(), "wb", m_strProxy.c_str());
        if (fpo == NULL)
        {
            std::string strError = "Error opening file: " + strOutFile + "\n";
            strError += strerror(errno);
            setError(strError, DVDERRORCODE_FILEOPEN);
            res = -1;
        }
        else
        {
            m_pWriteFile(strXML.c_str(), 1, strXML.size(), fpo);
            m_pCloseFile(fpo);
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

string dvddatabase::getProxy() const
{
    return m_strProxy;
}

const std::string & dvddatabase::getServerUrl() const
{
    return m_strServerUrl;
}

void dvddatabase::setServerUrl(const std::string & strServerUrl)
{
    m_strServerUrl = strServerUrl;
}


