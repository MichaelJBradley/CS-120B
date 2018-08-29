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

//Set by Output MUX (OM) SM
unsigned char rows_g;
unsigned char cols_g;

unsigned char play_g;

//Set by Player 1 Paddle (P1) SM
displayable p1_g;

//Set by Player 2 Paddle (P2) SM
displayable p2_g;

void SetPaddle(displayable * p, unsigned char col, unsigned char row) {
	p->col = col;
	p->row = row;
}

#endif /* GLOBALS_H_ */