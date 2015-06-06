#include "SerialPort.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <termios.h>

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(...)    fprintf(stderr, __VA_ARGS__)
#else
#define DEBUG_PRINT(...)    ;
#endif

// constructor
SerialPort::SerialPort()
{
    m_error = NO_ERROR;
    m_isOpen = false;
    m_timeout = 1000; // 1msec
}

// destructor
SerialPort::~SerialPort()
{
    this->close();
}

// open
bool SerialPort::open()
{
    return this->open("/dev/tty0", 9600);
}

bool SerialPort::open(int baud)
{
    return this->open("/dev/tty0", baud);
}

bool SerialPort::open(const void* port, int baud)
{
    unsigned int baudMask = B9600;
    switch(baud){
        case 9600:
            baudMask = B9600;
            break;
        case 19200:
            baudMask = B19200;
            break;
        case 38400:
            baudMask = B38400;
            break;
        case 57600:
            baudMask = B57600;
            break;
        case 115200:
            baudMask = B115200;
            break;
        default:
            m_error = ERROR_UNSUPPORTED_BAUD;
            return false;
    }
    
    if(m_isOpen)
    {
        DEBUG_PRINT("UART already opened! (%s)\n", (char*)port);
        m_error = ERROR_ALREADY_OPEN;
        return false;
    }
    
    // ::oprn is a C starndard function, Non-blocking mode
    if((m_handle = ::open((char*)port, O_RDWR | O_NOCTTY | O_NONBLOCK))==-1)
    {
        DEBUG_PRINT("Can't open UART! (%s)\n",(char*)port);
        m_error = ERROR_CANT_OPEN;
        return false;
    }else{
        DEBUG_PRINT("UART opened\n");
        m_error = NO_ERROR;
        m_isOpen = true;
        //return true;
    }
    
//    // save settings of the serial port
//    struct termios  oldSetting;
//    tcgetattr(m_handle, &oldSetting);
    
    // new settings of the serial port
    struct termios  newSetting;
    bzero(&newSetting, sizeof(newSetting));
    
    // baudMask: baud rate
    // CS8     : 8n1 (Data 8bit, Non Parity, Stop 1bit)
    // CLOCAL  : local connection, non modem control
    // CREAD   : enable receiving characters
    newSetting.c_cflag = (baudMask | CS8 | CLOCAL);
    
    // IGNPAR  : ignore parity error data
    newSetting.c_iflag = (IGNPAR);
    
    // output by raw mode
    newSetting.c_oflag = 0;
    newSetting.c_lflag = 0;
    
    // inter-character timeout [0.1sec] (blocking limit)
    newSetting.c_cc[VTIME] = 0;
    // minimum data size to read (blocking limit)
    newSetting.c_cc[VMIN] = 0;
    
    // clear the serial port buffer, and set the new settings
    tcflush(m_handle, TCIFLUSH);
    
    tcsetattr(m_handle,TCSANOW,&newSetting);

    return true;
}

// close
void SerialPort::close()
{
    if(!m_isOpen)
    {
        //DEBUG_PRINT("UART not opened!\n");
        //m_error = ERROR_NOT_OPEN;
        return;
    }
    
    ::close(m_handle); // ::close is a C starndard function
    m_isOpen = false;
}

// is open?
bool SerialPort::isOpen()
{
    return m_isOpen;
}

// set the time out limit [usec] 
void SerialPort::setTimeOut(unsigned long usec)
{
    m_timeout = usec;
}

// read
bool SerialPort::read(char *buff, int len)
{
    if(!m_isOpen)
    {
        DEBUG_PRINT("SerialPort not opened!\n");
        m_error = ERROR_NOT_OPEN;
        return false;
    }

    // get the time
    struct timeval s, e;
    gettimeofday(&s, NULL);

    // until timeout or data received
    int read_size;
    int read_index = 0;
    unsigned long elapsed;
    do{
        // ::read is a C starndard function
        read_size = ::read(m_handle, &buff[read_index], len-read_index);
        if(read_size > 0){
            read_index += read_size;
            
            // data received
            if(read_index >= len){
                return true;
            }
        }
        
        // get the time, and calculate elapsed time
        gettimeofday(&e, NULL);
        elapsed = e.tv_sec - s.tv_sec;
        elapsed = ((elapsed * 1000000) + e.tv_usec) - s.tv_usec;
    }
    while( elapsed < m_timeout );
    
    // timeout
    m_error = ERROR_CANT_READ_DATA;
    return false;
}

// write
bool SerialPort::write(char *buff, int len)
{
    if(!m_isOpen)
    {
        DEBUG_PRINT("SerialPort not opened!\n");
        m_error = ERROR_NOT_OPEN;
        return false;
    }
    // send data
    if ((::write(m_handle, buff, len)) != len) // ::write is a C starndard function
    {
//      DEBUG_PRINT("Can't write required size data!\n");
        m_error = ERROR_CANT_WRITE_DATA;
        return false;
    }
    return true;
}

// get the error number
int SerialPort::getError()
{
    return m_error;
}

#if 0
// unit test
int main(void)
{
    SerialPort serialPort;
    unsigned char buff[32];
    
    if(!serialPort.open("/dev/ttyUSB0", 9600)){
        printf("can't open ttyUSB0\n");
        return 1;
    }
    
    serialPort.setTimeOut(5000000);
    
    while(1){
        
        serialPort.write((unsigned char*)"hoge\n",5);
        
        memset(buff,0,32);
        printf("try to read\n");
        if(!serialPort.read(buff,5)){
            printf("read error = %d\n",serialPort.getError());
        }else{
            printf("read: %s\n",buff);
        }
    }
}
#endif
