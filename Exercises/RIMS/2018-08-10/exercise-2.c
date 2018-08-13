#include "Rims.h"

enum States {Start, OFF, ON} state;

void Tick();

int main() {
    B = 0;
    state = Start;
    while (1) {
        Tick();
    }

    return 0;
}

void Tick() {
    switch (state) { //Transitions
        case Start:
	    state = OFF;
	    B = 0;
	    break;

	case OFF:
	    if (A0) {
		state = ON;
                B = 0x01;
	    } else {
                state = OFF;
	    }
	    break;

	case ON:
            if (A0) {
                state = ON;
	    } else {
                state = OFF;
		B = 0;
	    }
	    break;

	default:
	    state = Start;
    } //Transitions

    //No state actions needed
    /*
    switch (state) { //State Actions
        case Start:
	    break;

	case OFF:
	    break;

	case ON:
	    break;

	default:
	    break;
    } //State Actions
    */
}
