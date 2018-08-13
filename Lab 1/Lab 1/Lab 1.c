/*
 * Lab_1.c
 *
 * Created: 2018-08-06 12:00:01 AM
 *  Author: Michael Bradley
 */ 


#include <avr/io.h>

int main(void)
{
	DDRA = 0X00;
	PORTA = 0xFF;
    DDRB = 0xFF;
	PORTB = 0X00;
	while (1) {
		if (PINA & 0x01) {
			tmpB = (tmpB & 0xFC) | 0x01;
		} else {
			tmpB = (tmpB & 0xFC) | 0x02;
		}
		PORTB = tmpB;
	}
	return 0;
}