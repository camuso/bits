/*//////////////////////////////////////////////////////////////////////////////
//
// File		: bits.cpp
// Author	: Tony Camuso
// Date		: 20110730 - 30-Jul-2011
//
// Overview:
// Provides	the	routines that instantiate and manage the "bits"
// application.	Developed with QtSDK.
//
// Licencing: GPL V2
//
// History
//
// 20110730: Initial Entry
//
// Commentary:
//
// Signals and Slots in	the	QtSDK
//=============================================================================
//
// A signal	is an event	initiated by an	object in the window associated
// with	the	class, in this case	the	bits class that	inherits from
// QMainWindow.
//
// A slot is the responder to a	signal that	was	sent from the window.
//
// The mapping is usually done magically by	something called the
// Meta	Object Compiler	(MOC). This	ccmpiler parses	the	classes	for
// slots that begin	with "on_" and maps	them to	the	corresponding
// signal having the same root identifier in the ui_<name> created by
// QDesigner.
//
// For example,	in QDesigner, you can right	click a	QPushButton	object
// and select "Go to slot..." from the pop up menu.	You	will be	prompted
// to select an	appropriate	signal,	for	example, "clicked()". If the
// QPushButton object has the name "close",	for	example, QDesigner will
// create a	slot called	"on_close_click()" in the class	that owns the
// "close" QPuxhButton object. The MOC will	automatically map the
// clicked signal from that	object to the "on_close__click()" slot at
// compile time.
//
// However,	explicit mapping can be	done in	QDesigner. The pop up menu
// from	right-click->object	in QDesigner also offers an	item called
// "Change slots and signals". This	option allows you to add custom
// signals and slots.
//
// Then, using the "Signals	and	Slots" editor at the bottom	of the
// QDesigner screen, you can map objects to	signals	and	slots.
//
// QDesigner creates a ui_<name>.h file	at compile time. This file cannot
// be edited, because it is	destroyed and recreated	with every compile.
//
// What	I did was to make a	copy of	this file and include it instead of
// the ui_<name>.h file. In	this case, I copied	ui_bits.h to bits.ui.h
// and included	bits.ui.h instead of ui_bits.h.	This allowed me	to do
// custom editing of the layout	after QDesigner	had	done the work I	needed
// to get started.
//
// When	I make edits to	the	ui with	QDesigner, I simply	copy them to my
// private copy	of the ui header file and customize them at will.
//
//=============================================================================*/

//#include "ui_bits.h"
#include "bits.h"

#define Init_1(x) ui->x->
#define Init_2(x,y) ui->x->y->

char const *hexeditNames[hex_array_size] = {"Left", "Right", "Result"};

