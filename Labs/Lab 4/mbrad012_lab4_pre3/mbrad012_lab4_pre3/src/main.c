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

int main (void)
{
	// Insert system clock initialization code here (sysclk_init()).

	board_init();
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRC = 0xFF;
	PORTC = 0x00;
	// Insert application code here, after the board has been initialized.
	
	unsigned char led = 0x00;
	unsigned char button = 0x00;
	
	while (1) {
		button = (~PINA) & 0x01;
		
		if (button) {
			led = 0xFF;
		} else {
			led = 0;
		}
		
		PORTC = led;
	}
}
