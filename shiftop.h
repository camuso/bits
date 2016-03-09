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

#ifndef SHIFTOP_H
#define SHIFTOP_H

//#include <QtCore>
//#include <QtGui>

#include <QtGlobal>

#if QT_VERSION < 0x50000
#include <QtGui/QCheckBox>
#include <QtGui/QKeyEvent>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include <QtDesigner/QDesignerCustomWidgetInterface>
#endif

#if QT_VERSION >= 0x50000
#include <QCheckBox>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QtWidgets>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#endif

#include "controlgroup.h"
#include <QtCore/QStringBuilder>

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
    int currentShiftVal;
    Twidget *tw;            // Twidget for "<<" and ">>" PushButtons
};



#endif // SHIFTOP_H
