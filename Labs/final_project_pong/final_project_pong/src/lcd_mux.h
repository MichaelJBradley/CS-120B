/*
 * lcd_mux.h
 *
 * Created: 2018-08-30 3:22:29 PM
 *  Author: Michael Bradley
 */ 


#ifndef LCD_MUX_H_
#define LCD_MUX_H_

#include "lcd_io.h"

#include "game_manager.h"
#include "globals.h"

enum LM_States {LM_Start, LM_Intro, LM_Play, LM_GameOver};

void LM_SafeClear() {
	LCD_ClearScreen();
	delay_ms(1);
}

int LM_DisplayWinner() {
	LCD_DisplayString(1,"Paddle ");
	//if paddle 1 wins write char 1 otherwise write char 2
	LCD_WriteData((winner_g == GAME_PAD1 ? GAME_PAD1 : GAME_PAD2) + '0');
	LCD_WriteData(' ');
	LCD_WriteData('W');
	LCD_WriteData('i');
	LCD_WriteData('n');
	LCD_WriteData('s');
	LCD_WriteData('!');
}

void LM_DisplayScore(unsigned char score, unsigned char playerNum) {
	LCD_WriteData('P');
	LCD_WriteData(playerNum + '0');
	LCD_WriteData(':');
	LCD_WriteData(' ');
	LCD_WriteData(score + '0');
}

void LM_DisplayScores() {
	LM_SafeClear();
	LM_DisplayScore(score1_g, 1);
	for (unsigned char i = 0; i < 6; i++) {
		LCD_WriteData(' ');
	}
	LM_DisplayScore(score2_g, 2);
}

int LM_Tick(int state) {
	static unsigned char prevScore1;
	static unsigned char prevScore2;
	
	switch (state) {
		case LM_Start:
			//start scores off with highest number so that a score write
			//   is inevitable on the first tick of play
			prevScore1 = -1;
			prevScore2 = -1;
			state = LM_Intro;
			break;
			
		case LM_Intro:
			if (intro_g) {
				state = LM_Intro;
			} else if (play_g) {
				state = LM_Play;
			} else if (gameOver_g) {
				state = LM_GameOver;
				LM_DisplayWinner();
			}
			
		case LM_Play:
			if (intro_g) {
				state = LM_Intro;
			} else if (play_g) {
				state = LM_Play;
			} else if (gameOver_g) {
				state = LM_GameOver;
				LM_DisplayWinner();
			}
		
		case LM_GameOver:
			if (intro_g) {
				state = LM_Intro;
			} else if (play_g) {
				state = LM_Play;
			} else if (gameOver_g) {
				state = LM_GameOver;
			}
			break;
			
		default:
			state = LM_Start;
			break;
	}
	
	switch (state) {
		case LM_Start:
		
		break;
		
		case LM_Intro:
			LCD_DisplayString(1, "Intro");
			break;
		
		case LM_Play:
			if (prevScore1 != score1_g || prevScore2 != score2_g) {
				LM_DisplayScores();
				prevScore1 = score1_g;
				prevScore2 = score2_g;
			}
			break;
		
		case LM_GameOver:
			break;
		
		default:
		
		break;
	}
	
	return state;
}

#endif /* LCD_MUX_H_ */