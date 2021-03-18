/*
 * SyntMate.h
 *
 *  Created on: Feb 4, 2021
 *      Author: fil
 */

#ifndef INC_SINTMATE_SYSTEM_H_
#define INC_SINTMATE_SYSTEM_H_

#include <stdio.h>
#include <string.h>

#include <DigitsBitmap.h>
#include <flash_manager.h>
#include <fonts.h>
#include <ili9341.h>
#include <ili9341_calibrate_touch.h>
#include <ili9341_touch.h>
#include <ImageUploader.h>
#include <SpiFlash.h>
#include <SyntmateDownCounter.h>
#include <SyntMateButtons.h>
#include <SyntMateDebug.h>
#include <SyntMateSettings.h>
#include <SyntMateTouchProcess.h>
#include <ws2812b.h>
#include <SyntMate_Digits.h>
#include "StepperMotor.h"
#include "VL53L0X.h"
#include "VL53L0X_utility.h"

#define	SintMateNAME		"SyntMate"
#define	SintMateVERSION		"0.0.1"
#define	LOGO_Y_POS			10

#define 	DIGIT_BUFFERS 		__attribute__((section(".digit_buffers" ".noinit")))
#define 	XDIGIT_BUFFERS 		__attribute__((section(".xdigit_buffers" ".noinit")))

extern	TIM_HandleTypeDef htim8;
extern	TIM_HandleTypeDef htim16;
extern	TIM_HandleTypeDef htim5;
extern	TIM_HandleTypeDef htim7;
extern	SPI_HandleTypeDef hspi1;
extern	SPI_HandleTypeDef hspi6;
extern	I2C_HandleTypeDef hi2c2;

#define	LED_TIMER				htim8
#define	LED_TIMER_CHANNEL		TIM_CHANNEL_2
#define	BACKLIGHT_TIMER			htim5
#define	BACKLIGHT_TIMER_CHANNEL	TIM_CHANNEL_1
#define	STEPPER_TIMER			htim16
#define	STEPPER_TIMER_CHANNEL	TIM_CHANNEL_1
#define	TICK100MS_TIMER			htim7

#define	FlashSPIport	hspi6
#define	LcdSPIport		hspi1
#define	TOF_I2C			hi2c2
#define	TOF_ADDRESS		0x52

extern SPI_HandleTypeDef ILI9341_SPI_PORT;


typedef struct _SystemParametersTypeDef
{
	char					Header[16];
	char					Version[16];
	uint32_t 				step_rpm;
	uint32_t 				max_running_time;
	uint32_t 				flash_capacity;
	uint32_t 				touch_is_calibrated;	// 0 not calibrated, !=0 calibrated
	uint32_t 				touch_x_minimum;
	uint32_t 				touch_y_minimum;
	uint32_t 				touch_x_correction;
	uint32_t 				touch_y_correction;
}SystemParametersTypeDef;

#define		RUN_STATE_IDLE		0
#define		RUN_STATE_RUNNING	1
#define		RUN_STATE_SETTINGS	2

#define		TOUCH_DISABLE_WINDOW	5	// 500 mSec delay after touch released the touch is considered not active

#define	USB_BUFLEN			2048

typedef struct _SystemVarDef
{
	uint32_t 				run_state;
	uint32_t 				touch_flag;
	uint32_t 				touch_x, touch_y;
	uint32_t 				touch_pen_down;
	uint32_t 				touch_disable_window;
	uint32_t 				lcd_dma_busy;
	uint32_t 				counter;
	uint32_t 				counter_flag;
	uint32_t 				Session_DownCounter;
	uint32_t 				DownCounter;
	uint8_t 				suppress_trailing_zero;
	uint32_t 				usb_rxed_byte_count;
	uint32_t 				usb_byte_count;
	uint8_t 				usb_do_cp;
	uint32_t 				usb_pkt_found;
	uint16_t 				usb_rx_index;
	uint8_t 				usb_packet_ready;
	uint8_t 				usb_rxed_packet[USB_BUFLEN];
	uint8_t 				usb_packet[USB_BUFLEN];
	uint16_t 				usb_image_number;
	char 					usb_image_name[32];
}SystemVarDef;

extern	SystemParametersTypeDef	SystemParameters;
extern	SystemVarDef			SystemVar;

extern	void Init_SintMate(void);
extern	void SintMateLoop(void);
extern	void SintMate_SystemSetDefaults(void);


#define	DIGIT_GREEN			1
#define	DIGIT_RED			1
#define	DIGIT_YELLOW		1

#endif /* INC_SINTMATE_SYSTEM_H_ */
