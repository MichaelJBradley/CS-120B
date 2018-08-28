/*
 * led_out.h
 *
 * Created: 2018-08-29 12:10:51 AM
 *  Author: Michael Bradley
 */ 


#ifndef LED_OUT_H_
#define LED_OUT_H_

#include "../../../../atmel.h"
#include "../../../../utils.h"
#include "globals.h"

#define OUT_BITS 16
#define OUT_WRITE 0x02

enum OUT_States {OUT_Start, OUT_Input, OUT_Output};

int OUT_Tick(int state) {
	static unsigned char i;
	switch (state) { //Transitions
		case OUT_Start:
		state = OUT_Input;
		i = 1;
		break;
		
		case OUT_Input:
		state = (i >= 16) ? OUT_Output : OUT_Input;
		
		//Combine cols_g and rows_g into one unsigned short
		//Shift that short a specified number of bits depending on
		//	which iteration we are on
		//Set PORTB to bit0 of the shifted short
		PORTB = (ConcatChars(cols_g, rows_g) >> (OUT_BITS - i++)) & 0x01;
		PORTB = SetBit(PORTB, 2, 1);
		break;
		
		case OUT_Output:
		state = OUT_Start;
		PORTB = OUT_WRITE;
		break;
		
		default:
		state = OUT_Start;
		break;
	} //Transitions
	
	//Stat actions are not used
	switch (state) { //State Actions
		case OUT_Start:
		break;
		
		case OUT_Input:
		break;
		
		case OUT_Output:
		break;
		
		default:
		break;
	} //State Actions

	return state;
}

#endif /* LED_OUT_H_ */