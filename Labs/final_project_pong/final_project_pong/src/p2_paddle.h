/*
 * p2_paddle.h
 *
 * Created: 2018-08-29 3:42:35 PM
 *  Author: Michael Bradley
 */ 


#ifndef P2_PADDLE_H_
#define P2_PADDLE_H_


#include "../../../../atmel.h"
#include "globals.h"

#define P2_COL_START 0x80
#define P2_ROW_START 0xE0
#define P2_ROW_MIN 0x07

#define P2_SPEED 75

enum P2_States {P2_Start, P2_Wait, P2_Move};

int P2_Tick(int state) {
	unsigned char a3 = GetBit(~PINA, 3);
	unsigned char a4 = GetBit(~PINA, 4);
	
	static unsigned char t;
	
	switch (state) { //Transitions
		case P2_Start:
		state = P2_Wait;
		SetPaddle(&p2_g, P2_COL_START, P2_ROW_START);
		t = 0;
		break;
		
		case P2_Wait:
		state = (play_g && (a3 ^ a4)) ? P2_Move : P2_Wait;
		break;
		
		case P2_Move:
		state = (play_g && (a3 ^ a4)) ? P2_Move : P2_Wait;
		break;
		
		default:
		state = P2_Start;
		break;
	} //Transitions
	
	switch (state) { //State Actions
		case P2_Start:
		break;
		
		case P2_Wait:
		if (!play_g) {
			SetPaddle(&p2_g, P2_COL_START, P2_ROW_START);
			t = 0;
		}
		break;
		
		case P2_Move:
		t++;
		if (t > P2_SPEED) {
			if (a4 && (p2_g.row > P2_ROW_MIN)) {
				p2_g.row >>= 1;
				} else if (a3 && (p2_g.row < P2_ROW_START)) {
				p2_g.row <<= 1;
			}
			t = 0;
		}
		break;
		
		default:
		break;
	} //State Actions
	
	PORTC = SetBit(PORTC, 0, a3);
	PORTC = SetBit(PORTC, 1, a4);
	
	return state;
}


#endif /* P2_PADDLE_H_ */