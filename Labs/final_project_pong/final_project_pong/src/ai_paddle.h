/*
 * ai_paddle.h
 *
 * Created: 2018-08-29 8:05:25 PM
 *  Author: Michael Bradley
 */ 
#include <stdlib.h>

#include "p2_paddle.h"
#include "ball.h"

#ifndef AI_PADDLE_H_
#define AI_PADDLE_H_

#define AI_MOVE_CHANCE 3

enum AI_States {AI_Start, AI_Wait, AI_Move};
	
void MoveAI() {
	//Follow the ball
	//Shift the ball 1 to the left to balance the fact that ai_g.row tends to be larger
	//Check to see if it will hit the paddle to avoid extra movement
	//If paddle is smaller move down if it's larger then move up
	//(This should never happen, but) If they're equal do nothing
	if ((ai_g.row < (ball_g.row << 1)) && (ai_g.row < P2_ROW_START) && !WillHitPaddle(yDir_g, ai_g)) {
		ai_g.row <<= 1;
	} else if ((ai_g.row > (ball_g.row << 1)) && (ai_g.row > P2_ROW_MIN) && !WillHitPaddle(yDir_g, ai_g)) {
		ai_g.row >>= 1;
	}
}

int AI_Tick(int state) {
	switch (state) { //Transitions
		case AI_Start:
			state = AI_Wait;
			SetDisplayable(&ai_g, P2_COL_START, P2_ROW_START);
			break;
			
		case AI_Wait:
			state = (play_g && (rand() % AI_MOVE_CHANCE == 0)) ? AI_Move : AI_Wait;
			break;
			
		case AI_Move:
			state = AI_Wait;
			MoveAI();
			break;
			
		default:
			state = AI_Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case AI_Start:
			break;
		
		case AI_Wait:
			if (!play_g) {
				SetDisplayable(&ai_g, P2_COL_START, P2_ROW_START);
			}
			break;
		
		case AI_Move:
			break;
		
		default:
			break;
	} //State Actions
	
	return state;
}


#endif /* AI_PADDLE_H_ */