#include "Rims.h"

enum States {Start, OFF_RELEASE, ON_PRESS, ON_RELEASE, OFF_PRESS} state;

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
            state = OFF_RELEASE;
	    B = 0;
	    break;

	case OFF_RELEASE:
	    state = A0 ? ON_PRESS : OFF_RELEASE;
	    B = A0 ? 0x01 : B;
	    break;

	case ON_PRESS:
	    state = A0 ? ON_PRESS : ON_RELEASE;
	    break;

	case ON_RELEASE:
	    state = A0 ? OFF_PRESS : ON_RELEASE;
	    B = A0 ? 0 : B;
	    break;

	case OFF_PRESS:
	    state = A0 ? OFF_PRESS : OFF_RELEASE;

	default:
	    state = Start;
    } //Transitions

    //No state actions needed
}
