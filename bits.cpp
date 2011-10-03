/*//////////////////////////////////////////////////////////////////////////////
//
// File     : bits.cpp
// Author   : Tony Camuso
// Date     : 20110730 - 30-Jul-2011
//
// Overview:
// Provides the routines that instantiate and manage the "bits"
// application. Developed with QtSDK.
//
// Licencing: GPL V2
//
// History
//
// 20110730: Initial Entry
//
// Commentary:
//
// Signals and Slots in the QtSDK
//=============================================================================
//
// A signal is an event initiated by an object in the window associated
// with the class, in this case the bits class that inherits from
// QMainWindow.
//
// A slot is the responder to a signal that was sent from the window.
//
// The mapping is usually done magically by something called the
// Meta Object Compiler (MOC). This ccmpiler parses the classes for
// slots that begin with "on_" and maps them to the corresponding
// signal having the same root identifier in the ui_<name> created by
// QDesigner.
//
// For example, in QDesigner, you can right click a QPushButton object
// and select "Go to slot..." from the pop up menu. You will be prompted
// to select an appropriate signal, for example, "clicked()". If the
// QPushButton object has the name "close", for example, QDesigner will
// create a slot called "on_close_click()" in the class that owns the
// "close" QPuxhButton object. The MOC will automatically map the
// clicked signal from that object to the "on_close__click()" slot at
// compile time.
//
// However, explicit mapping can be done in QDesigner. The pop up menu
// from right-click->object in QDesigner also offers an item called
// "Change slots and signals". This option allows you to add custom
// signals and slots.
//
// Then, using the "Signals and Slots" editor at the bottom of the
// QDesigner screen, you can map objects to signals and slots.
//
// QDesigner creates a ui_<name>.h file at compile time. This file cannot
// be edited, because it is destroyed and recreated with every compile.
//
// What I did was to make a copy of this file and include it instead of
// the ui_<name>.h file. In this case, I copied ui_bits.h to bits.ui.h
// and included bits.ui.h instead of ui_bits.h. This allowed me to do
// custom editing of the layout after QDesigner had done the work I needed
// to get started.
//
// When I make edits to the ui with QDesigner, I simply copy them to my
// private copy of the ui header file and customize them at will.
//
//=============================================================================*/

#include "bits.h"

#define Init_1(x) ui->x->
#define Init_2(x,y) ui->x->y->

char const *hexeditNames[hex_array_size] = {"Left", "Right", "Result"};

