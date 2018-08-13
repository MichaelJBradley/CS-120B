/*	mbrad012_lab2_part3.c - $date$
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

const unsigned char PIN_KEY = 0x10;
const unsigned char PIN_SEATED = 0x20;
const unsigned char PIN_SEATBELT = 0x40;


unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b);
unsigned char GetBit(unsigned char x, unsigned char k);
void ControlFuelDisplay();
void ControlSeatbeltWarn();

int main(void)
{
	DDRA = 0x00;
	PORTA = 0xFF;
	
	DDRC = 0xFF;
	PORTC = 0x00;
	
	while(1)
	{
		ControlFuelDisplay();
		ControlSeatbeltWarn();
	}
}

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

void ControlFuelDisplay() {
	if (PINA <= 2) {
		PORTC = 0x60; //1-2 PC5 and PC6 (Low Fuel)
	} else if (PINA <= 4) {
		PORTC = 0x70; //3-4 PC4-PC5 and PC6 (Low Fuel)
	} else if (PINA <= 6) {
		PORTC = 0x38; //5-6 PC3-PC5
	} else if (PINA <= 9) {
		PORTC = 0x3C; //7-9 PC2-PC5
	} else if (PINA <= 12) {
		PORTC = 0x3E; //10-12 PC1-PC5;
	} else if (PINA <= 15) {
		PORTC = 0x3F; //13-15 PC0-PC5
	}
}

void ControlSeatbeltWarn() {
	if ((PINA & PIN_KEY) && (PINA & PIN_SEATED) && !(PINA & PIN_SEATBELT)) {
		PORTC = SetBit(PORTC, 7, 1);
	} else {
		PORTC = SetBit(PORTC, 7, 0);
	}
}