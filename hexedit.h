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

#ifndef HEXEDIT_H
#define HEXEDIT_H

#include <QtGlobal>

#if QT_VERSION < 0x50000
#include <QtGui/QFontComboBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#endif

#if QT_VERSION >= 0x50000
#include <QFontComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#endif

#include <QtCore/QTextStream>
#include <QtDesigner/QDesignerCustomWidgetInterface>
#include "BitButton.h"
#include "bitfield.h"

class HexEdit : public QComboBox
{
    Q_OBJECT
public:
    HexEdit(int hexenum, QWidget *parent = 0, bitfield_t bf = bit_32);
    ~HexEdit();

    bitfield *hexBitField;
    QLabel *hexeditLabel;
    QRadioButton *hexbits;
    QRadioButton *hexshift;
    int hexeditId;

    quint64 hexstr2int(QString& hexstr);
    QString& int2hexstr(QString &hexStr, quint64 hexVal);
    void updateHexEdit(quint64 hexVal);
    void updateHexEditBitField(bitfield_t bf);
    bool isNewData(quint64 numData);
    bool isNewData(QString& strData);
    quint64 getHexVal();
    quint64 maskHexVal(quint64 hexVal);

signals:

public slots:

private:

    QString prevHexStr;
    quint64 prevHexVal;
};

#endif // HEXEDIT_H
