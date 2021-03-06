/**
 * @file BasicalFunc.c
 * @author WuChengpei_Sky (w2563070231@outlook.com)
 * @brief 人人卓越工程51单片机常用函数库的实现
 * @version 1.0
 * @date 2022-01-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <REGX52.H>
// #include <NomalFunc.h>

/*---------------------------------------
Delay
---------------------------------------*/

/**
 * @brief 延时X毫秒
 *
 * @param X_ms 延迟的毫秒数
 * @retval 无
 */
void Delay(unsigned int X_ms) //@12.000MHz
{
    unsigned char i, j;
    while (X_ms--)
    {
        /* code */
        i = 2;
        j = 239;
        do
        {
            while (--j)
                ;
        } while (--i);
    }
}

/*---------------------------------------
SEG Tube
---------------------------------------*/
unsigned char code SEGNum[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
    0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,
    0x00}; // Make an Arry to save the one's complement of SEG

/**
 * @brief 数码管显示数字
 *
 * @param Location 显示位置
 * @param Number 显示的数字
 * @retval 无
 */
void SEG(unsigned char Location, unsigned char Number)
{ // Use SEG Tube Show Setted Number
    P1 = ~(0x01 << (8 - Location));
    P0 = ~SEGNum[Number];
    Delay(1);
    P0 = 0xFF;
}

/*---------------------------------------
LCD1602
---------------------------------------*/
//引脚配置：
sbit LCD_RS = 0x90;
sbit LCD_RW = 0x91;
sbit LCD_EN = 0x92;
#define LCD_DataPort P0

//函数定义：
/**
 * @brief  LCD1602延时函数，12MHz调用可延时1ms
 * @param  无
 * @retval 无
 */
void LCD_Delay()
{
    unsigned char i, j;

    i = 2;
    j = 239;
    do
    {
        while (--j)
            ;
    } while (--i);
}

/**
 * @brief  LCD1602写命令
 * @param  Command 要写入的命令
 * @retval 无
 */
void LCD_WriteCommand(unsigned char Command)
{
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_DataPort = Command;
    LCD_EN = 1;
    LCD_Delay();
    LCD_EN = 0;
    LCD_Delay();
}

/**
 * @brief  LCD1602写数据
 * @param  Data 要写入的数据
 * @retval 无
 */
void LCD_WriteData(unsigned char Data)
{
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_DataPort = Data;
    LCD_EN = 1;
    LCD_Delay();
    LCD_EN = 0;
    LCD_Delay();
}

/**
 * @brief  LCD1602设置光标位置
 * @param  Line 行位置，范围：1~2
 * @param  Column 列位置，范围：1~16
 * @retval 无
 */
void LCD_SetCursor(unsigned char Line, unsigned char Column)
{
    if (Line == 1)
    {
        LCD_WriteCommand(0x80 | (Column - 1));
    }
    else if (Line == 2)
    {
        LCD_WriteCommand(0x80 | (Column - 1 + 0x40));
    }
}

/**
 * @brief  LCD1602初始化函数
 * @param  无
 * @retval 无
 */
void LCD_Init()
{
    LCD_WriteCommand(0x38); //八位数据接口，两行显示，5*7点阵
    LCD_WriteCommand(0x0c); //显示开，光标关，闪烁关
    LCD_WriteCommand(0x06); //数据读写操作后，光标自动加一，画面不动
    LCD_WriteCommand(0x01); //光标复位，清屏
}

/**
 * @brief  在LCD1602指定位置上显示一个字符
 * @param  Line 行位置，范围：1~2
 * @param  Column 列位置，范围：1~16
 * @param  Char 要显示的字符
 * @retval 无
 */
void LCD_ShowChar(unsigned char Line, unsigned char Column, char Char)
{
    LCD_SetCursor(Line, Column);
    LCD_WriteData(Char);
}

/**
 * @brief  在LCD1602指定位置开始显示所给字符串
 * @param  Line 起始行位置，范围：1~2
 * @param  Column 起始列位置，范围：1~16
 * @param  String 要显示的字符串
 * @retval 无
 */
void LCD_ShowString(unsigned char Line, unsigned char Column, char *String)
{
    unsigned char i;
    LCD_SetCursor(Line, Column);
    for (i = 0; String[i] != '\0'; i++)
    {
        LCD_WriteData(String[i]);
    }
}

/**
 * @brief  返回值=X的Y次方
 */
int LCD_Pow(int X, int Y)
{
    unsigned char i;
    int Result = 1;
    for (i = 0; i < Y; i++)
    {
        Result *= X;
    }
    return Result;
}

/**
 * @brief  在LCD1602指定位置开始显示所给数字
 * @param  Line 起始行位置，范围：1~2
 * @param  Column 起始列位置，范围：1~16
 * @param  Number 要显示的数字，范围：0~65535
 * @param  Length 要显示数字的长度，范围：1~5
 * @retval 无
 */
