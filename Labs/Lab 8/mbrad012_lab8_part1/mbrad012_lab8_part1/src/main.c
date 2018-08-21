#include "../../../../atmel.h"
#include "../../../../utils.h"
#include "../../../../adc.h"

#define TASKS 2

enum RD_States {RD_Start, RD_Sample};
enum DS_States {DS_Start, DS_Display};
	
task tasks[TASKS];

const unsigned long periodGDC = 50;
const unsigned long periodRD = 50;
const unsigned long periodDS = 100;
	
unsigned short analog_g;
unsigned char b;
unsigned char d;
	
int RD_Tick();
int DS_Tick();

void SplitDisplay();

int main (void) {	
	unsigned char i = 0;
	
	board_init();
	
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	DDRD = 0xFF;
	PORTD = 0x00;
	
	DDRC = 0xFF;
	PORTC = 0x00;
	
	TimerSet(periodGDC);
	TimerOn();
	ADC_init();
	
	tasks[i++] = CreateTask(RD_Start, periodRD, &RD_Tick);
	tasks[i] = CreateTask(DS_Start, periodDS, &DS_Tick);
	
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

int RD_Tick(int state) {
	switch (state) { //Transitions
		case RD_Start:
			state = RD_Sample;
			analog_g = 0;
			break;
			
		case RD_Sample:
			state = RD_Sample;
			break;
			
		default:
			state = RD_Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case RD_Start:
			break;
			
		case RD_Sample:
			analog_g = ADC;
			break;
			
		default:
			break;
	} //State Actions
	
	return state;
}

int DS_Tick(int state) {
	switch (state) { //Transitions
		case DS_Start:
			state = DS_Display;
			b = 0;
			d = 0;
			break;
			
		case DS_Display:
			state = DS_Display;
			break;
			
		default:
			state = DS_Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case DS_Start:
			break;
		
		case DS_Display:
			SplitDisplay();
			break;
		
		default:
			break;
	} //State Actions
	
	PORTB = b;
	PORTD = d;
	
	PORTC = d;
	return state;
}

void SplitDisplay() {
	b = analog_g & 0xFF;
	d = (analog_g & 0x0300) >> 8; //0000 0011 0000 0000
}