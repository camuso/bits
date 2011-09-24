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
