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

#ifndef BITFIELD_H
#define BITFIELD_H

//#include <QtCore>

enum bitfield_t {bit_8, bit_16, bit_32, bit_64, bitfield_array_size};

class bitfield
{
public:
    bitfield (bitfield_t bf=bit_32);
    ~bitfield();

    void setBitField (bitfield_t bf);
    bitfield_t getCurrentBitField();
    const char* getBitmask (bitfield_t bf);
    const char* getCurrentBitMask();
    int getCurrentBinDigits();
    int getCurrentHexDigits();

private:
    bitfield_t currentBitField;
    int hexDigits[bitfield_array_size];
    int binDigits[bitfield_array_size];
    const char *bfMask[bitfield_array_size];
};

#endif // BITFIELD_H
