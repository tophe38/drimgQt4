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
#ifndef TRANSFERZ_H
#define TRANSFERZ_H

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <qwidget.h>
#include <qpushbutton.h>



#include "ui_tranZ4.h"

class transferZ: public tranZDlg {
Q_OBJECT
public:
    transferZ(QWidget *parent = 0, const char *name = 0);
    void printuf();
    void decomp();
    void updatcc();
public slots:
    virtual void exitBu_clicked();
    virtual void addBu_clicked();
    virtual void gueBu_clicked();
    virtual void eraBu_clicked();
    virtual void saveBu_clicked();
    virtual void clearBu_clicked();
 //   virtual void catLB_clicked(QListBoxItem*);
    virtual void chaLB_clicked(QListBoxItem*);
    virtual void charBu_clicked();
 //   virtual void catLB_selected(QListBoxItem*);
 //   virtual void catLB_pressed(QListBoxItem*);
   // virtual void catLB_pressed(QListBoxItem*,const QPoint&);
    virtual void catLB_clicked(QListBoxItem*);

   
	    
};

#endif
