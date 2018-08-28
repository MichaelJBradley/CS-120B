#include "../../../atmel.h"
#include "../../../utils.h"
#include "led_out.h"
#include "globals.h"

#define TASKS 1
task tasks[TASKS];

const unsigned char periodGDC = 1;

int main (void)
{	
	unsigned char i = 0;
	
	TimerSet(periodGDC);
	TimerOn();

	board_init();

	DDRB = 0xFF;
	PORTB = 0x00;
	
	tasks[i++] = CreateTask(OUT_Start, periodGDC, &OUT_Tick);

	rows_g = 0x01;
	cols_g = ~0x01;

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
}
