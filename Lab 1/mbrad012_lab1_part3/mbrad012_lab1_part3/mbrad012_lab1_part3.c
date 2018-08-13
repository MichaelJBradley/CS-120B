/*
 * mbrad012_lab1_part3.c
 *
 * Created: 2018-08-06 12:57:10 PM
 *  Author: Michael Bradley
 */ 



#include <avr/io.h>

const unsigned char SPACES = 4;

void CountCars();

int main(void)
{
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRC = 0xFF;
	PORTC = 0x00;
	
	while (1) {
		CountCars();
	}
}

void CountCars() {
	unsigned char cntavail = SPACES;
	for (unsigned char i = 0; i < SPACES; i++) {
		if (PINA & (0x01 << i)) {
			cntavail--;
		}
	}
	
	if (cntavail == 0) {
		PORTC = cntavail | 0x80;
	} else {
		PORTC = cntavail;
	}
}