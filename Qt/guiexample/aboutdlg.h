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

/** \file aboutdlg.h
 *
 *  \brief Qt GUI about dialog code
 */

#pragma once

#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <QDialog>

namespace Ui {
class aboutdlg;
}

class aboutdlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit aboutdlg(QWidget *parent = 0);
    ~aboutdlg();
    
private slots:
    void on_pushButtonOK_clicked();

private:
    Ui::aboutdlg *ui;
};

#endif // ABOUTDLG_H
