#include <hexedit.h>

HexEdit::HexEdit(int hexenum, QWidget *parent, bitfield_t bf) :
	QComboBox(parent)
{
	// All the objects owned by this object must be instantiated here.
	// To expect them to be correctly instantiated outside of this is
	// dangerous.
	//
	hexeditId =	hexenum;
	hexBitField = new bitfield(bf);
	hexeditLabel = new QLabel(parent);
	hexbits = new QRadioButton(parent);
	hexshift = new QRadioButton(parent);

	this->setInsertPolicy(QComboBox::InsertAtTop);
	this->setEditable(true);
	this->hexBitField->setBitField(bf);
	this->lineEdit()->setInputMask(hexBitField->getCurrentBitMask());

	hexbits->setFocusPolicy(Qt::ClickFocus);
	hexshift->setFocusPolicy(Qt::ClickFocus);

	quint64 initVal = 0;
	prevHexVal = ~initVal;
	prevHexStr = "";
	this->updateHexEdit(initVal);

	QFont font;
#ifdef Q_WS_WIN
		font.setPointSize(font.pointSize() + 3);
		font.setFamily("Lucida Console");
#endif
#ifdef Q_WS_X11
		font.setFamily("Monospace");
		font.setPointSize(12);
#endif
	this->setFont(font);
	this->setStyleSheet("color: aqua; background-color: black");
}

HexEdit::~HexEdit(){}

QString& HexEdit::int2hexstr(QString &hexStr, quint64 hexVal)
{
	int width = this->hexBitField->getCurrentHexDigits();
	int binDigits = this->hexBitField->getCurrentBinDigits();

	// Mask off any unused upper bits
	//
	hexVal &= 0xFFFFFFFFFFFFFFFF >> (64 - binDigits);

	hexStr = QString("%1").arg((quint64)hexVal, width, 16, QChar('0'));
	hexStr = hexStr.toUpper();
	return hexStr;
}

quint64 HexEdit::hexstr2int(QString& hexStr)
{
	bool ok;

	hexStr = hexStr.remove('.');
	hexStr = hexStr.trimmed();

	// Convert the string to unsigned 64 bit integer, interpreting the
	// string as base 16 (hexadecimal).
	//
	quint64 hexVal = hexStr.toULongLong(&ok, 16);
	return hexVal;
}

void HexEdit::updateHexEdit(quint64 hexVal)
{
	QString hexStr;

	hexStr = int2hexstr(hexStr, hexVal);
	this->setEditText(hexStr);

	if(hexVal != prevHexVal)
	{
		this->insertItem(0, hexStr);
		prevHexVal = hexVal;
		prevHexStr = hexStr;
	}
}

void HexEdit::updateHexEditBitField(bitfield_t bf)
{
	this->hexBitField->setBitField(bf);
	this->lineEdit()->setInputMask(hexBitField->getCurrentBitMask());
}

bool HexEdit::isNewData( quint64 numData) {return( numData != prevHexVal );}
bool HexEdit::isNewData( QString &strData) {return( strData != prevHexStr );}
