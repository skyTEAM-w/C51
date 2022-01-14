#include <REGX52.H>
#include "BasicalFunc.h"

unsigned char Sec = 0;

void main()
{
    Uart_Init();
    Uart_SendByte(0x11);
    while (1)
    {
        Uart_SendByte(Sec++);
        Delay(1000);
    }
}