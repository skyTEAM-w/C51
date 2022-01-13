#include <REGX52.H>
#include "BasicalFunc.h"

unsigned char KeyNumber, OutView[4] = "----";
unsigned int PassWord, Count = 0, SettedPass = 2004;

void main()
{

    LCD_Init();
    LCD_ShowString(1, 1, "Passwords:");
    while (1)
    {
        KeyNumber = MatrixKey();
        if (KeyNumber)
        {
            if (KeyNumber <= 10) //密码区
            {
                
                if (Count < 4)
                {   OutView[Count] = '*';
                    PassWord *= 10;
                    PassWord += KeyNumber % 10;
                    Count++;
                }
            }
            if (KeyNumber == 11)
            {
                if (PassWord == SettedPass)
                {
                    LCD_Init();
                    LCD_ShowString(1, 1, "Welcome");
                    LCD_ShowString(2, 1, "admin");
                    Delay(20000);
                }
                else
                {
                    LCD_ShowString(1, 14, "ERR");
                    PassWord = 0;
                    *OutView = "----";
                }
                Count = 0;
            }
            if (KeyNumber == 12)
            {
                PassWord = 0;
                Count = 0;
                *OutView = "----";
            }

			
            LCD_ShowString(2, 1, OutView);  
        }
    }
}