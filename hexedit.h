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

	quint64 hexstr2int(QString& hexstr);
	QString& int2hexstr(QString &hexStr, quint64 hexVal);
	void updateHexEdit(quint64 hexVal);
	void updateHexEditBitField(bitfield_t bf);
	bool isNewData(quint64 numData);
	bool isNewData(QString& strData);
	quint64 getHexVal();
	quint64 maskHexVal(quint64 hexVal);

signals:

public slots:

private:

	QString prevHexStr;
	quint64 prevHexVal;
};

#endif // HEXEDIT_H
