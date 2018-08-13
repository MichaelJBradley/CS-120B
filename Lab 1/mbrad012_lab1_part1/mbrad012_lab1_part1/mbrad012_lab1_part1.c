/*
 * mbrad012_lab1_part1.c
 *
 * Created: 2018-08-06 1:28:26 AM
 *  Author: Michael Bradley
 */ 


#include <avr/io.h>

enum GD_States {GD_SMStart, GD_Unlit, GD_Lit} GD_State;

void TickFct_GarageDoorLED();

int main(void)
{
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	
    while(1)
    {
        TickFct_GarageDoorLED();
    }
	return 0;
}


void TickFct_GarageDoorLED() {
	switch (GD_State) {
		case GD_SMStart:
			GD_State = GD_Unlit;
			break;
		
		case GD_Unlit:
			if ((PINA & 0x01) && !(PINA & 0x02)) {
				GD_State = GD_Lit;
			} else if (!(PINA & 0x01) || (PINA & 0x02)) {
				GD_State = GD_Unlit;
			} else {
				GD_State = GD_SMStart;
			}
			break;
			
		case GD_Lit:
			if (!(PINA & 0x01) || (PINA & 0x02)) {
				GD_State = GD_Unlit;
			} else if ((PINA & 0x01) && !(PINA & 0x02)) {
				GD_State = GD_Lit;
			} else {
				GD_State = GD_SMStart;
			}
			break;
			
		default:
			GD_State = GD_SMStart;		
	}
	
	switch (GD_State) {
		case GD_SMStart:
			PORTB = 0x00;
			break;
			
		case GD_Unlit:
			PORTB = 0x00;
			break;
			
		case GD_Lit:
			PORTB = 0x01;
			break;
			
		default:
			PORTB = 0x00;
			break;
	}
}