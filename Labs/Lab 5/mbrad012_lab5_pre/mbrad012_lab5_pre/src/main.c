#include <asf.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;	//TimerISR() sets this to 1. C programmer
										//should clear to 0.
										
//Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1;			//Start count from here, down to 0.
										//Default 1 ms.
unsigned long _avr_timer_cntcurr = 0;	//Current internal count of 1 ms ticks.

void TimerOn() {
	//AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;	//bit3 = 0: CTC mode (clear timer on compare)
					//bit2bit1bit0 = 011: pre-scaler /64
					//00001011: 0x0B
					//so, 8 MHz clock of 8,000,000 / 64 = 125,000 tick/s
					//Thus, TCNT1 register will count at 125,000 tick/s
					
	//AVR output compare register OCR1A.
	OCR1A = 125;	//Timer interrupt will be generated when TCNT1 == OCR1A
					//We want a 1 ms tick. 0x001 s * 125,000 tick/s = 125
					//So when TCNT1 register = 125, 1 ms has passed.
					//Thus, we compare to 125.
					
	//AVR timer interrupt mask register
	TIMSK1 = 0x02;	//bit1: OCIE1A -- enables compare match interrupt
	
	//Initialize AVR counter
	TCNT1 = 0;
	
	_avr_timer_cntcurr = _avr_timer_M;
	//TimerISR will be called every _avr_timer_cntcurr milliseconds
	
	//Enable global interrupts
	SREG |= 0x80;	//0x80: 10000000
}

void TimerOff() {
	TCCR1B = 0x00; //bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	//CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--;	//Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { //results in a more efficient compare
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

//Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}



int main (void)
{
	board_init();
	DDRB = 0xFF;
	PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
	unsigned char tmpB = 0x00;
	
	while (1) {
		//User code (i.e. synchSM calls)
		tmpB = ~tmpB;	//Toggle PORTB; Temporary, bad programming style
		PORTB = tmpB;
		while (!TimerFlag);
		TimerFlag = 0;
		//Note: For the above a better style would use a synchSM with a TickSM()
		//This example just illustrates the use of ISR and flag.
	}
}
