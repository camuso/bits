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

#include "bitfield.h"

bitfield::bitfield (bitfield_t bf)
{
    currentBitField = bf;

    // Note:
    // See http://doc.qt.nokia.com/latest/qlineedit.html#inputMask-prop
    // for a thorough explanation of QLineEdit inputMask properties.
    //
    // Here, the lowercase "h" means hex characters only upto the field
    // width in the mask. Uppercase "H" would mean that the entire field
    // must be filled-in.
    //
    bfMask[bit_8]  = "hh";
    bfMask[bit_16] = "hhhh";
    bfMask[bit_32] = "hhhhhhhh";
    bfMask[bit_64] = "hhhhhhhh.hhhhhhhh";

    for(int index = 0; index < bitfield_array_size; index++)
    {
        hexDigits[index] = (2 << index);
        binDigits[index] = (8 << index);
    }
}

bitfield::~bitfield()
{
    for(int index = 0; index < bitfield_array_size; index++)
        delete bfMask[index];
}

void bitfield::setBitField (bitfield_t bf)
{
    currentBitField = bf;
    for(int index = 0; index < bitfield_array_size; index++)
    {
        hexDigits[index] = (2 << index);
        binDigits[index] = (8 << index);
    }
}

bitfield_t bitfield::getCurrentBitField() {return currentBitField;}
const char* bitfield::getBitmask (bitfield_t bf) {return bfMask[bf];}
const char* bitfield::getCurrentBitMask() {return bfMask[currentBitField];}
int bitfield::getCurrentHexDigits() {return hexDigits[currentBitField];}
int bitfield::getCurrentBinDigits() {return binDigits[currentBitField];}

