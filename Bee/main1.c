#include <REGX52.H>
#include "BasicalFunc.h"

sbit Buzzer = P3 ^ 7;

unsigned char KeyNum;
unsigned int i;

void main()
{
    while (1)
    {
        KeyNum = Key();
        if (KeyNum)
        {
            Buzzer_Time(100);
            SEG(1, KeyNum);
        }
    }
}