#include <asf.h>
#include <avr/io.h>
#include "../../../../utils.h"

enum States {Start, Wait, Inc, Dec} state;

unsigned char count;
unsigned char led;
unsigned char a;

void Tick();
void Reset();

int main (void)
{
	board_init();

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
	a = ~PINA;
	
	switch (state) { //Transitions
		case Start:
		state = Wait;
		break;
		
		case Wait:
		if (GetBit(a, 0) && !GetBit(a, 1) && count < 9) {
			state = Inc;
			count++;
			} else if (GetBit(a, 1) && !GetBit(a, 0) && count > 0) {
			state = Dec;
			count--;
			} else {
			state = Wait;
		}
		led = count;
		break;
		
		case Inc:
		state = GetBit(a, 0) ? Inc : Wait;
		break;
		
		case Dec:
		state = GetBit(a, 1) ? Dec : Wait;
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
	
	PORTC = led;
}

void Reset() {
	if (GetBit(a, 0) && GetBit(a, 1)) {
		count = 0;
	}
	led = count;
}