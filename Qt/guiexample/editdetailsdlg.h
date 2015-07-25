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

/** \file editdetailsdlg.h
 *
 *  \brief Qt GUI example details editor declaration
 */

#pragma once

#ifndef DLGEDITDETAILS_H
#define DLGEDITDETAILS_H

#include <stdint.h>

#include <dvdetect/dvdetectc++.h>

#include <QDialog>

namespace Ui {
class editdetailsdlg;
}

class dvdparse;

class editdetailsdlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit editdetailsdlg(QWidget *parent = 0);
    ~editdetailsdlg();

    void                setParser(dvdparse *pDVD);
    
protected:
    void                setDVDDetailsColumnSize();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::editdetailsdlg *ui;
    dvdparse *          m_pDVD;
};

#endif // DLGEDITDETAILS_H
