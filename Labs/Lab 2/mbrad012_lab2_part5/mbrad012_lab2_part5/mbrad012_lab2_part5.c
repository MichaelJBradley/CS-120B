	/*	mbrad012_lab2_part5.c - $date$
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
#include <stdio.h>

unsigned char PORT_AB_ENABLED = 1;
unsigned char PORT_AB_DISABLED = 2;
unsigned char PIN_B_IN = 0x01;

void ControlAirbagDisplay();
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b);
unsigned char GetBit(unsigned char x, unsigned char k);

int main(void)
{
	DDRB = 0xFE;
	PORTB = 0x01;
	DDRD = 0x00;
	PORTD = 0xFF;
	
	while(1)
	{
		ControlAirbagDisplay();
	}
}

void ControlAirbagDisplay() {
	unsigned short weight = PIND;
	weight = weight << 1;
	weight = weight | (PINB & 0x01);

	if (weight > 70) {
		PORTB = SetBit(PORTB, PORT_AB_ENABLED, 1);
		PORTB = SetBit(PORTB, PORT_AB_DISABLED, 0);
	} else if (weight > 5) {
		PORTB = SetBit(PORTB, PORT_AB_ENABLED, 0);
		PORTB = SetBit(PORTB, PORT_AB_DISABLED, 1);
	} else {
		PORTB = SetBit(PORTB, PORT_AB_ENABLED, 0);
		PORTB = SetBit(PORTB, PORT_AB_DISABLED, 0);
	}

	if (weight == 511) {
		PORTB = SetBit(PORTB, 7, 0);
	}
}

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}