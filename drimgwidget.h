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


#ifndef _DRIMGWIDGET_H_
#define _DRIMGWIDGET_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <qwidget.h>
//#include <kapp.h>
//#include <klocale.h>
//#include <kfile.h>
//#include <kfiledialog.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
//#include <stdlib.h>
//#include <kaboutdialog.h>

//#include <kapplication.h>
//#include <kstatusbar.h>
//#include <kmainwindow.h>
#include <qapplication.h>
#include <qeventloop.h>
//#include <qtcore.h>

//#include "sg_cmds.h"
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <sys/types.h>

#include <linux/unistd.h>


#include "drimgwidgetbase.h"

class QAboutDialog;


class drimgWidget : public drimgWidgetBase


{
    Q_OBJECT

private:
		    
    QAboutDialog *aboutDlg;
	
	
public:
    drimgWidget(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = 0 );
   ~drimgWidget();
    
   // drimgWidget(QWidget* parent = 0, const char* name = 0 );
  //  ~drimgWidget();
    
    
    /*$PUBLIC_FUNCTIONS$*/

  // virtual void getForm();
   void getForm();
   void getCHS();
   void detSDloop();
   
    
public slots:
    /*$PUBLIC_SLOTS$*/
  //  virtual void button_clicked();
    virtual void quitButton_clicked();
    virtual void FileTrButton_clicked();
    virtual void openIfButton_clicked();
    //virtual void rawRB_clicked();
    
    virtual void readButton_clicked();
    virtual void lbaButton_clicked();
    virtual void sizeCHSButton_clicked();
    virtual void initzButton_clicked();
    virtual void creimfButton_clicked();
    virtual void aboutButton_clicked();
    virtual void refrButton_clicked();
    virtual void abortButton_clicked();
    virtual void listBox1_clicked(QListBoxItem*);
    virtual void writeButton_clicked();
    virtual void helpButton_clicked();
    virtual void ov2roCB_clicked();
    virtual void swapCB_clicked();
    
    //virtual void kURLLabel1_leftClickedURL(const QString&);
    
    
   // virtual void listBox1_pressed(QListBoxItem*);
    
    
protected:
    /*$PROTECTED_FUNCTIONS$*/

protected slots:
    /*$PROTECTED_SLOTS$*/

};

#endif


