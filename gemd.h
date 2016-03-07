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
#ifndef GEMD_H
#define GEMD_H


#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <qwidget.h>
#include <qpushbutton.h>

#include "ui_gemdDlg4.h"

#include <QDialog>

namespace Ui {
    class gemdBase;
}

class gemd: public QDialog{
Q_OBJECT
public:
    gemd(QWidget *parent = 0);
    
    
    void ShowUF();
    void opensubd();
    void loadroot();
    void subdirh();
    void subdl();
  //  void setFiledt(int fdat, int ftim);

	public slots:
      
    virtual void filesLB_clicked(QListBoxItem*);
    virtual void partLB_clicked(QListBoxItem*);
   
    virtual void dirupP_clicked(int,int);
    virtual void quitP_clicked(int,int);
    virtual void newfP_clicked(int,int);
    virtual void addfP_clicked(int,int);
    virtual void setddP_clicked(int,int);
    virtual void extrP_clicked(int,int);
    virtual void opdirP_clicked(int,int);
    virtual void desallP_clicked(int,int);
    virtual void filesLB_doubleClicked(QListBoxItem*);
    virtual void timeCB_clicked();
    virtual void partsBut_clicked();
protected:
   Ui::gemdBase *ui;

};

#endif

