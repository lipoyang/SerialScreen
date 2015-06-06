#ifndef _SERIAL_SCREEN_H_
#define _SERIAL_SCREEN_H_

#include "common.h"
#include "SerialPort.h"

// color
typedef unsigned long Color;

// point
struct Point
{
    int x;
    int y;
};

// SerialScreen class
class SerialScreen
{
public:
    SerialScreen();
    ~SerialScreen();
    
    // open / close
    bool open();
    bool open(int baud);
    bool open(const void* port, int baud);
    void close();
    bool isOpen();
    
    // graphic APIs
    void setScreen(int w, int h);
    void clearScreen(Color color);
    void setColor(Color color, int width);
    void drawLine(int x1, int y1, int x2, int y2);
    void drawRectangle(int x, int y, int w, int h);
    void fillRectangle(int x, int y, int w, int h);
    void drawPolygon(int num, Point* points);
    void fillPolygon(int num, Point* points);
    void drawCircle(int x, int y, int w, int h);
    void fillCircle(int x, int y, int w, int h);
    void drawPie(int x, int y, int w, int h, int start, int sweep);
    void fillPie(int x, int y, int w, int h, int start, int sweep);
    void setFont(int size);
    void drawString(int x, int y, const char* s);
    void drawImage(int x, int y, int index);
    void loadImage(int index, const char* path);

public:
    // colors
    static const Color BLACK;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color YELLOW;
    static const Color CYAN;
    static const Color MAGENTA;
    static const Color WHITE;
    static const Color GRAY;

private:
    // serial port
    SerialPort m_serialPort;
    
    // command buffer
    enum {SS_COMMAND_SIZE = 256};
    char m_command[SS_COMMAND_SIZE];
};

#endif
