/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 8/15/2018 15:18:6 PST
*/

#include "rims.h"

unsigned char i;
unsigned char s;
unsigned char H = 10;
unsigned char L = 10;
unsigned desire;
unsigned tacho;
unsigned char timeToSample = 750;
unsigned char sampleTime = 5;
unsigned char once;
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_High, SM1_Low, SM1_Sample } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         i = 0;
         s = 0;
         once = 0;
         SM1_State = SM1_High;
         break;
         case SM1_High: 
         if (i > H) {
            SM1_State = SM1_Low;
            i = 0;
         }
         else if (s > timeToSample) {
            SM1_State = SM1_Sample;
            s = 0;
         }
         else if (i <= H && s <= timeToSample) {
            SM1_State = SM1_High;
         }
         break;
      case SM1_Low: 
         if (i > L) {
            SM1_State = SM1_High;
            i = 0;
         }
         else if (s > timeToSample) {
            SM1_State = SM1_Sample;
            i = 0;
         }
         else if (i <= L && s <= timeToSample) {
            SM1_State = SM1_Low;
         }
         break;
      case SM1_Sample: 
         if (i <= sampleTime) {
            SM1_State = SM1_Sample;
         }
         else if (i > sampleTime) {
            SM1_State = SM1_High;
            once = 0;
            s = 0;
         }
         break;
      default:
         SM1_State = SM1_High;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_High:
         i++;
         s++;
         B = 0x01;
         break;
      case SM1_Low:
         i++;
         s++;
         B = 0;
         break;
      case SM1_Sample:
         tacho = (A & 0xF0) >> 4;
         desire = A & 0x0F;
         
         if (tacho > desire && !once && L > 0) {
             H++;
             L--;
             once = 0x01;
         } else if (tacho < desire && !once && H > 6) {
             H--;
             L++;
             once = 0x01;
         }
         
         i++;
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodState_machine_1 = 50;
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