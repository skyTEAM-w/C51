#include <REGX52.H>
#include "Delay.h"

sbit XPT2046_CS = P3 ^ 5;
sbit XPT2046_DCLK = P3 ^ 6;
sbit XPT2046_DIN = P3 ^ 4;
sbit XPT2046_DOUT = P3 ^ 7;

/**
 * @brief XPT2046 AD读取函数
 *
 * @param Command 指令字（具体配置见参考手册）
 * @return unsigned int 等效数字值
 */

unsigned int XPT2046_READ_AD(unsigned char Command)
{
    unsigned char i;
    unsigned int AD_VAL;
    XPT2046_DCLK = 0;
    XPT2046_CS = 0;

    for (i = 0; i < 8; i++)
    {
        XPT2046_DIN = Command & (0x80 >> i);
        XPT2046_DCLK = 1;
        Delay(1);
        XPT2046_DCLK = 0;
    }

    for (i = 0; i < 16; i++)
    {
        XPT2046_DCLK = 1;
        XPT2046_DCLK = 0;
        if (XPT2046_DOUT)
        {
            AD_VAL |= (0x8000 >> i);
        }
    }

    XPT2046_CS = 1;

    if(Command & 0x08) {
        return AD_VAL >> 8;
    }
    else {
        return AD_VAL >> 4;
    }
}