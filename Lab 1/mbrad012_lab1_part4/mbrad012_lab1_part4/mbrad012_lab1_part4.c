/*	mbrad012_lab1_part4.c - $date$
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
#include <math.h>

const unsigned char MAX_WEIGHT = 140;
const unsigned char MAX_DIFF = 80;
const unsigned char MAX_BITS = 6;


unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b);
void ControlWeightDisplay();
unsigned char RemapBits(unsigned char weight);

unsigned char magicDiv;
unsigned weight = 0;
unsigned lastWeight = 0;

int main(void)
{

	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0x00;
	PORTB = 0xFF;
	DDRC = 0x00;
	PORTC = 0xFF;
	DDRD = 0xFF;
	PORTD = 0x00;
	
	magicDiv = MAX_WEIGHT/ pow(2, MAX_BITS);
	
	while(1)
	{
		ControlWeightDisplay();
	}
}

void ControlWeightDisplay() {
	
 	weight = PINA + PINB + PINC;
	
	if (weight != lastWeight) {
		PORTD = RemapBits(weight);
	}
	
	if (weight > MAX_WEIGHT) {
		PORTD = SetBit(PORTD, 0, 1);
	} else {
		PORTD = SetBit(PORTD, 0, 0);
	}

	if (PINA - PINC > MAX_DIFF || PINC - PINA > MAX_DIFF) {
		PORTD = SetBit(PORTD, 1, 1);
	} else {
		PORTD = SetBit(PORTD, 1, 0);
	}
	
	lastWeight = weight;
}

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

// Remap an 8 bit unsigned char to 6 bits then shift two to the left.
unsigned char RemapBits(unsigned char weight) {
	return (weight / magicDiv) << 2;
}