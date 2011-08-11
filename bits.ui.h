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

#ifndef	UI_BITS_H
#define	UI_BITS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include <QtGui/QRadioButton>

#include "BitButton.h"
#include "hexedit.h"
#include "controlgroup.h"
#include "shiftop.h"

#define	QT_USE_FAST_CONCATENATION	// use % instead of	+ for string concatenation

// Main	Window Geometry
// ====================
#define	MAINWINDOW_W 716
#define	MAINWINDOW_H 360

#define INIT_BITSIZE 32

// BitButton and BitButton Label Geometry
// ======================================
#define	BITS 64
#define	ROWS (BITS/32)
#define	COLS (BITS/2)

#define	BB_X 670
#define	BB_Y 220
#define	BB_W 18
#define	BB_H 22
#define BB_MARGIN 0

#define	NYB_MARGIN 12
#define	ROW_MARGIN (BB_H + (BB_H / 2))

#define	BB_LABEL_W BB_W
#define	BB_LABEL_H BB_H

// HexEdit Combo Box Geometry
// ==========================

enum hexedit_t
{
	hex_left, hex_right, hex_result, hex_array_size
};


#define	HE_W 200
#define	HE_H 30

#define	HE_LEFT_X 30
#define	HE_LEFT_Y 30
#define	HE_SKIP_W 38

QT_BEGIN_NAMESPACE

class Ui_bits
{
public:
	QWidget	*centralWidget;
	QTextEdit *messages;

	BitButton *bb[BITS];
	QLabel *bbLabel[BITS];
	QFrame *bbFrame;

	HexEdit	*hexedit[hex_array_size];
	QButtonGroup *bbConnectGroup;
	QButtonGroup *shiftConnectGroup;

	//QMenuBar *menuBar;
	//QToolBar *mainToolBar;
	//QStatusBar *statusBar;

	void setupUi(QMainWindow *bits)
	{
		if (bits->objectName().isEmpty())
			bits->setObjectName(QString::fromUtf8("bits"));
		bits->resize(MAINWINDOW_W, MAINWINDOW_H);

		centralWidget =	new	QWidget(bits);
		centralWidget->setObjectName(QString::fromUtf8("centralWidget"));

		bits->setCentralWidget(centralWidget);
#if 0
		menuBar	= new QMenuBar(bits);
		menuBar->setObjectName(QString::fromUtf8("menuBar"));
		menuBar->setGeometry(QRect(0, 0, 760, 20));
		bits->setMenuBar(menuBar);

		mainToolBar	= new QToolBar(bits);
		mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
		bits->addToolBar(Qt::TopToolBarArea, mainToolBar);

		statusBar =	new	QStatusBar(bits);
		statusBar->setObjectName(QString::fromUtf8("statusBar"));
		bits->setStatusBar(statusBar);
#endif
		QMetaObject::connectSlotsByName(bits);

	} // setupUi
};

namespace Ui {
	class bits:	public Ui_bits {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BITS_H
