/******************************************************************************
**
**  bits - an application to arithmeticall and logically combine and
**         manipulate bit patterns and their corresponding hex numbers.
**
**  Tony Camuso
**  Created December, 2011
**
**    bits (bitview) is free software. You can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**  GNU General Public License http://www.gnu.org/licenses/gpl.html
**
**  Copyright (c) 2011-2014 by Tony Camuso.
**
******************************************************************************/

/********************************************************************************
** bits.ui.h
**
** Author: Tony Camuso
** Date  : 20110729 29-Jul-2011
**
** Ordinarily, the QtDesigner creates a bits.ui xml file that gets compiled
** into a C++ header file at build time. That file is destroyed and recreated
** on every build, so edits to it are pointless.

** The header file created from the xml file instantiates the User Interface,
** which becomes the main window when its "show()" member function is called.
**
** The file name of this auto-generated header is prepended with "ui_" and
** postpended with the name of the project. For example, the file generated
** for this project is ui_bits.h.
**
** In order to ease the learning curve, having had little success with
** creating and adding layouts to the main window ui, I decided to not
** include the generated file, and instead copy its contents into a new
** header file, where customizations can take place easily.
**
** I'm certain there's a better way, but I just need to get going. I will
** experiment with the finer points later.
**
********************************************************************************/

#ifndef UI_BITS_H
#define UI_BITS_H

#if QT_VERSION < 0x50000
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#endif

#if QT_VERSION == 0x50101
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHeaderView>
#include <QMainWindow>
#include <QMenuBar>
#include <QPushButton>
#include <QRadioButton>
#include <QStatusBar>
#include <QTextEdit>
#include <QToolBar>
#include <QWidget>
#endif

#include "BitButton.h"
#include "hexedit.h"
#include "controlgroup.h"
#include "shiftop.h"

#define QT_USE_FAST_CONCATENATION   // use % instead of + for
                                    // string concatenation

// Main Window Geometry
// ====================
#define MAINWINDOW_W 716
#define MAINWINDOW_H 360
//#define   MAINWINDOW_H 260

#define INIT_BITSIZE 32     // The default bit count

#define X_START 30
#define Y_START 30

// BitButton and BitButton Label Geometry
// ======================================
#define BITS 64
#define ROWS (BITS/32)
#define COLS (BITS/2)

#define BB_X 670
#define BB_Y 220
#define BB_W 18
#define BB_H 22
#define BB_MARGIN 0

#define NYB_MARGIN 12
#define ROW_MARGIN (BB_H + (BB_H / 2))

#define BB_LABEL_W BB_W
#define BB_LABEL_H BB_H

// HexEdit Combo Box Geometry
// ==========================

enum hexedit_t
{
    hex_left, hex_right, hex_result, hex_array_size
};


#define HE_W 200
#define HE_H 30

#define HE_LEFT_X X_START
#define HE_LEFT_Y Y_START
#define HE_SKIP_W 38

QT_BEGIN_NAMESPACE

class Ui_bits
{
public:
    QWidget *centralWidget;
    QTextEdit *messages;

    BitButton *bb[BITS];
    QLabel *bbLabel[BITS];
    QFrame *bbFrame;

    HexEdit *hexedit[hex_array_size];
    QButtonGroup *bbConnectGroup;
    QButtonGroup *shiftConnectGroup;

    QMenuBar *menuBar;
    //QToolBar *mainToolBar;
    //QStatusBar *statusBar;

    void setupUi(QMainWindow *bits)
    {
        if (bits->objectName().isEmpty())
            bits->setObjectName(QString::fromUtf8("bits"));
        bits->resize(MAINWINDOW_W, MAINWINDOW_H);

        centralWidget = new QWidget(bits);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));

        bits->setCentralWidget(centralWidget);

        menuBar = new QMenuBar(bits);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 760, 20));
        bits->setMenuBar(menuBar);
#if 0
        mainToolBar = new QToolBar(bits);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        bits->addToolBar(Qt::TopToolBarArea, mainToolBar);

        statusBar = new QStatusBar(bits);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        bits->setStatusBar(statusBar);
#endif
        QMetaObject::connectSlotsByName(bits);

    } // setupUi
};

namespace Ui {
    class bits: public Ui_bits {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BITS_H
