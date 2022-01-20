#include <REGX52.H>
#include "BasicalFunc.h"

unsigned char KeyNum, MODE, SetTimeMODE, Flash;

void ShowTime(void)
{
    DS1302_ReadTime();
    LCD_ShowNum(1, 1, DS1302_Time[0], 2);
    LCD_ShowNum(1, 4, DS1302_Time[1], 2);
    LCD_ShowNum(1, 7, DS1302_Time[2], 2);
    LCD_ShowNum(2, 1, DS1302_Time[3], 2);
    LCD_ShowNum(2, 4, DS1302_Time[4], 2);
    LCD_ShowNum(2, 7, DS1302_Time[5], 2);
}

void SetTime(void)
{
    if (KeyNum == 2)
    {
        SetTimeMODE++;
        SetTimeMODE %= 6;
    }
    if (KeyNum == 3)
    {
        DS1302_Time[SetTimeMODE]++;
        if (DS1302_Time[0] > 99)
        {
            DS1302_Time[0] = 0;
        }
        if (DS1302_Time[1] > 12)
        {
            DS1302_Time[1] = 1;
        }
        if (DS1302_Time[2] > DS1302_DayOf(DS1302_Time[0], DS1302_Time[1]))
        {
            DS1302_Time[2] = 1;
        }
        if (DS1302_Time[3] > 23)
        {
            DS1302_Time[3] = 0;
        }
        if (DS1302_Time[4] > 59)
        {
            DS1302_Time[4] = 0;
        }
        if (DS1302_Time[5] > 59)
        {
            DS1302_Time[5] = 0;
        }
    }
    if (KeyNum == 4)
    {
        DS1302_Time[SetTimeMODE]--;
        if (DS1302_Time[0] > 100)
        {
            DS1302_Time[0] = 99;
        }
        if (DS1302_Time[1] == 0)
        {
            DS1302_Time[1] = 12;
        }
        if (DS1302_Time[2] > DS1302_DayOf(DS1302_Time[0], DS1302_Time[1]))
        {
            DS1302_Time[2] = DS1302_DayOf(DS1302_Time[0], DS1302_Time[1]);
        }
        if (DS1302_Time[2] == 0)
        {
            DS1302_Time[2] = DS1302_DayOf(DS1302_Time[0], DS1302_Time[1]);
        }
        if (DS1302_Time[3] > 100)
        {
            DS1302_Time[3] = 23;
        }
        if (DS1302_Time[4] > 100)
        {
            DS1302_Time[4] = 59;
        }
        if (DS1302_Time[5] > 100)
        {
            DS1302_Time[5] = 59;
        }
    }
    if (SetTimeMODE == 0 && Flash == 1)
    {
        LCD_ShowString(1, 1, "  ");
    }
    else
        LCD_ShowNum(1, 1, DS1302_Time[0], 2);
    if (SetTimeMODE == 1 && Flash == 1)
    {
        LCD_ShowString(1, 4, "  ");
    }
    else
        LCD_ShowNum(1, 4, DS1302_Time[1], 2);
    if (SetTimeMODE == 2 && Flash == 1)
    {
        LCD_ShowString(1, 7, "  ");
    }
    else
        LCD_ShowNum(1, 7, DS1302_Time[2], 2);
    if (SetTimeMODE == 3 && Flash == 1)
    {
        LCD_ShowString(2, 1, "  ");
    }
    else
        LCD_ShowNum(2, 1, DS1302_Time[3], 2);
    if (SetTimeMODE == 4 && Flash == 1)
    {
        LCD_ShowString(2, 4, "  ");
    }
    else
        LCD_ShowNum(2, 4, DS1302_Time[4], 2);
    if (SetTimeMODE == 5 && Flash == 1)
    {
        LCD_ShowString(2, 7, "  ");
    }
    else
        LCD_ShowNum(2, 7, DS1302_Time[5], 2);
}

void main()
{
    LCD_Init();
    LCD_ShowString(1, 1, "  -  -");
    LCD_ShowString(2, 1, "  :  :");
    DS1302_Init();

    DS1302_SetTime();

    while (1)
    {
        KeyNum = Key();
        if (KeyNum == 1)
        {
            if (MODE == 0)
            {
                MODE = 1;
            }
            else
            {
                MODE = 0;
                DS1302_SetTime();
            }
        }
        switch (MODE)
        {
        case 0:
            SetTimeMODE = 0;
            ShowTime();

            break;
        case 1:
            SetTime();
            break;
        }
    }
}

void Timer0_Routine() interrupt 1
{
    static unsigned int T0_Count = 0;
    TL0 = 0x18;
    TH0 = 0xFC;
    T0_Count++;
    if (T0_Count >= 1000)
    {
        T0_Count = 0;
        Flash = !Flash;
    }
}