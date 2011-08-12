#include "shiftop.h"

ShiftBox::ShiftBox(QWidget *parent) : QLineEdit(parent)
{
	// Turn-off the built-in frame so we can create a custom frame for this
	// object.
	//
	this->setFrame(false);
	shiftBoxFrame = new QFrame(this);
	shiftBoxFrame->setObjectName(QString::fromUtf8("shiftBoxFrame"));
	shiftBoxFrame->setFrameShape(QFrame::WinPanel);
	shiftBoxFrame->setFrameShadow(QFrame::Sunken);
	//shiftBoxFrame->setLineWidth(4);
	//shiftBoxFrame->setMidLineWidth(2);
}

ShiftOpGroup::ShiftOpGroup(QPoint *start, QWidget *parent) :
	QWidget(parent)
{
	const int x = start->x();
	const int y = start->y();
	const int sbw = 48;
	const int sbh = 26;
	const int margin = 3;

	shiftBox = new ShiftBox(parent);
	shiftBox->setGeometry(x, y, sbw, sbh);
	shiftBox->setObjectName("ShiftBox");
	shiftBox->setInputMask("   00 ");

	// Set up the initial shift value and the edit box to display it.
	//
	currentShiftVal = 1;
	QString text = QString("1");
	setupText(text);

	// Create the shift operator push buttons as a ControlGroup.
	//
	const int shlx = start->x();
	const int shly = start->y()+sbh + margin;
	const int shlw = 24;
	const int shlh = 20;
	const int shrx = shlx + margin + shlw;

	tw = new Twidget;
	tw->objName = QString("shifters");
	tw->objCount = 2;
	tw->objText << "<<" << ">>";
	tw->sizes   << QSize(shlw, shlh) << QSize(shlw, shlh);
	tw->layout  << QPoint(shlx, shly) << QPoint(shrx, shly);
	tw->grouped = true;
	tw->labeled = false;
	pShiftButtons = new ControlGroup <QPushButton>(tw, parent);

	// Create the Rotate checkbox
	//
	const int crx = start->x();
	const int cry = shly + shlh;
	const int crw = 70;
	const int crh = 24;

	chkRotate = new QCheckBox(parent);
	chkRotate->setObjectName("chkRotate");;
	chkRotate->setGeometry(crx, cry, crw, crh);
	chkRotate->setText("Rotate");
	chkRotate->show();

	// Connect the signal from the shiftbuttons to the "shift" signal in this
	// class.
	// The parent class of this class will connect the "shift" signal from
	// here to one of its slots. Because the signals are coming from a button
	// group, they will be indexed and we will be able to determine wheter
	// it was the left or right button that was pressed.
	//
	connect(tw->buttonGroup, SIGNAL(buttonClicked(int)), this, SIGNAL(shift(int)));
	connect(shiftBox, SIGNAL(returnPressed()), this, SLOT(onReturnPressed()));
}

int ShiftOpGroup::getCurrentShiftVal() {return currentShiftVal;}
void ShiftOpGroup::setCurrentShiftVal(int val) {currentShiftVal = val;}

void ShiftOpGroup::onReturnPressed()
{
	bool ok;
	QString text = shiftBox->text();
	currentShiftVal = text.toInt(&ok);
	setupText(text);
}

void ShiftOpGroup::setupText (QString& text)
{
	shiftBox->setText(QString("   " % text));
}
