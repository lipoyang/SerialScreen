#ifndef _SERIAL_PORT_
#define _SERIAL_PORT_

#include <string.h>
#include "mbed.h"

// 'Serial' class of the 'mbed' library highly abstracts serial ports.
// this class 'SerialPort' is just a wrapper.
// so method functions are implemented as inline functions.

class SerialPort
{
public:
    // constructor & destructor
    SerialPort()
    {
        m_isOpen = false;
        m_port = NULL;
    }
    
    ~SerialPort()
    {
        this->close();
    }
    
    // open
    bool open()
    {
        // this is only for compatibility
        return false;   
    }
    bool open(int baud)
    {
        // this is only for compatibility
        return false;   
    }
    
    bool open(const void* port, int baud)
    {
        if(m_isOpen){
            return false;
        }
        
        if(port != NULL){
            m_port = (Serial*)port;
        }else{
            return false;
        }
        
        m_port->baud(baud);
        
        m_port->format(8, SerialBase::None, 1);
        
        return true;
    }
    
    // close
    void close()
    {
        // do nothing here
        m_isOpen = false;
    }
    
    // is open?
    bool isOpen()
    {
        return m_isOpen;
    }
    
    // read
    bool read(char *buff, int len)
    {
        // TODO not implemented yet!
        return false;
    }
    
    // write
    bool write(char *buff, int len)
    {
        buff[len] = '\0';
        int ret = m_port->printf(buff);
        return (ret == len) ? true : false;
    }
    
private:
    bool m_isOpen;
    Serial*  m_port;
};

#endif
