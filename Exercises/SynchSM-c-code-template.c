#include "RIMS.h"

enum States { Start, ... } state;
volatile unsigned char TimerFlag = 0; //ISR raises, main() lowers

void Tick();

void main() {

    B = 0x00; //Initialize outputs
    TimerSet( ... ); //Set period in ms
    TimerOn();
    state = Start; //Indicates initial call

    while(1) {
        Tick();
        while (!TimerFlag) {} //Wait for timer's period
        TimerFlag = 0;
    }
}

void TimerISR() {
    TimerFlag = 1;
}

void Tick() {
    switch (state) { //Transitions
        ...        
    } //Transitions

    switch (state) { //State Actions
        ...        
    } //State Actions

}
