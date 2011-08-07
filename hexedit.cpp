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
	this->updateHexEdit((void*)&initVal);

	QFont font;
#ifdef Q_WS_WIN
		font.setPointSize(font.pointSize() + 4);
		font.setFamily("Consolas");
#endif
#ifdef Q_WS_X11
		font.setFamily("Monospace");
		font.setPointSize(12);
#endif
	this->setFont(font);
	this->setStyleSheet("color: aqua; background-color: black");
}

HexEdit::~HexEdit(){}

void HexEdit::int2hexstr(void* intx, QString& hexstr)
{
	int width = this->hexBitField->getCurrentHexDigits();
	quint64 anint = *(quint64*)intx;
	hexstr = QString("%1").arg(anint, width, 16, QChar('0'));
	hexstr = hexstr.toUpper();
}

void HexEdit::hexstr2int(QString& hexstr, void* intx)
{
	int width = this->hexBitField->getCurrentHexDigits();
	bool ok;
	quint64 anint = hexstr.toULongLong(&ok, width);
	*(quint64*)intx = anint;
}

void HexEdit::updateHexEdit(void* intx)
{
	quint64 anint = *(quint64*)intx;
	QString hexstr;
	int2hexstr(intx, hexstr);
	this->setEditText(hexstr);

	if(anint != prevHexVal)
	{
		this->insertItem(0, hexstr);
		prevHexVal = anint;
		prevHexStr = hexstr;
	}
}

void HexEdit::updateHexEditBitField(bitfield_t bf)
{
	this->hexBitField->setBitField(bf);
	this->lineEdit()->setInputMask(hexBitField->getCurrentBitMask());
}

bool HexEdit::isNewData( quint64 numData) {return( numData != prevHexVal );}
bool HexEdit::isNewData( QString &strData) {return( strData != prevHexStr );}
