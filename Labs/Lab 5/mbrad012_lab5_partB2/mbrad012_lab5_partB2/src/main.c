#include "../../../../atmel.h"
#include "../../../../utils.h"
#include "../../../../lcd/io.c"

enum States {Start, Blink, HoldDown, HoldUp, ResDown, Win, Res} state;

const unsigned char MAX_LED = 2;
const unsigned char MAX_TIME = 3;
unsigned char i;
unsigned char t;
unsigned char score;
unsigned char preScore;
unsigned char winFlag;

unsigned char a;
unsigned char b;

void Tick();
void Reset();

int main (void)
{
	board_init();
	
	DDRB = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
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
			state = Blink;
			Reset();
			score = 0;
			preScore = 10;
			winFlag = 0;
			break;
		
		case Blink:
			state = GetBit(a, 0) ? HoldDown : Blink;
			if (GetBit(a, 0)) {
				if (b & 0x02) {
					if (score < 9) {
						score++;
					}
					} else {
					if (score > 0) {
						score--;
					}
				}
			}
			break;
		
		case HoldDown:
			state = GetBit(a, 0) ? HoldDown : HoldUp;
			break;
		
		case HoldUp:
			if (score == 9) {
				state = Win;
			} else {
				state = GetBit(a, 0) ? ResDown : HoldUp;
			}
			break;
		
		case ResDown:
			state = GetBit(a, 0) ? ResDown : Blink;
			break;
		
		case Win:
			state = (GetBit(a, 0) && GetBit(a, 1)) ? Res : Win;
			break;
		
		case Res:
			state = a ? Res : Blink;
		
		default:
			state = Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case Start:
			break;
		
		case Blink:
			t++;
			if (t > MAX_TIME) {
				t = 0;
				if (i > MAX_LED) {
					i = 0;
					} else {
					i++;
				}
			}
			b = 0x01 << i;
			break;
		
		case HoldDown:

			break;
		
		case HoldUp:
			break;
		
		case ResDown:
			Reset();
			break;
			
		case Win:
			if (!winFlag) {
				LCD_DisplayString(1, "You're like     super manly cool");
				winFlag = 1;
			}
			break;
			
		case Res:
			Reset();
			winFlag = 0;
			score = 0;
		
		default:
			break;
	} //State Actions
	
	PORTB = b;
	PORTB = SetBit(PORTB, 3, GetBit(a, 0));
	
	if (score != preScore) {
		LCD_ClearScreen();
		LCD_WriteData(score + '0');
		LCD_WriteData(score + '0');
		preScore = score;
	}
}

void Reset() {
	i = 0;
	t = 0;
	b = 0;
}
