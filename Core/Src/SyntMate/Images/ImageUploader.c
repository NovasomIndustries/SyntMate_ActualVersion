/*
 * ImageUploader.c
 *
 *  Created on: Feb 15, 2021
 *      Author: fil
 */

#include "main.h"

#define	BKG_BUF_SIZE_IN_BYTES	32768
uint8_t		usb_uploader_tx_buf[64];
//uint8_t		image_buffer[LOGO_SIZE*2]; // the biggest one
uint8_t		image_buffer[BKG_BUF_SIZE_IN_BYTES];
uint32_t	usb_rxed_byte_count , usb_byte_count, usb_rx_index , usb_bytes_to_receive,restart_flag , stored_image;
uint32_t	error=0;

extern	uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

void usbusb_error(void)
{
	error++;
}

void ReceiveUSBPacket(void)
{
uint32_t	i;

	for(i=0;i<SystemVar.usb_rxed_byte_count;i+=2)
	{
		if ( i > 4 )
		{
			restart_flag = ((SystemVar.usb_rxed_packet[i]<<24 ) | (SystemVar.usb_rxed_packet[i-1] << 16 ) | (SystemVar.usb_rxed_packet[i-2] << 8 ) | (SystemVar.usb_rxed_packet[i-3] << 0 ) );
			if ( restart_flag == 0xADBEEFDE)
			{
				SystemVar.usb_pkt_found = USB_WAIT_FOR_HEADER;
				SystemVar.usb_rx_index  = 0;
				return;
			}
		}

		image_buffer[usb_rx_index] =  ((SystemVar.usb_rxed_packet[i] & 0x0f) << 4)|(SystemVar.usb_rxed_packet[i+1] & 0x0f);
		usb_rx_index ++;
		if ( usb_rx_index > usb_bytes_to_receive-1)
		{
			usb_rx_index = 0;
			if ( strcmp ( (char *)SystemVar.usb_image_name , "Background") == 0 )
			{
				uint32_t	offset = SystemVar.usb_image_number * BKG_BUF_SIZE_IN_BYTES/2;
				if ( i >= BKG_BUF_SIZE_IN_BYTES)
					usbusb_error();
				if ( usb_rx_index+offset >= ILI9341_WIDTH*ILI9341_HEIGHT)
					usbusb_error();
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					BackGround[usb_rx_index+offset] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "settings") == 0 )
			{
				bzero(Settings,ICONS_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					Settings[usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
				SystemVar.usb_image_number = 0;
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "settings_disabled") == 0 )
			{
				bzero(Settings_disabled,ICONS_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					Settings_disabled[usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
				SystemVar.usb_image_number = 0;
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "home") == 0 )
			{
				bzero(Home,ICONS_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					Home[usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
				SystemVar.usb_image_number = 0;
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "home_disabled") == 0 )
			{
				bzero(Home_disabled,ICONS_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					Home_disabled[usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
				SystemVar.usb_image_number = 0;
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "increase") == 0 )
			{
				bzero(Increase,ICONS_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					Increase[usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
				SystemVar.usb_image_number = 0;
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "increase_disabled") == 0 )
			{
				bzero(Increase_disabled,ICONS_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					Increase_disabled[usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
				SystemVar.usb_image_number = 0;
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "decrease") == 0 )
			{
				bzero(Decrease,ICONS_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					Decrease[usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
				SystemVar.usb_image_number = 0;
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "decrease_disabled") == 0 )
			{
				bzero(Decrease_disabled,ICONS_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					Decrease_disabled[usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
				SystemVar.usb_image_number = 0;
			}

			if ( strcmp ( (char *)SystemVar.usb_image_name , "play") == 0 )
			{
				bzero(play,BUTTONS_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					play[usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
				SystemVar.usb_image_number = 0;
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "stop") == 0 )
			{
				bzero(stop,BUTTONS_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					stop[usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
				SystemVar.usb_image_number = 0;
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "SintMateLogo") == 0 )
			{
				bzero(SyntmateLogo,LOGO_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					SyntmateLogo[usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
				SystemVar.usb_image_number = 0;
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "DigitOff") == 0 )
			{
				bzero(DigitOFF,DIGITOFF_BASE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					DigitOFF[usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
				SystemVar.usb_image_number = 0;
			}

			if ( strcmp ( (char *)SystemVar.usb_image_name , "plus") == 0 )
			{
				bzero(Plus,ICONS_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					Plus[usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
				SystemVar.usb_image_number = 0;
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "minus") == 0 )
			{
				bzero(Minus,ICONS_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					Minus[usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
				SystemVar.usb_image_number = 0;
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "back") == 0 )
			{
				bzero(Back,BACK_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					Back[usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
				SystemVar.usb_image_number = 0;
			}


			/************************************************************************************/

			if ( strcmp ( (char *)SystemVar.usb_image_name , "green") == 0 )
			{
				bzero(Green[SystemVar.usb_image_number],DIGIT_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					Green[SystemVar.usb_image_number][usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "yellow") == 0 )
			{
				bzero(Yellow[SystemVar.usb_image_number],DIGIT_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					Yellow[SystemVar.usb_image_number][usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "red") == 0 )
			{
				bzero(Red[SystemVar.usb_image_number],DIGIT_SIZE);
				for(i=0;i<usb_bytes_to_receive;i+=2)
				{
					Red[SystemVar.usb_image_number][usb_rx_index] = (image_buffer[i]<<8) | image_buffer[i+1];
					usb_rx_index++;
				}
			}
			SystemVar.usb_pkt_found = USB_WAIT_FOR_HEADER;
			SystemVar.usb_rx_index  = 0;

			bzero(usb_uploader_tx_buf,64);
			sprintf((char *)usb_uploader_tx_buf,"Finished %d %d",(int )SystemVar.usb_image_number,(int )usb_bytes_to_receive);
			CDC_Transmit_FS(usb_uploader_tx_buf,strlen((char *)usb_uploader_tx_buf));

			return;
		}
	}
}

static void store_image(void)
{
uint32_t	i,j,base=0;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "green") == 0 )
		base = DIGITGREEN_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "red") == 0 )
		base = DIGITRED_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "yellow") == 0 )
		base = DIGITYELLOW_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "settings") == 0 )
		base = SETTINGS_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "settings_disabled") == 0 )
		base = SETTINGS_DISABLED_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "home") == 0 )
		base = HOME_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "home_disabled") == 0 )
		base = HOME_DISABLED_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "increase") == 0 )
		base = INCREASE_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "increase_disabled") == 0 )
		base = INCREASE_DISABLED_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "decrease") == 0 )
		base = DECREASE_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "decrease_disabled") == 0 )
		base = DECREASE_DISABLED_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "SintMateLogo") == 0 )
		base = LOGO_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "DigitOff") == 0 )
		base = DIGITOFF_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "play") == 0 )
		base = PLAY_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "stop") == 0 )
		base = STOP_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "minus") == 0 )
		base = MINUS_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "plus") == 0 )
		base = PLUS_BASE;
	if ( strcmp ( (char *)SystemVar.usb_image_name , "back") == 0 )
		base = BACK2NORMAL_BASE;

	if ( base == MINUS_BASE )
	{
		flash_EraseSector(base);
		flash_EraseSector(base+1);
	}
	if ( base == PLUS_BASE )
	{
		flash_EraseSector(base);
		flash_EraseSector(base+1);
	}
	if ( base == BACK2NORMAL_BASE )
	{
		for(j=0;j<10;j++)
		{
			flash_EraseSector(base+j);
		}
	}

	if (( base >= PLAY_BASE ) && ( base < STOP_BASE ))
	{
		for(j=0;j<16;j++)
		{
			flash_EraseSector(base+j);
		}
	}
	if (( base >= STOP_BASE ) && ( base < STOP_BASE+16 ))
	{
		for(j=0;j<16;j++)
		{
			flash_EraseSector(base+j);
		}
	}
	if (( base >= DIGITRED_BASE ) && ( base < INCREASE_BASE ))
	{
		for(j=0;j<32;j++)
		{
			flash_EraseSector(base+j);
		}
	}

	if (( base >= INCREASE_BASE ) && ( base <= HOME_DISABLED_BASE ))
	{
		flash_EraseSector(base);
		flash_EraseSector(base+1);
	}

	if ( base == DIGITOFF_BASE )
	{
		flash_EraseSector(base);
		flash_EraseSector(base+1);
	}

	if ( base == LOGO_BASE )
	{
		flash_EraseSector(base);
		flash_EraseSector(base+1);
		flash_EraseSector(base+2);
		flash_EraseSector(base+3);
		flash_EraseSector(base+4);
		flash_EraseSector(base+5);
		flash_EraseSector(base+6);
		flash_EraseSector(base+7);
	}

	if ( base == PLAY_BASE )
	{
		flash_WriteBytes((uint8_t *)play ,flash_SectorToAddress(PLAY_BASE),BUTTONS_SIZE*2);
	}
	if ( base == STOP_BASE )
	{
		flash_WriteBytes((uint8_t *)stop ,flash_SectorToAddress(STOP_BASE),BUTTONS_SIZE*2);
	}

	if ( base == LOGO_BASE )
	{
		flash_WriteBytes((uint8_t *)SyntmateLogo ,flash_SectorToAddress(LOGO_BASE),LOGO_SIZE*2);
	}

	if ( base == DIGITOFF_BASE )
	{
		flash_WriteBytes((uint8_t *)DigitOFF ,flash_SectorToAddress(DIGITOFF_BASE),DIGIT_SIZE*2);
	}

	if (( base >= DIGITRED_BASE ) && ( base < INCREASE_BASE ))
	{
		for(i=0;i<10;i++)
		{
			if ( strcmp ( (char *)SystemVar.usb_image_name , "green") == 0 )
			{
				flash_WriteBytes((uint8_t *)Green[i] ,flash_SectorToAddress(i*2+DIGITGREEN_BASE),DIGIT_SIZE*2);
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "red") == 0 )
			{
				flash_WriteBytes((uint8_t *)Red[i] ,flash_SectorToAddress(i*2+DIGITRED_BASE),DIGIT_SIZE*2);
			}
			if ( strcmp ( (char *)SystemVar.usb_image_name , "yellow") == 0 )
			{
				flash_WriteBytes((uint8_t *)Yellow[i] ,flash_SectorToAddress(i*2+DIGITYELLOW_BASE),DIGIT_SIZE*2);
			}
		}
	}

	if (( base >= INCREASE_BASE ) && ( base <= HOME_DISABLED_BASE ))
	{
		if ( strcmp ( (char *)SystemVar.usb_image_name , "settings") == 0 )
			flash_WriteBytes((uint8_t *)Settings 			,flash_SectorToAddress(SETTINGS_BASE),ICONS_SIZE*2);
		if ( strcmp ( (char *)SystemVar.usb_image_name , "settings_disabled") == 0 )
			flash_WriteBytes((uint8_t *)Settings_disabled	,flash_SectorToAddress(SETTINGS_DISABLED_BASE),ICONS_SIZE*2);
		if ( strcmp ( (char *)SystemVar.usb_image_name , "home") == 0 )
			flash_WriteBytes((uint8_t *)Home	 			,flash_SectorToAddress(HOME_BASE),ICONS_SIZE*2);
		if ( strcmp ( (char *)SystemVar.usb_image_name , "home_disabled") == 0 )
			flash_WriteBytes((uint8_t *)Home_disabled		,flash_SectorToAddress(HOME_DISABLED_BASE),ICONS_SIZE*2);
		if ( strcmp ( (char *)SystemVar.usb_image_name , "increase") == 0 )
			flash_WriteBytes((uint8_t *)Increase 			,flash_SectorToAddress(INCREASE_BASE),ICONS_SIZE*2);
		if ( strcmp ( (char *)SystemVar.usb_image_name , "increase_disabled") == 0 )
			flash_WriteBytes((uint8_t *)Increase_disabled	,flash_SectorToAddress(INCREASE_DISABLED_BASE),ICONS_SIZE*2);
		if ( strcmp ( (char *)SystemVar.usb_image_name , "decrease") == 0 )
			flash_WriteBytes((uint8_t *)Decrease 			,flash_SectorToAddress(DECREASE_BASE),ICONS_SIZE*2);
		if ( strcmp ( (char *)SystemVar.usb_image_name , "decrease_disabled") == 0 )
			flash_WriteBytes((uint8_t *)Decrease_disabled	,flash_SectorToAddress(DECREASE_DISABLED_BASE),ICONS_SIZE*2);
	}
	if ( base == MINUS_BASE )
		flash_WriteBytes((uint8_t *)Minus 				,flash_SectorToAddress(MINUS_BASE),ICONS_SIZE*2);
	if ( base == PLUS_BASE )
		flash_WriteBytes((uint8_t *)Plus	 			,flash_SectorToAddress(PLUS_BASE),ICONS_SIZE*2);
	if ( base == BACK2NORMAL_BASE )
		flash_WriteBytes((uint8_t *)Back 				,flash_SectorToAddress(BACK2NORMAL_BASE),BACK_SIZE*2);


	bzero(usb_uploader_tx_buf,64);
	sprintf((char *)usb_uploader_tx_buf,"STORED %s\n",SystemVar.usb_image_name);
	CDC_Transmit_FS(usb_uploader_tx_buf,strlen((char *)usb_uploader_tx_buf));
	SystemVar.usb_pkt_found = USB_WAIT_FOR_HEADER;
}

