/*	mbrad012_lab3_part2.c - $date$
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
#include "../../../utils.h"

enum States {Start, Wait, Inc, Dec} state;
unsigned char count;

void Tick();
void Reset();

int main(void)
{
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRC = 0x0FF;
	PORTC = 0x00;
	
	state = Start;
	
	while(1)
	{
		Tick();
	} 
}

void Tick() {
	switch (state) { //Transitions
		case Start:
			state = Wait;
			break;
			
		case Wait:
			if (GetBit(PINA, 0) && !GetBit(PINA, 1) && count < 9) {
				state = Inc;
				count++;
			} else if (GetBit(PINA, 1) && !GetBit(PINA, 0) && count > 0) {
				state = Dec;
				count--;
			} else {
				state = Wait;
			}
			PORTC = count;
			break;
			
		case Inc:
			state = GetBit(PINA, 0) ? Inc : Wait;
			break;
			
		case Dec:
			state = GetBit(PINA, 1) ? Dec : Wait;
			break;
			
		default:
			state = Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case Start:
			count = 0;
			break;
			
		case Wait:
			Reset();
			break;
			
		case Inc:
			Reset();
			break;
			
		case Dec:
			Reset();
			break;
			
		default:
			break;
	} //State Actions
}

void Reset() {
	if (GetBit(PINA, 0) && GetBit(PINA, 1)) {
		count = 0;
	}
	PORTC = count;
}