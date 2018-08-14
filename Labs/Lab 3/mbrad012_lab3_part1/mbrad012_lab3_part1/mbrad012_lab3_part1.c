/*	mbrad012_lab3_part1.c - 2018-08-13
 *	Name & E-mail:  - Michael Bradley mbrad012@ucr.edu
 *	CS Login: mbrad012
 *	Partner(s) Name & E-mail:  - 
 *	Lab Section: 
 *	Assignment: Lab #  Exercise # 
 *	Exercise Description:
 *	Create a SM that switches lights on button presses.
 *	
 *	I acknowledge all content contained herein, excluding template or example 
 *	code, is my own original work.
 */ 


#include <avr/io.h>
#include "../../../utils.h"

enum States {Start, FirstPress, FirstRelease, SecondPress, SecondRelease} state;

void Tick();

int main(void)
{
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	
	state = Start;
	
	while(1)
	{
		Tick();
	}
}

void Tick() {
	switch (state) { //Transitions
		case Start:
			state = SecondPress;
			break;
			
		case SecondRelease:
			state = GetBit(PINA, 0) ? FirstPress : SecondRelease;
			break;
			
		case FirstPress:
			state = GetBit(PINA, 0) ? FirstPress : FirstRelease;
			break;
			
		case FirstRelease:
			state = GetBit(PINA, 0) ? SecondPress : FirstRelease;
			break;
			
		case SecondPress:
			state = GetBit(PINA, 0) ? SecondPress : SecondRelease;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case Start:
		break;
				
		case SecondRelease:
		break;
		
		case FirstPress:
		PORTB = 0x02;
		break;
				
		case FirstRelease:
		break;
		
		case SecondPress:
		PORTB = 0x01;
		break;
	} //State Actions
}