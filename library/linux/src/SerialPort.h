#ifndef _SERIAL_PORT_
#define _SERIAL_PORT_

class SerialPort
{
public:
    SerialPort();
    ~SerialPort();
    
    bool open();
    bool open(int baud);
    bool open(const void* port, int baud);
    void close();
    bool isOpen();
    void setTimeOut(unsigned long usec);
    bool read(char *buff, int len);
    bool write(char *buff, int len);
    int getError();
    
    // error code
    enum ErrorCode{
        NO_ERROR,
        ERROR_ALREADY_OPEN,
        ERROR_CANT_OPEN,
        ERROR_NOT_OPEN,
        ERROR_CANT_READ_DATA,
        ERROR_CANT_WRITE_DATA,
        ERROR_UNSUPPORTED_BAUD
    };
    
private:
    bool m_isOpen;
    int  m_handle;            // file descriptor of serial port devide
    int m_error;              // error number
    unsigned long m_timeout;  // receive timeout [usec]
};

#endif
