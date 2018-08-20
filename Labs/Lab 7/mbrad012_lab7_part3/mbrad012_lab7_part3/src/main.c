#include "../../../../pwm.h"
#include "../../../../utils.h"

enum States {Start, Stop, Play, Down, Finish} state;

enum Notes {C4, D4, E4, F4, G4, A4, B4, C5, D5};
double scale[] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25, 587.33};
unsigned char notes[] = {F4, B4, C5, D5, C5, A4, G4, A4, B4, F4};
unsigned short upTime[] = {800, 400, 400, 400, 400, 400, 400, 400, 400, 1600};
unsigned char downTime[] = {100, 50, 50, 50, 50, 50, 50, 50, 50, 0};
const unsigned char MAX_NOTES = 10;

unsigned char a;
unsigned char b;
	
unsigned char i;
unsigned short t;
unsigned char next;


void Tick();
unsigned char PlayNote();
unsigned char WaitNote();

int main (void)
{
	board_init();

	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	
	TimerSet(1);
	TimerOn();
	
	state = Stop;
	
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
			i = 0;
			t = 0;
			next = 0;
			break;
			
		case Stop:
			if (GetBit(a, 0)) {
				state = Play;
				i = 0;
				t = 0;
				next = 0;
				PWM_on();
				set_PWM(scale[notes[i]]);
			} else {
				state = Stop;
			}
			break;
			
		case Play:
			if (i >= MAX_NOTES) {
				state = Finish;
				PWM_off();
			} else if (next && i < MAX_NOTES) {
				state = Down;
				PWM_off();
				t = 0;
				next = 0;
			} else if (!next && i < MAX_NOTES) {
				state = Play;
			}
			break;
			
		case Down:
			if (next) {
				state = Play;
				PWM_on();
				set_PWM(scale[notes[i++]]);
				next = 0;
				t = 0;
			} else {
				state = Down;
			}
			break;	
		
		case Finish:
			state = GetBit(a, 0) ? Finish : Stop;
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
			next = PlayNote();
			break;
			
		case Down:
			next = WaitNote();
			break;	
		
		case Finish:
			break;
		
		default:
			break;
	} //State Actions
	
	PORTB = SetBit(PORTB, 0, GetBit(a, 0));
	PORTB = SetBit(PORTB, 1, state == Play);
}

/*
 * Increases t until the play time (upTime[i]) is reached.
 * Returns 0 if play time has not been reached or 1 otherwise.
 */
unsigned char PlayNote() {
	t++;
	if (t <= upTime[i]) {
		return 0;
	} else if (t > upTime[i]) {
		return 1;
	}
	
	return 1;
}

/*
 * Increases t until the down time (downTime[i]) is reached.
 * Returns 0 if down time has not been reached or 1 otherwise.
 */
unsigned char WaitNote() {
	t++;
	if (t <= downTime[i]) {
		return 0;
	} else if (t > downTime[i]) {
		return 1;
	}
	
	return 1;
}