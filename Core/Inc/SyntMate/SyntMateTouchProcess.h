/*
 * SintMateTouchProcess.h
 *
 *  Created on: Feb 10, 2021
 *      Author: fil
 */

#ifndef INC_SYNTMATE_SYNTMATETOUCHPROCESS_H_
#define INC_SYNTMATE_SYNTMATETOUCHPROCESS_H_

#define TOUCH_X0AREA_PLAYPAUSE		PLAYPAUSE_BUTTON_POSX
#define TOUCH_X1AREA_PLAYPAUSE		PLAYPAUSE_BUTTON_POSX+BUTTONS_WIDTH
#define TOUCH_Y0AREA_PLAYPAUSE		ILI9341_HEIGHT-(PLAYPAUSE_BUTTON_POSY+BUTTONS_HEIGHT)
#define TOUCH_Y1AREA_PLAYPAUSE		ILI9341_HEIGHT-PLAYPAUSE_BUTTON_POSY

#define TOUCH_X0AREA_INCREASE		INCREASE_BUTTON_POSX
#define TOUCH_X1AREA_INCREASE		INCREASE_BUTTON_POSX+BUTTONS_WIDTH
#define TOUCH_Y0AREA_INCREASE		ILI9341_HEIGHT-(INCREASE_BUTTON_POSY+BUTTONS_HEIGHT)
#define TOUCH_Y1AREA_INCREASE		ILI9341_HEIGHT-INCREASE_BUTTON_POSY

#define TOUCH_X0AREA_DECREASE		DECREASE_BUTTON_POSX
#define TOUCH_X1AREA_DECREASE		DECREASE_BUTTON_POSX+BUTTONS_WIDTH
#define TOUCH_Y0AREA_DECREASE		ILI9341_HEIGHT-(DECREASE_BUTTON_POSY+BUTTONS_HEIGHT)
#define TOUCH_Y1AREA_DECREASE		ILI9341_HEIGHT-DECREASE_BUTTON_POSY

#define TOUCH_X0AREA_SETTINGS		SETTINGS_BUTTON_POSX
#define TOUCH_X1AREA_SETTINGS		SETTINGS_BUTTON_POSX+ICONS_WIDTH
#define TOUCH_Y0AREA_SETTINGS		ILI9341_HEIGHT-(SETTINGS_BUTTON_POSY+ICONS_HEIGHT)
#define TOUCH_Y1AREA_SETTINGS		ILI9341_HEIGHT-SETTINGS_BUTTON_POSY

#define TOUCH_X0AREA_HOME			HOME_BUTTON_POSX
#define TOUCH_X1AREA_HOME			HOME_BUTTON_POSX+ICONS_WIDTH
#define TOUCH_Y0AREA_HOME			ILI9341_HEIGHT-(HOME_BUTTON_POSY+ICONS_HEIGHT)
#define TOUCH_Y1AREA_HOME			ILI9341_HEIGHT-HOME_BUTTON_POSY

#define TOUCH_X0AREA_BACK			BACK_BUTTON_POSX
#define TOUCH_X1AREA_BACK			BACK_BUTTON_POSX+BACK_WIDTH
#define TOUCH_Y0AREA_BACK			ILI9341_HEIGHT-(BACK_BUTTON_POSY+BACK_HEIGHT)
#define TOUCH_Y1AREA_BACK			ILI9341_HEIGHT-BACK_BUTTON_POSY

#define TOUCH_X0AREA_RPMPLUS		ILI9341_WIDTH-ICONS_WIDTH
#define TOUCH_X1AREA_RPMPLUS		ILI9341_WIDTH
#define TOUCH_Y0AREA_RPMPLUS		ILI9341_HEIGHT-(64+ICONS_HEIGHT)
#define TOUCH_Y1AREA_RPMPLUS		ILI9341_HEIGHT-64

#define TOUCH_X0AREA_RPMMINUS		0
#define TOUCH_X1AREA_RPMMINUS		ICONS_WIDTH
#define TOUCH_Y0AREA_RPMMINUS		ILI9341_HEIGHT-(64+ICONS_HEIGHT)
#define TOUCH_Y1AREA_RPMMINUS		ILI9341_HEIGHT-64

#define TOUCH_X0AREA_SECPLUS		ILI9341_WIDTH-ICONS_WIDTH
#define TOUCH_X1AREA_SECPLUS		ILI9341_WIDTH
#define TOUCH_Y0AREA_SECPLUS		ILI9341_HEIGHT-(128+ICONS_HEIGHT)
#define TOUCH_Y1AREA_SECPLUS		ILI9341_HEIGHT-128

#define TOUCH_X0AREA_SECMINUS		0
#define TOUCH_X1AREA_SECMINUS		ICONS_WIDTH
#define TOUCH_Y0AREA_SECMINUS		ILI9341_HEIGHT-(128+ICONS_HEIGHT)
#define TOUCH_Y1AREA_SECMINUS		ILI9341_HEIGHT-128

#define TOUCH_X0AREA_CALIBRATE		0
#define TOUCH_X1AREA_CALIBRATE		ILI9341_WIDTH
#define TOUCH_Y0AREA_CALIBRATE		90
#define TOUCH_Y1AREA_CALIBRATE		120


#define	TOUCH_CALLBACK_NOTOUCH		0
#define	TOUCH_CALLBACK_RPMPLUS		1
#define	TOUCH_CALLBACK_RPMMINUS		2
#define	TOUCH_CALLBACK_SECPLUS		3
#define	TOUCH_CALLBACK_SECMINUS		4
#define	TOUCH_CALLBACK_CALIBRATE	5
#define	TOUCH_CALLBACK_BACK			6

extern	uint32_t SintMateTouchProcess(void);

#endif /* INC_SYNTMATE_SYNTMATETOUCHPROCESS_H_ */
