#include "..\..\..\..\atmel.h"
#include "..\..\..\..\utils.h"

enum States {Start, Blink, HoldDown, HoldUp, ResDown} state;

const unsigned char MAX_LED = 2;
const unsigned char MAX_TIME = 3;
unsigned char i;
unsigned char t;

unsigned char a;
unsigned char b;

void Tick();
void Reset();

int main (void)
{
	board_init();
	
	DDRB = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	TimerSet(100);
	TimerOn();
	
	state = Start;
	
	while (1) {
		Tick();
		WaitTimer();
	}
}

void Tick() {
	a = ~PINA;
	
	switch (state) { //Transitions
		case Start:
			state = Blink;
			break;
		
		case Blink:
			state = GetBit(a, 0) ? HoldDown : Blink;
			break;
		
		case HoldDown:
			state = GetBit(a, 0) ? HoldDown : HoldUp;
			break;
			
		case HoldUp:
			state = GetBit(a, 0) ? ResDown : HoldUp;
			break;
			
		case ResDown:
			state = GetBit(a, 0) ? ResDown : Blink;
			break;
		
		default:
			state = Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case Start:
			Reset();
			break;
		
		case Blink:
			t++;
			if (t > MAX_TIME) {
				t = 0;
				if (i > MAX_LED) {
					i = 0;
				} else {
					i++;
				}
			}
			b = 0x01 << i;
			break;
		
		case HoldDown:
			break;
			
		case HoldUp:
			break;
			
		case ResDown:
			Reset();
		default:
			break;
	} //State Actions
	
	PORTB = b;
	PORTB = SetBit(PORTB, 3, GetBit(a, 0));
}

void Reset() {
	i = 0;
	t = 0;
	b = 0;
}
