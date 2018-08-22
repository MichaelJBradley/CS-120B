#include "../../../../atmel.h"
#include "../../../../utils.h"

#define TASKS 2
#define LED_MAX 4

enum TH_States {TH_Start, TH_Cycle};
enum BL_States {BL_Start, BL_Blink};

task tasks[TASKS];

const unsigned long periodGDC = 100;

int TH_Tick(int state);
int BL_Tick(int state);

int main (void)
{
	unsigned char i = 0;

	board_init();
	
	tasks[i] = CreateTask(TH_Start, 1000, &TH_Tick);
	i++;
	tasks[i] = CreateTask(BL_Start, 1000, &BL_Tick);
	
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
	unsigned char led;
	switch (state) { //Transitions
		case TH_Start:
			state = TH_Cycle;
			led = 0x01;
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
			if (led > LED_MAX) {
				led <<= 1;
			} else {
				led = 0x01;
			}
			break;
		
		default:
			break;
	} //State Actions
	
	PORTB = led;
	
	return state;
}

int BL_Tick(int state) {
	return state;
}