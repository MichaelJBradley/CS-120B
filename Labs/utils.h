#ifndef MJB_UTILS
#define MJB_UTILS

unsigned char GetBit(unsigned char x, unsigned char k) {
    return ((x & (0x01 << k)) != 0);
}

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
    return (b ? (x | (0x01 << k)) : (x & ~(0x01 << k)));
}

#endif
