#ifndef SHIFTOP_H
#define SHIFTOP_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include "controlgroup.h"
//#include <QtDesigner/QDesignerCustomWidgetInterface>


class ShiftOp : public QWidget
{
	Q_OBJECT
public:
	explicit ShiftOp(QWidget *parent = 0);

	QPlainTextEdit *shiftBox;
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
