/*
 * intro.h
 *
 * Created: 2018-08-30 4:44:20 PM
 *  Author: Michael Bradley
 */ 


#ifndef INTRO_H_
#define INTRO_H_

#include "../../../atmel.h"
#include "../../../utils.h"


enum IN_States {IN_Start, IN_Wait, IN_SelectGame, IN_ReleaseGame,
	 IN_SelectAI, IN_ReleaseAI, IN_Finish};
	
int IN_Tick(int state) {
	unsigned char a0 = GetBit(~PINA, 0);
	unsigned char a1 = GetBit(~PINA, 1);
	unsigned char a2 = GetBit(~PINA, 2);
	
	static unsigned char incAIOnce;
	
	switch (state) { //Transitions
		case IN_Start:
			state = IN_Wait;
			aiFlag_g = 1;
			aiLevel_g = 1;
			aiSelect_g = 0;
			introFinish_g = 0;
			incAIOnce = 0;
			break;
			
		case IN_Wait:
			state = (intro_g && (a1 ^ a2)) ? IN_SelectGame : IN_Wait;
			break;
			
		case IN_SelectGame:
			//if intro_g and only a1 or only a2 is pressed
			//then stay in select game
			if (intro_g && (a1 ^ a2)) {
				state = IN_SelectGame;
			
			//if not intro_g or both a1 and a2 are pressed then go back
			//to intro wait
			} else if (!intro_g || (a1 && a2)) {
				state = IN_Wait;
				
			//if we're already in select game and neither a1 nor a2 are pressed
			//then that means we've already chosen the game type and we're
			//ready to move on
			} else if (intro_g && !a1 && !a2) {
				if (aiFlag_g) {
					state = IN_SelectAI;
					aiSelect_g = 1;
				} else {
					state = IN_Finish;
				}
			}
			break;
			
		case IN_SelectAI:
			if (!intro_g) {
				state = IN_Wait;
			} else {
				if (a0) {
					state = IN_ReleaseAI;
				} else {
					state = IN_SelectAI;
				}
			}
			break;
			
		case IN_ReleaseAI:
			if (intro_g && a0) {
				state = IN_ReleaseAI;
			} else if (intro_g && !a0) {
				state = IN_Finish;
				
			} else {
				state = IN_Wait;
			}
			break;
			
		case IN_Finish:
			state = intro_g ? IN_Finish : IN_Wait;
			break;
			
		default:
			state = IN_Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case IN_Start:
		
		break;
		
		case IN_Wait:
			aiSelect_g = 0;
			incAIOnce = 0;
			if (intro_g) {
				aiFlag_g = 1;
				aiLevel_g = 1;
			} else {
				introFinish_g = 0;
			}
			break;
		
		case IN_SelectGame:
			if (a1) {
				aiFlag_g = 1;
			} else if (a2) {
				aiFlag_g = 0;
			}
			break;
		
		case IN_SelectAI:
			if (!incAIOnce && a1 && aiLevel_g < AI_MOVE_CHANCE) {
				aiLevel_g++;
				incAIOnce = 1;
			} else if (!incAIOnce && a2 && aiLevel_g > 1) {
				aiLevel_g--;
				incAIOnce = 1;
			}
			
			if (!a1 && !a2) {
				incAIOnce = 0;
			}
			break;
			
		case IN_ReleaseAI:
			break;
			
		case IN_Finish:
			introFinish_g = 1;
			break;
		
		default:
			break;
	} //State Actions
	
	return state;
}


#endif /* INTRO_H_ */