#include "../../../atmel.h"
#include "../../../utils.h"
#include "display.h"

const unsigned char periodGDC = 10;

int main (void)
{		
	TimerSet(periodGDC);
	TimerOn();

	board_init();

	DDRB = 0xFF;
	PORTB = 0x00; //controls the led rows
	DDRC = 0xFF;
	PORTC = 0xFF; //controls the led cols; according to datasheet the matrix flips outputs

	while (1) {
		WaitTimer();
	}
}
