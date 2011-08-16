///////////////////////////////////////////////////////////////////////////////
//
// File		: bits.h
// Author	: Tony Camuso
// Date		: 20110730 - 30-Jul-2011
//
// Overview:
// Declares	the	bits class for the bits	App.
//
// Licencing: GPL V2
//
// History
//
// 20110730: Initial Entry
#ifndef	BITS_H
#define	BITS_H

#include <QObject>
#include <QMainWindow>
#include <QtCore>
#include <QtCore/QTextStream>
#include <QtGui>
#include <QtDesigner/QDesignerCustomWidgetInterface>

//#include "debug.h"
#include "bits.ui.h"
#include "BitButton.h"
#include "hexedit.h"
#include "controlgroup.h"

// Convenient Macros for placing objects in columns and rows.
// It's probably better to just use QLayout, but this is what we've got
// for now.
//
// LAYOUT - takes the x any coordinates in pixels relative to the upper left
//          corner of the window on which the widget will be placed. In this
//          case, the MainWindow.
//
// x = x starting point - pixel x-coordinate of upper left corner of widget
//                        relative to upper left corner of the window upon
//                        which it is placed.
// y = y starting point - y coordinate of upper left corner of widget.
// hs = horizontal span - Distance in pixels to the next widget to the right
//                        in the layout. Zero here indicates the layout is
//                        vertical.
// vs = vertical span   - Distance in pixels to the next widget below in the
//                        layout. Zero here indicates the layout is horizontal.
//
#define LAYOUT(x,y,hs,vs) \
	const int _Lx = x; \
	const int _Ly = y; \
	const int _Lhs = hs; \
	const int _Lvs = vs;

// This macro allows us to use "column,row" placement of widgets, once the
// LAYOUT macro is called to express the layout in pixels. The POINT macro
// takes the const int definitions made by LAYOUT and converts them to
// pixel coordinates in a QPoint. All the user has to do is indicate which
// column,row for the widget, wich makes layout a lot easier.
//
#define POINT(c, r) QPoint((_Lx + (c * _Lhs)), (_Ly + (r * _Lvs)))

// Message types, each associated with its own color.
//
enum {msg_info, msg_notify, msg_alert};

// Enumeration of the calculator buttons.
//
enum {calc_and, calc_or, calc_xor,
	  calc_add,           calc_sub,
	  calc_mul, calc_div, calc_mod};

namespace Ui {
	class bits;
}

class bits : public QMainWindow
{
	Q_OBJECT

public:
	explicit bits(QWidget *parent=0);
	~bits();
	QString getSystem();

signals:

private	slots:
	void mapped_bbClick(int);
	void mapped_hexedit(int);
	void bbRadioClick(int);
	void shiftRadioClick(int);
	void bitSizeClick(int);
	void onInvert(int);
	void onShift(int);
	void onCalc (int);
	void onFormat(int);

private:
	Ui::bits *ui;

	quint64	getBits();
	void init_messageBox();
	void init_bbArray();
	void init_bitSizes();
	void init_heArray();
	void init_invert();
	void init_shiftOp();
	void init_calc();
	void init_format();
	void keyPressEvent(QKeyEvent *event);
	QString& dec2hex(quint64 qiNumber);
	void updateBits(int hexIndex);
	void updateEdits();
	void updateMessageBox();
	void updateWinSizes();
	void showDecimals(quint64 val);
	void sendMessage(QString& line, int level);
	void showBits();
	void hideBits();
	void setAppStyles();

	ControlGroup <QRadioButton> *pBitSizes;
	ControlGroup <QPushButton> *pInvert;
	QFrame *bitSizeFrame;
	ShiftOpGroup *shiftOp;
	ControlGroup <QPushButton> *pCalc;
	ControlGroup <QRadioButton> *pConnectFormat;
};

#endif // BITS_H
