#include "../../../../adc.h"
#include "../../../../atmel.h"
#include "../../../../utils.h"

#define TASKS 2
#define LIGHT_MAX (0x47 << 1)
#define LIGHT_THRESH (LIGHT_MAX >> 3)
#define LED_BITS 8

enum RD_States {RD_Start, RD_Sample};
enum LED_States {LED_Start, LED_Disp};

task tasks[TASKS];

const unsigned long periodGDC = 50;
const unsigned long periodRD = 50;
const unsigned long periodLED = 100;

unsigned short light_g;

int RD_Tick(int state);
int LED_Tick(int state);
unsigned char SetLedBits();

int main (void)
{
	unsigned char i = 0;
	
	board_init();

	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	
	TimerSet(periodGDC);
	TimerOn();
	ADC_init();
	
	tasks[i++] = CreateTask(RD_Start, periodRD, &RD_Tick);
	tasks[i] = CreateTask(LED_Start, periodLED, &LED_Tick);
	
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
			light_g = 0;
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
			light_g = ADC;
			break;
			
		default:
			break;
	} //State Actions
	
	return state;
}

int LED_Tick(int state) {
	unsigned char led;
	
	switch (state) { //Transitions
		case LED_Start:
			state = LED_Disp;
			led = 0;
			break;
			
		case LED_Disp:
			state = LED_Disp;
			break;
			
		default:
			state = LED_Start;
			break;
	} //Transitions

	switch (state) { //State Actions
		case LED_Start:
			break;
		
		case LED_Disp:
			led = SetLedBits();
			break;
		
		default:
			break;
	} //State Actions
	
	PORTB = led;
	return state;
}

unsigned char SetLedBits() {
	unsigned char ret = 0;
	for (unsigned char i = 0; i < LED_BITS; i++) {
		if (light_g > LIGHT_THRESH * i) {
			ret = SetBit(ret, i, 1);
		}
	}
	
	return ret;
}