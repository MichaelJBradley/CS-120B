#include "../../../../pwm.h"
#include "../../../../utils.h"

enum States {Start, Wait, Play} state;
unsigned char a;
unsigned char b;	

void Tick();
unsigned char IsSingleInput();

int main(void) {
	board_init();
	
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	TimerSet(1);
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
			state = Wait;
			b = 0;
			break;
			
		case Wait:
			if (IsSingleInput()) {
				state = Play;
				PWM_on();
			} else {
				state = Wait;
			}
			break;
			
		case Play:
			if (IsSingleInput()) {
				state = Play;
			} else {
				state = Wait;
				PWM_off();
			}
			break;
			
		default:
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case Start:
			break;
			
		case Wait:
			break;
			
		case Play:
			if (GetBit(a, 0)) {
				set_PWM(261.63);
			} else if (GetBit(a, 1)) {
				set_PWM(293.66);
			} else if (GetBit(a, 2)) {
				set_PWM(329.63);
			}
			break;
			
		default:
			break;
	} //State Actions
	
	b = SetBit(b, 0, IsSingleInput());
	b = SetBit(b, 1, GetBit(a, 0) || GetBit(a, 1) || GetBit(a, 2));
	
	PORTB = b;
}

unsigned char IsSingleInput() {
	return (
		(GetBit(a, 0) && !(GetBit(a, 1) || GetBit(a, 2)))
		||
		(GetBit(a, 1) && !(GetBit(a, 0) || GetBit(a, 2)))
		||
		(GetBit(a, 2) && !(GetBit(a, 0) || GetBit(a, 1)))
	);
}