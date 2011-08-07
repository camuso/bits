//#ifndef BITBUTTON_CPP
//#define BITBUTTON_CPP
//#endif //	BITBUTTON_CPP


#include <BitButton.h>


BitButton::BitButton(int number, QWidget *parent)
	: QPushButton(parent)
{
	bbId = number;

	bbStyle[0] = "color: aqua; background-color: black";
	bbStyle[1] = "color: black;	background-color: aqua";

	QFont font;
#ifdef Q_WS_WIN
		font.setPointSize(font.pointSize() + 4);
		font.setFamily("Consolas");
#endif
#ifdef Q_WS_X11
		font.setFamily("Monospace");
		font.setPointSize(12);
#endif
	this->setFont(font);
	this->setState(0);
}

void BitButton::bbToggle()
{
	this->setState(bbState ^=	1);
}

void BitButton::setState(int state)
{
	bbState	= state;
	this->setText(QString::number(state));
	this->setStyleSheet(bbStyle[state]);
}

#if 0
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
