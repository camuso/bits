/******************************************************************************
**
**  bits - an application to arithmeticall and logically combine and
**         manipulate bit patterns and their corresponding hex numbers.
**
**  Tony Camuso
**  Created December, 2011
**
**    bits (bitview) is free software. You can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**  GNU General Public License http://www.gnu.org/licenses/gpl.html
**
**  Copyright (c) 2011-2014 by Tony Camuso.
**
******************************************************************************/

#include <BitButton.h>


BitButton::BitButton(int number, QWidget *parent)
    : QPushButton(parent)
{
    bbId = number;

    bbStyle[1] = "color: white; "
                 "background-color: "
                    "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                    "stop: 0 #08080a, stop: 1 #66777a); "
                "border:6pxsolidwhite; "
                "border-style:inset;border-width:1px;"
                "border-radius6px;border-color:black;";

    bbStyle[0] = "color: black; "
                 "background-color: "
                    "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                    "stop: 0 #eaebfe, stop: 1 #76878a); "
                "border-style:outset;border-width:1px;"
                "border-radius4px;border-color:gray;";

    QFont font;
#ifdef Q_WS_WIN
        //font.setBold(true);
        font.setPointSize(font.pointSize()+2);
        font.setFamily("Helvetica");
#endif
#ifdef Q_WS_X11
        //font.setBold(true);
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
    bbState = state;
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
