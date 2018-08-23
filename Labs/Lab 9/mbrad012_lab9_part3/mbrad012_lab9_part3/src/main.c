#include "../../../../atmel.h"
#include "../../../../utils.h"

#define TASKS 3

task tasks[TASKS];

enum INC_States {INC_Start, INC_Wait, INC_Slow, INC_Fast};
enum IN_States {IN_Start, IN_Wait, IN_Inc, IN_Dec, IN_Res};
enum OUT_States {OUT_Start, OUT_MUX};

unsigned char periodGDC = 100;

unsigned char incFlag_g;
unsigned char decFlag_g;

unsigned char count_g;

int INC_Tick(int state);
int DEC_Tick(int state);
int IN_Tick(int state);
int OUT_Tick(int state);

int IN_DetState(unsigned char A0, unsigned char A1);
void IN_SetFlags(unsigned char inc, unsigned char dec);
void INC_IncrementCount();

int main (void)
{
	unsigned char i = 0;
	
	TimerSet(periodGDC);
	TimerOn();
	board_init();

	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	DDRC = 0xFF;
	PORTC = 0x00;
	
	tasks[i++] = CreateTask(IN_Start, 100, &IN_Tick);
	tasks[i++] = CreateTask(INC_Start, 100, &INC_Tick);
	tasks[i] = CreateTask(OUT_Start, 100, &OUT_Tick);
	
	while (1) {
		for (unsigned char i = 0; i < TASKS; i++) {
			if (tasks[i].elapsedTime >= tasks[i].period) {
				tasks[i].state = tasks[i].Tick(tasks[i].state);
				tasks[i].elapsedTime = 0;
			}
			tasks[i].elapsedTime += periodGDC;
		}
		WaitTimer();
	}
}


/************************************************************************/
/*                          Input MUX Functions                         */
/************************************************************************/
int IN_Tick(int state) {
	unsigned char A0 = GetBit(~PINA, 0);
	unsigned char A1 = GetBit(~PINA, 1);
	
	
	switch (state) { //Transitions
		case IN_Start:
			state = IN_Wait;
			IN_SetFlags(0, 0);
			count_g = 0;
			break;
			
		case IN_Wait:
		case IN_Inc:			
		case IN_Dec:
		case IN_Res:
			//The cases are meant to fall through
			//Every state is accessible through every other state
			state = IN_DetState(A0, A1);		
			break;
			
		default:
			state = IN_Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case IN_Start:
			break;
		
		case IN_Wait:
			IN_SetFlags(0, 0);
			break;
		
		case IN_Inc:
			IN_SetFlags(1, 0);
			break;
		
		case IN_Dec:
			IN_SetFlags(0, 1);
			break;
		
		case IN_Res:
			IN_SetFlags(0, 0);
			count_g = 0;
			break;
		
		default:
			break;
	} //State Actions
	
	PORTC = SetBit(PORTC, 1, incFlag_g);
	PORTC = SetBit(PORTC, 0, decFlag_g);
	
	return state;
}

int IN_DetState(unsigned char A0, unsigned char A1) {
	if (!A0 && !A1) {
		return IN_Wait;
	} else if (A0 && !A1) {
		return IN_Inc;
	} else if (!A0 && A1) {
		return IN_Dec;
	} else if (A0 && A1) {
		return IN_Res;
	}
	return IN_Start;
}

void IN_SetFlags(unsigned char inc, unsigned char dec) {
	incFlag_g = inc;
	decFlag_g = dec;
}


/************************************************************************/
/*                       Output MUX Functions                           */
/************************************************************************/
int OUT_Tick(int state) {
	switch (state) { //Transitions
		case OUT_Start:
			state = OUT_MUX;
			break;
			
		case OUT_MUX:
			state = OUT_MUX;
			break;
			
		default:
			state = OUT_Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case OUT_Start:
			PORTB = 0;
			break;
			
		case OUT_MUX:
			PORTB = count_g;
			break;
			
		default:
			break;
	} //State Actions
	
	return state;
}


/************************************************************************/
/*                     Increment Count Functions                        */
/************************************************************************/
int INC_Tick(int state) {
	static unsigned char t;
	static unsigned char slowCount;
	
	switch (state) { //Transitions
		case INC_Start:
			state = INC_Wait;
			t = 0;
			slowCount = 0;
			break;
			
		case INC_Wait:
			if (incFlag_g || decFlag_g) {
				state = INC_Slow;
				INC_IncrementCount();
			} else if (!incFlag_g && !decFlag_g) {
				state = INC_Wait;
			}
			break;	
			
		case INC_Slow:
			if ((incFlag_g || decFlag_g) && slowCount < 3) {
				state = INC_Slow;
			} else if ((incFlag_g || decFlag_g) && slowCount >= 3) {
				state = INC_Fast;
				t = 0;
			} else if (!incFlag_g && !decFlag_g) {
				state = INC_Wait;
			}
			break;
			
		case INC_Fast:
			state = (incFlag_g || decFlag_g) ? INC_Fast : INC_Wait;
			break;
			
		default:
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case INC_Start:
			break;
		
		case INC_Wait:
			t = 0;
			slowCount = 0;
			break;
		
		case INC_Slow:
			if (t >= 10) {
				INC_IncrementCount();
				slowCount++;
				t = 0;
			}
			t++;
			break;
		
		case INC_Fast:
			if (t >= 4) {
				INC_IncrementCount();
				t = 0;
			}
			t++;
			break;
		
		default:
			break;
	} //State Actions
	
	return state;
}

void INC_IncrementCount() {
	if (incFlag_g && (count_g < 9)) {
		count_g++;
	} else if (decFlag_g && (count_g > 0)) {
		count_g--;
	}
}