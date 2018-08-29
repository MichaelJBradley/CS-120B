#include "../../../atmel.h"
#include "../../../utils.h"

#include "p1_paddle.h"
#include "p2_paddle.h"
#include "output_mux.h"
#include "globals.h"

#define TASKS 3
task tasks[TASKS];

const unsigned char periodGDC = 1;

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
	
	//tasks[i++] = CreateTask(OUT_Start, periodGDC, &OUT_Tick);
	tasks[i++] = CreateTask(P1_Start, 50, &P1_Tick);
	tasks[i++] = CreateTask(P2_Start, 50, &P2_Tick);
	tasks[i++] = CreateTask(OM_Start, 5, &OM_Tick);
	
	//Debug
	play_g = 1;
	SetDisplayable(&ball_g, 0x00, 0x00);
	
	/*
	SetDisplayable(&p1_g, 0x01, 0x07);
	OutDisplayable(p1_g);
	OUT_Print();
	
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
