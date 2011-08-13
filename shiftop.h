#ifndef SHIFTOP_H
#define SHIFTOP_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QtDesigner/QDesignerCustomWidgetInterface>
#include "controlgroup.h"

/*////////////////////////////////////////////////////////////////////////////
*/
class ShiftOpGroup : public QWidget
{
	Q_OBJECT
public:
	explicit ShiftOpGroup(QPoint *start, QWidget *parent = 0);

	QLineEdit *shiftBox;
	ControlGroup <QPushButton> *pShiftButtons;
	QCheckBox *chkRotate;

	int getCurrentShiftVal();
	void setCurrentShiftVal(int val);

signals:
	void shift(int);

public slots:
	void onTextEdited(QString text);

private:
	int	currentShiftVal;
	Twidget *tw;			// Twidget for "<<" and ">>" PushButtons
};



#endif // SHIFTOP_H
