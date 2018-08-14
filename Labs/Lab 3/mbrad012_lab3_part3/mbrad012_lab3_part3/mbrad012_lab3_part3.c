/*	mbrad012_lab3_part3.c - $date$
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

#define X 0x01
#define Y 0x02
#define P 0x04

const unsigned char CODE_LEN = 2;
unsigned char code[] = {P, Y};
unsigned char i;
unsigned char once;

enum States {Start, Enter, Unlocked} state;

void Tick();

int main(void)
{
	
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	DDRC = 0xFF;
	PORTC = 0x00;
	
	state = Start;
	
	while(1)
	{
		Tick();
	}
}

void Tick() {
	switch (state) { //Transition
		case Start:
			state = Enter;
			break;
			
		case Enter:
			state = (i == CODE_LEN) ? Unlocked : Enter;
			if (PINA && !once) {
				if (PINA == code[i]) {
					i++;
				} else {
					i = 0;
				}
				once = 1;
			}
			
			if (!PINA) {
				once = 0;
			}
			break;
			
		case Unlocked:
			state = GetBit(PINA, 7) ? Start : Unlocked;
			break;
			
		default:
			state = Start;
	} //Transitions
	
	switch (state) { //State Actions
		case Start:
			i = 0;
			once = 0;
			break;
			
		case Enter:
			PORTB = 0;
			break;
			
		case Unlocked:
			PORTB = 0x01;
			break;	
		
		default:
			break;
		
	} //State Actions
	
	PORTC = state;
}