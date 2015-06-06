#include <string.h>
#include "SerialScreen.h"

// STX/ETX code
#define CODE_STX    0x02
#define CODE_ETX    0x03

// API command code
#define CODE_setScreen     0x20
#define CODE_clearScreen   0x21
#define CODE_setColor      0x22
#define CODE_drawLine      0x23
#define CODE_drawRectangle 0x24
#define CODE_fillRectangle 0x25
#define CODE_drawPolygon   0x26
#define CODE_fillPolygon   0x27
#define CODE_drawCircle    0x28
#define CODE_fillCircle    0x29
#define CODE_drawPie       0x2A
#define CODE_fillPie       0x2B
#define CODE_setFont       0x2C
#define CODE_drawString    0x2D
#define CODE_drawImage     0x2E
#define CODE_loadImage     0x2F

// colors
const Color SerialScreen::BLACK   = 0x000000;
const Color SerialScreen::RED     = 0xFF0000;
const Color SerialScreen::GREEN   = 0x00FF00;
const Color SerialScreen::BLUE    = 0x0000FF;
const Color SerialScreen::YELLOW  = 0xFFFF00;
const Color SerialScreen::CYAN    = 0x00FFFF;
const Color SerialScreen::MAGENTA = 0xFF00FF;
const Color SerialScreen::WHITE   = 0xFFFFFF;
const Color SerialScreen::GRAY    = 0x808080;

// constructor
SerialScreen::SerialScreen()
{
    ;
}

// destructor
SerialScreen::~SerialScreen()
{
    ;
}

// open
bool SerialScreen::open()
{
    return m_serialPort.open();
}

bool SerialScreen::open(int baud)
{
    return m_serialPort.open(baud);
}

bool SerialScreen::open(const void* port, int baud)
{
    return m_serialPort.open(port, baud);
}

// close
void SerialScreen::close()
{
    m_serialPort.close();
}

// is open?
bool SerialScreen::isOpen()
{
    return m_serialPort.isOpen();
}

/********** Graphic APIs **********/

// set the screen size
void SerialScreen::setScreen(int w, int h)
{
    m_command[0] = CODE_STX;
    m_command[1] = CODE_setScreen;
    
    IntToHex(&m_command[2], w, 3);
    IntToHex(&m_command[5], h, 3);
    
    m_command[8] = CODE_ETX;
    
    m_serialPort.write(m_command, 9);
}

// clear the screen
void SerialScreen::clearScreen(Color color)
{
    m_command[0] = CODE_STX;
    m_command[1] = CODE_clearScreen;
    
    IntToHex(&m_command[2], color, 6);
    
    m_command[8] = CODE_ETX;
    
    m_serialPort.write(m_command, 9);
}

// set a color
void SerialScreen::setColor(Color color, int width)
{
    m_command[0] = CODE_STX;
    m_command[1] = CODE_setColor;
    
    IntToHex(&m_command[2], color, 6);
    IntToHex(&m_command[8], width, 2);
    
    m_command[10] = CODE_ETX;
    
    m_serialPort.write(m_command, 11);
}

// draw a line
void SerialScreen::drawLine(int x1, int y1, int x2, int y2)
{
    m_command[0] = CODE_STX;
    m_command[1] = CODE_drawLine;
    
    IntToHex(&m_command[2], x1, 3);
    IntToHex(&m_command[5], y1, 3);
    IntToHex(&m_command[8], x2, 3);
    IntToHex(&m_command[11], y2, 3);
    
    m_command[14] = CODE_ETX;
    
    m_serialPort.write(m_command, 15);
}

// draw a rectangle
void SerialScreen::drawRectangle(int x, int y, int w, int h)
{
    m_command[0] = CODE_STX;
    m_command[1] = CODE_drawRectangle;
    
    IntToHex(&m_command[2], x, 3);
    IntToHex(&m_command[5], y, 3);
    IntToHex(&m_command[8], w, 3);
    IntToHex(&m_command[11], h, 3);
    
    m_command[14] = CODE_ETX;
    
    m_serialPort.write(m_command, 15);
}

// fill a rectangle
void SerialScreen::fillRectangle(int x, int y, int w, int h)
{
    m_command[0] = CODE_STX;
    m_command[1] = CODE_fillRectangle;
    
    IntToHex(&m_command[2], x, 3);
    IntToHex(&m_command[5], y, 3);
    IntToHex(&m_command[8], w, 3);
    IntToHex(&m_command[11], h, 3);
    
    m_command[14] = CODE_ETX;
    
    m_serialPort.write(m_command, 15);
}

