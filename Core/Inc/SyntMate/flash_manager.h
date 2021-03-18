/*
 * flash_manager.h
 *
 *  Created on: Feb 5, 2021
 *      Author: fil
 */

#ifndef INC_SYNTMATE_FLASH_MANAGER_H_
#define INC_SYNTMATE_FLASH_MANAGER_H_

#include <SyntMate/SpiFlash.h>

#define	HEADER_SECTOR			0x000
#define	DIGITOFF_BASE			0x001
#define	LOGO_BASE				0x008
#define	DIGITRED_BASE			0x010
#define	DIGITGREEN_BASE			0x030
#define	DIGITYELLOW_BASE		0x050
#define	DIGITBLUE_BASE			0x070
#define	INCREASE_BASE			0x100
#define	INCREASE_DISABLED_BASE	0x102
#define	DECREASE_BASE			0x104
#define	DECREASE_DISABLED_BASE	0x106
#define	SETTINGS_BASE			0x108
#define	SETTINGS_DISABLED_BASE	0x10a
#define	HOME_BASE				0x10c
#define	HOME_DISABLED_BASE		0x10e

#define	PLUS_BASE				0x110
#define	MINUS_BASE				0x112
#define	BACK2NORMAL_BASE		0x114

#define	PLAY_BASE				0x120
#define	STOP_BASE				0x140
#define	BACKGROUND_BASE			0x200

#define	DIGITSECTORPERCOLOR		32


extern	uint32_t SetupFlash(void);
extern	void StoreSettingsInFlash(void);
extern	void LoadSettingsFromFlash(void);

#endif /* INC_SYNTMATE_FLASH_MANAGER_H_ */
