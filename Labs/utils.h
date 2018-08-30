#ifndef MJB_UTILS
#define MJB_UTILS

unsigned char GetBit(unsigned char x, unsigned char k) {
    return ((x & (0x01 << k)) != 0);
}

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
    return (b ? (x | (0x01 << k)) : (x & ~(0x01 << k)));
}

typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*Tick)(int);
} task;

task CreateTask(int state, unsigned long period, int (*Tick)(int)) {
	task ret;
	ret.state = state;
	ret.period = period;
	ret.elapsedTime = 0;
	ret.Tick = Tick;

	return ret;
}

// Combine two unsigned chars, creating one unsigned short
// Inputs:
//	   upper:	the upper byte of the short
//	   lower:	the lower byte of the short
// Output:	An unsigned short consisting of upper and lower
unsigned short ConcatChars(unsigned char upper, unsigned char lower) {
	unsigned short ret = upper;
	ret <<= 8;
	ret |= lower;
	return ret;
}

#endif
