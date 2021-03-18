/*
 * DigitsBitmap.c
 *
 *  Created on: Feb 8, 2021
 *      Author: fil
 */

#include <SyntMate_Digits.h>
#include "main.h"

__attribute__ ((aligned (16)))	uint16_t BackGround[ILI9341_WIDTH*ILI9341_HEIGHT];

DIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t Green[10][DIGIT_SIZE];
DIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t Red[10][DIGIT_SIZE];
DIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t Yellow[10][DIGIT_SIZE];
DIGIT_BUFFERS 	__attribute__ ((aligned (16)))	uint16_t DigitOFF[DIGIT_SIZE];
DIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t play[BUTTONS_SIZE];
DIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t stop[BUTTONS_SIZE];
DIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t SyntmateLogo[LOGO_SIZE];
DIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t Increase[ICONS_SIZE];
DIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t Increase_disabled[ICONS_SIZE];
DIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t Decrease[ICONS_SIZE];
DIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t Decrease_disabled[ICONS_SIZE];
XDIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t Settings[ICONS_SIZE];
XDIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t Settings_disabled[ICONS_SIZE];
XDIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t Home[ICONS_SIZE];
XDIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t Home_disabled[ICONS_SIZE];
XDIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t Plus[ICONS_SIZE];
XDIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t Minus[ICONS_SIZE];
XDIGIT_BUFFERS	__attribute__ ((aligned (16)))	uint16_t Back[BACK_SIZE];

void GetDigitsFromFlash(void)
{
uint32_t i;

	for(i=0;i<10;i++)
	{
		flash_ReadBytes((uint8_t*)&Red[i][0], flash_SectorToAddress(i*2+DIGITRED_BASE), DIGIT_SIZE*2);
		flash_ReadBytes((uint8_t*)&Green[i][0], flash_SectorToAddress(i*2+DIGITGREEN_BASE), DIGIT_SIZE*2);
		flash_ReadBytes((uint8_t*)&Yellow[i][0], flash_SectorToAddress(i*2+DIGITYELLOW_BASE), DIGIT_SIZE*2);
	}
	flash_ReadBytes((uint8_t*)&DigitOFF[0], 			flash_SectorToAddress(DIGITOFF_BASE), DIGIT_SIZE*2);
	flash_ReadBytes((uint8_t*)&SyntmateLogo[0], 		flash_SectorToAddress(LOGO_BASE), LOGO_SIZE*2);
	flash_ReadBytes((uint8_t*)&Increase[0], 			flash_SectorToAddress(INCREASE_BASE), ICONS_SIZE*2);
	flash_ReadBytes((uint8_t*)&Increase_disabled[0], 	flash_SectorToAddress(INCREASE_DISABLED_BASE), ICONS_SIZE*2);
	flash_ReadBytes((uint8_t*)&Decrease[0], 			flash_SectorToAddress(DECREASE_BASE), ICONS_SIZE*2);
	flash_ReadBytes((uint8_t*)&Decrease_disabled[0], 	flash_SectorToAddress(DECREASE_DISABLED_BASE), ICONS_SIZE*2);
	flash_ReadBytes((uint8_t*)&Settings[0], 			flash_SectorToAddress(SETTINGS_BASE), ICONS_SIZE*2);
	flash_ReadBytes((uint8_t*)&Settings_disabled[0], 	flash_SectorToAddress(SETTINGS_DISABLED_BASE), ICONS_SIZE*2);
	flash_ReadBytes((uint8_t*)&Home[0], 				flash_SectorToAddress(HOME_BASE), ICONS_SIZE*2);
	flash_ReadBytes((uint8_t*)&Home_disabled[0], 		flash_SectorToAddress(HOME_DISABLED_BASE), ICONS_SIZE*2);
	flash_ReadBytes((uint8_t*)&Minus[0], 				flash_SectorToAddress(MINUS_BASE), ICONS_SIZE*2);
	flash_ReadBytes((uint8_t*)&Plus[0], 				flash_SectorToAddress(PLUS_BASE), ICONS_SIZE*2);
	flash_ReadBytes((uint8_t*)&play[0], 				flash_SectorToAddress(PLAY_BASE), BUTTONS_SIZE*2);
	flash_ReadBytes((uint8_t*)&stop[0], 				flash_SectorToAddress(STOP_BASE), BUTTONS_SIZE*2);
	flash_ReadBytes((uint8_t*)&BackGround[0], 			flash_SectorToAddress(BACKGROUND_BASE), ILI9341_WIDTH*ILI9341_HEIGHT*2);
	flash_ReadBytes((uint8_t*)&Back[0], 				flash_SectorToAddress(BACK2NORMAL_BASE), BACK_SIZE*2);
}

