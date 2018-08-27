/*
 * display.h
 *
 * Created: 2018-08-27 12:53:01 PM
 *  Author: Michael Bradley
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#define DISP_SIZE 8


typedef struct point {
	unsigned char on;
	unsigned char row;
	unsigned char col;	
} point;

point CreatePoint(unsigned char row, unsigned char col);
void InitMatrix(point matrix[][DISP_SIZE]);
unsigned char GetRows(point matrix[][DISP_SIZE]);
unsigned char GetCols(point matrix[][DISP_SIZE]);
void TurnMatrix(point matrix[][DISP_SIZE], unsigned char on);

#endif /* DISPLAY_H_ */