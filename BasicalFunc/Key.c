#include <REGX52.H>
#include "Delay.h"

/**
 * @brief 获取独立按键键盘码
 * @retval 键盘号 范围：0~4
 * @return unsigned char 
 */
unsigned char Key()
{
    unsigned char KeyNum = 0;

    if(P2_3 == 0)   {Delay(20); while(P2_3 == 0); Delay(20); KeyNum = 1;}
    if(P2_2 == 0)   {Delay(20); while(P2_2 == 0); Delay(20); KeyNum = 2;}
    if(P2_1 == 0)   {Delay(20); while(P2_1 == 0); Delay(20); KeyNum = 3;}
    if(P2_0 == 0)   {Delay(20); while(P2_0 == 0); Delay(20); KeyNum = 4;}

    return KeyNum;
}
