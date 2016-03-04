#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './initZdlg.ui'
**
** Created: Tue Feb 13 12:49:38 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "initZdlg.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a initZbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
initZbase::initZbase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "initZbase" );
    setMinimumSize( QSize( 400, 600 ) );
    setMaximumSize( QSize( 400, 600 ) );
    QFont f( font() );
    f.setFamily( "Helvetica" );
    f.setPointSize( 11 );
    setFont( f ); 

    promInf = new QLabel( this, "promInf" );
    promInf->setGeometry( QRect( 280, 210, 91, 20 ) );
    promInf->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    textLabel8 = new QLabel( this, "textLabel8" );
    textLabel8->setGeometry( QRect( 270, 240, 70, 21 ) );

    textLabel11 = new QLabel( this, "textLabel11" );
    textLabel11->setGeometry( QRect( 280, 380, 40, 20 ) );

    textLabel1 = new QLabel( this, "textLabel1" );
    textLabel1->setGeometry( QRect( 240, 10, 130, 20 ) );

    totdcLab = new QLabel( this, "totdcLab" );
    totdcLab->setGeometry( QRect( 250, 30, 111, 20 ) );
    totdcLab->setAlignment( int( QLabel::AlignCenter ) );

    textLabel3 = new QLabel( this, "textLabel3" );
    textLabel3->setGeometry( QRect( 240, 60, 121, 21 ) );

    placLab = new QLabel( this, "placLab" );
    placLab->setGeometry( QRect( 250, 80, 91, 20 ) );
    placLab->setAlignment( int( QLabel::AlignCenter ) );

    textLabel6 = new QLabel( this, "textLabel6" );
    textLabel6->setGeometry( QRect( 260, 160, 101, 21 ) );

    datsizEB = new QLineEdit( this, "datsizEB" );
    datsizEB->setGeometry( QRect( 260, 180, 90, 21 ) );
    datsizEB->setReadOnly( TRUE );

    caslEB = new QLineEdit( this, "caslEB" );
    caslEB->setGeometry( QRect( 260, 260, 91, 22 ) );
    caslEB->setReadOnly( TRUE );

    textLabel9 = new QLabel( this, "textLabel9" );
    textLabel9->setGeometry( QRect( 250, 330, 111, 21 ) );

    totsizesInf = new QLabel( this, "totsizesInf" );
    totsizesInf->setGeometry( QRect( 240, 350, 111, 20 ) );
    totsizesInf->setAlignment( int( QLabel::AlignCenter ) );

    freeInf = new QLabel( this, "freeInf" );
    freeInf->setGeometry( QRect( 260, 400, 81, 21 ) );
    freeInf->setAlignment( int( QLabel::AlignCenter ) );

    dataatEB = new QLineEdit( this, "dataatEB" );
    dataatEB->setGeometry( QRect( 360, 120, 25, 21 ) );
    dataatEB->setAlignment( int( QLineEdit::AlignHCenter ) );

    dataatBut = new QPushButton( this, "dataatBut" );
    dataatBut->setGeometry( QRect( 290, 120, 60, 20 ) );
    dataatBut->setFlat( TRUE );

    dasdBut = new QPushButton( this, "dasdBut" );
    dasdBut->setGeometry( QRect( 220, 180, 30, 30 ) );
    QFont dasdBut_font(  dasdBut->font() );
    dasdBut_font.setBold( TRUE );
    dasdBut->setFont( dasdBut_font ); 

    dasuBut = new QPushButton( this, "dasuBut" );
    dasuBut->setGeometry( QRect( 360, 170, 31, 30 ) );
    QFont dasuBut_font(  dasuBut->font() );
    dasuBut_font.setBold( TRUE );
    dasuBut->setFont( dasuBut_font ); 

    casdBut = new QPushButton( this, "casdBut" );
    casdBut->setGeometry( QRect( 220, 260, 30, 30 ) );
    QFont casdBut_font(  casdBut->font() );
    casdBut_font.setBold( TRUE );
    casdBut->setFont( casdBut_font ); 

    casuBut = new QPushButton( this, "casuBut" );
    casuBut->setGeometry( QRect( 360, 250, 31, 30 ) );
    QFont casuBut_font(  casuBut->font() );
    casuBut_font.setBold( TRUE );
    casuBut->setFont( casuBut_font ); 

    bigsCB = new QCheckBox( this, "bigsCB" );
    bigsCB->setGeometry( QRect( 250, 430, 93, 23 ) );

    exitBut = new QPushButton( this, "exitBut" );
    exitBut->setGeometry( QRect( 290, 550, 91, 30 ) );
    QFont exitBut_font(  exitBut->font() );
    exitBut_font.setBold( TRUE );
    exitBut->setFont( exitBut_font ); 

    performBut = new QPushButton( this, "performBut" );
    performBut->setGeometry( QRect( 250, 490, 101, 31 ) );
    QFont performBut_font(  performBut->font() );
    performBut_font.setBold( TRUE );
    performBut->setFont( performBut_font ); 

    listBox1 = new QListBox( this, "listBox1" );
    listBox1->setGeometry( QRect( 10, 10, 200, 580 ) );
    QFont listBox1_font(  listBox1->font() );
    listBox1_font.setFamily( "Fixed [Misc]" );
    listBox1_font.setPointSize( 10 );
    listBox1_font.setBold( TRUE );
    listBox1->setFont( listBox1_font ); 
    languageChange();
    resize( QSize(400, 600).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( exitBut, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( listBox1, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( listBox1_clicked(QListBoxItem*) ) );
    connect( dataatBut, SIGNAL( clicked() ), this, SLOT( dataatBut_clicked() ) );
    connect( dasdBut, SIGNAL( clicked() ), this, SLOT( dasdBut_clicked() ) );
    connect( dasuBut, SIGNAL( clicked() ), this, SLOT( dasuBut_clicked() ) );
    connect( casdBut, SIGNAL( clicked() ), this, SLOT( casdBut_clicked() ) );
    connect( casuBut, SIGNAL( clicked() ), this, SLOT( casuBut_clicked() ) );
    connect( bigsCB, SIGNAL( clicked() ), this, SLOT( bigsCB_clicked() ) );
    connect( performBut, SIGNAL( clicked() ), this, SLOT( performBut_clicked() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
initZbase::~initZbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void initZbase::languageChange()
{
    setCaption( tr2i18n( "Disk initialization - creating Chareas" ) );
    promInf->setText( tr2i18n( "10 prom." ) );
    textLabel8->setText( tr2i18n( "CAT slots:" ) );
    textLabel11->setText( tr2i18n( "Free:" ) );
    textLabel1->setText( tr2i18n( "Total disk capacity:" ) );
    totdcLab->setText( tr2i18n( "totcap" ) );
    textLabel3->setText( tr2i18n( "Place for Chareas:" ) );
    placLab->setText( tr2i18n( "plac" ) );
    textLabel6->setText( tr2i18n( "Data size, KB:" ) );
    textLabel9->setText( tr2i18n( "Total sizes, KB:" ) );
    totsizesInf->setText( tr2i18n( "totsizes" ) );
    freeInf->setText( tr2i18n( "freeinf" ) );
    dataatEB->setText( tr2i18n( "1" ) );
    dataatBut->setText( tr2i18n( "Data &at:" ) );
    dataatBut->setAccel( QKeySequence( tr2i18n( "Alt+A" ) ) );
    dasdBut->setText( tr2i18n( "<" ) );
    dasdBut->setAccel( QKeySequence( QString::null ) );
    dasuBut->setText( tr2i18n( ">" ) );
    dasuBut->setAccel( QKeySequence( QString::null ) );
    casdBut->setText( tr2i18n( "<" ) );
    casdBut->setAccel( QKeySequence( QString::null ) );
    casuBut->setText( tr2i18n( ">" ) );
    casuBut->setAccel( QKeySequence( QString::null ) );
    bigsCB->setText( tr2i18n( "Bi&g sectors" ) );
    bigsCB->setAccel( QKeySequence( tr2i18n( "Alt+G" ) ) );
    exitBut->setText( tr2i18n( ">> E&XIT >>" ) );
    exitBut->setAccel( QKeySequence( tr2i18n( "Alt+X" ) ) );
    performBut->setText( tr2i18n( "Perf&orm INIT" ) );
    performBut->setAccel( QKeySequence( tr2i18n( "Alt+O" ) ) );
    listBox1->clear();
    listBox1->insertItem( tr2i18n( "New Item" ) );
}

void initZbase::exitBut_clicked()
{
    qWarning( "initZbase::exitBut_clicked(): Not implemented yet" );
}

void initZbase::listBox1_clicked(QListBoxItem*)
{
    qWarning( "initZbase::listBox1_clicked(QListBoxItem*): Not implemented yet" );
}

void initZbase::dataatBut_clicked()
{
    qWarning( "initZbase::dataatBut_clicked(): Not implemented yet" );
}

void initZbase::dasdBut_clicked()
{
    qWarning( "initZbase::dasdBut_clicked(): Not implemented yet" );
}

void initZbase::dasuBut_clicked()
{
    qWarning( "initZbase::dasuBut_clicked(): Not implemented yet" );
}

void initZbase::casdBut_clicked()
{
    qWarning( "initZbase::casdBut_clicked(): Not implemented yet" );
}

void initZbase::casuBut_clicked()
{
    qWarning( "initZbase::casuBut_clicked(): Not implemented yet" );
}

void initZbase::bigsCB_clicked()
{
    qWarning( "initZbase::bigsCB_clicked(): Not implemented yet" );
}

void initZbase::performBut_clicked()
{
    qWarning( "initZbase::performBut_clicked(): Not implemented yet" );
}

#include "initZdlg.moc"
