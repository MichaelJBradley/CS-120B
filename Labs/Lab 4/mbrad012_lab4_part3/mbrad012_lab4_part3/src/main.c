#include <asf.h>
#include <avr/io.h>
#include "../../../../utils.h"

enum States {Start, Alt, AltWait, Seq, SeqWait} state;

const unsigned char MAX_ALT = 1;
const unsigned char MAX_SEQ = 5;
const unsigned char PAT_ALT = 0x55;

unsigned char a;
unsigned char c;
unsigned char i;

void Tick();

int main (void)
{
	board_init();

	DDRA = 0x00;
	PORTA = 0xFF;
	DDRC = 0xFF;
	PORTC = 0x00;
	
	state = Start;
	a = 0;
	c = 0;
	
	while (1) {
		Tick();
	}
	
}

void Tick() {
	a = ~PINA;
	
	switch (state) { //Transitions
		case Start:
			state = Alt;
			break;
			
		case Alt:
			state = (a & 0x01) ? AltWait : Alt;
			break;
			
		case AltWait:
			if (!(a & 0x01) && (i < MAX_ALT)) {
				state = Alt;
				i++;
			} else if (!(a & 0x01) && (i >= MAX_ALT)) {
				state = Seq;
				i = 0;
			} else if (a & 0x01) {
				state = AltWait;
			} else {
				state = Start;
			}
			break;
			
		case Seq:
			state = (a & 0x01) ? SeqWait : Seq;
			break;
			
		case SeqWait:
			if (!(a & 0x01) && (i < MAX_SEQ)) {
				state = Seq;
				i++;
			} else if (!(a & 0x01) && (i >= MAX_SEQ)) {
				state = Alt;
				i = 0;
			} else if (a & 0x01) {
				state = SeqWait;
			} else {
				state = Start;
			}
			break;
			
		default:
			state = Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case Start:
			i = 0;
			break;
		
		case Alt:
			c = PAT_ALT << i;
			break;
		
		case AltWait:
			break;
		
		case Seq:
			c = 0x01 << i;
			break;
			
		case SeqWait:
			break;
		
		default:
			break;		
	} //State Actions
	
	PORTC = c;
}
