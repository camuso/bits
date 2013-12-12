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

#include "shiftop.h"

/*////////////////////////////////////////////////////////////////////////////
*/
ShiftOpGroup::ShiftOpGroup(QPoint *start, QWidget *parent) :
    QWidget(parent)
{
    const int x = start->x()+7;
    const int y = start->y();
    const int sbw = 38;
    const int sbh = 26;
    const int margin = 3;

    shiftBox = new QLineEdit(parent);
    shiftBox->setGeometry(x, y, sbw, sbh);
    shiftBox->setObjectName("ShiftBox");
    shiftBox->setInputMask("   00 ");

    // Set up the initial shift value and the edit box to display it.
    //
    currentShiftVal = 1;
    QString text = QString("1");
    shiftBox->setText(text);

    // Create the shift operator push buttons as a ControlGroup.
    //
    const int shlx = start->x();
    const int shly = start->y()+sbh + margin;
    const int shlw = 24;
    const int shlh = 20;
    const int shrx = shlx + margin + shlw;

    tw = new Twidget;
    tw->objName = QString("shifters");
    tw->objText << "<<" << ">>";
    tw->sizes   << QSize(shlw, shlh);
    tw->layout  << QPoint(shlx, shly) << QPoint(shrx, shly);
    tw->grouped = true;
    quint64 getBits();
    pShiftButtons = new ControlGroup <QPushButton>(tw, parent);

    // Create the Rotate checkbox
    //
    const int crx = start->x();
    const int cry = shly + shlh;
    const int crw = 70;
    const int crh = 24;

    chkRotate = new QCheckBox(parent);
    chkRotate->setObjectName("chkRotate");;
    chkRotate->setGeometry(crx, cry, crw, crh);
    chkRotate->setText("Rotate");
    chkRotate->show();

    // Allow the shift buttons to autorepeat.
    //
    pShiftButtons->widgetList[0]->setAutoRepeat(true);
    pShiftButtons->widgetList[1]->setAutoRepeat(true);

    // Connect the signal from the shiftbuttons to the "shift" signal in this
    // class.
    // The parent class of this class will connect the "shift" signal from
    // here to one of its slots. Because the signals are coming from a button
    // group, they will be indexed and we will be able to determine wheter
    // it was the left or right button that was pressed.
    //
    connect(tw->buttonGroup, SIGNAL(buttonClicked(int)),
            this, SIGNAL(shift(int)));
    connect(shiftBox, SIGNAL(textEdited(QString)),
            this, SLOT(onTextEdited(QString)));
}

int ShiftOpGroup::getCurrentShiftVal() {return currentShiftVal;}

void ShiftOpGroup::onTextEdited(QString text)
{
    bool ok;
    currentShiftVal = text.toInt(&ok);
}
