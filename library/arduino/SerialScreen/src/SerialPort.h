#ifndef _SERIAL_PORT_
#define _SERIAL_PORT_

#include <Arduino.h>

// 'Serial' class of the 'Arduino' library highly abstracts serial ports.
// this class 'SerialPort' is just a wrapper.
// so method functions are implemented as inline functions.

class SerialPort
{
public:
    // constructor & destructor
    SerialPort()
    {
        m_isOpen = false;
    }
    ~SerialPort()
    {
        this->close();
    }
    
    // open
    bool open()
    {
        this->open(9600);
    }
    
    bool open(int baud)
    {
        if(m_isOpen)
        {
            return false;
        }
        Serial.begin(baud);
        m_isOpen = true;
        return true;
    }
    bool open(const void* port, int baud)
    {
        // just for compatibility
        // port not used
        this->open(baud);
    }
    
    // close
    void close()
    {
        if(!m_isOpen)
        {
            return;
        }
        m_isOpen = false;
        Serial.end();
    }
    
    // is open?
    bool isOpen()
    {
        return m_isOpen;
    }
    
    // read
    bool read(char *buff, int len)
    {
        // ! no time out
        
        int cnt = 0;
        while(cnt < len){ 
            if(Serial.available() > 0) {
                buff[cnt] = Serial.read();
                cnt++;
            }
        }
        return true;
    }
    
    // write
    bool write(char *buff, int len)
    {
        int ret = Serial.write(buff, len);
        return (ret == len) ? true : false;
    }
    
private:
    bool m_isOpen;
};

#endif
