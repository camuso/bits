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
	shiftBox->setInputMask("00");
	shiftBox->setText(QString("1"));
	currentShiftVal = 1;

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
	tw->grouped = false;
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
}

int ShiftOpGroup::getCurrentShiftVal() {return currentShiftVal;}
void ShiftOpGroup::setCurrentShiftVal(int val) {currentShiftVal = val;}

void ShiftOpGroup::onEditActivate()
{

}
