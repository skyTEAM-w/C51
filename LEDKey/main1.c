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