#include <REGX52.H>
#include <INTRINS.H>
#include "BasicalFunc.h"

unsigned char KeyNum, LedMode = 0;

void main()
{
    P0 = 0xFE;
    Timer0_Init();
    while (1)
    {
        KeyNum = Key();
        if(KeyNum == 1)
        {
            LedMode++;
            LedMode = LedMode % 2;
        }
    }
}

void Timer0_Routine() interrupt 1
{
    static unsigned int T0_Count;
    TL0 = 0x18;
    TH0 = 0xFC;
    T0_Count++;
    if (T0_Count >= 500)
    {
        T0_Count = 0;
        if(LedMode == 0)
        {
            P0 = _crol_(P0, 1);
        }
        if(LedMode == 1)
        {
            P0 = _cror_(P0, 1);
        }
    }
}