/*
 * ImageUploader.h
 *
 *  Created on: Feb 15, 2021
 *      Author: fil
 */

#ifndef INC_SYNTMATE_IMAGEUPLOADER_H_
#define INC_SYNTMATE_IMAGEUPLOADER_H_


#define	USB_WAIT_FOR_HEADER		0
#define	USB_RECEIVING_PACKET	1

extern	uint32_t ParseUSBPacket(void);
extern	uint32_t USB_ImageUploader(void);

#endif /* INC_SYNTMATE_IMAGEUPLOADER_H_ */
