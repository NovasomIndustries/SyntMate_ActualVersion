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

int send_digits(int serial_port,char   *folder)
{
int size_in_bytes,i,j,pnum,packet_id,rxed,ret_val;
char   digit[32],rx_string[32];
char   file_in[128];

    sprintf(digit,"DigitOff");
    sprintf(file_in,"%s/%s.bmp",folder,digit);
    size_in_bytes = read_and_convert(file_in);
    bzero(tx_buf,BUFSIZE);
    sprintf(tx_buf,"<DIGITOFF %s SIZE %d>",digit,size_in_bytes);
    if ( serial_tx_rx_command(serial_port,tx_buf,rx_buf) != -1 )
    {
        pnum = sscanf(rx_buf,"DIGITOFF %s %d OK",rx_string,&rxed);
        if ( pnum == 2)
        {
            printf("Digit %s accepted, size %d\n",digit, rxed);
            ret_val = serial_tx_rx(serial_port,(char *)ascii_image,size_in_bytes,rx_buf,digit);
            if ( ret_val == -1 )
            {
                printf("Error sending digit %s\n", digit);
                printf("Reply : %s\n", rx_buf);
                return -1;
            }
        }
    }
    else
    {
        printf("Serial error sending digit %s\n", digit);
        return -1;
    }
    printf("Writing %s\n",digit);
    sprintf(tx_buf,"<STORE %s>",digit);
    if ( serial_tx_rx_command(serial_port,tx_buf,rx_buf) != -1 )
    {
        pnum = sscanf(rx_buf,"STORED %s",rx_string);
        if ( pnum == 1)
        {
            printf("Digit %s written\n", digit);
        }
        else
        {
            printf("Error writing digit %s\n", digit);
            printf("Reply : %s\n", rx_buf);
            //printf("Reply : %s , expected STORED %s , pnum = %d\n", rx_buf,digit,pnum);
            return -1;
        }
    }

    for(j=0;j<3;j++)
    {
        switch(j)
        {
        case    0   :   sprintf(digit,"green");break;
        case    1   :   sprintf(digit,"yellow");break;
        case    2   :   sprintf(digit,"red");break;
        }

        for(i=0;i<10;i++)
        {
            sprintf(file_in,"%s/D%d%s.bmp",folder,i,digit);
            size_in_bytes = read_and_convert(file_in);
            bzero(tx_buf,BUFSIZE);
            sprintf(tx_buf,"<DIGIT %s SIZE %d %d>",digit,size_in_bytes,i);
            if ( serial_tx_rx_command(serial_port,tx_buf,rx_buf) != -1 )
            {
                pnum = sscanf(rx_buf,"DIGIT %s %d %d OK",rx_string,&packet_id,&rxed);
                if ( pnum == 3)
                {
                    printf("Digit %s %d accepted, size %d\n",digit,packet_id, rxed);
                    ret_val = serial_tx_rx(serial_port,(char *)ascii_image,size_in_bytes,rx_buf,digit);
                    if ( ret_val == -1 )
                    {
                        printf("Error sending digit %s\n", digit);
                        printf("Reply : %s\n", rx_buf);
                        return -1;
                    }
                }
            }
            else
            {
                printf("Serial error sending digit %s\n", digit);
                return -1;
            }
        }
        printf("Writing %s\n",digit);
        sprintf(tx_buf,"<STORE %s>",digit);
        if ( serial_tx_rx_command(serial_port,tx_buf,rx_buf) != -1 )
        {
            pnum = sscanf(rx_buf,"STORED %s",rx_string);
            if ( pnum == 1)
            {
                printf("Digit %s written\n", digit);
            }
            else
            {
                printf("Error writing digit %s\n", digit);
                printf("Reply : %s\n", rx_buf);
                //printf("Reply : %s , expected STORED %s , pnum = %d\n", rx_buf,digit,pnum);
                return -1;
            }
        }
    }
    return 0;
}

