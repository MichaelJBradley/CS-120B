#include "../../../atmel.h"
#include "../../../utils.h"
#include "display.h"

#define TASKS 1
task tasks[TASKS];

const unsigned char periodGDC = 10;

int main (void)
{	
	unsigned char left = 0;
	
	TimerSet(periodGDC);
	TimerOn();

	board_init();

	DDRB = 0xFF;
	PORTB = 0x00;

	while (1) {
		WaitTimer();
	}
}
