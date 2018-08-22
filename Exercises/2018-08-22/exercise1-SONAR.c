/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 8/22/2018 14:35:5 PST
*/

#include "rims.h"

/*This code will be shared between state machines.*/
unsigned char TimerFlag = 0;
void TimerISR() {
   TimerFlag = 1;
}


enum SM1_States { SM1_Wait, SM1_PulseHigh, SM1_PulseLow } SM1_State;

TickFct_State_machine_1() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
static unsigned char t;
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_Wait;
         break;
      case SM1_Wait:
         if (A0) {
            SM1_State = SM1_PulseHigh;
         }
         else if (!A0) {
            SM1_State = SM1_Wait;
         }
         break;
      case SM1_PulseHigh:
         if (!A0) {
            SM1_State = SM1_Wait;
         }
         else if (A0 && t >= 5) {
            SM1_State = SM1_PulseLow;
            t = 0;
         }
         break;
      case SM1_PulseLow:
         if (A0 && t >= 5) {
            SM1_State = SM1_PulseHigh;
            t = 0;
         }
         else if (!A0) {
            SM1_State = SM1_Wait;
         }
         break;
      default:
         SM1_State = SM1_Wait;
      } // Transitions

   switch(SM1_State) { // State actions
      case SM1_Wait:
         B = 0;
         t = 0;
         break;
      case SM1_PulseHigh:
         B0 = 0x01;
         t++;
         break;
      case SM1_PulseLow:
         B0 = 0;
         t++;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SM2_States { SM2_Listen, SM2_EchoLocated } SM2_State;

TickFct_State_machine_2() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
static unsigned char echoes;
   switch(SM2_State) { // Transitions
      case -1:
         echoes = 0;
         SM2_State = SM2_Listen;
         break;
      case SM2_Listen:
         if (!A1) {
            SM2_State = SM2_Listen;
         }
         else if (A1) {
            SM2_State = SM2_EchoLocated;
         }
         break;
      case SM2_EchoLocated:
         if (A1) {
            SM2_State = SM2_EchoLocated;
         }
         else if (!A1) {
            SM2_State = SM2_Listen;
         }
         break;
      default:
         SM2_State = SM2_Listen;
      } // Transitions

   switch(SM2_State) { // State actions
      case SM2_Listen:
         B1 = 0;
         break;
      case SM2_EchoLocated:
         B1 = 0x01;
         echoes++;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}
int main() {
   B = 0; //Init outputs
   TimerSet(200);
   TimerOn();
   SM1_State = -1;
   SM2_State = -1;
   while(1) {
      TickFct_State_machine_1();
      TickFct_State_machine_2();
      while (!TimerFlag);
      TimerFlag = 0;
   }
}