/*/////////////////////////////////////////////////////////////////////////////
//
// bits	app	constructor
//
// Instantiate the ui and everythiing on it by calling the respective init
// routines.
*/
bits::bits(QWidget *parent)	:
	QMainWindow(parent),
	ui(new Ui::bits)
{
	ui->setupUi(this);

	ui->bbConnectGroup = new QButtonGroup(ui->centralWidget);
	ui->shiftConnectGroup = new QButtonGroup(ui->centralWidget);

	init_messageBox();
	init_bbArray();		// The following sequence must occur in the order
	init_bitSizes();	// : in which it is written. The bit sizes must
	init_heArray();		// : first be determined
	init_invert();		// create the "invert" buttonsCcasd
	init_shiftOp();
	updateWinSizes();
	showBits();

}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits	app	destructor
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
	&&(ui->bbConnectGroup->checkedId() == index))
	{
		updateBits(index);
		showDecimals(getBits());
	}
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
*/
void bits::shiftRadioClick(int)
{

}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::bitSizeClick
//
// The integer that's returned is the index of the clicked radio button int
// the QList that contains it and the other QRadioButtons in this group.
// That index conveniently maps to the bitfield_t defined in bitfield.h
// The bitfield width will control the number of BitButtons presented to
// the user interface and the width of the fields in the HexEdit boxes.
//
// On click, of any button in this group, we must do the following.
// . In a loop, cycle through all the HexEdit boxes and call the
//   hexBitField->setBitField routine.
//.  Call updateHexEdit for each of the boxes.
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
	switch (bitSize)
	{
	case 0: bf = bit_8; break;
	case 1: bf = bit_16; break;
	case 2: bf = bit_32; break;
	case 3: bf = bit_64; break;
	default: bf = bit_32; break;
	}

	//quint64 qiBitsVal = getBits();
	for (int index = 0; index < hex_array_size; index++)
	{
		ui->hexedit[index]->updateHexEditBitField(bf);
		//ui->hexedit[index]->updateHexEdit(qiBitsVal);
	}
	updateWinSizes();
	showBits();
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::onInvert - invert the value in the hex box
//
*/
void bits::onInvert(int index)
{
	QString hexStr = ui->hexedit[index]->currentText();
	quint64 hexVal = ui->hexedit[index]->hexstr2int(hexStr);
	int bbConnectIndex = ui->bbConnectGroup->checkedId();
	quint64 bitMask = 0xFFFFFFFFFFFFFFFF;

	bitMask >>= (64 - ui->hexedit[index]->hexBitField->getCurrentBinDigits());
	hexVal = ~hexVal;
	hexVal &= bitMask;
	ui->hexedit[ index ]->updateHexEdit(hexVal);
	if(index == bbConnectIndex)
		updateBits(index);
	showDecimals(hexVal);
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::updateBits(index)
//
// Obtain the new value in the HexEdit box indicated by the hexIndex argument.
// Logic upstream from here should have checked that the BitButton array is
// connected to the HexEdit QComboBox indicated by the hexIndex argument.
//
// The string valus is converted to an integer, and each bit of the new
// is OR'd into the corresponding BitButton state variable.
*/
void bits::updateBits(int hexIndex)
{
	QString hexStr = ui->hexedit[hexIndex]->currentText();
	quint64 hexVal = ui->hexedit[hexIndex]->hexstr2int(hexStr);
	int binDigits = ui->hexedit[hexIndex]->hexBitField->getCurrentBinDigits();

	// Shift through the hexVal and set or clear the BitButtons accordingly.
	//
	for(int index = 0; index < binDigits; index++)
	{
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

	switch(bf)
	{
	case bit_8:  signedVal = QString("%1").arg(sint_8); break;
	case bit_16: signedVal = QString("%1").arg(sint_16); break;
	case bit_32: signedVal = QString("%1").arg(sint_32); break;
	case bit_64: signedVal = QString("%1").arg(sint_64); break;
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

	QString	line = "unsigned : " % QString::number(quint64(val));
	line = line	% redOpen % " signed : " % signedVal % redClose;
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

	switch(level)
	{
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
quint64	bits::getBits()
{
	quint64	bitVal = 0;

	int hexIndex = ui->bbConnectGroup->checkedId();
	int binDigits = ui->hexedit[hexIndex]->hexBitField->getCurrentBinDigits();

	for(int index = 0; index < binDigits; index++)
	{
		quint64	bbState	= ui->bb[index]->bbState;
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

	for(int index = 0; index < BITS; index++)
	{
		if(index < binDigits)
		{
			ui->bb[index]->show();
			ui->bbLabel[index]->show();
		}
		else
		{
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
	updateMessageBox();
	showDecimals(getBits());
}


/*//////////////////////////////////////////////////////////////////////////////
//
// bits::init_bbArray
*/
void bits::init_bbArray()
{
	// Instantiate a QSignalMapper to map the many BitButton signals to	s
	// single slot in the bits class.
	//
	// See http://doc.qt.nokia.com/qq/qq10-signalmapper.html for in-depth
	// explanaation.
	//
	QSignalMapper *bbMapper	= new QSignalMapper(this);

	// The following line connects the bbMapper's one signal to	the	one	and
	// only	slot in	this object	(bits) that	we want	to respond to the many
	// BitButton objects when they are clicked.
	//
	connect(bbMapper, SIGNAL(mapped(int)), this, SLOT(mapped_bbClick(int)));

	ui->bbFrame = new QFrame(ui->centralWidget);
	ui->bbFrame->setObjectName(QString::fromUtf8("frame"));
	ui->bbFrame->setFrameShape(QFrame::StyledPanel );
	//ui->bbFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	//ui->bbFrame->setFrameShadow(QFrame::Raised);
	//ui->bbFrame->show();

	for	(int index = 0;	index <	BITS; index++)
	{
		// BitButton
		// =========
		// Calculate the X and Y coordinates of	the	BitButton.
		// Have	fun	trying to remember how this	works.
		//
		// NOTE:
		// When the bits application starts, the default focus will be on
		// bb[0], because it is the first widget created. Tab focus will
		// progress through each of the BitButtons, whose state can be
		// changed with the space key.
		//
		int bbw = BB_W + BB_MARGIN;
		int	row	= index	/ COLS;
		int	col	= index	% COLS;
		int	nybMargin =	(col / 4) *	NYB_MARGIN;
		int	bb_x = BB_X	- (col * bbw) - nybMargin;
		int	bb_y = BB_Y	+ (row * ROW_MARGIN);

		QString	bbName = "bb_" % QString::number(index);

		// Instantiate the BitButton and connect it	to the ui (User	Interface)
		// which is	the	Main Window.
		//
		ui->bb[index] =	new	BitButton(index, ui->centralWidget);
		ui->bb[index]->setObjectName(bbName);
		ui->bb[index]->setGeometry(QRect(bb_x, bb_y, BB_W, BB_H));
		//ui->bb[index]->set

		// BitButton Label
		// ===============
		// Instantiate the bbLabel object and give it an object	name
		//
		ui->bbLabel[index] = new QLabel(ui->centralWidget);
		QString	bbLabelName	= "bb_label_" %	QString::number(index);
		ui->bbLabel[index]->setObjectName(bbLabelName);

		// Calculate the x and y coordinates of	the	BitButton's	label based	on
		// the x and y coordinates of the BitButton
		//
		int	bb_label_x = bb_x +	3;
		int	bb_label_y = bb_y -	BB_H + (row	* 2	* BB_H);

		// Place the bbLabel onto the ui (user interface), set its Font, and
		// write the text into the label.
		//
		ui->bbLabel[index]->setGeometry(
					QRect(bb_label_x, bb_label_y, BB_LABEL_W, BB_LABEL_H));
		ui->bbLabel[index]->setFont(QFont("Arial", 9, QFont::Bold));
		ui->bbLabel[index]->setText(QString("%1").arg(index, 2));

		// BitButton Signal	Mapping
		// ========================
		// Set the mapping index of	this BitButton
		//
		bbMapper->setMapping(ui->bb[index],	index);

		// Connect the BitButton bbClicked signal to the bbMapper's	one	and
		// only	slot, mapp().
		//
		connect(ui->bb[index], SIGNAL(clicked()), bbMapper,	SLOT(map()));
	}
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::init_heArray
//
// Init	the	array of Hexadicimal combo edit	boxes.
//
// The HexEdit subclass of QComboBox has these other classes in it and will
// instantiate them when it's constructor function is invoked.
//
// HexEdit		- The QComboBox itself
// HexBitField	- generic C++ class for managing bitfield widths
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
		HEXMAC(hexbits)->setText(QString("Bit Buttons"));

		left_y += 18;
		ObjName = "he_shift_rb_" % NameSubstr;
		HEXMAC(hexshift)->setObjectName(ObjName);
		HEXMAC(hexshift)->setGeometry(QRect(left_x, left_y, HE_W, HE_H));
		HEXMAC(hexshift)->setText(QString("Shift Button"));

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
	connect(ui->bbConnectGroup,	SIGNAL(buttonClicked(int)),
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
	const int x = X_START;
	const int y = Y_START + 76;
	const int i = 17;
	const int w = 50;
	const int h = 20;

	Twidget *tw = new Twidget;
	tw->objCount = 4;
	tw->objName = "bitsizes";
	tw->objText << "8-bit" << "16-bit" << "32-bit" << "64-bit";
	tw->labelText = NULL;
	tw->sizes << QSize(w, h) << QSize(w, h) << QSize(w, h) << QSize(w, h);
	tw->layout << QPoint(x, y)       << QPoint(x, y + i)
			   << QPoint(x, y + 2*i) << QPoint(x, y + 3*i);
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

	//bitSizeFrame = new QFrame(ui->centralWidget);
	//bitSizeFrame->setObjectName(QString::fromUtf8("frame"));
	//bitSizeFrame->setGeometry(QRect(x-6, y-2, w+8, h*4));
	//bitSizeFrame->setFrameShape(QFrame::StyledPanel );
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::init_invert
*/
void bits::init_invert()
{
	const int x = 210;
	const int y = 64;
	const int inc = HE_W + HE_SKIP_W;
	const int w = 20;
	const int h = 20;
	Twidget *tw = new Twidget;
	tw->objCount = 3;
	tw->objName = "invert";
	tw->objText << "~" << "~" << "~";
	tw->labelText = NULL;
	tw->sizes << QSize(w, h) << QSize(w, h) << QSize(w, h);
	tw->layout << QPoint(x, y) << QPoint(x+inc, y) << QPoint(x+2*inc, y);
	tw->grouped = false;  // These buttons are not grouped.
	pInvert = new ControlGroup <QPushButton>(tw, ui->centralWidget);
	connect(tw->mapper, SIGNAL(mapped(int)), this, SLOT(onInvert(int)));
}

/*/////////////////////////////////////////////////////////////////////////////
//
// bits::init_shiftOp
*/
void bits::init_shiftOp()
{
	const int x = X_START + 90;
	const int y = Y_START + 78;
	QPoint start = QPoint(x,y);
	shiftOp = new ShiftOpGroup(&start, ui->centralWidget);
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