// draw a polygon
void SerialScreen::drawPolygon(int num, Point* points)
{
    if( num > 8 ) return;
    
    m_command[0] = CODE_STX;
    m_command[1] = CODE_drawPolygon;
    
    IntToHex(&m_command[2], num, 1);
    for(int i=0;i<num;i++){
        IntToHex(&m_command[3+i*6], points[i].x, 3);
        IntToHex(&m_command[6+i*6], points[i].y, 3);
    }
    
    m_command[3+num*6] = CODE_ETX;
    
    m_serialPort.write(m_command, 4+num*6);
}

// fill a polygon
void SerialScreen::fillPolygon(int num, Point* points)
{
    if( num > 8 ) return;
    
    m_command[0] = CODE_STX;
    m_command[1] = CODE_fillPolygon;
    
    IntToHex(&m_command[2], num, 1);
    for(int i=0;i<num;i++){
        IntToHex(&m_command[3+i*6], points[i].x, 3);
        IntToHex(&m_command[6+i*6], points[i].y, 3);
    }
    
    m_command[3+num*6] = CODE_ETX;
    
    m_serialPort.write(m_command, 4+num*6);
}

// draw a circle
void SerialScreen::drawCircle(int x, int y, int w, int h)
{
    m_command[0] = CODE_STX;
    m_command[1] = CODE_drawCircle;
    
    IntToHex(&m_command[2], x, 3);
    IntToHex(&m_command[5], y, 3);
    IntToHex(&m_command[8], w, 3);
    IntToHex(&m_command[11], h, 3);
    
    m_command[14] = CODE_ETX;
    
    m_serialPort.write(m_command, 15);
}

// fill a circle
void SerialScreen::fillCircle(int x, int y, int w, int h)
{
    m_command[0] = CODE_STX;
    m_command[1] = CODE_fillCircle;
    
    IntToHex(&m_command[2], x, 3);
    IntToHex(&m_command[5], y, 3);
    IntToHex(&m_command[8], w, 3);
    IntToHex(&m_command[11], h, 3);
    
    m_command[14] = CODE_ETX;
    
    m_serialPort.write(m_command, 15);
}

// draw a pie
void SerialScreen::drawPie(int x, int y, int w, int h, int start, int sweep)
{
    m_command[0] = CODE_STX;
    m_command[1] = CODE_drawPie;
    
    IntToHex(&m_command[2], x, 3);
    IntToHex(&m_command[5], y, 3);
    IntToHex(&m_command[8], w, 3);
    IntToHex(&m_command[11], h, 3);
    IntToHex(&m_command[14], start, 3);
    IntToHex(&m_command[17], sweep, 3);
    
    m_command[20] = CODE_ETX;
    
    m_serialPort.write(m_command, 21);
}

// fill a pie
void SerialScreen::fillPie(int x, int y, int w, int h, int start, int sweep)
{
    m_command[0] = CODE_STX;
    m_command[1] = CODE_fillPie;
    
    IntToHex(&m_command[2], x, 3);
    IntToHex(&m_command[5], y, 3);
    IntToHex(&m_command[8], w, 3);
    IntToHex(&m_command[11], h, 3);
    IntToHex(&m_command[14], start, 3);
    IntToHex(&m_command[17], sweep, 3);
    
    m_command[20] = CODE_ETX;
    
    m_serialPort.write(m_command, 21);
}

// set a font
void SerialScreen::setFont(int size)
{
    m_command[0] = CODE_STX;
    m_command[1] = CODE_setFont;
    
    IntToHex(&m_command[2], size, 2);
    
    m_command[4] = CODE_ETX;
    
    m_serialPort.write(m_command, 5);
}

// draw a string
void SerialScreen::drawString(int x, int y, const char* s)
{
    int len = strlen(s);
    if((len + 10) > SS_COMMAND_SIZE) return;
    
    m_command[0] = CODE_STX;
    m_command[1] = CODE_drawString;
    
    IntToHex(&m_command[2], x, 3);
    IntToHex(&m_command[5], y, 3);
    memcpy(&m_command[8], s, len);
    
    m_command[8 + len] = CODE_ETX;
    
    m_serialPort.write(m_command, 9 + len);
}

// draw a image
void SerialScreen::drawImage(int x, int y, int index)
{
    m_command[0] = CODE_STX;
    m_command[1] = CODE_drawImage;
    
    IntToHex(&m_command[2], x, 3);
    IntToHex(&m_command[5], y, 3);
    IntToHex(&m_command[8], index, 2);
    
    m_command[10] = CODE_ETX;
    
    m_serialPort.write(m_command, 11);
}

// load a image
void SerialScreen::loadImage(int index, const char* path)
{
    int len = strlen(path);
    if((len + 5) > SS_COMMAND_SIZE) return;
    
    m_command[0] = CODE_STX;
    m_command[1] = CODE_loadImage;
    
    IntToHex(&m_command[2], index, 2);
    memcpy(&m_command[4], path, len);
    
    m_command[4 + len] = CODE_ETX;
    
    m_serialPort.write(m_command, 5 + len);
}
