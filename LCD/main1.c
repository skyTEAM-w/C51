#include <REGX52.H>
#include "BasicalFunc.h"
//#include "LCD1602.h"
//#include "Delay.h"

void main()
{
    int Result = 0;
    LCD_Init();
    // LCD_ShowChar(1, 1, 'A');
    // LCD_ShowString(1, 3, "Hello");
    // LCD_ShowNum(2, 1, 1665, 4);
    // LCD_ShowSignedNum(2, 5, -233, 2);
    // LCD_ShowHexNum(1, 1, 0x8A, 2);
    // LCD_ShowBinNum(2, 1, 0xAA, 8);

    //    Result = 1 + 1;
    LCD_ShowNum(1, 1, Result, 3);
    while (1)
    {
        Result++;
        Delay(1000);
        LCD_ShowNum(1, 1, Result, 5);
    }
}