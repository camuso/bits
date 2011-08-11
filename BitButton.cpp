#include <BitButton.h>


BitButton::BitButton(int number, QWidget *parent)
	: QPushButton(parent)
{
	bbId = number;

	bbStyle[0] = "color: white; "
				 "background-color: "
					"qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
					"stop: 0 #08080a, stop: 1 #66777a); "
				 "border: 6px solid white; "
				 "border-style: sunken; border-width: 2px; "
				 "border-radius 6px; border-color: white; ";

	bbStyle[1] = "color: black; background-color: aqua;"
				 "background-color: "
					"qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
					"stop: 0 #eaebfe, stop: 1 #76878a); "
				 "border-style: raised; border-width: 3px;"
				 "border-radius 4px; border-color: black;";

	QFont font;
#ifdef Q_WS_WIN
	font.setBold(true);
		font.setPointSize(font.pointSize()+2);
		font.setFamily("Helvetica");
#endif
#ifdef Q_WS_X11
		font.setBold();
		font.setFamily("Monospace");
		font.setPointSize(12);
#endif

	this->setFont(font);
	this->setState(0);
}

void BitButton::bbToggle()
{
	this->setState(bbState ^= 1);
}

void BitButton::setState(int state)
{
	bbState	= state;
	this->setText(QString::number(state));
	this->setStyleSheet(bbStyle[state]);
}

#if 0
palette.setColor(widget->backgroundRole(), color);
QComboBox {
	font-family: "Lucida Grande", Lucida, Verdana, sans-serif;
	border: 1px solid #D3D3D3;
	border-radius: 8px;
	background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #EEEEEE, stop: 1 #FFFFFF);
	color: #333;
	font-size: 6pt;
	padding: 8px;
}
#endif
