#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './tranZ.ui'
**
** Created: Tue Feb 13 12:49:38 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "tranZ.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a tranZDlg as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
tranZDlg::tranZDlg( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "tranZDlg" );
    setMinimumSize( QSize( 720, 360 ) );
    setMaximumSize( QSize( 720, 360 ) );

    catLa = new QLabel( this, "catLa" );
    catLa->setGeometry( QRect( 270, 10, 30, 20 ) );

    selLa = new QLabel( this, "selLa" );
    selLa->setGeometry( QRect( 460, 10, 110, 20 ) );

    savbCB = new QCheckBox( this, "savbCB" );
    savbCB->setGeometry( QRect( 610, 20, 90, 20 ) );

    hlEd = new QLineEdit( this, "hlEd" );
    hlEd->setGeometry( QRect( 670, 130, 25, 21 ) );
    hlEd->setMaxLength( 2 );

    reEd = new QLineEdit( this, "reEd" );
    reEd->setGeometry( QRect( 670, 160, 25, 21 ) );
    reEd->setMaxLength( 2 );

    textLabel4 = new QLabel( this, "textLabel4" );
    textLabel4->setGeometry( QRect( 610, 130, 50, 20 ) );

    textLabel5 = new QLabel( this, "textLabel5" );
    textLabel5->setGeometry( QRect( 600, 160, 71, 20 ) );

    usfreLa = new QLabel( this, "usfreLa" );
    usfreLa->setGeometry( QRect( 160, 330, 261, 21 ) );

    charBu = new QPushButton( this, "charBu" );
    charBu->setGeometry( QRect( 30, 10, 70, 20 ) );
    charBu->setFlat( TRUE );

    addBu = new QPushButton( this, "addBu" );
    addBu->setGeometry( QRect( 460, 290, 111, 41 ) );

    saveBu = new QPushButton( this, "saveBu" );
    saveBu->setGeometry( QRect( 600, 50, 101, 30 ) );

    eraBu = new QPushButton( this, "eraBu" );
    eraBu->setGeometry( QRect( 600, 90, 101, 30 ) );

    clearBu = new QPushButton( this, "clearBu" );
    clearBu->setGeometry( QRect( 451, 210, 130, 21 ) );

    gueBu = new QPushButton( this, "gueBu" );
    gueBu->setGeometry( QRect( 620, 210, 81, 30 ) );

    trinfLa = new QLabel( this, "trinfLa" );
    trinfLa->setGeometry( QRect( 440, 250, 220, 21 ) );

    selLB = new QListBox( this, "selLB" );
    selLB->setGeometry( QRect( 440, 30, 151, 161 ) );

    exitBu = new QPushButton( this, "exitBu" );
    exitBu->setGeometry( QRect( 610, 310, 91, 31 ) );

    chaLB = new QListBox( this, "chaLB" );
    chaLB->setGeometry( QRect( 0, 30, 141, 321 ) );
    QFont chaLB_font(  chaLB->font() );
    chaLB_font.setFamily( "Fixed [Misc]" );
    chaLB_font.setPointSize( 12 );
    chaLB_font.setBold( TRUE );
    chaLB->setFont( chaLB_font ); 

    catLB = new QListBox( this, "catLB" );
    catLB->setGeometry( QRect( 150, 30, 281, 291 ) );
    QFont catLB_font(  catLB->font() );
    catLB_font.setFamily( "Fixed [Misc]" );
    catLB_font.setPointSize( 11 );
    catLB_font.setBold( TRUE );
    catLB->setFont( catLB_font ); 
    languageChange();
    resize( QSize(720, 360).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( exitBu, SIGNAL( clicked() ), this, SLOT( exitBu_clicked() ) );
    connect( addBu, SIGNAL( clicked() ), this, SLOT( addBu_clicked() ) );
    connect( charBu, SIGNAL( clicked() ), this, SLOT( charBu_clicked() ) );
    connect( saveBu, SIGNAL( clicked() ), this, SLOT( saveBu_clicked() ) );
    connect( eraBu, SIGNAL( clicked() ), this, SLOT( eraBu_clicked() ) );
    connect( clearBu, SIGNAL( clicked() ), this, SLOT( clearBu_clicked() ) );
    connect( gueBu, SIGNAL( clicked() ), this, SLOT( gueBu_clicked() ) );
    connect( chaLB, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( chaLB_clicked(QListBoxItem*) ) );
    connect( catLB, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( catLB_clicked(QListBoxItem*) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
tranZDlg::~tranZDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void tranZDlg::languageChange()
{
    setCaption( tr2i18n( "Charea DoS browser" ) );
    catLa->setText( tr2i18n( "Cat:" ) );
    selLa->setText( tr2i18n( "Selected/copied:" ) );
    savbCB->setText( tr2i18n( "Save &bin." ) );
    savbCB->setAccel( QKeySequence( tr2i18n( "Alt+B" ) ) );
    hlEd->setText( tr2i18n( "hl" ) );
    reEd->setText( tr2i18n( "re" ) );
    textLabel4->setText( tr2i18n( "H'less:" ) );
    textLabel5->setText( tr2i18n( "Rename:" ) );
    usfreLa->setText( tr2i18n( "usfreLa" ) );
    charBu->setText( tr2i18n( "Chareas:" ) );
    charBu->setAccel( QKeySequence( QString::null ) );
    addBu->setText( tr2i18n( "&Add files" ) );
    addBu->setAccel( QKeySequence( tr2i18n( "Alt+A" ) ) );
    saveBu->setText( tr2i18n( "Save sel." ) );
    saveBu->setAccel( QKeySequence( QString::null ) );
    eraBu->setText( tr2i18n( "Erase sel." ) );
    eraBu->setAccel( QKeySequence( QString::null ) );
    clearBu->setText( tr2i18n( "C&lear selected" ) );
    clearBu->setAccel( QKeySequence( tr2i18n( "Alt+L" ) ) );
    gueBu->setText( tr2i18n( "Guess ge&o." ) );
    gueBu->setAccel( QKeySequence( tr2i18n( "Alt+O" ) ) );
    trinfLa->setText( tr2i18n( "trinfo" ) );
    exitBu->setText( tr2i18n( "E&XIT" ) );
    exitBu->setAccel( QKeySequence( tr2i18n( "Alt+X" ) ) );
    chaLB->clear();
    chaLB->insertItem( tr2i18n( "New Item" ) );
    catLB->clear();
    catLB->insertItem( tr2i18n( "New Item" ) );
}

void tranZDlg::exitBu_clicked()
{
    qWarning( "tranZDlg::exitBu_clicked(): Not implemented yet" );
}

void tranZDlg::addBu_clicked()
{
    qWarning( "tranZDlg::addBu_clicked(): Not implemented yet" );
}

void tranZDlg::charBu_clicked()
{
    qWarning( "tranZDlg::charBu_clicked(): Not implemented yet" );
}

void tranZDlg::saveBu_clicked()
{
    qWarning( "tranZDlg::saveBu_clicked(): Not implemented yet" );
}

void tranZDlg::eraBu_clicked()
{
    qWarning( "tranZDlg::eraBu_clicked(): Not implemented yet" );
}

void tranZDlg::clearBu_clicked()
{
    qWarning( "tranZDlg::clearBu_clicked(): Not implemented yet" );
}

void tranZDlg::gueBu_clicked()
{
    qWarning( "tranZDlg::gueBu_clicked(): Not implemented yet" );
}

void tranZDlg::chaLB_clicked(QListBoxItem*)
{
    qWarning( "tranZDlg::chaLB_clicked(QListBoxItem*): Not implemented yet" );
}

void tranZDlg::catLB_clicked(QListBoxItem*)
{
    qWarning( "tranZDlg::catLB_clicked(QListBoxItem*): Not implemented yet" );
}

#include "tranZ.moc"
