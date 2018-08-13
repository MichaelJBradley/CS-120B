/*	mbrad012_lab2_part4.c - $date$
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

unsigned char NUM_BITS = 4;

void ShiftBits();

int main(void)
{

	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;

	while(1)
	{
		ShiftBits();
	}
}

void ShiftBits() {
	PORTB = PINA << NUM_BITS;
	PORTC = PINA >> NUM_BITS;
}