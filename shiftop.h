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

private:
	QFrame *shiftBoxFrame;
};

/*////////////////////////////////////////////////////////////////////////////
*/
class ShiftOpGroup : public QWidget
{
	Q_OBJECT
public:
	explicit ShiftOpGroup(QPoint *start, QWidget *parent = 0);

	ShiftBox *shiftBox;
	ControlGroup <QPushButton> *pShiftButtons;
	QCheckBox *chkRotate;

	int getCurrentShiftVal();
	void setCurrentShiftVal(int val);
	void setupText (QString& text);

signals:
	void shift(int);

public slots:
	void onReturnPressed();

private:
	int	currentShiftVal;
	Twidget *tw;			// Twidget for "<<" and ">>" PushButtons

};



#endif // SHIFTOP_H
