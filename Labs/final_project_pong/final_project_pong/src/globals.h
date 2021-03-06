/*
 * globals.h
 *
 * Created: 2018-08-28 5:02:22 PM
 *  Author: Michael Bradley
 */ 


#ifndef GLOBALS_H_
#define GLOBALS_H_

typedef struct displayable {
	unsigned char col;
	unsigned char row;
} displayable;


//Set by main for debugging


//Set by Game Manager (GM) SM
unsigned char intro_g;
unsigned char play_g;
unsigned char gameOver_g;
unsigned char winner_g;

//Set by Intro (IN) SM
unsigned char aiFlag_g;
unsigned char aiLevel_g; //In the range [1, AI_MOVE_CHANCE]
unsigned char aiSelect_g;
unsigned char introFinish_g;

//Set by Output MUX (OM) SM
unsigned char rows_g;
unsigned char cols_g;
unsigned char readyOut_g;

//Set by Player 1 Paddle (P1) SM
displayable p1_g;

//Set by Player 2 Paddle (P2) SM
displayable p2_g;

//Set by AI Paddle (AI) SM
displayable ai_g;

//Set by Ball (BL) SM
displayable ball_g;
displayable * secondPaddle_g;
unsigned char score1_g;
unsigned char score2_g;
unsigned char xDir_g;
unsigned char yDir_g;


void SetDisplayable(displayable * p, unsigned char col, unsigned char row) {
	p->col = col;
	p->row = row;
}

void OutDisplayable(displayable p) {
	cols_g = ~p.col;
	rows_g = p.row;
}

#endif /* GLOBALS_H_ */