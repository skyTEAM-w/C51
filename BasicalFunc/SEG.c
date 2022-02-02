#include <REGX52.H>
#include "Delay.h"

/*---------------------------------------
SEG Tube
---------------------------------------*/

// Make an Arry to save the one's complement of SEG
unsigned char code SEG_Table[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
    0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,
    0x00, 0x40}; 

unsigned char code SEGSign[] = {
    0x7F, 0xBF};

//数码管缓存区表
unsigned char SEG_Buf[9] = {0, 16, 16, 16, 16, 16, 16, 16, 16};

/**
 * @brief 缓存区设置函数
 * 
 * @param Location 显示位置
 * @param Number 数字
 */
void SEG_SetBuf(unsigned char Location, unsigned char Number)
{
    SEG_Buf[Location] = Number;
}

/**
 * @brief 数码管显示数字
 *
 * @param Location 显示位置
 * @param Number 显示的数字
 * @retval 无
 */
void SEG(unsigned char Location, unsigned char Number)
{ // Use SEG Tube Show Setted Number
    P0 = 0xFF;
    P1 = ~(0x01 << (8 - Location));
    P0 = ~SEG_Table[Number];
    
}

/**
 * @brief 数码管扫描函数，最大可在数码管中显示0~65535范围内的整数。
 * 格式：右对齐
 *
 * @param Data 显示的数据 范围：
 * L_MODE与R_MODE：0~9999
 * N_MODE:0~65535
 * @param ScanMode 输出模式：
 * 'L''l'：左侧数码管输出
 * 'R''r'：右侧数码管输出
 * 'N''n'：双侧数码管共同输出
 */
void SEGScan(unsigned int Data, char ScanMode)
{
    unsigned char TempData[8];
    int count = 0;
    switch (ScanMode)
    {
    case 'l':
    case 'L':
    {
        TempData[0] = SEG_Table[Data / 1000];
        TempData[1] = SEG_Table[(Data % 1000) / 100];
        TempData[2] = SEG_Table[((Data % 1000) % 100) / 10];
        TempData[3] = SEG_Table[((Data % 1000) % 100) % 10];
        for (count = 0; count < 3 && TempData[count] == 0x3F; count++)
        {
            TempData[count] = 0x00;
        }
        for (count = 1; count <= 4; count++)
        {
            P1 = ~(0x01 << (8 - count));
            P0 = ~TempData[count - 1];
            Delay(1);
            P0 = 0xFF;
        }
        break;
    }
    case 'r':
    case 'R':
    {
        TempData[4] = SEG_Table[Data / 1000];
        TempData[5] = SEG_Table[(Data % 1000) / 100];
        TempData[6] = SEG_Table[((Data % 1000) % 100) / 10];
        TempData[7] = SEG_Table[((Data % 1000) % 100) % 10];
        for (count = 4; count < 7 && TempData[count] == 0x3F; count++)
        {
            TempData[count] = 0x00;
        }
        for (count = 4; count <= 8; count++)
        {
            P1 = ~(0x01 << (8 - count));
            P0 = ~TempData[count - 1];
            Delay(1);
            P0 = 0xFF;
        }
        break;
    }
    case 'n':
    case 'N':
    {
        unsigned int Mode = 10;
        for (count = 7; count >= 0; count--)
        {
            TempData[count] = SEG_Table[Data % Mode];
            Data = Data / 10;
        }
        for (count = 0; count < 4 && TempData[count] == 0x3F; count++)
        {
            TempData[count] = 0x00;
        }
        for (count = 4; count < 7 && TempData[count] == 0x3F; count++)
        {
            TempData[count] = 0x00;
        }
        for (count = 1; count <= 8; count++)
        {
            P1 = ~(0x01 << (8 - count));
            P0 = ~TempData[count - 1];
            Delay(1);
            P0 = 0xFF;
        }
        break;
    }
    }
}

/**
 * @brief 数码管定时器扫描函数
 * 
 */
void SEG_Loop()
{
    static unsigned char i = 1;
    SEG(i, SEG_Buf[i]);
    i++;
    if (i >= 9)
        i = 1;
}