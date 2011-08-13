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

// Convenient Macros are for placing objects in columns and rows.
// It's probably better to just use QLayout, but this is what we've got
// for now.
//
// x = x starting point
// y = y starting point
// hs = horizontal span
// vs = vertical span
//
#define LAYOUT(x,y,hs,vs) \
	const int _Lx = x; \
	const int _Ly = y; \
	const int _Lhs = hs; \
	const int _Lvs = vs;
#define POINT(c, r) QPoint((_Lx + (c * _Lhs)), (_Ly + (r * _Lvs)))

enum {msg_info, msg_notify, msg_alert};

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
};

#endif // BITS_H
