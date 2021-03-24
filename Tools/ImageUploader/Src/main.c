#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include "ImageUploader.h"

char            tx_buf[BUFSIZE];
char            rx_buf[BUFSIZE];

extern  unsigned short  image[];
extern  unsigned char   ascii_image[];

int send_images(int serial_port,char   *folder)
{
int     size_in_bytes,j,pnum,rxed,ret_val;
char    rx_string[32],button_name[32];
char    file_in[128];

    for(j=0;j<25;j++)
    {
        sleep(1);
        switch(j)
        {
        case   10   :   sprintf(button_name,"settings");break;
        case   11   :   sprintf(button_name,"settings_disabled");break;
        case   12   :   sprintf(button_name,"home");break;
        case   13   :   sprintf(button_name,"home_disabled");break;
        case   14   :   sprintf(button_name,"increase");break;
        case   15   :   sprintf(button_name,"increase_disabled");break;
        case   16   :   sprintf(button_name,"decrease");break;
        case   17   :   sprintf(button_name,"decrease_disabled");break;
        case   18   :   sprintf(button_name,"plus");break;
        case   19   :   sprintf(button_name,"minus");break;
        case   20   :   sprintf(button_name,"play");break;
        case   21   :   sprintf(button_name,"stop");break;
        case   22   :   sprintf(button_name,"back");break;
        case   23   :   sprintf(button_name,"DigitOff");break;
        case   24   :   sprintf(button_name,"SintMateLogo");break;
        case   0    :   sprintf(button_name,"digit0");break;
        case   1    :   sprintf(button_name,"digit1");break;
        case   2    :   sprintf(button_name,"digit2");break;
        case   3    :   sprintf(button_name,"digit3");break;
        case   4    :   sprintf(button_name,"digit4");break;
        case   5    :   sprintf(button_name,"digit5");break;
        case   6    :   sprintf(button_name,"digit6");break;
        case   7    :   sprintf(button_name,"digit7");break;
        case   8    :   sprintf(button_name,"digit8");break;
        case   9    :   sprintf(button_name,"digit9");break;
        }

        sprintf(file_in,"%s/%s.bmp",folder,button_name);
        size_in_bytes = read_and_convert(file_in);
        bzero(tx_buf,BUFSIZE);
        sprintf(tx_buf,"<IMAGE %s SIZE %d>",button_name,size_in_bytes);
        printf("TX : %s\n", tx_buf);
        if ( serial_tx_rx_command(serial_port,tx_buf,rx_buf) != -1 )
        {
            printf("RX : %s\n", rx_buf);
            pnum = sscanf(rx_buf,"IMAGE %s %d OK",rx_string,&rxed);
            if ( pnum == 2)
            {
                ret_val = serial_tx_rx(serial_port,(char *)ascii_image,size_in_bytes,rx_buf,button_name);
                printf("RX : %s\n", rx_buf);
                if ( ret_val == -1 )
                {
                    printf("RXERROR : %s\n", rx_buf);
                    return -1;
                }
                else
                {
                    sprintf(tx_buf,"<STORE %s>",button_name);
                    printf("TX : %s\n", tx_buf);
                    if ( serial_tx_rx_command(serial_port,tx_buf,rx_buf) != -1 )
                    {
                        pnum = sscanf(rx_buf,"STORED %s",rx_string);
                        if ( pnum == 1)
                        {
                            printf("RX : %s\n", rx_buf);
                        }
                        else
                        {
                            printf("RXERROR : %s\n", rx_buf);
                            return -1;
                        }
                    }
                }
            }
            else
            {
                printf("RXERROR : %s\n", rx_buf);
                return -1;
            }
        }
        else
        {
            printf("Serial error sending button %s\n", button_name);
            return -1;
        }
    }
    return 0;
}

int send_erasecmd(int serial_port)
{
int pnum=0, time_elapsed;
    printf("Erasing\n");
    sprintf(tx_buf,"<ERASE ALL>");
    if ( serial_tx_rx_command(serial_port,tx_buf,rx_buf) != -1 )
    {
        while(pnum == 0)
        {
            pnum = sscanf(rx_buf,"Flash Erased, %d elapsed",&time_elapsed);
            printf("%s\n",rx_buf);
            if ( pnum == 0 )
                serial_rx_command(serial_port,rx_buf);
        }
    }
    return 0;
}

void usage(void)
{
    printf("ImageUploader -f <images_path> : upload all images\n");
    printf("ImageUploader -E               : erases ALL flash!!\n");
}

int main(int argc, char **argv)
{
int serial_port,i,erase=0;
char   *folder=NULL;

    while ((i = getopt (argc, argv, "f:dblaE")) != -1)
    switch (i)
    {
        case 'f':   folder = optarg; break;
        case 'E':   erase = 1;   break;
        default:    usage();return -1;
    }

    if ( erase == 0 )
    {
        if ( folder == NULL )
        {
            usage();
            return -1;
        }
    }

    serial_port = serial_open(SERIAL_DEVICE);
    send_images(serial_port,folder);
    if (erase==1)
        send_erasecmd(serial_port);

    serial_close(serial_port);
    return 0;
}