static void erase_flash(void)
{
uint32_t	secs = 0 , status = 0;
char		msg[32];

	ILI9341_FillScreen(ILI9341_BLACK);
	sprintf(msg,"Erasing ...");
	ILI9341_WriteString(20, 100, msg, Font_16x26, ILI9341_RED, ILI9341_BLACK);
	HAL_Delay(1000);
	flash_ChipErase();
	while( status == 0 )
	{
		sprintf(msg,"Erasing, %d elapsed",(int )secs);
		CDC_Transmit_FS((uint8_t *)msg,strlen(msg));
		HAL_Delay(1000);
		secs += 1;
		status = flash_PollChipErase();
	}
	HAL_Delay(100);
	sprintf(msg,"Flash Erased, %d elapsed\n",(int )secs);
	CDC_Transmit_FS((uint8_t *)msg,strlen(msg));
	sprintf(msg,"Erase done  ");
	ILI9341_WriteString(20, 100, msg, Font_16x26, ILI9341_BLUE, ILI9341_BLACK);
	HAL_Delay(1000);
}

#define CHUNK_SIZE      8192

uint32_t ParseUSBPacket(void)
{
uint32_t	pnum;
char		item[32], size_label[32], usb_image_name[32];

	bzero(SystemVar.usb_image_name,sizeof(SystemVar.usb_image_name));
	bzero(usb_image_name,32);

	pnum = sscanf((char *)SystemVar.usb_packet,"%s %s %s %d %d",
		item,usb_image_name,size_label,(int *)&SystemVar.usb_byte_count,(int *)&SystemVar.usb_image_number);

	sprintf(SystemVar.usb_image_name,"%s",usb_image_name);
	if ( pnum == 2 )
	{
		if ( strcmp(item,"STORE") == 0 )
			store_image();
		if (( strcmp(item,"ERASE") == 0 ) && ( strcmp(usb_image_name,"ALL") == 0 ))
			erase_flash();
		return 0;
	}

	if ( pnum == 4 )
	{
		bzero(usb_uploader_tx_buf,64);
		if ( strcmp(item,"BUTTON") == 0 )
		{
			sprintf((char *)usb_uploader_tx_buf,"BUTTON %s %d OK",SystemVar.usb_image_name,(int )SystemVar.usb_byte_count);
		}
		if ( strcmp(item,"LOGO") == 0 )
		{
			sprintf((char *)usb_uploader_tx_buf,"LOGO %s %d OK",SystemVar.usb_image_name,(int )SystemVar.usb_byte_count);
		}
		if ( strcmp(item,"DIGITOFF") == 0 )
		{
			sprintf((char *)usb_uploader_tx_buf,"DIGITOFF %s %d OK",SystemVar.usb_image_name,(int )SystemVar.usb_byte_count);
		}

		CDC_Transmit_FS(usb_uploader_tx_buf,strlen((char *)usb_uploader_tx_buf));
		SystemVar.usb_rx_index  = 0;
		SystemVar.usb_pkt_found = USB_RECEIVING_PACKET;
		usb_rxed_byte_count = usb_byte_count = usb_rx_index = 0;
		usb_bytes_to_receive = SystemVar.usb_byte_count/2;
		return 0;
	}

	if ( pnum == 5 )
	{
		if ( strcmp(item,"BACKGROUND") == 0 )
		{
			if ( SystemVar.usb_image_number == 0 )
				bzero(BackGround,LOGO_SIZE);
			sprintf((char *)usb_uploader_tx_buf,"BACKGROUND %s %d %d OK",SystemVar.usb_image_name,(int )SystemVar.usb_byte_count,SystemVar.usb_image_number);
			CDC_Transmit_FS(usb_uploader_tx_buf,strlen((char *)usb_uploader_tx_buf));
			SystemVar.usb_rx_index  = 0;
			SystemVar.usb_pkt_found = USB_RECEIVING_PACKET;
			usb_rxed_byte_count = usb_byte_count = usb_rx_index = 0;
			usb_bytes_to_receive = SystemVar.usb_byte_count/2;
			return 0;
		}

		if ( strcmp(item,"STORE") == 0 )
		{
			/*
			for(j=0;j<((ILI9341_WIDTH*ILI9341_HEIGHT*2)/SECTOR_SIZE)+1;j++)
				flash_EraseSector(BACKGROUND_BASE+j);
			flash_WriteBytes((uint8_t *)&BackGround ,flash_SectorToAddress(BACKGROUND_BASE),ILI9341_WIDTH*ILI9341_HEIGHT*2);
			*/
			ILI9341_DrawImage(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT-1, BackGround);

			sprintf((char *)usb_uploader_tx_buf,"STORED %s %d %d OK",SystemVar.usb_image_name,SystemVar.usb_image_number,(int )SystemVar.usb_byte_count);
			CDC_Transmit_FS(usb_uploader_tx_buf,strlen((char *)usb_uploader_tx_buf));
			SystemVar.usb_rx_index  = 0;
			SystemVar.usb_pkt_found = USB_WAIT_FOR_HEADER;
			usb_rxed_byte_count = usb_byte_count = usb_rx_index = 0;
			usb_bytes_to_receive = SystemVar.usb_byte_count/2;
			while(SystemVar.lcd_dma_busy == 1);
			return 0;
		}

		if ( SystemVar.usb_image_number > 9 )
		{
			bzero(usb_uploader_tx_buf,64);
			sprintf((char *)usb_uploader_tx_buf,"NOK image_num %d\n",SystemVar.usb_image_number);
			CDC_Transmit_FS(usb_uploader_tx_buf,strlen((char *)usb_uploader_tx_buf));
			SystemVar.usb_pkt_found = USB_WAIT_FOR_HEADER;
			return 1;
		}
		if (( strcmp ( (char *)SystemVar.usb_image_name , "green") != 0 ) &&( strcmp ( (char *)SystemVar.usb_image_name , "yellow") != 0 ) &&( strcmp ( (char *)SystemVar.usb_image_name , "red") != 0 ) )
		{
			bzero(usb_uploader_tx_buf,64);
			sprintf((char *)usb_uploader_tx_buf,"NOK image_color %s\n",SystemVar.usb_image_name);
			CDC_Transmit_FS(usb_uploader_tx_buf,strlen((char *)usb_uploader_tx_buf));
			SystemVar.usb_pkt_found = USB_WAIT_FOR_HEADER;
			return 1;
		}

		bzero(usb_uploader_tx_buf,64);
		sprintf((char *)usb_uploader_tx_buf,"DIGIT %s %d %d OK",(char *)SystemVar.usb_image_name, (int )SystemVar.usb_image_number,(int )SystemVar.usb_byte_count);
		CDC_Transmit_FS(usb_uploader_tx_buf,strlen((char *)usb_uploader_tx_buf));

		SystemVar.usb_rx_index  = 0;
		SystemVar.usb_pkt_found = USB_RECEIVING_PACKET;
		usb_rxed_byte_count = usb_byte_count = usb_rx_index = 0;
		usb_bytes_to_receive = SystemVar.usb_byte_count/2;
		return 0;
	}

	bzero(usb_uploader_tx_buf,64);
	sprintf((char *)usb_uploader_tx_buf,"NOK pnum %d\n",(int )pnum );
	CDC_Transmit_FS(usb_uploader_tx_buf,strlen((char *)usb_uploader_tx_buf));
	SystemVar.usb_pkt_found = USB_WAIT_FOR_HEADER;
	return 1;
}

