/*	mbrad012_lab2_part2.c - $date$
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

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b);
unsigned char GetBit(unsigned char x, unsigned char k);
void ControlFuelDisplay();

int main(void)
{
	DDRA = 0x00;
	PORTA = 0xFF;
	
	DDRC = 0xFF;
	PORTC = 0x00;
	
	while(1)
	{
		ControlFuelDisplay();
	}
}

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

void ControlFuelSensor() {
	PORTC = 0x00;
	switch (PINA) {
		case 15:
		case 14:
		case 13:
			PORTC = SetBit(PORTC, 0, 1);
			
		case 12:
		case 11:
		case 10:
			PORTC = SetBit(PORTC, 1, 1);

		case 9:
		case 8:
		case 7:
			PORTC = SetBit(PORTC, 2, 1);

		case 6:
		case 5:
			PORTC = SetBit(PORTC, 3, 1);

		case 4:
		case 3:
			PORTC = SetBit(PORTC, 4, 1);

		case 2:
		case 1:
			PORTC = SetBit(PORTC, 5, 1);
			break;

		default:
			PORTC = SetBit(PORTC, 7, 1);
			break;
	}

	if (PINA <= 4) {
		PORTC = SetBit(PORTC, 6, 1);
	}
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
	} else {
		PORTC = 0x80;
	}
}