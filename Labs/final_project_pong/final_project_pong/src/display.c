/*
 * display.c
 *
 * Created: 2018-08-27 1:02:14 PM
 *  Author: Michael Bradley
 */ 
#include "display.h"

point CreatePoint(unsigned char row, unsigned char col) {
	point ret;
	ret.row = row;
	ret.col = col;
	ret.on = 0;
	
	return ret;
}

void InitMatrix(point matrix[][DISP_SIZE]) {
	unsigned char row;
	unsigned char col;
	for (unsigned char r = 0; r < DISP_SIZE; r++) {
		for (unsigned char c = 0; c < DISP_SIZE; c++) {
			row = 0x01 << r;
			col = 0x01 << c;
			matrix[r][c] = CreatePoint(row, col);
		}
	}
}

unsigned char GetCols(point matrix[][DISP_SIZE]) {
	unsigned char ret = 0;
	for (unsigned char r = 0; r < DISP_SIZE; r++) {
		for (unsigned char c = 0; c < DISP_SIZE; c++) {
			if (matrix[r][c].on) {
				ret |= matrix[r][c].row;
			}
		}
	}
	
	return ~ret;
}

unsigned char GetRowsFromCol(point matrix[][DISP_SIZE], unsigned char col) {
	unsigned char ret;
	for (unsigned char r = 0; r < DISP_SIZE; r++) {
		if (matrix[r][col].on) {
			ret |= matrix[r][col].row;
		}
	}
	
	return ret;
}

unsigned char GetRows(point matrix[][DISP_SIZE]) {
	unsigned char ret = 0;
	for (unsigned char r = 0; r < DISP_SIZE; r++) {
		for (unsigned char c = 0; c < DISP_SIZE; c++) {
			if (matrix[r][c].on) {
				ret |= matrix[r][c].col;
			}
		}
	}
	
	return ret;
}

void TurnMatrix(point matrix[][DISP_SIZE], unsigned char on) {
	for (unsigned char r = 0; r < DISP_SIZE; r++) {
		for (unsigned char c = 0; c < DISP_SIZE; c++) {
			matrix[r][c].on = on;
		}
	}
}