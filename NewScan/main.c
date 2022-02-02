#include <REGX52.H>
#include "Timer0Init.h"
#include "Key.h"
#include "SEG.h"
#include "AT24C02.h"
#include "Delay.h"

unsigned char KeyNum;
unsigned char Min, Sec, MinSec;
unsigned char RunFlag;

void main()
{
    Timer0_Init();

    while (1)
    {
        KeyNum = Key();
        if (KeyNum == 1)
        {
            RunFlag = !RunFlag;
        }
        if(KeyNum == 2)
        {
            Min = Sec = MinSec = 0;
        }
        if(KeyNum == 3)
        {
            AT24C02_WriteByte(0, Min);
            Delay(5);
            AT24C02_WriteByte(1, Sec);
            Delay(5);
            AT24C02_WriteByte(2, MinSec);
            Delay(5);
        }
        if(KeyNum == 4)
        {
            Min = AT24C02_ReadByte(0);
            Sec = AT24C02_ReadByte(1);
            MinSec = AT24C02_ReadByte(2);
        }
        SEG_SetBuf(1, Min / 10);
        SEG_SetBuf(2, Min % 10);
        SEG_SetBuf(3, 17);
        SEG_SetBuf(4, Sec / 10);
        SEG_SetBuf(5, Sec % 10);
        SEG_SetBuf(6, 17);
        SEG_SetBuf(7, MinSec / 10);
        SEG_SetBuf(8, MinSec % 10);
    }
}

void Sec_Loop()
{
    if (RunFlag == 1)
    {
        MinSec++;
        if (MinSec >= 100)
        {
            MinSec = 0;
            Sec++;
            if (Sec >= 60)
            {
                Sec = 0;
                Min++;
            }
        }
    }
}

void Timer0_Routine() interrupt 1
{
    static unsigned int T0_Count1 = 0, T0_Count2 = 0, T0_Count3 = 0;
    TL0 = 0x18;
    TH0 = 0xFC;
    T0_Count1++;

    if (T0_Count1 >= 20)
    {
        T0_Count1 = 0;
        Key_Loop();
    }

    T0_Count2++;
    if (T0_Count2 >= 2)
    {
        T0_Count2 = 0;
        SEG_Loop();
    }

    T0_Count3++;
    if (T0_Count3 >= 10)
    {
        T0_Count3 = 0;
        Sec_Loop();
    }
}