uint32_t AnalyzeUSBHeader(void)
{
uint16_t	i;
	for(i=0;i<SystemVar.usb_rxed_byte_count;i++)
	{
		if ( SystemVar.usb_rxed_packet[i] == '<' )
		{
			bzero(SystemVar.usb_packet,USB_BUFLEN);
			SystemVar.usb_byte_count = SystemVar.usb_rx_index = 0;
			SystemVar.usb_do_cp=1;
		}
		else
		{
			if ( SystemVar.usb_do_cp == 1 )
			{
				if ( SystemVar.usb_rxed_packet[i] == '>' )
				{
					SystemVar.usb_do_cp=0;
					return 0;
				}
				else
				{
					SystemVar.usb_packet[SystemVar.usb_rx_index] = SystemVar.usb_rxed_packet[i];
					SystemVar.usb_byte_count ++;
					SystemVar.usb_rx_index++;
					if ( SystemVar.usb_rx_index >= USB_BUFLEN)
						SystemVar.usb_rx_index = 0;
				}
			}
		}
	}
	return 1;
}

void default_case(void)
{
	SystemVar.usb_pkt_found = USB_WAIT_FOR_HEADER;
	SystemVar.usb_rx_index  = 0;
}

uint32_t USB_ImageUploader(void)
{
	switch ( SystemVar.usb_pkt_found )
	{
	case	USB_WAIT_FOR_HEADER 	:   if ( AnalyzeUSBHeader() == 0 )
										{
											ParseUSBPacket();
										}
										break;
	case	USB_RECEIVING_PACKET 	: ReceiveUSBPacket(); break;
	default 						: default_case();break;
	}
	return SystemVar.usb_pkt_found;
}



