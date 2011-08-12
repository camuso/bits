#include "shiftop.h"

ShiftBox::ShiftBox(QWidget *parent) : QLineEdit(parent)
{
	// Turn-off the built-in frame so we can create a custom frame for this
	// object.
	//
	this->setFrame(false);
	frame_3 = new QFrame(this);
	frame_3->setObjectName(QString::fromUtf8("frame_3"));
	frame_3->setFrameShape(QFrame::WinPanel);
	frame_3->setFrameShadow(QFrame::Sunken);
	//frame_3->setLineWidth(4);
	//frame_3->setMidLineWidth(2);
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

	setupText(QString("1"));

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
	const int crw = 50;
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
	shiftBox->setText(QString("  " % text));
}
