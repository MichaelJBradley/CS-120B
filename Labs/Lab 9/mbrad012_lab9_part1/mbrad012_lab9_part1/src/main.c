#include "../../../../atmel.h"
#include "../../../../utils.h"

#define TASKS 3
#define LED_MAX 4

enum TH_States {TH_Start, TH_Cycle};
enum BL_States {BL_Start, BL_BlinkOn, BL_BlinkOff};
enum MX_States {MX_Start, MX_MUX};

task tasks[TASKS];

const unsigned long periodGDC = 100;

unsigned char thLED_g;
unsigned char blLED_g;

int TH_Tick(int state);
int BL_Tick(int state);
int MX_Tick(int state);

int main (void)
{
	unsigned char i = 0;

	board_init();
	
	//Using the same project for both exercises 1 and 2
	//Move the comments to demonstrate either
	tasks[i++] = CreateTask(TH_Start, /*1000/**/300/**/, &TH_Tick);
	tasks[i++] = CreateTask(BL_Start, 1000, &BL_Tick);
	tasks[i] = CreateTask(MX_Start, /*1000/**/300/**/, &MX_Tick);
	
	TimerSet(periodGDC);
	TimerOn();
	
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
	
	return 0;
}

int TH_Tick(int state) {
	
	switch (state) { //Transitions
		case TH_Start:
			state = TH_Cycle;
			thLED_g = LED_MAX;
			break;
			
		case TH_Cycle:
			state = TH_Cycle;
			break;
			
		default:
			state = TH_Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case TH_Start:
			break;
		
		case TH_Cycle:
			if (thLED_g < LED_MAX) {
				thLED_g <<= 1;
			} else {
				thLED_g = 0x01;
			}
			break;
		
		default:
			break;
	} //State Actions
	
	return state;
}

int BL_Tick(int state) {
	switch (state) { //Transitions
		case BL_Start:
			state = BL_BlinkOn;
			blLED_g = SetBit(blLED_g, 3, 1);
			break;
			
		case BL_BlinkOn:
			state = BL_BlinkOff;
			break;
			
		case BL_BlinkOff:
			state = BL_BlinkOn;
			break;
			
		default:
			state = BL_Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case BL_Start:
			break;
		
		case BL_BlinkOn:
			blLED_g = SetBit(blLED_g, 3, 1);
			break;
		
		case BL_BlinkOff:
			blLED_g = SetBit(blLED_g, 3, 0);
			break;
		
		default:
			break;
	} //State Actions
	
	return state;
}

int MX_Tick(int state) {
	switch (state) { //Transitions
		case MX_Start:
			state = MX_MUX;
			PORTB = blLED_g | thLED_g;
			break;
		
		case MX_MUX:
			state = MX_MUX;
			break;
		
		default:
			state = MX_Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case MX_Start:
			break;
			
		case MX_MUX:
			PORTB = blLED_g | thLED_g;
			break;
			
		default:
			break;
	} //State Actions
	
	return state;
}