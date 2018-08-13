/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 8/13/2018 14:1:58 PST
*/

#include "rims.h"

unsigned char score;
unsigned char i;
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_WaitToken, SM1_Play, SM1_Display } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_WaitToken;
         break;
         case SM1_WaitToken: 
         if (A0) {
            SM1_State = SM1_Play;
            score = 0;
            i = 0;
         }
         else if (!A0) {
            SM1_State = SM1_WaitToken;
         }
         break;
      case SM1_Play: 
         if (i <= 120 && score < 115) {
            SM1_State = SM1_Play;
            if (A1) score += 1;
            else if (A2) score += 2;
            else if (A3) score += 3;
            else if (A4) score += 4;
            else if (A5) score += 5;
            else if (A6) score += 10;
            B = score;
         }
         else if (i > 120 || score >= 115) {
            SM1_State = SM1_Display;
            i = 0
         }
         break;
      case SM1_Display: 
         if (i <= 10) {
            SM1_State = SM1_Display;
            if (i % 4  == 0) B = score;
            else B = 0;
            if (i % 2 == 0) B = B | 0x80;
            else B = 0;
         }
         else if (i > 10) {
            SM1_State = SM1_WaitToken;
         }
         break;
      default:
         SM1_State = SM1_WaitToken;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_WaitToken:
         B = 0
         break;
      case SM1_Play:
         i++
         break;
      case SM1_Display:
         i++
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodState_machine_1 = 250;
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