/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 8/14/2018 15:58:24 PST
*/

#include "rims.h"

unsigned char i;
unsigned char H = 6;
unsigned char L = 34;
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_High, SM1_Low } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         i = 0;
         SM1_State = SM1_High;
         break;
         case SM1_High: 
         if (i <= H) {
            SM1_State = SM1_High;
         }
         else if (i < L) {
            SM1_State = SM1_Low;
            i = 0;
         }
         break;
      case SM1_Low: 
         if (i <= L) {
            SM1_State = SM1_Low;
         }
         else if (i > L) {
            SM1_State = SM1_High;
            i = 0;
         }
         break;
      default:
         SM1_State = SM1_High;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_High:
         B = 0x01;
         i++;
         break;
      case SM1_Low:
         B = 0;
         i++;
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodState_machine_1 = 25;
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