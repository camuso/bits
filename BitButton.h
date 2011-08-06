#ifndef	BITBUTTON_H
#define	BITBUTTON_H

#include <QtCore>
#include <QtCore/QTextStream>
#include <QtGui>
#include <QtDesigner/QDesignerCustomWidgetInterface>

class BitButton	: public QPushButton
{
	Q_OBJECT
public:
	BitButton(int number, QWidget *parent);
	void bbToggle();

	int	bbId;
	int	bbState;
	void setState(int state);

signals:
	void bbClicked();

private	slots:

private:
	QString	bbStyle[2];
};


#endif
