#include "hexedit.h"

/*/////////////////////////////////////////////////////////////////////////////
//
// HexEdit::HexEdit - constructor
//
*/
HexEdit::HexEdit(int hexenum, QWidget *parent, bitfield_t bf) :
    QComboBox(parent)
{
    // All the objects owned by this object must be instantiated here.
    // To expect them to be correctly instantiated outside of this is
    // dangerous. Ask me how I found that out.
    //
    hexeditId = hexenum;
    hexBitField = new bitfield(bf);
    hexeditLabel = new QLabel(parent);
    hexbits = new QRadioButton(parent);
    hexshift = new QRadioButton(parent);

    this->setInsertPolicy(QComboBox::InsertAtTop);
    this->setEditable(true);
    this->setMaxCount(32);
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

/*/////////////////////////////////////////////////////////////////////////////
//
// int2hexstr (QString& hexStr, quint64 hexVal)
//
// Converts an unsigned 64-bit integer to a string expressed as a
// hexadecimal number using the enigmatic QString::arg() member
// function. It's not too easy to understand the syntax of this
// thing, but once you get it right, it's rock solid. No more
// cryptic than printf, really.
//
// One thing that will bite you is that the "width" parameter is
// a minimum width, not a maximum, so unless you mask off the
// upper bits of the number, you won't get what you expected if
// you're looking for bitfields less than 64-bit.
*/
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

/*/////////////////////////////////////////////////////////////////////////////
//
// hexstr2int (QString& hexStr)
//
// Convert a string representing a hexidecimal number into an unsigned
// 64-bit integer using the somewhat cryptic QString member function,
// QString::toUlongLong(). Still beats doing it yourself, and once you
// get it right, you don't have to fool with it again.
//
*/
quint64 HexEdit::hexstr2int(QString& hexStr)
{
    bool ok;

    hexStr = hexStr.remove('.');
    hexStr = hexStr.trimmed();

    // Convert the string to unsigned 64 bit integer, interpreting the
    // string as base 16 (hexadecimal).
    //
    quint64 hexVal = hexStr.toULongLong(&ok, 16);
    hexVal = maskHexVal(hexVal);
    return hexVal;
}

/*/////////////////////////////////////////////////////////////////////////////
//
// updateHexEdit (quint64 hexVal)
//
// Updates the string in the ComboBox window with a string that represents
// the unsigned 64-bit number passed as "quint64 hexVal". The string will
// be formatted to the correct bitfield width by the call to int2hexstr().
//
// Additionally, if the value being passed as an argument is different
// than the previous value being displayed, we will save the item in
// the ComboBox drop list and save the new string and number values to
// be compared with the value passed to this routine the next time it's
// called.
//
// The reason we update the contents of the ComboBox display (currentText)
// on every call, is that we might be getting called because of a change
// in bitfield width, where there is no change in the value of the data,
// but there is a change in the way it must be displayed.
//
*/
void HexEdit::updateHexEdit(quint64 hexVal)
{
    QString hexStr;

    hexVal = maskHexVal(hexVal);
    hexStr = int2hexstr(hexStr, hexVal);
    this->setEditText(hexStr);

    if(hexVal != prevHexVal)
    {
        this->insertItem(1, hexStr);
        prevHexVal = hexVal;
        prevHexStr = hexStr;
    }
}

/*/////////////////////////////////////////////////////////////////////////////
//
// updateHexEditBitField (bitfiedl_t bf)
//
// This function is called whenever the user has changed the size of the
// bitfield. In order to make the transition without having junk appear
// in the combo box window.
// . Read the current contents of the ComboBox window
// . Update the bitfield width parameters, including the input mask.
// . Convert to a hex value the contents we read from the ComboBox
//   at the beginning of the procedure.
// . Update the contents of the ComboBox with the new string, which was
//   created from the previous number, but is now in the context of the
//   new bitfield width.
*/
void HexEdit::updateHexEditBitField(bitfield_t bf)
{
    QString hexStr = this->currentText();
    this->hexBitField->setBitField(bf);
    this->lineEdit()->setInputMask(hexBitField->getCurrentBitMask());
    quint64 hexVal = this->hexstr2int(hexStr);
    this->updateHexEdit(hexVal);
}

quint64 HexEdit::getHexVal()
{
    QString hexStr = this->currentText();
    quint64 hexVal = this->hexstr2int(hexStr);
    return hexVal;
}

quint64 HexEdit::maskHexVal(quint64 hexVal)
{
    quint64 bitMask = 0xFFFFFFFFFFFFFFFF;
    bitMask >>= (64 - this->hexBitField->getCurrentBinDigits());
    quint64 newVal = hexVal & bitMask;
    return newVal;
}

bool HexEdit::isNewData( quint64 numData) {return( numData != prevHexVal );}
bool HexEdit::isNewData( QString &strData) {return( strData != prevHexStr );}
