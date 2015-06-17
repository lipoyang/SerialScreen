#include <stdio.h>
#include "SerialScreen.h"

int main(void)
{
    SerialScreen ss;
    
    if(!ss.open("/dev/ttyUSB0", 115200 ))
    {
        printf("Can't open SerialScreen!\n");
        return 1;
    }
    // setScreen, clearScreen
    ss.setScreen(720,540);
    ss.clearScreen(ss.BLUE);
    // setColor, drawLine
    ss.setColor(ss.BLACK, 5);
    ss.drawLine(0,0,500,500);
    // drawRectangle, fillRectangle
    ss.setColor(ss.WHITE, 5);
    ss.drawRectangle(100,100,100,100);
    ss.setColor(ss.YELLOW, 5);
    ss.fillRectangle(150,150,150,100);
    // drawCircle, fillCircle
    ss.setColor(ss.GREEN, 5);
    ss.drawCircle(200,200,150,100);
    ss.setColor(ss.MAGENTA, 5);
    ss.fillCircle(300,200,150,100);
    // drawPolygon, fillPolygon
    ss.setColor(ss.CYAN,3);
    Point triangle[3] ={{400,100}, {450,250}, {500,150}};
    ss.drawPolygon(3,triangle);
    ss.setColor(ss.RED,3);
    Point pentagon[5] ={{500,100}, {525,175}, {625,175},{650,100},{575,50}};
    ss.fillPolygon(5,pentagon);
    // drawPie, fillPie
    ss.setColor(0xFF8000,3);
    ss.drawPie(500,200,100,100,45,315);
    ss.setColor(0x8080FF,3);
    ss.fillPie(500,350,150,200,225,90);
    // setFont, drawString
    ss.setFont(20);
    ss.setColor(ss.CYAN,5);
    ss.drawString(50,50,"あいうえお");
    ss.setColor(ss.RED,5);
    ss.drawString(200,50,"ABC123");
    // loadImage, drawImage
    ss.loadImage(0, "cn.png");
    ss.loadImage(1, "jp.png");
    ss.loadImage(2, "us.png");
    ss.drawImage(100, 350, 0);
    ss.drawImage(200, 350, 1);
    ss.drawImage(300, 350, 2);
    ss.redraw();
    
    ss.close();
    
    return 0;
}
