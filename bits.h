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

#include "bits.ui.h"
#include "BitButton.h"
#include "hexedit.h"
#include "controlgroup.h"

enum {msg_info, msg_notify, msg_alert};

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

private:
	Ui::bits *ui;

	quint64	getBits();
	void init_messageBox();
	void init_bbArray();
	void init_bitSizes();
	void init_heArray();
	void init_invert();
	void init_shiftOp();
	QString& dec2hex(quint64 qiNumber);
	void updateBits(int hexIndex);
	void updateEdits();
	void updateMessageBox();
	void showDecimals(quint64 val);
	void sendMessage(QString& line, int level);
	void showBits();
	void hideBits();

	ControlGroup <QRadioButton> *pBitSizes;
	ControlGroup <QPushButton> *pInvert;
	QFrame *bitSizeFrame;
	ShiftOp *shiftBox;
};

#endif // BITS_H
