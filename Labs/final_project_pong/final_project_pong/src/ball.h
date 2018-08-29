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

enum BL_States {BL_Start, BL_WaitStart, BL_WaitGame, BL_WaitRelease, BL_Move};
	
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

void TestCollision(unsigned char * xDir, unsigned char * yDir, unsigned char * out) {
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
		if ((ball_g.row & p1_g.row) == 0) {
			*yDir = (*yDir == BALL_POS) ? BALL_NEG : BALL_POS;
		}
	} else if (ball_g.col == (P2_COL_START >> 1) && WillHitPaddle(*yDir, p2_g)) {
		*xDir = BALL_NEG;
		if ((ball_g.row & p2_g.row) == 0) {
			*yDir = (*yDir == BALL_POS) ? BALL_NEG : BALL_POS;
		}
	}
}
	
int BL_Tick(int state) {
	//const unsigned char speed = 1;
	unsigned char a0 = GetBit(~PINA, 0);
	
	static unsigned char out;
	static unsigned char xDir;
	static unsigned char yDir;
	
	static unsigned short speed;
	static unsigned short t;
	
	switch (state) {
		case BL_Start:
			state = BL_WaitStart;
			SetDisplayable(&ball_g, BALL_COL_START, BALL_ROW_START);
			out = 0;
			score1_g = 0;
			score2_g = 0;
			
			xDir = BALL_NEG;
			yDir = BALL_POS;
			
			speed = 200;
			t = 200;
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
			break;
			
		case BL_WaitGame:
			SetDisplayable(&ball_g, BALL_COL_START, BALL_ROW_START);
			break;
			
		case BL_WaitRelease:
			SetDisplayable(&ball_g, BALL_COL_START, BALL_ROW_START);
			out = 0;
			xDir = BALL_NEG;
			yDir = BALL_POS;
			t = 0;
			break;
		
		case BL_Move:
			t++;
			if (t >= speed) {
				TestCollision(&xDir, &yDir, &out);
		
				if (yDir == BALL_POS) {
					ball_g.row <<= 1;
				} else if (yDir == BALL_NEG) {
					ball_g.row >>= 1;
				}
			
				if (xDir == BALL_POS) {
					ball_g.col <<= 1;
				} else if (xDir == BALL_NEG) {
					ball_g.col >>= 1;
				}
				t = 0;
			}
			break;
		
		default:
		
			break;
	}
	
	PORTC = SetBit(PORTC, 0, ball_g.col == (P1_COL_START >> 1));
	PORTC = SetBit(PORTC, 0, WillHitPaddle(&yDir, p1_g));
	
	return state;
}


#endif /* BALL_H_ */