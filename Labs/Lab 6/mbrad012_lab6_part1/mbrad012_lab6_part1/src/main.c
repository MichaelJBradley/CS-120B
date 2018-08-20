#include "../../../../atmel.h"
#include "../../../../utils.h"

enum States {Start, Stop, Play} state;

unsigned char a;
unsigned char b;

unsigned char high;
unsigned char t;
const unsigned char MAX_T = 2;

void Tick();

int main (void)
{
	board_init();
	
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	
	TimerSet(1);
	TimerOn();
	
	while (1) {
		Tick();
		WaitTimer();
	}
}

void Tick() {
	a = ~PINA;
	
	switch (state) { //Transitions
		case Start:
			state = Stop;
			t = 0;
			high = 0;
			break;
			
		case Stop:
			if (GetBit(a, 2)) {
				 state = Play;
				 t = 0;
			} else {
				 state = Stop;
			}
			break;
			
		case Play:
			if (GetBit(a, 2)) {
				state = Play;
			} else {
				state = Stop;
				b = SetBit(b, 4, 0);
			}
			break;
			
		default:
			state = Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case Start:
			break;
		
		case Stop:
			
			break;
		
		case Play:
			if (high) {
				b = SetBit(b, 4, 1);
			} else {
				b = SetBit(b, 4, 0);
			}
			
			if (t >= MAX_T) {
				t = 0;
				high = ~high;
			}
			t++;
			break;
		
		default:	
			break;
	} //State Actions
	b = SetBit(b, 0, GetBit(a, 2));
	b = SetBit(b, 1, state == Play);
	PORTB = b;
}