/*/////////////////////////////////////////////////////////////////////////////
//
// bits app constructor
//
// Instantiate the ui and everythiing on it by calling the respective init
// routines.
*/
bits::bits(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bits)
{
    ui->setupUi(this);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    ui->bbConnectGroup = new QButtonGroup(ui->centralWidget);
    ui->shiftConnectGroup = new QButtonGroup(ui->centralWidget);

    init_messageBox();
    init_bbArray();     // The following sequence must occur in the order
    init_bitSizes();    // : in which it is written. The bit sizes must
    init_heArray();     // : first be determined
    init_invert();      // create the "invert" buttonsCcasd
    init_shiftOp();
    init_calc();
    init_format();
    setAppStyles();
    updateWinSizes();
    showBits();
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits app destructor
//
// Not much to do here. When the UI is destroyed, everything on it will be
// destroyed along with it.
*/
bits::~bits()
{
    delete ui;
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::mapped_bbClick
//
// This is a "slot" that was mapped by the signal mapper. All the clicks from
// the BitButton array are mapped to this one handler.
//
// If a BitButton is pressed, we must:
// . Toggle its state
// . Update the HexEdit box that's connected to the BitButtons by the
//   "BitButtons" QButtonGroup
// . Everytime you click a BitButton, you have new data appear in the
//   associated HexEdit QComboBox, so send the decimal output to the
//   messages area unconditionally.
*/
void bits::mapped_bbClick(int bbId)
{
    int index = ui->bbConnectGroup->checkedId();
    ui->bb[bbId]->bbToggle();

    quint64 qiBitsVal = getBits();
    ui->hexedit[index]->updateHexEdit(qiBitsVal);

    this->showDecimals(qiBitsVal);
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::mapped_hexedit - called on HexEdit Activate signal
//
// All the HexEdit QComboBoxes send their "activated" signals through a
// QSignalMapper to this handler.
//
// If the contents of the HexEdit QComboBox edit area contains new data,
// And the bbRadioButton has selected this HexEdit box to be connected
// Then update the BitButton array and send the equivalent decimal values
// to the messages area.
//
*/
void bits::mapped_hexedit(int index)
{
    ui->hexedit[index]->setFocus();
    QString currentText = QString(ui->hexedit[index]->currentText());

    if(ui->hexedit[index]->isNewData(currentText)
    &&(ui->bbConnectGroup->checkedId() == index)) {
        updateBits(index);
        showDecimals(getBits());
    }
    this->setFocus();
}

/*//////////////////////////////////////////////////////////////////////////////
//
// bits::mapped_bbRadioClick - called on bbRadioButton Click signal
//
// The radio buttons that connect the value of the BitButtons to corresponding
// HexEdit boxes send all their signals through a QSignalMapper to thsis one
// handler.
//
// All that's needed here is to send the current value of the BitButtons to
// the newly selected HexEdit combo box.
*/
void bits::bbRadioClick(int index)
{
    quint64 qiBitsVal = getBits();
    ui->hexedit[ index ]->updateHexEdit(qiBitsVal);
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::mapped_shiftRadioClick
//
// . Connect the corresponding HexEdit box to the ShiftOpGroup.
*/
void bits::shiftRadioClick(int)
{
    // I guess there's really nothing to do here, because the code that
    // will do the shifting will examine the buttonGroup that contains
    // these buttons to see which one is set. The HexEdit box that
    // "owns" that button is the one that will be shifted by the shift
    // operator group.
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::onShift(index) - index is the id of the shift button
//
// . Get the current shift value from the shiftOp control group
// . Get the current index of from the shiftConnectGroup, that is, which
//   of the HexEdit windows is connected to the shifter through its "Shift"
//   radio button.
// . Get the current value of the HexEdit box that's connected to the
//   shift control group through the shift radio button.
// . Shift/Rotate right or left depending on the index value passed to us
//   by the "clicked" signal from the shift control goup.
// . If the shift radio button is connected to the same HexEdit box as the
//   BitButton radio button, then update the bit buttons.
// . Send messages and exit.
*/
void bits::onShift(int index)
{
    int shiftVal = shiftOp->getCurrentShiftVal();
    QString qsShiftVal;
    qsShiftVal.setNum(shiftVal);

    int shiftConnectIndex = ui->shiftConnectGroup->checkedId();
    quint64 hexVal = ui->hexedit[shiftConnectIndex]->getHexVal();

    if (shiftOp->chkRotate->checkState() == Qt::Unchecked)
        hexVal = index == 0 ? hexVal << shiftVal : hexVal >> shiftVal;
    else {  //Rotate
        int binDigits =
            ui->hexedit[shiftConnectIndex]->hexBitField->getCurrentBinDigits();

        if (index == 0) {   // rotate left (ROL)
            quint64 rotMask = (quint64)1 << (binDigits-1);
            for (int j = 0; j < shiftVal; j++) {
                quint64 msb = hexVal & rotMask;
                hexVal <<= 1;
                msb >>= binDigits-1;
                hexVal |= msb;
            }
        }
        else {  // rotate right (ROR)
            for (int j = 0; j < shiftVal; j++) {
                quint64 lsb = hexVal & 1;
                hexVal >>= 1;
                lsb <<= binDigits-1;
                hexVal |= lsb;
            }
        }
    }

    ui->hexedit[shiftConnectIndex]->updateHexEdit(hexVal);

    int bbConnectIndex = ui->bbConnectGroup->checkedId();
    if (shiftConnectIndex == bbConnectIndex)
        updateBits(shiftConnectIndex);

    // Format the result box if it's connected to the formatter
    //
    Twidget *tw = pConnectFormat->getTwidget();
    int fmtIndex = tw->buttonGroup->checkedId();
    if (fmtIndex == hex_result)
        onFormat();

    //QString dirStr = index == 0 ? "Left by " : "Right by ";
    //QString msg = QString("Shifted " % dirStr % qsShiftVal);
    //sendMessage(msg, msg_notify);
    showDecimals(getBits());
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::onCalc
//
// Push Button map
//
//
*/
void bits::onCalc(int index)
{
    quint64 op1 = ui->hexedit[hex_left]->getHexVal();
    quint64 op2 = ui->hexedit[hex_right]->getHexVal();
        quint64 res = 0;

    // Check for attempted division by zero.
    //
    if(op2 == 0 && ((index == calc_div ) || (index == calc_mod))) {
        QString div0 = "Attempted division by zero - no result.";
        sendMessage(div0, msg_alert);
        return;
    }
    switch(index) {
    case calc_and: res = op1 & op2; break;
    case calc_or:  res = op1 | op2; break;
    case calc_xor: res = op1 ^ op2; break;
    case calc_add: res = op1 + op2; break;
    case calc_sub: res = op1 - op2; break;
    case calc_mul: res = op1 * op2; break;
    case calc_div: res = op1 / op2; break;
    case calc_mod: res = op1 % op2; break;
    }

    ui->hexedit[hex_result]->updateHexEdit(res);

    int bbConnectIndex = ui->bbConnectGroup->checkedId();
    if(bbConnectIndex == hex_result)
        this->updateBits(hex_result);

    // Format the result box if it's connected to the formatter
    //
    Twidget *tw = pConnectFormat->getTwidget();
    int fmtIndex = tw->buttonGroup->checkedId();
    if (fmtIndex == hex_result)
        onFormat();

    this->showDecimals(res);
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::kePressEvent
//
// Intercepts keys looking for operators.
//
//
*/
void bits::keyPressEvent(QKeyEvent *event)
{
    //int key = (int)event->key();
    //qDebug() << "key: " << key;
    switch (event->key()) {
    case Qt::Key_Left: onShift(0); break;
    case Qt::Key_Right: onShift(1); break;
    case Qt::Key_Less: onShift(0); break;
    case Qt::Key_Greater: onShift(1); break;
    case Qt::Key_Ampersand: onCalc(calc_and); break;
    case Qt::Key_Bar: onCalc(calc_or); break;
    case Qt::Key_AsciiCircum: onCalc(calc_xor); break;
    case Qt::Key_Plus: onCalc(calc_add); break;
    case Qt::Key_Minus: onCalc(calc_sub); break;
    case Qt::Key_Asterisk: onCalc(calc_mul); break;
    case Qt::Key_Slash: onCalc(calc_div); break;
    case Qt::Key_Percent: onCalc(calc_mod); break;
    default:QWidget::keyPressEvent(event); break;
    }
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::bitSizeClick
//
// The integer that's passed is the index of the clicked radio button in
// the QList that contains the QRadioButtons in this group.
// That index conveniently maps to the bitfield_t defined in bitfield.h
// The bitfield width will control the number of BitButtons presented to
// the user interface and the width of the fields in the HexEdit boxes.
//
// On click, of any button in this group, we must do the following.
// . In a loop, cycle through all the HexEdit boxes and call the
//   hexBitField->setBitField routine.
// . Call updateHexEdit for each of the boxes.
//
*/
void bits::bitSizeClick(int bitSize)
{
    // Even thoug we know that the integer returned maps directly to
    // the correspondig value in the bitfield_t enumeration (bitfield.h)
    // the right thing to do is to parse the value and map it explicitly.
    // Paranoia is good in this business.
    //
    bitfield_t bf;
    switch (bitSize) {
    case 0: bf = bit_8; break;
    case 1: bf = bit_16; break;
    case 2: bf = bit_32; break;
    case 3: bf = bit_64; break;
    default: bf = bit_32; break;
    }

    for (int index = hex_left; index < hex_array_size; index++)
        ui->hexedit[index]->updateHexEditBitField(bf);

    updateWinSizes();
    showBits();
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::onInvert - invert the value in the hex box
//
// . Get the unsigned 64-bit integer value of the hexadecimal string displayed
//   in the hexedit box.
// . Invert the integer
// . Write the result back to the hexedit display.
// . If the hexedit display is also connected to the BitButtons, then update
//   them also.
// . Show the decimal result.
*/
void bits::onInvert(int index)
{
    quint64 hexVal = ui->hexedit[index]->getHexVal();
    hexVal = ~hexVal;
    ui->hexedit[ index ]->updateHexEdit(hexVal);

    int bbConnectIndex = ui->bbConnectGroup->checkedId();
    if(index == bbConnectIndex)
        updateBits(index);

    // Format the combo box if it's connected to the formatter
    //
    onFormat();

    showDecimals(hexVal);
}

/*/////////////////////////////////////////////////////////////////////////////
//
// onFmtClr() - simply clear out the Format Combo box. When the box is clear,
//              no formatting will be done.
//
*/
void bits::onFmtClr()
{
    fmtBox->setEditText("");
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::onFormat - invert the value in the hex box
//
// . See which HexEdit box is connected to the formatter by reading the
//   buttonGroup checked() id in the pConnectFormat ControlGroup.
// . Format the contents of that HexEdit box according to the format
//   specified in the fmtBox (Format ComboBox) and send it to the
//   result HexEdit box.
// . The format defines bit fields delimited by periods. so that,
//   4.5.3.4 defines a 16-bit number to be displayed as bitfields
//   of 4, 5, 3, and 4 bits, separated by periods.
//
*/
void bits::onFormat()
{
    QString fmtStr = fmtBox->currentText();
    Twidget *tw = pConnectFormat->getTwidget();
    int fmtIndex = tw->buttonGroup->checkedId();

    // TO DO:
    // If the fmtIndex == hex_result, then we have to do some thing different.
    // than just grab the text. The format stuff must be subclassed so we can
    // maintain at least a copy of the displayed/formatted value as a quint64.
    // The difficulty is that converting the formatted text back to a quint64
    // is problematic because the number has been broken up into fields.

    quint64 hexVal = ui->hexedit[fmtIndex]->getHexVal();
    int binDigits = ui->hexedit[fmtIndex]->hexBitField->getCurrentBinDigits();

    // If there's nothing in the format string, then use the default format.
    //
    if ((fmtStr == "") || (fmtStr == "0"))
    {
        ui->hexedit[hex_result]->updateHexEdit(hexVal);
        return;
    }

    // Tokenize the Field Widths from the fmtStr
    //
    QList<QString> fwStrList = fmtStr.split(QChar('.'), QString::SkipEmptyParts);

    // Now we have a list of strings that represent the desired field widths.
    // . Iterate backwards through this list, converting each field width
    //   string to an integer. Using java-style iterator provided by QtSDK.
    // . Create a bitmask from the field width and extract the numeric data
    //   from the HexVal.
    // . Convert the extracted numeric data to a hex string and prepend it
    //   to the results QString to build the string that will ultimately be
    //   displayed. Prepend a period to each field.
    // . Shift the HexVal by the field width to put the next bits into position.
    //   If HexVal is zero after the shift, then exit the loop, we're done.
    //
    QString results;
    QListIterator<QString> i(fwStrList);
    int bitCount = binDigits;
    i.toBack();
    while (i.hasPrevious())
    {
        bool ok;
        QString fwStr = i.previous();
        int fldWid = fwStr.toInt(&ok);

        // Check for the validity of the field string.
        //
        if ((ok != true) || (fldWid > binDigits) || (fldWid <= 0))
        {
            QString invalidMsg = fmtStr % " : is not a valid format string.\n";
            sendMessage(invalidMsg, msg_notify);
            return;
        }

        // Limit the field width to the remaining bits that have not been
        // formatted into fields yet.
        //
        fldWid = fldWid > bitCount ? bitCount : fldWid;
        int hexWid = Bits2HexDigits(fldWid);

        quint64 bitMask = (((quint64) 1) << fldWid) - 1;
        quint64 fldData = hexVal & bitMask;
        QString hexFld = QString("%1").arg((quint64)fldData, hexWid, 16, QChar('0'));

        hexFld.prepend(QChar('.'));
        results.prepend(hexFld);

        // If we've done all the bits possible for the current bit size,
        // then quit now.
        //
        bitCount -= fldWid;
        if (bitCount <= 0)
            break;

        // Get the next bits in place.
        //
        hexVal >>= fldWid;
    }

    results.remove(0,1);    // don't need the leading dot for first field

    // Clear the inputMask before sending the format to the results box.
    // It won't display properly otherwise.
    //
    ui->hexedit[hex_result]->lineEdit()->setInputMask("");

    ui->hexedit[hex_result]->setEditText(results);

    // Save the format string in the fmtBox list.
    //
    if (fmtStr != fmtBox->itemText(0))
            fmtBox->insertItem(0, fmtStr);

    // Set the inputMask again so that all other results will be shown
    // formatted correctly.
    //
    //QString mask = ui->hexedit[hex_result]->hexBitField->getCurrentBitMask();
    //ui->hexedit[hex_result]->lineEdit()->setInputMask(mask);
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::updateBits(hexIndex)
//
// This routine updates the bits with the integer value of the hexadecimal
// number displayed in the hexedit box given by the index parameter.
//
// Obtain the value in the HexEdit box indicated by the hexIndex argument.
// Logic upstream from here should have checked that the BitButton array is
// connected to the HexEdit QComboBox indicated by the hexIndex argument.
//
// The string valus is converted to an integer, and each bit of the new
// is OR'd into the corresponding BitButton state variable.
*/
void bits::updateBits(int hexIndex)
{
    quint64 hexVal = ui->hexedit[hexIndex]->getHexVal();
    int binDigits = ui->hexedit[hexIndex]->hexBitField->getCurrentBinDigits();

    // Shift through the hexVal and set or clear the BitButtons accordingly.
    //
    for(int index = 0; index < binDigits; index++) {
        ui->bb[index]->setState(hexVal & 1);
        hexVal >>= 1;
    }
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::showDecimal()
//
// In order to obtain the current bitfield width, that is, whether it's
// 8, 16, 32, or 64 bits, we must first acquite the index of the HexEdit
// QComboBox that is currently connected to the BitButton array.
//
// The HexEdit boxes each maintain their own "bitfield" class, which provides
// the necessary houskeeping for operations that depend on the bitfield
// width of numeric data.
//
// After acguiring the currently employed bitfield width, we cast the current
// value of the BitButton array as that data type to perform the corresponding
// conversion from string to signed integer.
//
// The unsigned conversion requires no such special handling.
*/
void bits::showDecimals(quint64 val)
{
    static quint64 prevHexVal = 0;

    // If nothing's changed, no need to print anything.
    //
    if (val == prevHexVal)
        return;

    prevHexVal = val;

    QString signedVal;
    int index = ui->bbConnectGroup->checkedId();
    bitfield_t bf = ui->hexedit[index]->hexBitField->getCurrentBitField();

    // This may be overkill, but as a wise engineer once commented
    // paranoia is good in this business.
    //
    qint8  sint_8  = (qint8) val & (qint8) 0xFF;
    qint16 sint_16 = (qint16)val & (qint16)0xFFFF;
    qint32 sint_32 = (qint32)val & (qint32)0xFFFFFFFF;
    qint64 sint_64 = (qint64)val & (qint64)0xFFFFFFFFFFFFFFFF;

    switch(bf) {
    case bit_8:  signedVal = QString("%1").arg(sint_8); break;
    case bit_16: signedVal = QString("%1").arg(sint_16); break;
    case bit_32: signedVal = QString("%1").arg(sint_32); break;
    case bit_64: signedVal = QString("%1").arg(sint_64); break;
        //
        // The only reason this is here is thatthe compiler complained that
        // we weren't handling it.
        //
        case bitfield_array_size: return;
    }

    // Let's see if this is a negative number. It's ok if we do it in
    // discrete steps and let the compiler optimize it.
    // Never sacrifice clarity for brevity, and clever is not always
    // smart. Besides, it makes debugging a lot easier. :)
    //
    int bin = ui->hexedit[index]->hexBitField->getCurrentBinDigits();
    quint64 bitMask = (quint64)1 << (bin - 1);
    bool negFlag = val & bitMask ? true : false;

    QString redOpen = negFlag ? "<font color=\"Magenta\">" : "";
    QString redClose = negFlag ? "</font>" : "";

    QString line = "unsigned : " % QString::number(quint64(val));
    line = line % redOpen % " signed : " % signedVal % redClose;
    sendMessage(line, msg_info);
}

/*//////////////////////////////////////////////////////////////////////////////
//
// bits::sendMessage(Qstring& line, int level)
*/
void bits::sendMessage(QString& line, int level)
{
    QTextCursor cursor = ui->messages->textCursor();
    QString alertHtml = "<font color=\"DeepPink\">";
    QString notifyHtml = "<font color=\"Lime\">";
    QString infoHtml = "<font color=\"Aqua\">";
    QString endHtml = "</font><br>";

    switch(level) {
        case msg_alert: line = alertHtml % line; break;
        case msg_notify: line = notifyHtml % line; break;
        case msg_info: line = infoHtml % line; break;
        default: line = infoHtml % line; break;
    }

    line = line % endHtml;
    ui->messages->insertHtml(line);
    cursor.movePosition(QTextCursor::End);
    ui->messages->setTextCursor(cursor);
    //ui->messages->append(line);
}

/*//////////////////////////////////////////////////////////////////////////////
//
// bits::getBits
*/
quint64 bits::getBits()
{
    quint64 bitVal = 0;

    int hexIndex = ui->bbConnectGroup->checkedId();
    int binDigits = ui->hexedit[hexIndex]->hexBitField->getCurrentBinDigits();

    for(int index = 0; index < binDigits; index++) {
        quint64 bbState = ui->bb[index]->bbState;
        bitVal |= (bbState << index);
    }
    return bitVal;
}

/*//////////////////////////////////////////////////////////////////////////////
//
// bits::showBits(index)
*/
void bits::showBits()
{
    int hexIndex = ui->bbConnectGroup->checkedId();
    int binDigits = ui->hexedit[hexIndex]->hexBitField->getCurrentBinDigits();

    for(int index = 0; index < BITS; index++) {
        if(index < binDigits) {
            ui->bb[index]->show();
            ui->bbLabel[index]->show();
        }
        else {
            ui->bb[index]->hide();
            ui->bbLabel[index]->hide();
        }
    }
}

/*//////////////////////////////////////////////////////////////////////////////
//
// bits::updateWinSizes
*/
void bits::updateWinSizes()
{
    // Frame dimensions
    //
    int hexIndex = ui->bbConnectGroup->checkedId();
    int binDigits = ui->hexedit[hexIndex]->hexBitField->getCurrentBinDigits();
    bool large = binDigits > 32 ? true : false;

    const int fx = 14;
    const int fy = 190;
    const int fw = 690;
    int fh = large ? 110 : 60;
    int wh = large ? MAINWINDOW_H + 50: MAINWINDOW_H;

    ui->bbFrame->setGeometry(QRect(fx, fy, fw, fh));
    this->resize(MAINWINDOW_W, wh);
    showDecimals(getBits());
    updateMessageBox();
}

/*//////////////////////////////////////////////////////////////////////////////
//
// bits::init_bbArray
*/
void bits::init_bbArray()
{
    // Instantiate a QSignalMapper to map the many BitButton signals to s
    // single slot in the bits class.
    //
    // See http://doc.qt.nokia.com/qq/qq10-signalmapper.html for in-depth
    // explanaation.
    //
    QSignalMapper *bbMapper = new QSignalMapper(this);

    // The following line connects the bbMapper's one signal to the one and
    // only slot in this object (bits) that we want to respond to the many
    // BitButton objects when they are clicked.
    //
    connect(bbMapper, SIGNAL(mapped(int)), this, SLOT(mapped_bbClick(int)));

    ui->bbFrame = new QFrame(ui->centralWidget);
    ui->bbFrame->setObjectName(QString::fromUtf8("frame"));
    ui->bbFrame->setFrameShape(QFrame::StyledPanel );
    //ui->bbFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    //ui->bbFrame->setFrameShadow(QFrame::Raised);
    //ui->bbFrame->show();

    for (int index = 0; index < BITS; index++)
    {
        // BitButton
        // =========
        // Calculate the X and Y coordinates of the BitButton.
        // Have fun trying to remember how this works.
        //
        // NOTE:
        // When the bits application starts, the default focus will be on
        // bb[0], because it is the first widget created. Tab focus will
        // progress through each of the BitButtons, whose state can be
        // changed with the space key.
        //
        int bbw = BB_W + BB_MARGIN;
        int row = index / COLS;
        int col = index % COLS;
        int nybMargin = (col / 4) * NYB_MARGIN;
        int bb_x = BB_X - (col * bbw) - nybMargin;
        int bb_y = BB_Y + (row * ROW_MARGIN);

        QString bbName = "bb_" % QString::number(index);

        // Instantiate the BitButton and connect it to the ui (User
                // Interface) which is the Main Window.
        //
        ui->bb[index] = new BitButton(index, ui->centralWidget);
        ui->bb[index]->setObjectName(bbName);
        ui->bb[index]->setGeometry(QRect(bb_x, bb_y, BB_W, BB_H));
        //ui->bb[index]->set

        // BitButton Label
        // ===============
        // Instantiate the bbLabel object and give it an object name
        //
        ui->bbLabel[index] = new QLabel(ui->centralWidget);
        QString bbLabelName = "bb_label_" % QString::number(index);
        ui->bbLabel[index]->setObjectName(bbLabelName);

        // Calculate the x and y coordinates of the BitButton's label
                // based on the x and y coordinates of the BitButton
        //
        int bb_label_x = bb_x + 3;
        int bb_label_y = bb_y - BB_H +  (row * 2 * BB_H);

        // Place the bbLabel onto the ui (user interface), set its Font, and
        // write the text into the label.
        //
        ui->bbLabel[index]->setGeometry(
            QRect(bb_label_x, bb_label_y, BB_LABEL_W, BB_LABEL_H));
        ui->bbLabel[index]->setFont(QFont("Arial", 9, QFont::Bold));
        ui->bbLabel[index]->setText(QString("%1").arg(index, 2));

        // BitButton Signal Mapping
        // ========================
        // Set the mapping index of this BitButton
        //
        bbMapper->setMapping(ui->bb[index], index);

        // Connect the BitButton bbClicked signal to the bbMapper's one and
        // only slot, mapp().
        //
        connect(ui->bb[index], SIGNAL(clicked()), bbMapper, SLOT(map()));
    }
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::init_heArray
//
// Init the array of Hexadicimal combo edit boxes.
//
// The HexEdit subclass of QComboBox has these other classes in it and will
// instantiate them when it's constructor function is invoked.
//
// HexEdit      - The QComboBox itself
// HexBitField  - generic C++ class for managing bitfield widths
// HexeditLabel - QLabel that provides a label for the HexEdit combo box
// hexbits      - QRadioButtonthat the HexEdit box to the BitButton objects.
// hexshift     - QRadioButton that connects the HexEdit to a shift button
//
// Use this macro to make it easier to read and fit on one line.
// The Init_1 macro is defined at the top of this file and goes
// out of scope at the end of the file.
*/
#define HEXMAC(x) Init_1(hexedit[index])x

void bits::init_heArray()
{
    // Create a signal mapper to connect the signals from the three
    // HexEdit combo boxes to one slot in the bits app.
    //
    QSignalMapper *hexeditMapper = new QSignalMapper(this);
    connect(hexeditMapper, SIGNAL(mapped(int)),
            this, SLOT(mapped_hexedit(int)));

    for(int index = hex_left; index < hex_array_size; index++)
    {
        QString NameSubstr = QString(hexeditNames[index]);

        int left_x = HE_LEFT_X + ((HE_SKIP_W + HE_W)* index);
        int left_y = HE_LEFT_Y;
        QString ObjName = "he_" % NameSubstr;

        // Default is 32 bits
        //
        ui->hexedit[index] = new HexEdit(index, ui->centralWidget, bit_32);
        ui->hexedit[index]->setObjectName(ObjName);
        ui->hexedit[index]->setGeometry(QRect(left_x, left_y, HE_W, HE_H));

        left_y = HE_LEFT_Y - 24;
        ObjName = "he_label_" % NameSubstr;
        QString LabelName = index < hex_result ? " Operand" : "";
        HEXMAC(hexeditLabel)->setObjectName(ObjName);
        HEXMAC(hexeditLabel)->setGeometry(QRect(left_x, left_y, HE_W, HE_H));
        HEXMAC(hexeditLabel)->setText(NameSubstr % LabelName);

        // For the Radio Buttons, the FocusPolicy will be Click only.
        // Tab focus is useles, unless we want to create an event hander
        // to use kestrokes to change Radio Button state.
        // Not yet ...
        //
        left_y = HE_LEFT_Y + 27;
        ObjName = "he_bits_rb_" % NameSubstr;
        HEXMAC(hexbits)->setObjectName(ObjName);
        HEXMAC(hexbits)->setGeometry(QRect(left_x, left_y, HE_W, HE_H));
        HEXMAC(hexbits)->setText(QString("Bits"));

        left_y += 17;
        ObjName = "he_shift_rb_" % NameSubstr;
        HEXMAC(hexshift)->setObjectName(ObjName);
        HEXMAC(hexshift)->setGeometry(QRect(left_x, left_y, HE_W, HE_H));
        HEXMAC(hexshift)->setText(QString("Shift"));

        ui->bbConnectGroup->addButton(HEXMAC(hexbits), index);
        ui->shiftConnectGroup->addButton(HEXMAC(hexshift), index);

        // Connect the "activated" signal from the individual HexEdit
        // boxes to the hexedit signal Mapper
        //
        hexeditMapper->setMapping(ui->hexedit[index], index);
        connect(ui->hexedit[index], SIGNAL(activated(int)),
                hexeditMapper, SLOT(map()));
    }

    // We don't want the Result window to be editable, because it is an
    // output window, not an input window.
    //
    // Note:
    //    setEditable(false) grays-out the edit box! So, we can't use
    //                       that.
    //
    //    setEnabled(false) allows us to see the data in the box, but
    //                      does not allow us to select and copy
    //
    // However, the mechanism for editing lines in the QComboBox class is
    // derived from the following declaration in that class's header file.
    //
    //    QLineEdit lineEdit() const;
    //
    // This allows us to access the public functions provided by the
    // QLineEdit class, one of which is setReadOnly. So with the
    // following declaration we avail ourselves of the QLineEdit::setReadOnly()
    // function and apply that attribute to our Result HexEdit Box.
    //
    ui->hexedit[hex_result]->lineEdit()->setReadOnly(true);
    ui->hexedit[hex_result]->lineEdit()->setInputMask("");
    ui->hexedit[hex_result]->updateHexEditBitField(bit_32);

    // Set the Right HexEdit to gain focus from a tab pressed in the
    // Left HexEdit. From there, it will proceed to the Bit Buttons.
    //
    this->setTabOrder(ui->hexedit[hex_left],
                      ui->hexedit[hex_right]);

    // Set the left radio buttons checked at init time.
    //
    ui->hexedit[hex_left]->hexbits->setChecked(true);
    ui->hexedit[hex_left]->hexshift->setChecked(true);

    // Map the Radio Button group signals to their respective slots.
    // See bits.h
    //
    connect(ui->bbConnectGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(bbRadioClick(int)));
    connect(ui->shiftConnectGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(shiftRadioClick(int)));
}
#undef HEXMAC

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::init_bitSizes
//
// Initialize the radio buttons that will set the bit field width.
//
// An instance of the ControlGroup template is invoked here to do all
// the dirty work, including mapping the signals to a QSignalMapper.
// When it's all done, we'll access the signal mapper to map its
// output (the mapped() function of the QSignalMapper to a slot in
// this appliation.
*/
void bits::init_bitSizes()
{
    // Initialize a Twidget class to convey all the information needed
    // by the ControlGroup to create the button control group.
    // See controlgroup.h
    //
    const int x = X_START;      // x-coordinate
    const int y = Y_START + 76; // y-coordinate
    const int i = 17;           // vert span
    const int w = 70;
    const int h = 20;

    LAYOUT(x,y,0,i);            // x-coord, y-coord, horiz-span, vert-span
    Twidget *tw = new Twidget;
    tw->objName = "bitsizes";
    tw->objText << "8-bit" << "16-bit" << "32-bit" << "64-bit";
    tw->sizes   << QSize(w, h);
    tw->layout  << POINT(0,0) << POINT(0,1) << POINT(0,2) << POINT(0,3);
    tw->grouped = true;

    // . Create the new ControlGroup for bit width
    // . Set the 32-bit width as the default, which is widget[2] in
    //   the QList of button objects, widgetList.
    // . Connect the output of the QSignalMapper that was created by
    //   the ControlGroup class to the slot (handler) in this class.
    // . Set the 32-bit button as the default.
    //
    pBitSizes = new ControlGroup <QRadioButton>(tw, ui->centralWidget);
    pBitSizes->widgetList[2]->setChecked(true); // 32-bit button default
    connect(tw->buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(bitSizeClick(int)));
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::init_invert
*/
void bits::init_invert()
{
    const int x = X_START + 180;        // x-coordinate
    const int y = Y_START - 24;         // y-coordinate
    const int inc = HE_W + HE_SKIP_W;   // horizontal span
    const int w = 20;
    const int h = 20;
    LAYOUT(x, y, inc, 0)                // x, y, horiz-span, vertical-span
    Twidget *tw = new Twidget;
    tw->objName = "invert";
    tw->objText << "~" << "~" << "~";
    tw->sizes   << QSize(w, h);
    tw->layout  << POINT(0,0) << POINT(1,0) << POINT(2,0);
    tw->grouped = true;
    pInvert = new ControlGroup <QPushButton>(tw, ui->centralWidget);
    connect(tw->buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(onInvert(int)));
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::init_shiftOp
*/
void bits::init_shiftOp()
{
    const int x = X_START + 146;
    const int y = Y_START + 78;
    QPoint start = QPoint(x,y);
    shiftOp = new ShiftOpGroup(&start, ui->centralWidget);
    connect(shiftOp, SIGNAL(shift(int)), this, SLOT(onShift(int)));
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::init_calc
*/
void bits::init_calc()
{
    const int x = X_START + 146 + 152;  // x-coordinate
    const int y = Y_START + 76;         // y-coordinate
    const int w = 24;                   // horizontal span
    const int h = 24;                   // vertical span

    LAYOUT(x, y, w, h)                  // x, y, horiz-span, vertical-span
    Twidget *tw = new Twidget;
    tw->objName = "calc";
    tw->sizes   << QSize(w, h);
    tw->grouped = true;
    tw->objText << "&&"  << "|"  << "^"
                << "+"           << "-"
                << "*"   << "/"  << "%";
    tw->layout  << POINT(0, 0) << POINT(1, 0) << POINT(2, 0)
                << POINT(0, 1)                << POINT(2, 1)
                << POINT(0, 2) << POINT(1, 2) << POINT(2, 2);

    pCalc = new ControlGroup <QPushButton>(tw, ui->centralWidget);
    connect(tw->buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(onCalc(int)));
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::init_format
//
// The formatter takes input from a QComboBox and uses it to format one of the
// HexEdit comboboxes according to user input. The format utility allows the
// user to separate data into bitfields for easier analysis.
//
// This utility consists of three readio buttons to connect one of the three
// HexEdit comboboxes to the formatter, the combo box, where the user can
// enter the desired format, and a pushbutton to execute.
*/
void bits::init_format()
{
    int x = X_START + 100;      // x-coordinate
    int y = Y_START + 34;           // y-coordinate
    int inc = HE_W + HE_SKIP_W; // horizontal span
    LAYOUT(x,y,inc,0)

    int w = 100;
    int h = 20;

    Twidget *tw = new Twidget;
    tw->objName = "formatter";
    tw->grouped = true;
    tw->objText << "Formatter";
    tw->sizes   << QSize(w, h);
    tw->layout  << POINT(0, 0) << POINT(1, 0) << POINT(2, 0);

    pConnectFormat = new ControlGroup <QRadioButton>(tw, ui->centralWidget);

    // Set the default formatter connection to the left hex window, or
    // "left operand". No need to map a slot for any change in checked
    // status, because the "Format" pushbutton will determine which of
    // the "Formatter" radio buttons is checked.
    //
    pConnectFormat->widgetList[hex_left]->setChecked(true);

    x = X_START + 2*HE_W + 2*HE_SKIP_W;
    y = Y_START + 75;
    w = HE_W;
    h = HE_H;
    fmtBox = new QComboBox(ui->centralWidget);
    fmtBox->setGeometry(QRect(x, y, w, h));
    fmtBox->setEditable(true);
    fmtBox->setInsertPolicy(QComboBox::InsertAtTop);
    fmtBox->setMaxCount(32);

    fmtCmd = new QPushButton(ui->centralWidget);
    y += 34;
    w = w/2 - 1;
    fmtCmd->setGeometry(QRect(x, y, w, h));
    fmtCmd->setText(QString("&Format"));
    connect(fmtCmd, SIGNAL(clicked()), this, SLOT(onFormat()));
    connect(fmtBox, SIGNAL(activated(int)), this, SLOT(onFormat()));

    fmtClr = new QPushButton(ui->centralWidget);
    x += w + 1;
    fmtClr->setGeometry(QRect(x, y, w, h));
    fmtClr->setText(QString("&Clear"));
    connect(fmtClr, SIGNAL(clicked()), this, SLOT(onFmtClr()));
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::init_messageBox
*/
void bits::init_messageBox()
{
    // Initialize the message box.
    // I created this for debugging, but it has proved useful for
    // informationals messages.
    //
    ui->messages = new QTextEdit(ui->centralWidget);

    QFont font;
#ifdef Q_WS_WIN
    font.setPointSize(font.pointSize() + 4);
    font.setFamily("Consolas");
#endif
#ifdef Q_WS_X11
    font.setFamily("Monospace");
    font.setPointSize(12);
#endif
    ui->messages->setFont(font);
    ui->messages->setStyleSheet("color: aqua; background-color: black");

    // Leave the window Read Only, so users can copy contents.
    // The setEnabled(false) call not only makes it read-only, but you cannot
    // copy from it.
    //
    //ui->messages->setEnabled(false);
    ui->messages->setReadOnly(true);
    ui->messages->setFocusPolicy(Qt::ClickFocus);
    ui->messages->setUpdatesEnabled(true);
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::updateMessageBox()
//
// Simply re-size the dimensions of the Message Box.
*/
void bits::updateMessageBox()
{
    int index = ui->bbConnectGroup->checkedId();
    int binDigits = ui->hexedit[index]->hexBitField->getCurrentBinDigits();
    const int mx = BB_X - 655;
    int my = binDigits <= 32 ? BB_Y + 40 : BB_Y + 88;
    const int mw = 690;
    const int mh = 80;

    ui->messages->hide(); // Makes the transition cleaner
    ui->messages->setGeometry(QRect(mx, my, mw, mh));
    ui->messages->show();
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::setPushButtonStyle
//
// This will set the style for all of the QPushButtons in this application.
// Subclasses can override this style with style declarations of their own.
//
*/
void bits::setAppStyles()
{
    // Style sheet must be one big string.
    //
    QString style =
        "QPushButton {"
            "color: black; "
            "background-color: "
                "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                "stop: 0 #eaebfe, stop: 1 #76878a); "
            "border-style:outset;border-width:1px;"
            "border-radius4px;border-color:gray; }"

        "QPushButton:pressed{"
            "color: white; "
            "background-color: "
                "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                "stop: 0 #08080a, stop: 1 #66777a); "
            "border:6pxsolidwhite; "
            "border-style:inset;border-width:1px;"
            "border-radius6px;border-color:white;}"

        "QLineEdit {"
            "color: black; "
            "border-style:inset;border-width:2px;"
            "border-radius4px;border-color:gray; }"

        "QComboBox {"
            "color: aqua; "
            "background-color: black; "
            "border-style:inset;border-width:2px;"
            "border-radius4px;border-color:gray; }"

        "QTextEdit {"
            "color: aqua; "
            "background-color: black; "
            "border-style:inset;border-width:2px;"
            "border-radius4px;border-color:gray; }"
            ;

    setStyleSheet(style);
}

QString getSystem()
{
#ifdef Q_WS_X11
return QString("Linux");
#endif

#ifdef Q_WS_MAC
return QString("Mac");
#endif

#ifdef Q_WS_QWS
return QString("Embedded Linux");
#endif

#ifdef Q_WS_WIN
return QString("Windows");
#endif

}

#undef Init_1
#undef Init_2
