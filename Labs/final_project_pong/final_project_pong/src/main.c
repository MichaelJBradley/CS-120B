#include "../../../atmel.h"
#include "../../../utils.h"

#include "ai_paddle.h"
#include "ball.h"
#include "output_mux.h"
#include "p1_paddle.h"
#include "p2_paddle.h"
#include "globals.h"

#define TASKS 4
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
	
	tasks[i++] = CreateTask(P1_Start, periodGDC, &P1_Tick);
	//tasks[i++] = CreateTask(P2_Start, periodGDC, &P2_Tick);
	tasks[i++] = CreateTask(AI_Start, 75, &AI_Tick);
	tasks[i++] = CreateTask(BL_Start, periodGDC, &BL_Tick);
	tasks[i++] = CreateTask(OM_Start, periodGDC, &OM_Tick);
	
	
	//Debug
	play_g = 1;
	aiFlag_g = 1;
	
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
	/**/
}
