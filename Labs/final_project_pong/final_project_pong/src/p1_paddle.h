/*
 * P1_Paddle.h
 *
 * Created: 2018-08-29 2:20:07 PM
 *  Author: Michael Bradley
 */ 


#ifndef P1_PADDLE_H_
#define P1_PADDLE_H_

#include "../../../../atmel.h"
#include "globals.h"

#define P1_COL_START 0x01
#define P1_ROW_START 0x07
#define P1_ROW_MAX 0xE0

#define P1_SPEED 75

enum P1_States {P1_Start, P1_Wait, P1_Move};
	
int P1_Tick(int state) {
	unsigned char a1 = GetBit(~PINA, 1);
	unsigned char a2 = GetBit(~PINA, 2);
	
	static unsigned char t;
	
	switch (state) { //Transitions
		case P1_Start:
			state = P1_Wait;
			SetPaddle(&p1_g, P1_COL_START, P1_ROW_START);
			t = 0;
			break;
			
		case P1_Wait:
			state = (play_g && (a1 ^ a2)) ? P1_Move : P1_Wait;
			break;
			
		case P1_Move:
			state = (play_g && (a1 ^ a2)) ? P1_Move : P1_Wait;
			break;
			
		default:
			state = P1_Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case P1_Start:
			break;
		
		case P1_Wait:
			if (!play_g) {
				SetPaddle(&p1_g, P1_COL_START, P1_ROW_START);
				t = 0;
			}
			break;
		
		case P1_Move:
			t++;
			if (t > P1_SPEED) {
				if (a1 && (p1_g.row > P1_ROW_START)) {
					p1_g.row >>= 1;
				} else if (a2 && (p1_g.row < P1_ROW_MAX)) {
					p1_g.row <<= 1;
				}
				t = 0;
			}
			break;
		
		default:
			break;
	} //State Actions
	
	return state;
}


#endif /* P1_PADDLE_H_ */