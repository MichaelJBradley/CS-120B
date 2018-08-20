#include "../../../../pwm.h"
#include "../../../../utils.h"

enum States {Start, StopSound, PlaySound, IncWait, Inc} soundState, incState;

enum Notes {C4, D4, E4, F4, G4, A4, B4, C5};

double scale[] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

unsigned char a;
unsigned char b;

unsigned char i;
unsigned char SCALE_SIZE = 8;

void SoundTick();
void IncTick();

int main (void)
{
	board_init();
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	
	TimerSet(1);
	TimerOn();
	
	soundState = Start;
	incState = Start;
	b = 0;
	
	while (1) {
		SoundTick();
		IncTick();
		WaitTimer();
	}
}

void SoundTick() {
	a = ~PINA;
	
	switch (soundState) { //Transitions
		case Start:
			soundState = StopSound;
			break;
			
		case StopSound:
			if (GetBit(a, 0)) {
				soundState = PlaySound;
				PWM_on();
			} else {
				soundState = StopSound;
			}
			break;
			
		case PlaySound:
			if (GetBit(a, 0)) {
				soundState = StopSound;
				PWM_off();
			} else {
				soundState = PlaySound;
			}
			break;
			
		default:
			soundState = Start;
			break;
	} //Transitions
	
	switch (soundState) { //State Actions
		case Start:
			break;
			
		case StopSound:
			break;
				
		case PlaySound:
			set_PWM(scale[i]);
			break;
				
		default:
			break;
	} //State Actions
	
	PORTB = SetBit(PORTB, 0, GetBit(a, 0));
	PORTB = SetBit(PORTB, 1, (GetBit(a, 1) || GetBit(a, 2)));
	
}

void IncTick() {
	a = ~PINA;
	
	switch (incState) { //Transitions
		case Start:
			incState = IncWait;
			i = 0;
			break;
			
		case IncWait:
			incState = (GetBit(a, 1) || GetBit(a, 2)) ? Inc : IncWait;
			if (GetBit(a, 1) && (i < (SCALE_SIZE - 1))) {
				i++;
			} else if (GetBit(a, 2) && (i > 0)) {
				i--;
			}
			break;
			
		case Inc:
			incState = (GetBit(a, 1) || GetBit(a, 2)) ? Inc : IncWait;
			break;
			
		default:
			incState = Start;
			break;
	} //Transitions
	
	switch (incState) { //State Actions
		case Start:
			break;
			
		case IncWait:
			break;
			
		case Inc:
			break;
			
		default:
			break;
	} //State Actions
}