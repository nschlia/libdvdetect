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

#include "localutils.h"
#include "xmldoc.h"
#include "http.h"

#define SERVER_URL      "http://db.dvdetect.de/"

dvddatabase::dvddatabase(const std::string &strClientName) :
    m_eErrorCode(DVDERRORCODE_NOERROR),
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

void dvddatabase::setError(const xmldoc & xmlDoc)
{
    m_strErrorString    = xmlDoc.getErrorString();
    m_eErrorCode        = DVDERRORCODE_XML_ERROR;
}

void dvddatabase::setError(const http & httpServer)
{
    m_strErrorString    = httpServer.getErrorString();
    m_eErrorCode        = DVDERRORCODE_HTML_ERROR;
}

int dvddatabase::query(dvdparselst & dvdParseLstIn, const dvdparse & dvdParseOut)
{
    std::string strXML;
    xmldoc xmlDoc(m_strClientName);

    int res = xmlDoc.query(strXML, dvdParseOut);

    if (!res)
    {
        http httpServer;

        if (!m_strProxy.empty())
        {
            httpServer.setProxy(m_strProxy);
        }

        res = httpServer.request(http::POST, SERVER_URL "query.php", "xml=" + uriEncode(strXML));

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

            xmldoc xmlDoc(m_strClientName);
            XMLMODE eXmlMode = XMLMODE_INVALID;

            dvdParseLstIn.clear();

            res  = xmlDoc.parseXml(httpServer.getContent(), eXmlMode, dvdParseLstIn);

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

int dvddatabase::search(dvdparselst & dvdParseLstIn, const std::string & strSearchOut)
{
    std::string strXML;
    xmldoc xmlDoc(m_strClientName);

    int res = xmlDoc.search(strXML, dvdParseLstIn, strSearchOut);

    if (!res)
    {
        http httpServer;

        if (!m_strProxy.empty())
        {
            httpServer.setProxy(m_strProxy);
        }

        res = httpServer.request(http::POST, SERVER_URL "search.php", "xml=" + uriEncode(strXML));

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

            xmldoc xmlDoc(m_strClientName);
            XMLMODE eXmlMode = XMLMODE_INVALID;

            dvdParseLstIn.clear();

            res  = xmlDoc.parseXml(httpServer.getContent(), eXmlMode, dvdParseLstIn);

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

int dvddatabase::submit(const dvdparselst & dvdParseLstOut)
{
    std::string strXML;
    xmldoc xmlDoc(m_strClientName);

    int res = xmlDoc.submit(strXML, dvdParseLstOut, true);

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

        res = httpServer.request(http::POST, SERVER_URL "submit.php", "xml=" + uriEncode(strXML));

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

            xmldoc xmlDoc(m_strClientName);
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

std::string dvddatabase::getErrorString() const
{
    return m_strErrorString;
}

DVDERRORCODE dvddatabase::getErrorCode() const
{
    return m_eErrorCode;
}

dvddatabase& dvddatabase::operator= (dvddatabase const& rhs)
{
    if (this != &rhs)
    {
        m_strClientName     = rhs.m_strClientName;
        m_eErrorCode        = rhs.m_eErrorCode;
        m_strErrorString    = rhs.m_strErrorString;
        m_strProxy          = rhs.m_strProxy;
    }
    return *this;
}

void dvddatabase::setClientName(const std::string & strClientName)
{
    m_strClientName = strClientName;
}

std::string dvddatabase::getClientName() const
{
    return m_strClientName;
}

void dvddatabase::setProxy(const std::string & strProxy)
{
    m_strProxy = strProxy;
}



