/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 8/14/2018 11:24:31 PST
*/

#include "rims.h"

/*Define user variables and functions for this state machine here.*/
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_FirstPress, SM1_FirstRelease, SM1_SecondPress, SM1_SecondRelease } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_SecondPress;
         break;
         case SM1_FirstPress: 
         if (A0) {
            SM1_State = SM1_FirstPress;
         }
         else if (!A0) {
            SM1_State = SM1_FirstRelease;
         }
         break;
      case SM1_FirstRelease: 
         if (A0) {
            SM1_State = SM1_SecondPress;
         }
         else if (!A0) {
            SM1_State = SM1_FirstRelease;
         }
         break;
      case SM1_SecondPress: 
         if (!A0) {
            SM1_State = SM1_SecondRelease;
         }
         else if (A0) {
            SM1_State = SM1_SecondPress;
         }
         break;
      case SM1_SecondRelease: 
         if (A0) {
            SM1_State = SM1_FirstPress;
         }
         else if (!A0) {
            SM1_State = SM1_SecondRelease;
         }
         break;
      default:
         SM1_State = SM1_SecondPress;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_FirstPress:
         B = 0x02;
         break;
      case SM1_FirstRelease:
         break;
      case SM1_SecondPress:
         B = 0x01;
         break;
      case SM1_SecondRelease:
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodState_machine_1 = 1000; // 1000 ms default
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