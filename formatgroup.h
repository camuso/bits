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

#ifndef FORMATGROUP_H
#define FORMATGROUP_H

#include <QtGlobal>

#if QT_VERSION < 0x50000
#include <QtGui/QWidget>
#endif

#if QT_VERSION >= 0x50000
#include <QWidget>
#endif

class FormatGroup : public QWidget
{
    Q_OBJECT
public:
    explicit FormatGroup(QWidget *parent = 0);

signals:

public slots:

};

#endif // FORMATGROUP_H