int send_buttons(int serial_port,char   *folder)
{
int     size_in_bytes,j,pnum,rxed,ret_val;
char    rx_string[32],button_name[32];
char    file_in[128];

    for(j=0;j<13;j++)
    {
        switch(j)
        {
        case    0   :   sprintf(button_name,"play");break;
        case    1   :   sprintf(button_name,"stop");break;
        case    2   :   sprintf(button_name,"settings");break;
        case    3   :   sprintf(button_name,"settings_disabled");break;
        case    4   :   sprintf(button_name,"home");break;
        case    5   :   sprintf(button_name,"home_disabled");break;
        case    6   :   sprintf(button_name,"increase");break;
        case    7   :   sprintf(button_name,"increase_disabled");break;
        case    8   :   sprintf(button_name,"decrease");break;
        case    9   :   sprintf(button_name,"decrease_disabled");break;
        case   10   :   sprintf(button_name,"plus");break;
        case   11   :   sprintf(button_name,"minus");break;
        case   12   :   sprintf(button_name,"back");break;
        }

        sprintf(file_in,"%s/%s.bmp",folder,button_name);
        size_in_bytes = read_and_convert(file_in);
        bzero(tx_buf,BUFSIZE);
        sprintf(tx_buf,"<BUTTON %s SIZE %d>",button_name,size_in_bytes);
        if ( serial_tx_rx_command(serial_port,tx_buf,rx_buf) != -1 )
        {
            pnum = sscanf(rx_buf,"BUTTON %s %d OK",rx_string,&rxed);
            if ( pnum == 2)
            {
                printf("Button %s accepted, size %d\n",button_name, rxed);
                ret_val = serial_tx_rx(serial_port,(char *)ascii_image,size_in_bytes,rx_buf,button_name);
                if ( ret_val == -1 )
                {
                    printf("Error sending button %s\n", button_name);
                    printf("Reply : %s\n", rx_buf);
                    return -1;
                }
                else
                {
                    printf("Writing %s\n",button_name);
                    sprintf(tx_buf,"<STORE %s>",button_name);
                    if ( serial_tx_rx_command(serial_port,tx_buf,rx_buf) != -1 )
                    {
                        pnum = sscanf(rx_buf,"STORED %s",rx_string);
                        if ( pnum == 1)
                        {
                            printf("Button %s written\n", button_name);
                        }
                        else
                        {
                            printf("Error writing button %s\n", button_name);
                            printf("Reply : %s\n", rx_buf);
                            return -1;
                        }
                    }
                }
            }
        }
        else
        {
            printf("Error -1 sending button %s\n", button_name);
            return -1;
        }
    }

    return 0;
}

int send_logo(int serial_port,char   *folder)
{
int     size_in_bytes,pnum,rxed,ret_val;
char    rx_string[32],logo_name[32];
char    file_in[128];

    sprintf(logo_name,"SintMateLogo");

    sprintf(file_in,"%s/%s.bmp",folder,logo_name);
    size_in_bytes = read_and_convert(file_in);
    bzero(tx_buf,BUFSIZE);
    sprintf(tx_buf,"<LOGO %s SIZE %d>",logo_name,size_in_bytes);
    if ( serial_tx_rx_command(serial_port,tx_buf,rx_buf) != -1 )
    {
        pnum = sscanf(rx_buf,"LOGO %s %d OK",rx_string,&rxed);
        if ( pnum == 2)
        {
            printf("Logo %s accepted, size %d\n",logo_name, rxed);
            ret_val = serial_tx_rx(serial_port,(char *)ascii_image,size_in_bytes,rx_buf,logo_name);
            if ( ret_val == -1 )
            {
                printf("Error sending logo %s\n", logo_name);
                printf("Reply : %s\n", rx_buf);
                return -1;
            }
            else
            {
                printf("Writing %s\n",logo_name);
                sprintf(tx_buf,"<STORE %s>",logo_name);
                if ( serial_tx_rx_command(serial_port,tx_buf,rx_buf) != -1 )
                {
                    pnum = sscanf(rx_buf,"STORED %s",rx_string);
                    if ( pnum == 1)
                    {
                        printf("Logo %s written\n", logo_name);
                    }
                    else
                    {
                        printf("Error writing logo %s\n", logo_name);
                        printf("Reply : %s\n", rx_buf);
                        return -1;
                    }
                }
            }
        }
        else
        {
            printf("Error -1 sending logo %s\n", logo_name);
            return -1;
        }
    }

    return 0;
}

int get_background_file(char *background_name,char *folder)
{
int     size_in_bytes;
char    file_in[128];
    sprintf(file_in,"%s/%s.bmp",folder,background_name);
    size_in_bytes = read_and_convert(file_in);
    if ( size_in_bytes == -1 )
    {
        printf("Background %s not found\n",background_name);
        return -1;
    }
    return size_in_bytes;
}

