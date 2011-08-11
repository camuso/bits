#ifndef SHIFTOP_H
#define SHIFTOP_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QtDesigner/QDesignerCustomWidgetInterface>
#include "controlgroup.h"

/*////////////////////////////////////////////////////////////////////////////
*/
class ShiftBox : public QLineEdit
{
	Q_OBJECT

public:
	ShiftBox(QWidget *parent=0);
	//~ShiftBox();

	void set_geometry(QRect& rect);

private:
	QFrame *frame_3;
};

/*////////////////////////////////////////////////////////////////////////////
*/
class ShiftOp : public QWidget
{
	Q_OBJECT
public:
	explicit ShiftOp(QWidget *parent = 0);

	ShiftBox *shiftBox;
	ControlGroup <QPushButton> *pShiftButtons;


	int getCurrentShiftVal();
	void setCurrentShiftVal(int val);

signals:

public slots:
	void onEditActivate();

private:
	int	currentShiftVal;

};



#endif // SHIFTOP_H
