/*
 * game_manager.h
 *
 * Created: 2018-08-30 2:53:11 PM
 *  Author: Michael Bradley
 */ 


#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include "../../../atmel.h"
#include "../../../utils.h"

#include "globals.h"

#define GAME_PAD1 1
#define GAME_PAD2 2

#define GAME_MAX_SCORE 5

enum GM_States {GM_Start, GM_Intro, GM_Play, GM_GameOver, GM_GORelease,
	GM_Reset, GM_ResetRelease};

int GM_IsReset(unsigned char a0, unsigned char a1, unsigned char a2) {
	return a0 && a1 && a2;
}

void GM_SetFlags(unsigned char intro, unsigned char play, unsigned char over) {
	intro_g = intro;
	play_g = play;
	gameOver_g = over;
}

int GM_Tick(int state) {
	unsigned char a0 = GetBit(~PINA, 0);
	unsigned char a1 = GetBit(~PINA, 1);
	unsigned char a2 = GetBit(~PINA, 2);
	
	switch (state) { //Transitions
		case GM_Start:
			GM_SetFlags(1, 0, 0);
			state = GM_Intro;
			break;
			
		case GM_Intro:
			state = introFinish_g ? GM_Play : GM_Intro;	
			break;
			
		case GM_Play:
			if (score1_g >= GAME_MAX_SCORE || score2_g >= GAME_MAX_SCORE) {
				state = GM_GameOver;
			} else {
				state = GM_Play;
			}
			if (GM_IsReset(a0, a1, a2)) {
				state = GM_Reset;
			}			
			break;
			
		case GM_GameOver:
			state = a0 ? GM_GORelease : GM_GameOver;
			if (GM_IsReset(a0, a1, a2)) {
				state = GM_Reset;
			}
			break;
			
		case GM_GORelease:
			state = a0 ? GM_GORelease : GM_Intro;
			if (GM_IsReset(a0, a1, a2)) {
				state = GM_Reset;
			}
			break;
			
		case GM_Reset:
			state = GM_IsReset(a0, a1, a2) ? GM_Reset : GM_ResetRelease;
			break;
			
		case GM_ResetRelease:
			state = (!a0 && !a1 && !a2) ? GM_Intro : GM_ResetRelease;
			break;
			
		default:
			state = GM_Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case GM_Start:
			break;
		
		case GM_Intro:
			GM_SetFlags(1, 0, 0);
			break;
		
		case GM_Play:
			GM_SetFlags(0, 1, 0);
			break;
		
		case GM_GameOver:
			GM_SetFlags(0, 0, 1);
			//Not sure how a tie is possible, but if it happened
			//Then it should goto paddle 1
			if (score1_g < score2_g) {
				winner_g = GAME_PAD2;
			} else {
				winner_g = GAME_PAD1;
			}
			break;
			
		case GM_GORelease:
			break;
		
		case GM_Reset:
			break;
			
		case GM_ResetRelease:
			break;
		
		default:
			break;
	} //State Actions
	
	PORTD = SetBit(PORTD, 0, aiFlag_g);
	
	return state;
}

#endif /* GAME_MANAGER_H_ */