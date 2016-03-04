/***************************************************************************
 *   Copyright (C) 2006 by P. Putnik   *
 *   pp@ppest.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef INIZ_H
#define INIZ_H


//#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qwidget.h>
#include <kapp.h>
#include <klocale.h>
#include <kfile.h>
#include <kfiledialog.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <unistd.h>
#include <fcntl.h>

#include "ui_initZdlg4.h"



int fileclo(int fd);




class iniZ: public initZbase {
Q_OBJECT
public:
    iniZ(QWidget *parent = 0, const char *name = 0);
    void calctot();
    void getkb1();
    
public slots:
    virtual void exitBut_clicked();
    virtual void performBut_clicked();
    virtual void bigsCB_clicked();
    virtual void casuBut_clicked();
    virtual void casdBut_clicked();
    virtual void dasuBut_clicked();
    virtual void dasdBut_clicked();
    virtual void dataatBut_clicked();
    virtual void listBox1_clicked(QListBoxItem*);
};

#endif
