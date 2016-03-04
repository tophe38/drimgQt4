/****************************************************************************
** Form implementation generated from reading ui file './aboutDlg.ui'
**
** Created: Tue Feb 13 12:49:38 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "aboutDlg.h"

//#include <kdialog.h>
//#include <klocale.h>
//#include <kurllabel.h>

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a aboutForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
aboutForm::aboutForm(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl )
    : QDialog( parent, fl )
{
    if ( !name )
    setObjectName( "aboutForm" );
    setMinimumSize( QSize( 320, 240 ) );
    setMaximumSize( QSize( 320, 240 ) );

//    kURLLabel1 = new KURLLabel( this, "kURLLabel1" );
//    kURLLabel1->setGeometry( QRect( 71, 120, 170, 24 ) );
//    QFont kURLLabel1_font(  kURLLabel1->font() );
//    kURLLabel1_font.setPointSize( 12 );
//    kURLLabel1_font.setUnderline( TRUE );
//    kURLLabel1->setFont( kURLLabel1_font );
//    kURLLabel1->setAlignment( int( KURLLabel::AlignCenter ) );

//    aboexButton = new QPushButton( this, "aboexButton" );
//    aboexButton->setGeometry( QRect( 220, 180, 70, 31 ) );

//    textLabel1 = new QLabel( this, "textLabel1" );
//    textLabel1->setGeometry( QRect( 10, 20, 300, 20 ) );
//    QFont textLabel1_font(  textLabel1->font() );
//    textLabel1_font.setPointSize( 9 );
//    textLabel1_font.setBold( TRUE );
//    textLabel1->setFont( textLabel1_font );
//    textLabel1->setAlignment( int( QLabel::AlignCenter ) );

//    textLabel1_2 = new QLabel( this, "textLabel1_2" );
//    textLabel1_2->setGeometry( QRect( 110, 50, 70, 21 ) );
//    textLabel1_2->setAlignment( int( QLabel::AlignCenter ) );

//    textLabel2 = new QLabel( this, "textLabel2" );
//    textLabel2->setGeometry( QRect( 50, 80, 210, 21 ) );
//    textLabel2->setAlignment( int( QLabel::AlignCenter ) );
//    languageChange();
//    resize( QSize(320, 240).expandedTo(minimumSizeHint()) );
//    clearWState( WState_Polished );

//    // signals and slots connections
//    connect( aboexButton, SIGNAL( clicked() ), this, SLOT( close() ) );
//    connect( kURLLabel1, SIGNAL( leftClickedURL(const QString&) ), this, SLOT( kURLLabel1_leftClickedURL(const QString&) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
aboutForm::~aboutForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void aboutForm::languageChange()
{
//    setCaption( tr2i18n( "About Drive Image" ) );
//    kURLLabel1->setText( tr2i18n( "http://www.ppest.org" ) );
//    kURLLabel1->setProperty( "url", tr2i18n( "http://www.ppest.org" ) );
//    aboexButton->setText( tr2i18n( "E&XIT" ) );
//    aboexButton->setAccel( QKeySequence( tr2i18n( "Alt+X" ) ) );
//    textLabel1->setText( tr2i18n( "Drive Imaging, file transfer & partition extract" ) );
//    textLabel1_2->setText( tr2i18n( "V 0.86" ) );
//    textLabel2->setText( tr2i18n( "by P. Putnik   2006-7" ) );
}

void aboutForm::aboexButton_clicked()
{
    qWarning( "aboutForm::aboexButton_clicked(): Not implemented yet" );
}

void aboutForm::kURLLabel1_leftClickedURL(const QString&)
{
    qWarning( "aboutForm::kURLLabel1_leftClickedURL(const QString&): Not implemented yet" );
}

void aboutForm::testButton_clicked()
{
    qWarning( "aboutForm::testButton_clicked(): Not implemented yet" );
}
