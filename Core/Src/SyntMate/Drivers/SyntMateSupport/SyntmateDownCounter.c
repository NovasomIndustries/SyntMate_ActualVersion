/*
 * SintmateDownCounter.c
 *
 *  Created on: Feb 9, 2021
 *      Author: fil
 */

#include "main.h"

DIGIT_BUFFERS		__attribute__ ((aligned (4)))	uint16_t	DigitsCounter[DIGIT_WIDTH*DIGIT_HEIGHT*3+DOWNCOUNTER_DIGITSPACE*2];

void Tim100MSec_callback(void)
{
	if ( SystemVar.run_state == RUN_STATE_RUNNING)
	{
		SystemVar.counter++;
		if ( SystemVar.counter >= 10 )
		{
			SystemVar.counter = 0;
			SystemVar.counter_flag = 1;
		}
	}
	else
	{
		SystemVar.counter = 0;
		SystemVar.counter_flag = 0;
	}

	if ( SystemVar.touch_pen_down != 0 )
		if ( SystemVar.touch_disable_window > 0 )
			SystemVar.touch_disable_window --;
	if ( SystemVar.touch_disable_window == 0 )
		SystemVar.touch_pen_down = 0;
}

static void compileDigitsCounter(uint32_t color)
{
uint32_t	i,j,xcount;
uint32_t 	h,t,u;  // hundreds,tens,units

	u = SystemVar.DownCounter%10;
	t = (SystemVar.DownCounter/10)%10;
	h = (SystemVar.DownCounter/100)%10;
	xcount = 0;
	for(j=0;j<DIGIT_HEIGHT;j++)
	{
		for(i=0;i<DIGIT_WIDTH;i++,xcount++)
		{
			if ( h == 0 )
				DigitsCounter[xcount] = DigitOFF[j*DIGIT_WIDTH+i];
			else
				DigitsCounter[xcount] = Red[h][j*DIGIT_WIDTH+i];
		}
		for(i=0;i<DOWNCOUNTER_DIGITSPACE;i++,xcount++)
			DigitsCounter[xcount] = 0;
		for(i=0;i<DIGIT_WIDTH;i++,xcount++)
		{
			if (( h == 0 ) && ( t == 0 ))
				DigitsCounter[xcount] = DigitOFF[j*DIGIT_WIDTH+i];
			else
				DigitsCounter[xcount] = Red[t][j*DIGIT_WIDTH+i];
		}
		for(i=0;i<DOWNCOUNTER_DIGITSPACE;i++,xcount++)
			DigitsCounter[xcount] = 0;
		for(i=0;i<DIGIT_WIDTH;i++,xcount++)
			DigitsCounter[xcount] = Red[u][j*DIGIT_WIDTH+i];
	}
}

void SetCounter(uint32_t value, uint32_t color)
{
	SystemVar.DownCounter = value;
	compileDigitsCounter(color);
	ILI9341_DrawImage(DOWNCOUNTER_100_POSX, DOWNCOUNTER_POSY, DIGIT_WIDTH*3+DOWNCOUNTER_DIGITSPACE*2, DIGIT_HEIGHT-1, DigitsCounter);
}

void InitCounter(void)
{
	SystemVar.suppress_trailing_zero = 1;
	SystemVar.DownCounter = SystemVar.Session_DownCounter;
	compileDigitsCounter(ILI9341_RED);
	SetCounter(SystemVar.DownCounter,ILI9341_RED);
	HAL_Delay(10);
}

void DecrementCounter(void)
{
	SystemVar.DownCounter--;
	if (SystemVar.DownCounter < 31 )
	{
		SystemVar.worm_r = WORM_R_FINISHING;
		SystemVar.worm_g = WORM_G_FINISHING;
		SystemVar.worm_b = WORM_B_FINISHING;
	}
	else
	{
		SystemVar.worm_r = WORM_R_RUNNING;
		SystemVar.worm_g = WORM_G_RUNNING;
		SystemVar.worm_b = WORM_B_RUNNING;
	}

	if (SystemVar.DownCounter == 0 )
	{
		SyntMate_stop_rotations();
		SystemVar.DownCounter = SystemVar.Session_DownCounter;
		SystemVar.run_state = RUN_STATE_IDLE;
		DrawIdleButtons();
		WS2812_LedsOff();
		SystemVar.worm_r = WORM_R_RUNNING;
		SystemVar.worm_g = WORM_G_RUNNING;
		SystemVar.worm_b = WORM_B_RUNNING;
	}
	SetCounter(SystemVar.DownCounter,ILI9341_RED);
}

