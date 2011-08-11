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

ShiftOp::ShiftOp(QWidget *parent) :
	QWidget(parent)
{
	shiftBox = new ShiftBox(parent);
	shiftBox->setGeometry(QRect(120, 110, 48, 26));
	shiftBox->setObjectName("ShiftBox");
	shiftBox->setInputMask("00");
	shiftBox->setText(QString("1"));
	currentShiftVal = 1;

	// Create the shift operator push buttons as a ControlGroup.
	//
	Twidget *tw = new Twidget;
	tw->objName = QString("shifters");
	tw->objCount = 2;
	tw->objText << "<<" << ">>";
	tw->sizes   << QSize(24, 20) << QSize(24, 20);
	tw->layout  << QPoint(120, 140) << QPoint(146, 140);
	tw->grouped = false;
	tw->labeled = false;
	pShiftButtons = new ControlGroup <QPushButton>(tw, parent);

}

int ShiftOp::getCurrentShiftVal() {return currentShiftVal;}
void ShiftOp::setCurrentShiftVal(int val) {currentShiftVal = val;}

void ShiftOp::onEditActivate()
{

}
