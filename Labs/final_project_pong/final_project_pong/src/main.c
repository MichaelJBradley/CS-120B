#include "../../../atmel.h"
#include "../../../utils.h"

#include "ai_paddle.h"
#include "ball.h"
#include "game_manager.h"
#include "globals.h"
#include "intro.h"
#include "lcd_io.h"
#include "lcd_mux.h"
#include "output_mux.h"
#include "p1_paddle.h"
#include "p2_paddle.h"

#define TASKS 8
task tasks[TASKS];

const unsigned char periodGDC = 1;

int main (void)
{	
	unsigned char i = 0;
	
	TimerSet(periodGDC);
	TimerOn();

	board_init();

	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	DDRC = 0xFF;
	PORTC = 0x00;
	DDRD = 0xFF;
	PORTD = 0xFF;
	
	LCD_init();
	
	tasks[i++] = CreateTask(GM_Start, periodGDC, &GM_Tick);
	tasks[i++] = CreateTask(IN_Start, periodGDC, &IN_Tick);
	tasks[i++] = CreateTask(P1_Start, periodGDC, &P1_Tick);
	tasks[i++] = CreateTask(P2_Start, periodGDC, &P2_Tick);
	tasks[i++] = CreateTask(AI_Start, 75, &AI_Tick);
	tasks[i++] = CreateTask(BL_Start, periodGDC, &BL_Tick);
	tasks[i++] = CreateTask(OM_Start, periodGDC, &OM_Tick);
	tasks[i++] = CreateTask(LM_Start, 50, &LM_Tick);
	
	
	//Debug
	
	/*
	LCD_ClearScreen();
	delay_ms(1);
	LM_DisplayScore(5, 1);
	for (unsigned char i = 0; i < 6; i++) {
		LCD_WriteData(' ');
	}
	LM_DisplayScore(8, 2);
	while (1) {
		WaitTimer();
	}
	
	/**/
	
	while (1) {
		for (unsigned char i = 0; i < TASKS; i++) {
			if (tasks[i].elapsedTime >= tasks[i].period) {
				tasks[i].state = tasks[i].Tick(tasks[i].state);
				tasks[i].elapsedTime = 0;
			} 
			tasks[i].elapsedTime += periodGDC;
		}
		WaitTimer();
	}
	/**/
}
