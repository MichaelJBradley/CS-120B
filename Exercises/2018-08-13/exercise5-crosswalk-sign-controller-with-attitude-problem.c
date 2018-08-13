/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 8/13/2018 14:52:41 PST
*/

#include "rims.h"

unsigned char time;
unsigned char pauseTime;
unsigned char tude;
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_WaitPush, SM1_PushButton, SM1_FlashMan, SM1_SolidMan, SM1_WaitMan, SM1_WaitExtra } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_WaitPush;
         break;
         case SM1_WaitPush: 
         if (!A0) {
            SM1_State = SM1_WaitPush;
         }
         else if (A0) {
            SM1_State = SM1_PushButton;
         }
         break;
      case SM1_PushButton: 
         if (A0) {
            SM1_State = SM1_PushButton;
         }
         else if (!A0) {
            SM1_State = SM1_WaitMan;
         }
         break;
      case SM1_FlashMan: 
         if (time <= 12) {
            SM1_State = SM1_FlashMan;
            if (time % 2 == 0) {
                B = 0x01;
            } else {
                B = 0x00;
            }
         }
         else if (time > 12) {
            SM1_State = SM1_WaitPush;
         }
         break;
      case SM1_SolidMan: 
         if (time > 20) {
            SM1_State = SM1_FlashMan;
            time = 0;
         }
         else if (time <= 20) {
            SM1_State = SM1_SolidMan;
         }
         break;
      case SM1_WaitMan: 
         if (time > 20) {
            SM1_State = SM1_SolidMan;
            time = 0;
            B = 0x01;
         }
         else if (time <= 20 && !A0) {
            SM1_State = SM1_WaitMan;
         }
         else if (time <= 2 && A0 && !tude) {
            SM1_State = SM1_WaitExtra;
            pauseTime = time;
            time = 0;
            tude = 1;
         }
         break;
      case SM1_WaitExtra: 
         if (time <= 30) {
            SM1_State = SM1_WaitExtra;
         }
         else if (time > 30) {
            SM1_State = SM1_WaitMan;
            time = pauseTime;
         }
         break;
      default:
         SM1_State = SM1_WaitPush;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_WaitPush:
         B = 0x00;
         time = 0;
         tude = 0;
         break;
      case SM1_PushButton:
         B = 0x02;
         break;
      case SM1_FlashMan:
         time++;
         break;
      case SM1_SolidMan:
         time++;
         break;
      case SM1_WaitMan:
         time++;
         break;
      case SM1_WaitExtra:
         time++;
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