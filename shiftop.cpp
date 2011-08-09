#include "shiftop.h"

ShiftOp::ShiftOp(QWidget *parent) :
	QWidget(parent)
{
	shiftBox = new QPlainTextEdit(parent);
	shiftBox->setGeometry(QRect(120, 110, 44, 24));
	shiftBox->setObjectName("shiftEdit");
	shiftBox->setPlainText(QString("1"));
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
