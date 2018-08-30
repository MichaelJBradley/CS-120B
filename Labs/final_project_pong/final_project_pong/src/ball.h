/*
 * ball.h
 *
 * Created: 2018-08-29 5:24:27 PM
 *  Author: Michael Bradley
 */ 


#ifndef BALL_H_
#define BALL_H_

#include "p1_paddle.h"
#include "p2_paddle.h"

#define BALL_COL_START 0x10
#define BALL_ROW_START 0x01

#define BALL_POS 0
#define BALL_NEG 1
#define BALL_TOP 0x01
#define BALL_BOT 0x80

#define BALL_SPEED_MAX 50
#define BALL_SPEED_MIN 300

enum BL_States {BL_Start, BL_WaitStart, BL_WaitGame, BL_WaitRelease, BL_Move};
	
void SetSecondPaddle() {
	if (aiFlag_g) {
		secondPaddle_g = &ai_g;
	} else {
		secondPaddle_g = &p2_g;
	}
}

void IncreaseSpeed(unsigned short * speed) {
	//smaller number means faster ball because tick happens more frequently
	if (*speed > BALL_SPEED_MAX) {
		*speed -= 50;
	}
}

void DecreaseSpeed(unsigned short * speed) {
	//larger number means slower ball because tick happens less frequently
	if (*speed < BALL_SPEED_MIN) {
		*speed += 50;
	}
}
	
void IncScore() {
	if (ball_g.col == P1_COL_START) {
		score2_g++;
	} else if (ball_g.col == P2_COL_START) {
		score1_g++;
	}
}

unsigned char WillHitPaddle(unsigned char yDir, displayable paddle) {
	if (yDir == BALL_NEG) {
		return (ball_g.row >> 1) & paddle.row;
	}
	
	if (yDir == BALL_POS) {
		return (ball_g.row << 1) & paddle.row;
	}
	
	return 0;
}

void TestCollision(unsigned char * xDir, unsigned char * yDir, unsigned char * out, unsigned short * speed) {
	if (ball_g.col == P1_COL_START || ball_g.col == P2_COL_START) {
		*out = 1;
		return;
	}
	
	if (ball_g.row == BALL_TOP) {
		*yDir = BALL_POS;
	} else if (ball_g.row == BALL_BOT) {
		*yDir = BALL_NEG;
	}
	
	//if the ball is in the column next to the paddle and it will hit the paddle
	if (ball_g.col == (P1_COL_START << 1) && WillHitPaddle(*yDir, p1_g)) {
		*xDir = BALL_POS;
		
		//if the ball is not over the paddle then it will hit a corner
		if ((ball_g.row & p1_g.row) == 0) {
			*yDir = (*yDir == BALL_POS) ? BALL_NEG : BALL_POS;
			//if the ball hits a corner increase speed
			IncreaseSpeed(speed);
		} else {
			//if the ball hits the center decrease speed
			DecreaseSpeed(speed);
		}
	} else if (ball_g.col == (P2_COL_START >> 1) && WillHitPaddle(*yDir, *secondPaddle_g)) {
		*xDir = BALL_NEG;
		if ((ball_g.row & secondPaddle_g->row) == 0) {
			*yDir = (*yDir == BALL_POS) ? BALL_NEG : BALL_POS;
			IncreaseSpeed(speed);
		} else {
			DecreaseSpeed(speed);
		}
	}
}
	
int BL_Tick(int state) {
	//const unsigned char speed = 1;
	unsigned char a0 = GetBit(~PINA, 0);
	
	static unsigned char out;
	
	static unsigned short speed;
	static unsigned short t;
	
	switch (state) {
		case BL_Start:
			state = BL_WaitStart;
			SetDisplayable(&ball_g, BALL_COL_START, BALL_ROW_START);
			out = 0;
			score1_g = 0;
			score2_g = 0;
			
			xDir_g = BALL_NEG;
			yDir_g = BALL_POS;
			
			speed = 200;
			t = 200;
			
			SetSecondPaddle();
			break;
			
		case BL_WaitStart:
			state = play_g ? BL_WaitGame : BL_WaitStart;
			break;
			
		case BL_WaitGame:
			if (!play_g) {
				state = BL_WaitStart;
			} else {
				state = !a0 ? BL_WaitGame : BL_WaitRelease;
			}
			break;
			
		case BL_WaitRelease:
			if (!play_g) {
				state = BL_WaitStart;
			} else {
				state = !a0 ? BL_Move : BL_WaitRelease;
			}
			break;
			
		case BL_Move:
			if (!play_g) {
				state = BL_WaitStart;
			} else if (out) {
				state = BL_WaitGame;
				out = 0;
				IncScore();
			} else if (play_g && !out) {
				state = BL_Move;
			}
			break;
			
		default:
			state = BL_Start;
			break;
	}
	
	switch (state) {
		case BL_Start:
			break;
		
		case BL_WaitStart:
			SetDisplayable(&ball_g, BALL_COL_START, BALL_ROW_START);
			score1_g = 0;
			score2_g = 0;
			SetSecondPaddle();
			break;
			
		case BL_WaitGame:
			SetDisplayable(&ball_g, BALL_COL_START, BALL_ROW_START);
			break;
			
		case BL_WaitRelease:
			SetDisplayable(&ball_g, BALL_COL_START, BALL_ROW_START);
			out = 0;
			xDir_g = BALL_NEG;
			yDir_g = BALL_POS;
			t = 0;
			break;
		
		case BL_Move:
			t++;
			if (t >= speed) {
				TestCollision(&xDir_g, &yDir_g, &out, &speed);
		
				if (yDir_g == BALL_POS) {
					ball_g.row <<= 1;
				} else if (yDir_g == BALL_NEG) {
					ball_g.row >>= 1;
				}
			
				if (xDir_g == BALL_POS) {
					ball_g.col <<= 1;
				} else if (xDir_g == BALL_NEG) {
					ball_g.col >>= 1;
				}
				t = 0;
			}
			break;
		
		default:
		
			break;
	}
	
	return state;
}


#endif /* BALL_H_ */