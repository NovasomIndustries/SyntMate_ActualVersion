/*
 * SintMate.c
 *
 *  Created on: Feb 4, 2021
 *      Author: fil
 */
/*
 Clocks :
 pll1 			:  2  80 2 2 2
 pll2 			: 12 200 2 2 2
 pll3 			: 12  94 4 4 4
 clk spi 1,2,3 	: pll2p 200
 clk spi6      	: pclk4 120
 processor	   	: 480 240 120
 usb			: pll3q 48
 */
#include "main.h"

SystemParametersTypeDef	SystemParameters;
SystemVarDef			SystemVar;

void SintMate_SystemSetDefaults(void)
{
	bzero((uint8_t *)&SystemParameters,sizeof(SystemParameters));
	sprintf(SystemParameters.Header,SintMateNAME);
	sprintf(SystemParameters.Version,SintMateVERSION);
	SystemParameters.step_rpm = STEP_SPEED_RPM;
	SystemParameters.max_running_time = DOWNCOUNTER_TYP;
}

void SintMate_PinSetDefaults(void)
{
	  HAL_GPIO_WritePin(FLASH_SS_GPIO_Port, FLASH_SS_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOD, DIRSTEP_Pin|RST_Pin|DC_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOD, ENASTEP_Pin|CS_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(DEBUG_GPIO_Port, DEBUG_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(VL53L0X_RST_GPIO_Port, VL53L0X_RST_Pin, GPIO_PIN_RESET);
}

extern	uint16_t Logo[];
void SyntMateLedDemo(void)
{
uint32_t	i;
uint8_t		r , g , b;

	WS2812_WormReset(5);

	r = 0;
	g = 0x1f;
	b = 0;

	for(i=0;i<NUMLEDS;i++)
	{
		ringled_frame_complete=0;
		WS2812_Worm(r,g,b);
		HAL_Delay(1);
		while(ringled_frame_complete == 0);
	}

	r = 0x1f;
	g = 0;
	b = 0;

	for(i=0;i<NUMLEDS;i++)
	{
		ringled_frame_complete=0;
		WS2812_Worm(r,g,b);
		HAL_Delay(1);
		while(ringled_frame_complete == 0);
	}

	r = 0;
	g = 0;
	b = 0x1f;

	for(i=0;i<NUMLEDS;i++)
	{
		ringled_frame_complete=0;
		WS2812_Worm(r,g,b);
		HAL_Delay(1);
		while(ringled_frame_complete == 0);
	}
	ringled_frame_complete=0;
	WS2812_LedsOff();
	HAL_Delay(1);
	while(ringled_frame_complete == 0);
	ringled_frame_complete=0;
	WS2812_LedsOff();
	HAL_Delay(1);
	while(ringled_frame_complete == 0);
	WS2812_WormReset(5);
}

void Init_SintMate(void)
{
	SintMate_PinSetDefaults();
	SyntMateLedDemo();
	ILI9341_Init();
	ILI9341_FillScreen(ILI9341_BLACK);
	HAL_TIM_PWM_Start(&BACKLIGHT_TIMER, BACKLIGHT_TIMER_CHANNEL);
	SetupFlash();
	//SystemParameters.touch_is_calibrated = 0;
	if ( SystemParameters.touch_is_calibrated == 0 )
	{
		HAL_TIM_Base_Start_IT(&TICK100MS_TIMER);
		ILI9341_calibrate_touch();
		StoreSettingsInFlash();
		HAL_TIM_Base_Stop_IT(&TICK100MS_TIMER);
		HAL_Delay(200);
	}

	SystemVar.Session_DownCounter = SystemParameters.max_running_time;
	GetDigitsFromFlash();
	ILI9341_DrawImage(0, 0, LOGO_WIDTH, LOGO_HEIGHT-1, SyntmateLogo);
	while(SystemVar.lcd_dma_busy == 1);
	InitCounter();
	DrawIdleButtons();
	SystemVar.run_state = RUN_STATE_IDLE;
	StepperInit();
	SyntMate_VL53L0X_Init();

	WS2812_LedsOff();
	HAL_TIM_Base_Start_IT(&TICK100MS_TIMER);

}
static	uint8_t	worm_initialized = 0;

void SintMateLoop(void)
{
uint8_t		r , g , b;
uint32_t	was_running;
	if (SystemVar.run_state == RUN_STATE_IDLE)
	{
		if ( worm_initialized == 0 )
		{
			WS2812_WormReset(5);
			g = 0x3f;
			r=0;
			b=0;
			ringled_frame_complete=0;
			WS2812_Worm(r,g,b);
			while(ringled_frame_complete == 0);
			ringled_frame_complete=0;
			worm_initialized = 1;
			WS2812_WormReset(5);
		}
	}

	if (( SystemVar.counter_flag == 1 ) && (SystemVar.run_state == RUN_STATE_RUNNING))
	{
		SystemVar.counter_flag = 0;
		DecrementCounter();
		if (ringled_frame_complete == 1)
		{
			ringled_frame_complete=0;
			WS2812_Worm(r,g,b);
		}
	}
	if ( SystemVar.touch_flag == 1 )
	{
		SystemVar.touch_flag = 0;
		was_running = SystemVar.run_state;
		if ( ILI9341_GetTouch(&SystemVar.touch_x,&SystemVar.touch_y) != 0 )
		{
			SintMateTouchProcess();
			/*
			if ( SintMateTouchProcess() == 0 )
			{
				if ( was_running == RUN_STATE_RUNNING )
					WS2812_WormReset(5);
			}
			*/
		}
	}
	if ( SystemVar.usb_packet_ready == 1 )
	{
		USB_ImageUploader();
		SystemVar.usb_packet_ready = 0;
	}
	if (SystemVar.run_state == RUN_STATE_SETTINGS)
		SettingsLoop();

}

