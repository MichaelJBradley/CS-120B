#include "..\..\..\..\atmel.h"

enum States {Start, Blink} state;

const unsigned char MAX_LED = 3;
unsigned char i;
unsigned char b;

void Tick();

int main (void)
{
	board_init();
	
	DDRB = 0xFF;
	PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
	
	state = Start;
	
	while (1) {
		Tick();
		WaitTimer();
	}
}

void Tick() {
	switch (state) { //Transitions
		case Start:
			state = Blink;
			break;
			
		case Blink:
			state = Blink;
			if (i >= 2) {
				i = 0;
			} else {
				i++;
			}
			break;
			
		default:
			state = Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case Start:
			i = 0;
			b = 0;
			break;
		
		case Blink:
			b = 0x01 << i;
			break;
		
		default:
			break;		
	} //State Actions
	
	PORTB = b;
}
