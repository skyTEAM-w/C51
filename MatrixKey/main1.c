#include <REGX52.H>
#include "BasicalFunc.h"

unsigned char KeyNumber;

void main()
{

    LCD_Init();
    LCD_ShowString(1, 1, "MatrixKey");
    while (1)
    {
        KeyNumber = MatrixKey();
        if (KeyNumber)
        {
            LCD_ShowNum(2, 1, KeyNumber, 4);
        }
    }
}