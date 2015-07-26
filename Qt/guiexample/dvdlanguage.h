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

/** \file 
 *
 *  \brief 
 */

#pragma once

#ifndef DVDLANGUAGE_H
#define DVDLANGUAGE_H

#include <QObject>

#define DVDLANGUAGECOUNT    184

class dvdlanguage : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("author", "Norbert Schlia")
    Q_CLASSINFO("url", "http://www.dvdetect.de/")
    Q_CLASSINFO("project", "DVD Lookup Library")

    typedef struct
    {
        const char *    m_pszISO639Code;
        const char *    m_pszLanguageName;
    } LANGUAGETABLE;

public:
    explicit dvdlanguage(QObject *parent = 0);

    static QString  convertISOtoLanguage(const char *pszISO639Code);
    static int      getLanguageCount();
    static QString  getLanguage(int n);
    static QString  getISO639Code(int n);

signals:
    
public slots:

private:
    static const LANGUAGETABLE m_LanguageTable[DVDLANGUAGECOUNT];
};

#endif // DVDLANGUAGE_H
