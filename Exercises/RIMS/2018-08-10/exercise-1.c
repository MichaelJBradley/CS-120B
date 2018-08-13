#include "Rims.h"

enum States {Start, Off, On} state;

void Tick();

int main() {
    B = 0x00;
    state = Start;

    while (1) {
        Tick();
    }

    return 0;
}

void Tick() {
    switch (state) { //Transitions
        case Start:
	    state = Off;
	    break;

	case Off:
	    if (!A0) {
	        state = Off;
	    } else {
                state = On;
	    }
	    break;

        case On:
	    if (A0) {
                state = On;
	    } else {
                state = Off;
	    }
	    break;

	default:
	    state = Start;
	    break;
    } //Transitions

    switch (state) { //State Actions
        case Off:
	    B = 0;
	    break;

	case On:
	    B = 0x01;
	    break;

	default:
            B = 0;
	    break;
    } //State Actions
}
