/*	mbrad012_lab2_part1.c - $date$
 *	Name & E-mail:  - 
 *	CS Login: 
 *	Partner(s) Name & E-mail:  - 
 *	Lab Section: 
 *	Assignment: Lab #  Exercise # 
 *	Exercise Description:
 *	
 *	
 *	I acknowledge all content contained herein, excluding template or example 
 *	code, is my own original work.
 */ 


#include <avr/io.h>

const unsigned char MAX_BITS = 8;

unsigned char CountBits(unsigned char pins);

int main(void)
{
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0x00;
	PORTB = 0xFF;
	DDRC = 0xFF;
	PORTC = 0x00;
	
	unsigned char pins;
	unsigned char count;
	while(1)
	{
		pins = PINA;
		count = CountBits(pins);
		pins = PINB;
		count += CountBits(pins);
		
		PORTC = count;
	}
}

unsigned char CountBits(unsigned char pins) {
	unsigned char bitCount = 0;
	for (unsigned char i = 0; i < MAX_BITS; i++) {
		if (pins & (0x01 << i)) {
			bitCount++;
		}
	}
	
	return bitCount;
}