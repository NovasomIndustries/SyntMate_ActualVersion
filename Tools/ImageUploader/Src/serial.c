#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include "ImageUploader.h"

extern  char            rx_buf[BUFSIZE];

int serial_open(char *port)
{
struct termios tty;
int serial_port;

    serial_port = open(port, O_RDWR);

    // Read in existing settings, and handle any error
    if(tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s on port %s\n", errno, strerror(errno),port);
        return 1;
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 100;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be B115200
    cfsetispeed(&tty, B115200);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return -1;
    }
    return serial_port; // success
}

void serial_close(int serial_port)
{
    close(serial_port);
}

int serial_tx_rx(int serial_port,char *tx_packet,int tx_len,char *rx_packet,char *item_name)
{
int  num_bytes,pnum,rxed,packet_id ;

    //printf("%s : Sending %s, size is %d\n", __FUNCTION__,item_name,tx_len);
    num_bytes = write(serial_port, tx_packet, strlen(tx_packet));
    bzero(rx_packet,BUFSIZE);
    num_bytes = read(serial_port, rx_packet, BUFSIZE);
    //printf("%s : Received : rx_packet %s , size is %d\n",__FUNCTION__,rx_packet,num_bytes);

    if (num_bytes < 0)
    {
        printf("Error reading: %s", strerror(errno));
        return -1;
    }
    pnum = sscanf(rx_packet,"Finished %d %d OK",&packet_id,&rxed);
    if ( pnum == 2)
    {
        //printf("%s : %s downloaded, size %d\n",__FUNCTION__,item_name, rxed);
        return rxed;
    }
    printf("%s : Error : rx_packet %s\n",__FUNCTION__,rx_packet);
    return -1;
}

#define RESTART_PACKET      0xDEADBEEF
#define RESTART_PACKET_LEN  16
int serial_send_restart(int serial_port)
{
unsigned int restart_packet[RESTART_PACKET_LEN],i;

    for(i=0;i<RESTART_PACKET_LEN;i++)
        restart_packet[i] = RESTART_PACKET;
    write(serial_port, (char *)&restart_packet, RESTART_PACKET_LEN*4);
    return 0;
}

int serial_tx_rx_command(int serial_port,char *tx_packet,char *rx_packet)
{
int  num_bytes ;

    //printf("%s : %s %d\n",__FUNCTION__,tx_packet,strlen(tx_packet));
    num_bytes = write(serial_port, tx_packet, strlen(tx_packet));
    bzero(rx_packet,BUFSIZE);
    num_bytes = read(serial_port, rx_packet, BUFSIZE);
    //printf("%s : received %s\n",__FUNCTION__,rx_packet);

    if (num_bytes < 0)
    {
        printf("Error reading: %s", strerror(errno));
        return -1;
    }
    return num_bytes;
}

int serial_rx_command(int serial_port,char *rx_packet)
{
int  num_bytes ;

    bzero(rx_packet,BUFSIZE);
    num_bytes = read(serial_port, rx_packet, BUFSIZE);
    if (num_bytes < 0)
    {
        printf("Error reading: %s", strerror(errno));
        return -1;
    }
    return num_bytes;
}

