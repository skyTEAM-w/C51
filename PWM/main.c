#include <REGX52.H>
#include "Delay.h"
#include "Nixie.h"
#include "Timer0.h"
#include "Key.h"

unsigned char Counter, Compare;
unsigned char KeyNum, Speed = 0;
sbit LED = P2 ^ 0;
sbit Motor = P1 ^ 0;

void main()
{
    Timer0_Init();
    Compare = 0;
    while (1)
    {
        KeyNum = Key();
        if (KeyNum == 1)
        {
            Speed++;
            Speed %= 4;
            if (Speed == 0)
            {
                Compare = 0;
            }
            if (Speed == 1)
            {
                Compare = 30;
            }
            if (Speed == 2)
            {
                Compare = 50;
            }
            if (Speed == 3)
            {
                Compare = 100;
            }
        }
        Nixie(1, Speed);
    }
}

void Timer0_Routine() interrupt 1
{
    static unsigned int T0Count;
    TL0 = 0x9C; //设置定时初值
    TH0 = 0xFF; //设置定时初值
    T0Count++;
    Counter++;
    Counter %= 100;
    if (Counter < Compare)
    {
        LED = 0;
        Motor = 1;
    }
    else
    {
        LED = 1;
        Motor = 0;
    }
}