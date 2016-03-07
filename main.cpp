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

//#include <kapplication.h>
//#include <kaboutdata.h>
//#include <kcmdlineargs.h>
//#include <klocale.h>

#include "resources.h"

   
#include "drimgwidget.h" // Must after global var declarations!

#include <qwidget.h>
#include <qlineedit.h>
//#include <qmultilineedit.h>
#include <qcombobox.h>

	
int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    //
    a.setApplicationName( VER_INTERNALNAME_STR );
    a.setApplicationVersion( VER_PRODUCTVERSION_STR );
    a.setOrganizationName( VER_COMPANYNAME_STR );
    a.setOrganizationDomain( VER_COMPANYDOMAIN_STR );
    a.setWindowIcon( QIcon(":/LogoApp48") );

    MainWindow w;
    w.setWindowTitle( VER_FILEDESCRIPTION_STR );
    w.show();

    try
    {
        return a.exec();
    }
    catch(QString& E)
    {
        qDebug() << QString("Exception(str) : %1").arg(E);
        return -1;
    }
    catch(std::exception& E)
    {
        qDebug() << QString("Exception(ex) : %1").arg(E.what());
        return -1;
    }
    catch(...)
    {
        qDebug() << QString("Exception  : unknown");
        return -1;
    }

}

