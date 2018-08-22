#include "../../../../atmel.h"
#include "../../../../utils.h"
#include "../../../../adc.h"

#define TASKS 2
#define MAX_LIGHT 0x47
#define LED_LITMUS (MAX_LIGHT >> 1)

enum RD_States {RD_Start, RD_Sample};
enum LED_States {LED_Start, LED_On, LED_Off};

task tasks[TASKS];

const unsigned long periodGDC = 50;
const unsigned long periodRD = 50;
const unsigned long periodLED = 100;

unsigned short light_g;
unsigned char b;
unsigned char d;

int RD_Tick(int state);
int LED_Tick(int state);

void SplitDisplay();

int main (void) {
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
	static unsigned char led;
	
	switch (state) { //Transitions
		case LED_Start:
			state = LED_Off;
			led = 0;
			break;
			
		case LED_Off:
			state = light_g >= MAX_LIGHT ? LED_On : LED_Off;
			break;
			
		case LED_On:
			state = light_g >= MAX_LIGHT ? LED_On : LED_Off;
			break;
			
		default:
			state = LED_Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case LED_Start:
			break;
		
		case LED_Off:
			led = SetBit(led, 0, 0);
			break;
		
		case LED_On:
			led = SetBit(led, 0, 1);
			break;
		
		default:
			break;
	} //State Actions
	
	PORTB = led;
	return state;
}