int send_background_cmd(int serial_port,char *background_name,int size_in_bytes,int chunk_number)
{
int     pnum,rxed,chunk_rxed,ret_val;
char    rx_string[32];

    bzero(tx_buf,BUFSIZE);
    sprintf(tx_buf,"<BACKGROUND %s SIZE %d %d>",background_name,size_in_bytes,chunk_number);
    if ( serial_tx_rx_command(serial_port,tx_buf,rx_buf) != -1 )
    {
        pnum = sscanf(rx_buf,"BACKGROUND %s %d %d OK",rx_string,&rxed,&chunk_rxed);
        if ( pnum == 3)
        {
            printf("Background %s accepted, chunk %d size %d\n",rx_string,chunk_rxed, rxed);
            ret_val = serial_tx_rx(serial_port,(char *)ascii_image,size_in_bytes,rx_buf,background_name);
            if ( ret_val == -1 )
            {
                printf("Error sending %s chunk %d\n", background_name,chunk_number);
                printf("Reply : %s\n", rx_buf);
                return -1;
            }
            return 0;
        }
    }
    return -1;
}

int send_background_write(int serial_port,char *background_name,int chunk_number,int number_of_bytes)
{
char    rx_string[32];
int     pnum,rxed,rxnumber_of_bytes;

    printf("Writing %s\n",background_name);
    sprintf(tx_buf,"<STORE %s FLASH %d %d>",background_name,number_of_bytes,chunk_number);
    if ( serial_tx_rx_command(serial_port,tx_buf,rx_buf) != -1 )
    {
        pnum = sscanf(rx_buf,"STORED %s %d %d",rx_string,&rxed,&rxnumber_of_bytes);
        if ( pnum == 3)
        {
            printf("Background %s written %d bytes\n", background_name,rxnumber_of_bytes);
            return 0;
        }
        else
        {
            printf("Error writing background %s chunk %d\n", background_name,chunk_number);
            printf("Reply : %s\n", rx_buf);
        }
    }
    return -1;
}

#define CHUNK_SIZE      16384
int send_background(int serial_port,char *folder)
{
int     size_in_bytes,chunk_number,chunk;
char    background_name[32];

    sprintf(background_name,"Background");
    if ( (size_in_bytes = get_background_file(background_name,folder) ) == -1 )
    {
        return -1;
    }
    chunk = size_in_bytes;
    chunk_number = 0;
    printf("chunk %d , number of chunks %d\n",chunk,size_in_bytes/CHUNK_SIZE+1);

    while(chunk > CHUNK_SIZE)
    {
        if ( send_background_cmd(serial_port,background_name,CHUNK_SIZE,chunk_number) != -1)
            printf("Sent chunk %d\n",chunk_number);
        else
        {
            printf("Error on chunk %d\n",chunk_number);
            return -1;
        }
        chunk -= CHUNK_SIZE;
        chunk_number++;
    }
    if ( send_background_cmd(serial_port,background_name,chunk,chunk_number) == -1)
    {
            return -1;
    }
    if ( send_background_write(serial_port,background_name,chunk_number,CHUNK_SIZE) == -1)
            return -1;
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
    printf("ImageUploader -f <images_path> [-a all][-d digits only][-b buttons only][-l logo only][-B background only]\n");
    printf("ImageUploader -E Erases ALL flash!!\n");
}

int main(int argc, char **argv)
{
int serial_port,i,digit=0,buttons=0,logo=0,all=0,erase=0,background=0;
char   *folder=NULL;

    while ((i = getopt (argc, argv, "f:dblaEB")) != -1)
    switch (i)
    {
        case 'f':   folder = optarg;   break;
        case 'd':   digit = 1;   break;
        case 'b':   buttons = 1;   break;
        case 'l':   logo = 1;   break;
        case 'B':   background = 1;   break;
        case 'a':   digit = buttons = logo = 1;   break;
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
        if ((digit ==0) && (buttons == 0)&&( logo == 0) && (all==0)&& (background==0))
//            digit = buttons = logo = background = 1;
            digit = buttons = logo = 1;
    }
    if (background==1)
    {
        printf("Not yet implemented\n");
        return 0;
    }

    serial_port = serial_open(SERIAL_DEVICE);
    //serial_send_restart(serial_port);
    if (digit==1)
        send_digits(serial_port,folder);
    if (buttons==1)
        send_buttons(serial_port,folder);
    if (logo==1)
        send_logo(serial_port,folder);
    if (background==1)
        send_background(serial_port,folder);
    if (erase==1)
        send_erasecmd(serial_port);


    serial_close(serial_port);
    return 0;
}
