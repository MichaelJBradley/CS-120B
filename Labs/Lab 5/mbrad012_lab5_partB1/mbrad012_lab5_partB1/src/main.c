#include "../../../../atmel.h"
#include "../../../../utils.h"
#include "../../../../lcd/io.c"

enum States {Start, Wait, Inc, Dec} state;

unsigned char i;
unsigned char preI;
unsigned char t;

unsigned char a;

void Tick();
void CheckReset();
void CheckInc();
void CheckDec();

int main (void)
{
	board_init();

	DDRA = 0x00;
	PORTA = 0xFF;
	DDRC = 0xFF;
	PORTC = 0x00;
	DDRD = 0xFF;
	PORTD = 0x00;
	
	TimerSet(100);
	TimerOn();
	LCD_init();
	
	state = Start;
	
	while (1) {
		Tick();
		WaitTimer();
	}
}

void Tick() {
	a = ~PINA;
	
	switch (state) { //Transitions
		case Start:
			state = Wait;
			i = 0;
			preI = 10;
			t = 0;
			break;
			
		case Wait:
			if ((a & 0x01) && (a & 0x02) || (!a)) {
				state = Wait;
			} else if (a & 0x01) {
				state = Inc;
				CheckInc();
			} else if (a & 0x02) {
				state = Dec;
				CheckDec();
			}
			t = 0;
			break;
			
		case Inc:
			if (((a & 0x01) && (a & 0x02)) || (!a))  {
				state = Wait;
			} else if (a & 0x01) {
				state = Inc;
			}
			break;
			
		case Dec:
			if (((a & 0x01) && (a & 0x02)) || (!a))  {
				state = Wait;
			} else if (a & 0x01) {
				state = Dec;
			}
			break;
			
		default:
			state = Start;
			break;
	} //Transitions

	switch (state) { //State Actions
		case Start:
			break;
		
		case Wait:
			CheckReset();
			break;
		
		case Inc:
			//CheckReset();
			if (t >= 10) {
				CheckInc();
				t = 0;
			}
			t++;
			break;
		
		case Dec:
			//CheckReset();
			if (t >= 10) {
				CheckDec();
				t = 0;
			}
			t++;
			break;
		
		default:
			break;
	} //State Actions
	
	if (i != preI) {
		LCD_ClearScreen();
		LCD_WriteData(i + '0'); //ClearScreen() finishes after this call
		LCD_WriteData(i + '0');
		preI = i;
	}
}

void CheckReset() {
	if ((a & 0x01) && (a & 0x02)) {
		i = 0;
	}
}

void CheckInc() {
	if (i < 9) {
		i++;
	}
}

void CheckDec() {
	if (i > 0) {
		i--;
	}
}