/*
 * game_manager.h
 *
 * Created: 2018-08-30 2:53:11 PM
 *  Author: Michael Bradley
 */ 


#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include "globals.h"

#define GAME_PAD1 1
#define GAME_PAD2 2

enum GM_States {GM_Start, GM_Intro, GM_Play, GM_GameOver};

void GM_SetFlags(unsigned char intro, unsigned char play, unsigned char over) {
	intro_g = intro;
	play_g = play;
	gameOver_g = over;
}

int GM_Tick(int state) {
	switch (state) { //Transitions
		case GM_Start:
			GM_SetFlags(1, 0, 0);
			break;
			
		case GM_Intro:
			
			break;
			
		case GM_Play:
		
			break;
			
		case GM_GameOver:
			
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
			break;
		
		default:
			break;
	} //State Actions
	
	return state;
}

#endif /* GAME_MANAGER_H_ */