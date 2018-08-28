#include "../../../../atmel.h"
#include "../../../../utils.h"

#define TASKS 1

task tasks[TASKS];

enum SH_States {SH_Start, SH_In, SH_Out};
	
#define BITS 16
#define WRITE 0x02

int SH_Tick(int state);

void TransmitData(unsigned short data);

void OutputMatrix(unsigned char rows, unsigned char cols);
void OutputData(unsigned char data);

unsigned short out;
unsigned char periodGDC = 10;

int main (void)
{
	unsigned char row = 0x01;
	unsigned char col = 0x01;
	unsigned char i = 0;
	out = ~col;
	out <<= 8;
	out |= row;
	
	TimerSet(periodGDC);
	TimerOn();

	board_init();
	
	DDRB = 0xFF;
	PORTB = 0x00;
	
	tasks[i++] = CreateTask(SH_Start, periodGDC, &SH_Tick);

	/*
	PORTB = 0x00;
	PORTB = 0x04;
	
	PORTB = 0x00;
	PORTB = 0x04;
	
	PORTB = 0x00;
	PORTB = 0x04;
	
	PORTB = 0x00;
	PORTB = 0x04;
	
	PORTB = 0x00;
	PORTB = 0x04;
	
	PORTB = 0x00;
	PORTB = 0x04;
	
	PORTB = 0x00;
	PORTB = 0x04;
	
	PORTB = 0x01;
	PORTB = 0x05; //PORTB bits 1 and 0 are high
	
	PORTB = 0x00;
	PORTB = 0x04; 
	
	PORTB = 0x00;
	PORTB = 0x04;
	
	PORTB = 0x00;
	PORTB = 0x04;
	
	PORTB = 0x00;
	PORTB = 0X04;
	
	PORTB = 0x00;
	PORTB = 0x04;
	
	PORTB = 0x00;
	PORTB = 0x04;
	
	PORTB = 0x00;
	PORTB = 0x04;
	
	PORTB = 0x01;
	PORTB = 0x05;
	
	PORTB = 0x02;
	

	while (1) {
		WaitTimer();
	}

	/**/
	
	while (1) {
		for (unsigned char i = 0; i < TASKS; i++) {
			if (tasks[i].elapsedTime >= tasks[i].period) {
				tasks[i].state = tasks[i].Tick(tasks[i].state);
				tasks[i].elapsedTime = 0;
			}
			tasks[i].elapsedTime += tasks[i].period;
		}
		WaitTimer();
	}
	/**/
}

int SH_Tick(int state) {
	static unsigned char i;
	switch (state) { //Transitions
		case SH_Start:
			state = SH_In;
			i = 1;
			break;
			
		case SH_In:
			state = (i >= 16) ? SH_Out : SH_In;
			PORTB = (out >> (BITS - i++)) & 0x01;
			PORTB = SetBit(PORTB, 2, 1);
			break;
			
		case SH_Out:
			state = SH_Start;
			PORTB = WRITE;
			break;
			
		default:
			state = SH_Start;
			break;
	} //Transitions
	
	//Unused
	switch (state) { //State Actions
		case SH_Start:
		
		break;
		
		case SH_In:
		
		break;
		
		case SH_Out:
		
		break;
		
		default:
		
		break;
	} //State Actions

	return state;
}
/**/

// Combine two unsigned chars, creating one unsigned short
// Inputs:
//	   upper:	the upper byte of the short
//	   lower:	the lower byte of the short
// Output:	An unsigned short consisting of upper and lower
unsigned short ConcatChars(unsigned char upper, unsigned char lower) {
	unsigned short ret = upper;
	ret <<= 8;
	ret |= lower;
	return ret;
}