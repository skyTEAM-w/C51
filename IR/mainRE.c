#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "IR.h"

unsigned char Num = 0;
unsigned char Address, Command;

void main()
{
    LCD_Init();
    LCD_ShowString(1, 1, "ADD COM NUM");
    IR_Init();

    while (1)
    {
        // LCD_ShowNum(2, 1, Num, 3);
        if (IR_GetDataFlag() || IR_GetRepeatFlag())
        {
            Address = IR_GetAddress();
            Command = IR_GetCommand();

            if (Command == 0x15)
            {
                Num--;
            }
            else if (Command == 0x09)
            {
                Num++;
            }
        }
        LCD_ShowHexNum(2, 1, Address, 2);
        LCD_ShowHexNum(2, 5, Command, 2);
        LCD_ShowNum(2, 9, Num, 3);
    }
}

// void Int0_Routine(void) interrupt 0
// {
//     Num--;
// }