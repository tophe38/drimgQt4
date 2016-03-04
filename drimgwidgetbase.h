/****************************************************************************
** Form interface generated from reading ui file './drimgwidgetbase.ui'
**
** Created: Tue Feb 13 12:49:37 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef DRIMGWIDGETBASE_H
#define DRIMGWIDGETBASE_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QGroupBox;
class QPushButton;
class QButtonGroup;
class QLineEdit;
class QProgressBar;
class QRadioButton;
class QCheckBox;
class QListBox;
class QListBoxItem;

class drimgWidgetBase : public QWidget
{
    Q_OBJECT

public:
    drimgWidgetBase( QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = 0 );
    ~drimgWidgetBase();

    QLabel* textLabel1;
    QLabel* textLabel2;
    QLabel* textLabel3;
    QGroupBox* groupBox1;
    QLabel* inf1Label;
    QLabel* inf2Label;
    QLabel* inf6Label;
    QLabel* inf4Label;
    QLabel* inf3Label;
    QLabel* inf5Label;
    QPushButton* initzButton;
    QPushButton* creimfButton;
    QButtonGroup* buttonGroup2;
    QLabel* textLabel11;
    QLabel* textLabel12;
    QLabel* textLabel13;
    QLineEdit* edSecTr;
    QLineEdit* edHeads;
    QLineEdit* edCyl;
    QPushButton* sizeCHSButton;
    QPushButton* lbaButton;
    QPushButton* abortButton;
    QLineEdit* seccnEdit;
    QLineEdit* secofEdit;
    QProgressBar* progressBar1;
    QLabel* curopLabel;
    QPushButton* quitButton;
    QPushButton* refrButton;
    QPushButton* openIfButton;
    QPushButton* helpButton;
    QPushButton* FileTrButton;
    QButtonGroup* buttonGroup3;
    QRadioButton* hdfRB;
    QRadioButton* h256RB;
    QRadioButton* rawRB;
    QPushButton* readButton;
    QCheckBox* ov2roCB;
    QPushButton* writeButton;
    QListBox* listBox1;
    QCheckBox* swapCB;
    QPushButton* aboutButton;

public slots:
    virtual void button_clicked();
    virtual void quitButton_clicked();
    virtual void FileTrButton_clicked();
    virtual void openIfButton_clicked();
    virtual void readButton_clicked();
    virtual void sizeCHSButton_clicked();
    virtual void lbaButton_clicked();
    virtual void creimfButton_clicked();
    virtual void initzButton_clicked();
    virtual void aboutButton_clicked();
    virtual void refrButton_clicked();
    virtual void abortButton_clicked();
    virtual void listBox1_clicked(QListBoxItem*);
    virtual void writeButton_clicked();
    virtual void helpButton_clicked();
    virtual void ov2roCB_clicked();
    virtual void swapCB_clicked();

protected:

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // DRIMGWIDGETBASE_H
