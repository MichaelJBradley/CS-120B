/*
 * output_mux.h
 *
 * Created: 2018-08-29 4:09:55 PM
 *  Author: Michael Bradley
 */ 


#ifndef OUTPUT_MUX_H_
#define OUTPUT_MUX_H_

#include "../../../../utils.h"
#include "globals.h"

#define OM_OUTPUTS 3

#define OM_WRITE 0x02
#define OM_BITS 16

enum OM_States {OM_Start, OM_SetOut};
	
void OM_Write() {
	for (unsigned i = 1; i <= OM_BITS; i++) {
		PORTB = (ConcatChars(cols_g, rows_g) >> (OM_BITS - i)) & 0x01;
		PORTB = SetBit(PORTB, 2, 1);
	}
	PORTB = OM_WRITE;
}
	
int OM_Tick(int state) {
	static unsigned char i;
	
	switch (state) { //Transitions
		case OM_Start:
			state = OM_SetOut;
			i = 0;
			rows_g = 0x00;
			cols_g = 0x00;
			break;
			
		case OM_SetOut:
			state = OM_SetOut;
			break;
			
		default:
			state = OM_Start;
			break;
	} //Transitions
	
	switch (state) { //State Actions
		case OM_Start:
			break;
		
		case OM_SetOut:
			if (i % OM_OUTPUTS == 0) {
				OutDisplayable(p1_g);
			} else if (i % OM_OUTPUTS == 1) {
				if (aiFlag_g) {
					OutDisplayable(ai_g);
				} else {
					OutDisplayable(p2_g);
				}
			} else if (i % OM_OUTPUTS == 2) {
				OutDisplayable(ball_g);
			}
			i++;
			OM_Write();
			break;

		default:
			break;
	} //State Actions
	
	return state;
}

#endif /* OUTPUT_MUX_H_ */