void LCD_ShowNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length)
{
    unsigned char i;
    LCD_SetCursor(Line, Column);
    for (i = Length; i > 0; i--)
    {
        LCD_WriteData(Number / LCD_Pow(10, i - 1) % 10 + '0');
    }
}

/**
 * @brief  在LCD1602指定位置开始以有符号十进制显示所给数字
 * @param  Line 起始行位置，范围：1~2
 * @param  Column 起始列位置，范围：1~16
 * @param  Number 要显示的数字，范围：-32768~32767
 * @param  Length 要显示数字的长度，范围：1~5
 * @retval 无
 */
void LCD_ShowSignedNum(unsigned char Line, unsigned char Column, int Number, unsigned char Length)
{
    unsigned char i;
    unsigned int Number1;
    LCD_SetCursor(Line, Column);
    if (Number >= 0)
    {
        LCD_WriteData('+');
        Number1 = Number;
    }
    else
    {
        LCD_WriteData('-');
        Number1 = -Number;
    }
    for (i = Length; i > 0; i--)
    {
        LCD_WriteData(Number1 / LCD_Pow(10, i - 1) % 10 + '0');
    }
}

/**
 * @brief  在LCD1602指定位置开始以十六进制显示所给数字
 * @param  Line 起始行位置，范围：1~2
 * @param  Column 起始列位置，范围：1~16
 * @param  Number 要显示的数字，范围：0~0xFFFF
 * @param  Length 要显示数字的长度，范围：1~4
 * @retval 无
 */
void LCD_ShowHexNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length)
{
    unsigned char i, SingleNumber;
    LCD_SetCursor(Line, Column);
    for (i = Length; i > 0; i--)
    {
        SingleNumber = Number / LCD_Pow(16, i - 1) % 16;
        if (SingleNumber < 10)
        {
            LCD_WriteData(SingleNumber + '0');
        }
        else
        {
            LCD_WriteData(SingleNumber - 10 + 'A');
        }
    }
}

/**
 * @brief  在LCD1602指定位置开始以二进制显示所给数字
 * @param  Line 起始行位置，范围：1~2
 * @param  Column 起始列位置，范围：1~16
 * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
 * @param  Length 要显示数字的长度，范围：1~16
 * @retval 无
 */
void LCD_ShowBinNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length)
{
    unsigned char i;
    LCD_SetCursor(Line, Column);
    for (i = Length; i > 0; i--)
    {
        LCD_WriteData(Number / LCD_Pow(2, i - 1) % 2 + '0');
    }
}

/*---------------------------------------
Martix Key
---------------------------------------*/

/**
 * @brief 检测矩阵键盘按键
 * 
 * @return unsigned char 
 * @retval KeyNum 键盘号 松手返回
 */
unsigned char MatrixKey()
{
    unsigned char KeyNum = 0;

    P2 = 0xFF;
    P2_3 = 0;
    if(P2_7 == 0)   {Delay(15); while(P2_7 == 0); Delay(15); KeyNum = 1;}
    if(P2_6 == 0)   {Delay(15); while(P2_6 == 0); Delay(15); KeyNum = 5;}
    if(P2_5 == 0)   {Delay(15); while(P2_5 == 0); Delay(15); KeyNum = 9;}
    if(P2_4 == 0)   {Delay(15); while(P2_4 == 0); Delay(15); KeyNum = 13;}

    P2 = 0xFF;
    P2_2 = 0;
    if(P2_7 == 0)   {Delay(15); while(P2_7 == 0); Delay(15); KeyNum = 2;}
    if(P2_6 == 0)   {Delay(15); while(P2_6 == 0); Delay(15); KeyNum = 6;}
    if(P2_5 == 0)   {Delay(15); while(P2_5 == 0); Delay(15); KeyNum = 10;}
    if(P2_4 == 0)   {Delay(15); while(P2_4 == 0); Delay(15); KeyNum = 14;}

    P2 = 0xFF;
    P2_1 = 0;
    if(P2_7 == 0)   {Delay(15); while(P2_7 == 0); Delay(15); KeyNum = 3;}
    if(P2_6 == 0)   {Delay(15); while(P2_6 == 0); Delay(15); KeyNum = 7;}
    if(P2_5 == 0)   {Delay(15); while(P2_5 == 0); Delay(15); KeyNum = 11;}
    if(P2_4 == 0)   {Delay(15); while(P2_4 == 0); Delay(15); KeyNum = 15;}

    P2 = 0xFF;
    P2_0 = 0;
    if(P2_7 == 0)   {Delay(15); while(P2_7 == 0); Delay(15); KeyNum = 4;}
    if(P2_6 == 0)   {Delay(15); while(P2_6 == 0); Delay(15); KeyNum = 8;}
    if(P2_5 == 0)   {Delay(15); while(P2_5 == 0); Delay(15); KeyNum = 12;}
    if(P2_4 == 0)   {Delay(15); while(P2_4 == 0); Delay(15); KeyNum = 16;}

    return KeyNum;
}