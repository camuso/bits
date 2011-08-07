#ifndef	HEXEDIT_H
#define	HEXEDIT_H

#include <QtCore>
#include <QtCore/QTextStream>
#include <QtGui>
#include <QtDesigner/QDesignerCustomWidgetInterface>
#include <QFontComboBox>
#include <BitButton.h>
#include <bitfield.h>

class HexEdit :	public QComboBox
{
	Q_OBJECT
public:
	HexEdit(int hexenum, QWidget *parent = 0, bitfield_t bf = bit_32);
	~HexEdit();

	bitfield *hexBitField;
	QLabel *hexeditLabel;
	QRadioButton *hexbits;
	QRadioButton *hexshift;
	int	hexeditId;

	void hexstr2int(QString& hexstr, void* intx);
	void int2hexstr(void* intx, QString& hexstr);
	void updateHexEdit(void* intx);
	void updateHexEditBitField(bitfield_t bf);
	bool isNewData(quint64 numData);
	bool isNewData(QString& strData);

signals:

public slots:

private:

	QString prevHexStr;
	quint64 prevHexVal;
};

#endif // HEXEDIT_H
