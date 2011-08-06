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

void bitfield::setBitField (bitfield_t bf) {currentBitField = bf;}
bitfield_t bitfield::getCurrentBitField() {return currentBitField;}
char* bitfield::getBitmask (bitfield_t bf) {return bfMask[bf];}
char* bitfield::getCurrentBitMask() {return bfMask[currentBitField];}
int bitfield::getCurrentHexDigits() {return hexDigits[currentBitField];}
int bitfield::getCurrentBinDigits() {return binDigits[currentBitField];}

