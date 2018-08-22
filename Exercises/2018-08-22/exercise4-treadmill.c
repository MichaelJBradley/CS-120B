/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 8/22/2018 16:46:49 PST
*/

#include "rims.h"

/*This code will be shared between state machines.*/
unsigned char speed;
unsigned char time;
unsigned char low;
unsigned char motorOut;
unsigned char dispOut;
unsigned char TimerFlag = 0;
void TimerISR() {
   TimerFlag = 1;
}


enum SI_States { SI_Input, SI_ButtonDown, SI_Adjust } SI_State;

TickFct_SpeedInput() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
unsigned char t;
   switch(SI_State) { // Transitions
      case -1:
         speed = 0;
         low = 100;
         SI_State = SI_Input;
         break;
      case SI_Input:
         if (A0 ^ A1) {
            SI_State = SI_ButtonDown;
            t = 0;
         }
         break;
      case SI_ButtonDown:
         if (!(A0 ^ A1)) {
            SI_State = SI_Input;
         }
         else if ((t >= 4) && (A0 ^ A1)) {
            SI_State = SI_Adjust;
         }
         break;
      case SI_Adjust:
         if (!(A0 ^ A1)) {
            SI_State = SI_Input;
         }
         else if ((A0 ^ A1)) {
            SI_State = SI_Adjust;
         }
         break;
      default:
         SI_State = SI_Input;
      } // Transitions

   switch(SI_State) { // State actions
      case SI_Input:
         break;
      case SI_ButtonDown:
         t++;
         break;
      case SI_Adjust:
         if (A0 && speed < 100) {
             speed++;
             low--;
         } else if (A1 && speed > 0) {
             speed--;
             low++;
         }
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SW_States { SW_Wait, SW_Count } SW_State;

TickFct_Stopwatch() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
static unsigned char j;
   switch(SW_State) { // Transitions
      case -1:
         SW_State = SW_Wait;
         break;
      case SW_Wait:
         if (speed > 0) {
            SW_State = SW_Count;
            time = 0;
j = 0;
         }
         break;
      case SW_Count:
         if (speed > 0) {
            SW_State = SW_Count;
         }
         else if (!speed) {
            SW_State = SW_Wait;
         }
         break;
      default:
         SW_State = SW_Wait;
      } // Transitions

   switch(SW_State) { // State actions
      case SW_Wait:
         break;
      case SW_Count:
         if (j >= 20) {
             j = 0;
             time++;
         }
         
         j++;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum DS_States { DS_MUX } DS_State;

TickFct_Display() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(DS_State) { // Transitions
      case -1:
         DS_State = DS_MUX;
         break;
      case DS_MUX:
         if (1) {
            DS_State = DS_MUX;
         }
         break;
      default:
         DS_State = DS_MUX;
      } // Transitions

   switch(DS_State) { // State actions
      case DS_MUX:
         dispOut = (A7 ? time : speed) & 0x7F;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum MR_States { MR_High, MR_Low } MR_State;

TickFct_Motor() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
static unsigned char t;
   switch(MR_State) { // Transitions
      case -1:
         t = 0;
         MR_State = MR_Low;
         break;
      case MR_High:
         if (t >= speed) {
            MR_State = MR_Low;
            t = 0;
         }
         else if (t < speed) {
            MR_State = MR_High;
         }
         break;
      case MR_Low:
         if (t < low) {
            MR_State = MR_Low;
         }
         else if (t >= low) {
            MR_State = MR_High;
            t = 0;
         }
         break;
      default:
         MR_State = MR_Low;
      } // Transitions

   switch(MR_State) { // State actions
      case MR_High:
         t++;
         
         break;
      case MR_Low:
         t++;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum BO_States { BO_MUX } BO_State;

TickFct_BOut() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(BO_State) { // Transitions
      case -1:
         BO_State = BO_MUX;
         break;
      case BO_MUX:
         if (1) {
            BO_State = BO_MUX;
            B = dispOut | (motorOut << 7);
         }
         break;
      default:
         BO_State = BO_MUX;
      } // Transitions

   switch(BO_State) { // State actions
      case BO_MUX:
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}
int main() {
   B = 0; //Init outputs
   TimerSet(50);
   TimerOn();
   SI_State = -1;
   SW_State = -1;
   DS_State = -1;
   MR_State = -1;
   BO_State = -1;
   while(1) {
      TickFct_SpeedInput();
      TickFct_Stopwatch();
      TickFct_Display();
      TickFct_Motor();
      TickFct_BOut();
      while (!TimerFlag);
      TimerFlag = 0;
   }
}