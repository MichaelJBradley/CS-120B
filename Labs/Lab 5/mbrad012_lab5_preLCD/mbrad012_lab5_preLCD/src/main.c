#include <asf.h>
#include <avr/io.h>
#include "../../../../lcd/io.c"


int main (void)
{
	board_init();
	DDRC = 0xFF;
	PORTC = 0x00;
	DDRD = 0xFF;
	PORTD = 0x00;
	
	LCD_init();
	
	LCD_DisplayString(1, "Hello, World!");
	
	while(1) {continue;}
}
