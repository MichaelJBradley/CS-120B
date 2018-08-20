#include "../../../../atmel.h"
#include "../../../../utils.h"

enum States {Start, Stop, Play, Adjust, AdjustWait} state;

unsigned char a;
unsigned char b;

unsigned char high;
unsigned char t;
unsigned char pitch;
unsigned char once;

void Tick();
void AdjustPitch();

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
			once = 0;
			pitch = 2;
			break;
		
		case Stop:
			if (GetBit(a, 2) && !GetBit(a, 0) && !GetBit(a, 1)) {
				state = Play;
				t = 0;
			} else if (GetBit(a, 0) || GetBit(a, 1)) {
				state = Adjust;
			} else {
				state = Stop;
			}
			break;
		
		case Play:
			if (GetBit(a, 2) && !GetBit(a, 0) && !GetBit(a, 1)) {
				 state = Play;
			 } else if (GetBit(a, 0) || GetBit(a, 1)) {
				 state = Adjust;
			 } else {
				 state = Stop;
				 b = SetBit(b, 4, 0);
			 }
			 break;
			
		case Adjust:
			state = AdjustWait;
			AdjustPitch();
			break;
			
		case AdjustWait:
			if (GetBit(a, 0) || GetBit(a, 1)) {
				state = AdjustWait;
			} else {
				state = Stop;
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
		
			if (t >= pitch) {
				t = 0;
				high = ~high;
			}
			t++;
			break;
		
		case Adjust:
			break;
		
		case AdjustWait:
			break;
		
		default:
			break;
	} //State Actions
	b = SetBit(b, 0, GetBit(a, 0));
	b = SetBit(b, 1, state == AdjustWait);
	PORTB = b;
}

void AdjustPitch() {
	if (GetBit(a, 0) && pitch < 40) {
			pitch++;
	} else if (GetBit(a, 1) && pitch > 2) {
			pitch--;
	}
}