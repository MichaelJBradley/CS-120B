/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 8/13/2018 15:42:43 PST
*/

#include "rims.h"

unsigned char sequence[] = {4, 8, 15, 16, 23, 42};
unsigned char i;
unsigned char timer;
unsigned char once;
unsigned short MAX_TIME = 12960;
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_Init, SM1_Countdown, SM1_TimeTravel } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_Init;
         break;
         case SM1_Init: 
         if (1) {
            SM1_State = SM1_Countdown;
         }
         break;
      case SM1_Countdown: 
         if (i == 6 && timer <= MAX_TIME) {
            SM1_State = SM1_Init;
         }
         else if (timer <= MAX_TIME) {
            SM1_State = SM1_Countdown;
            if ((A & 0x3F ) == sequence[i] && A6 && !once) {
                i++;
                once = 1;
            } else if ((A & 0x3F) != sequence[i] && A6 && !once) {
                i = 0;
                once = 1;
            }
            
            if (!A6) {
                once = 0;
            }
            
            B = i;
         }
         else if (timer > MAX_TIME) {
            SM1_State = SM1_TimeTravel;
         }
         break;
      case SM1_TimeTravel: 
         if (1) {
            SM1_State = SM1_TimeTravel;
         }
         break;
      default:
         SM1_State = SM1_Init;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_Init:
         i = 0;
         timer = 0;
         once = 0;
         break;
      case SM1_Countdown:
         
         
         timer++;
         break;
      case SM1_TimeTravel:
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodState_machine_1 = 500;
   TimerSet(periodState_machine_1);
   TimerOn();
   
   SM1_State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_State_machine_1();
      while(!SM1_Clk);
      SM1_Clk = 0;
   } // while (1)
} // Main