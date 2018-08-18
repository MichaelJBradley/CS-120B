/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <avr/io.h>

void ControlFuelDisplay();

unsigned char led;
unsigned char a;

int main (void)
{
	// Insert system clock initialization code here (sysclk_init()).

	board_init();

	DDRA = 0x00;
	PORTA = 0xFF;
		
	DDRC = 0xFF;
	PORTC = 0x00;
	
	led = 0;
	a = 0;
	
	while(1)
	{
		ControlFuelDisplay();
	}
}

void ControlFuelDisplay() {
	a = ~PINA;
	if (a <= 2) {
		led = 0x60; //1-2 PC5 and PC6 (Low Fuel)
	} else if (a <= 4) {
		led = 0x70; //3-4 PC4-PC5 and PC6 (Low Fuel)
	} else if (a <= 6) {
		led = 0x38; //5-6 PC3-PC5
	} else if (a <= 9) {
		led = 0x3C; //7-9 PC2-PC5
	} else if (a <= 12) {
		led = 0x3E; //10-12 PC1-PC5;
	} else if (a <= 15) {
		led = 0x3F; //13-15 PC0-PC5
	} else {
		led = 0x80;
	}
	
	PORTC = led;
}

