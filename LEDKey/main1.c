#include <REGX52.H>
#include <INTRINS.H>
#include "BasicalFunc.h"

unsigned char KeyNum;

void main()
{
    P0 = 0xFE;
    // Timer0Init();
    while (1)
    {
        KeyNum = Key();
        switch (KeyNum)
        {
        case 1:
            P0_0 = ~P0_0;
            break;
        case 2:
            P0_1 = ~P0_1;
            break;
        case 3:
            P0_2 = ~P0_2;
            break;
        case 4:
            P0_3 = ~P0_3;
            break;
        default:
            break;
        }
    }
}

void Timer0_Routine() interrupt 1
{
    static unsigned int T0_Count;
    TH0 = 0x18;
    TL0 = 0xFC;
    T0_Count++;
    if (T0_Count >= 500)
    {
        T0_Count = 0;
        if()
    }
}