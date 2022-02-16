#include <REGX52.H>
#include "Delay.h"

unsigned char Key_Number;

/**
 * @brief 获取独立按键键盘码
 * @retval 键盘号 范围：0~4
 * @return unsigned char 
 */

unsigned char Key()
{
    unsigned char Temp;
    Temp = Key_Number;
    Key_Number = 0;
    return Temp;
}

/**
 * @brief 按键状态查询函数
 * 
 * @return unsigned char 键码
 */
unsigned char Key_Getstate()
{
    unsigned char KeyNum = 0;

    if(P2_3 == 0)   {KeyNum = 1;}
    if(P2_2 == 0)   {KeyNum = 2;}
    if(P2_1 == 0)   {KeyNum = 3;}
    if(P2_0 == 0)   {KeyNum = 4;}

    return KeyNum;
}

/**
 * @brief 按键循环函数
 * 用于定时器扫描按键
 * 
 */
void Key_Loop()
{
    static unsigned char NowState, LastState;
    LastState = NowState;
    NowState = Key_Getstate();
    if (LastState == 1 && NowState == 0)
    {
        Key_Number = 1;
    }
    if (LastState == 2 && NowState == 0)
    {
        Key_Number = 2;
    }
    if (LastState == 3 && NowState == 0)
    {
        Key_Number = 3;
    }
    if (LastState == 4 && NowState == 0)
    {
        Key_Number = 4;
    }
}