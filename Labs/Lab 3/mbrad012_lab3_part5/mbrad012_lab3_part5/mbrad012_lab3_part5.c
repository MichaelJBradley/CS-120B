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

#define X 1
#define Y 2
#define P 4

const unsigned char CODE_LEN = 4;
unsigned char code[] = {P, X, Y, X};
unsigned char i;
unsigned char once;

enum States {Start, Locked, Unlocked} state;

void Tick();
void GetCodeInput();
void Init();

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
			state = Locked;
			break;
			
		case Locked:
			if (i == CODE_LEN) {
				state = Unlocked;
				Init();
			} else {
				state = Locked;
			}
			GetCodeInput();
			break;
			
		case Unlocked:
			state = GetBit(PINA, 7) || (i == CODE_LEN) ? Start : Unlocked;
			GetCodeInput();
			break;
			
		default:
			state = Start;
	} //Transitions
	
	switch (state) { //State Actions
		case Start:
			Init();
			break;
			
		case Locked:
			PORTB = SetBit(PORTB, 0, 0);
			break;
			
		case Unlocked:
			PORTB = SetBit(PORTB, 0, 1);
			break;	
		
		default:
			break;
		
	} //State Actions
	
	PORTC = state;
}

void GetCodeInput() {
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
}

void Init() {
	i = 0;
	once = 0;
}