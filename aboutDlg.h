/****************************************************************************
** Form interface generated from reading ui file '/home/pet/drimg/src/aboutDlg.ui'
**
** Created: Tue Feb 13 11:31:14 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ABOUTFORM_H
#define ABOUTFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class KURLLabel;
class QPushButton;
class QLabel;

class aboutForm : public QDialog
{
    Q_OBJECT

public:
    aboutForm( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0 );
    ~aboutForm();

    KURLLabel* kURLLabel1;
    QPushButton* aboexButton;
    QLabel* textLabel1;
    QLabel* textLabel1_2;
    QLabel* textLabel2;

public slots:
    virtual void aboexButton_clicked();
    virtual void kURLLabel1_leftClickedURL(const QString&);
    virtual void testButton_clicked();

protected:

protected slots:
    virtual void languageChange();

};

#endif // ABOUTFORM_H
