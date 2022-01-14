#include <REGX52.H>
#include "BasicalFunc.h"

int main()
{
    Uart_Init();
    while(1)
    {

    }
}

void UART_Routine() interrupt 4
{
    if(RI == 1)
    {
        P0 = ~SBUF;
        Uart_SendByte(SBUF);
        RI = 0;         //RI软件复位
    }
}