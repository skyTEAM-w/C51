/**
 * @file BasicalFunc.c
 * @author WuChengpei_Sky (w2563070231@outlook.com)
 * @brief 人人卓越工程51单片机常用函数库的实现
 * @version 1.2
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

unsigned char code SEGSign[] = {
    0x7F, 0xBF
};

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

void SEGScan(unsigned char Data, char ScanMode)
{
    unsigned char TempData[8];
    int count = 0;
    switch (ScanMode)
    {
    case 'l':
    case 'L':
    {
        TempData[0] = SEGNum[Data / 1000];
        TempData[1] = SEGNum[(Data % 1000) / 100];
        TempData[2] = SEGNum[((Data % 1000) % 100) / 10];
        TempData[3] = SEGNum[((Data % 1000) % 100) % 10];
        for (count = 0; count < 3 && TempData[count] == 0xc0; count++)
        {
            TempData[count] = 0xff;
        }
        for (count = 1; count <= 4; count++)
        {
            P1 = ~(0x01 << (8 - count));
            P0 = ~TempData[count];
            Delay(1);
            P0 = 0xFF;
        }
        break;
    }
    case 'r':
    case 'R':
    {
        TempData[4] = SEGNum[Data / 1000];
        TempData[5] = SEGNum[(Data % 1000) / 100];
        TempData[6] = SEGNum[((Data % 1000) % 100) / 10];
        TempData[7] = SEGNum[((Data % 1000) % 100) % 10];
        for (count = 4; count < 7 && TempData[count] == 0xc0; count++)
        {
            TempData[count] = 0xff;
        }
        for (count = 4; count <= 7; count++)
        {
            P1 = ~(0x01 << (8 - count));
            P0 = ~TempData[count];
            Delay(1);
            P0 = 0xFF;
        }
        break;
    }
    case 'n':
    case 'N':
    {
        TempData[0] = SEGNum[Data / 1000];
        TempData[1] = SEGNum[(Data % 1000) / 100];
        TempData[2] = SEGNum[((Data % 1000) % 100) / 10];
        TempData[3] = SEGNum[((Data % 1000) % 100) % 10];
        TempData[4] = SEGNum[Data / 1000];
        TempData[5] = SEGNum[(Data % 1000) / 100];
        TempData[6] = SEGNum[((Data % 1000) % 100) / 10];
        TempData[7] = SEGNum[((Data % 1000) % 100) % 10];
        for (count = 0; count < 3 && TempData[count] == 0xc0; count++)
        {
            TempData[count] = 0xff;
        }
        for (count = 4; count < 7 && TempData[count] == 0xc0; count++)
        {
            TempData[count] = 0xff;
        }
        for (count = 1; count <= 8; count++)
        {
            P1 = ~(0x01 << (8 - count));
            P0 = ~TempData[count];
            Delay(1);
            P0 = 0xFF;
        }
        break;
    }
}

///*---------------------------------------
//LCD1602
//---------------------------------------*/
////引脚配置：
//sbit LCD_RS = 0x90;
//sbit LCD_RW = 0x91;
//sbit LCD_EN = 0x92;
//#define LCD_DataPort P0

////函数定义：
///**
// * @brief  LCD1602延时函数，12MHz调用可延时1ms
// * @param  无
// * @retval 无
// */
//void LCD_Delay()
//{
//    unsigned char i, j;

//    i = 2;
//    j = 239;
//    do
//    {
//        while (--j)
//            ;
//    } while (--i);
//}

///**
// * @brief  LCD1602写命令
// * @param  Command 要写入的命令
// * @retval 无
// */
//void LCD_WriteCommand(unsigned char Command)
//{
//    LCD_RS = 0;
//    LCD_RW = 0;
//    LCD_DataPort = Command;
//    LCD_EN = 1;
//    LCD_Delay();
//    LCD_EN = 0;
//    LCD_Delay();
//}

///**
// * @brief  LCD1602写数据
// * @param  Data 要写入的数据
// * @retval 无
// */
//void LCD_WriteData(unsigned char Data)
//{
//    LCD_RS = 1;
//    LCD_RW = 0;
//    LCD_DataPort = Data;
//    LCD_EN = 1;
//    LCD_Delay();
//    LCD_EN = 0;
//    LCD_Delay();
//}

///**
// * @brief  LCD1602设置光标位置
// * @param  Line 行位置，范围：1~2
// * @param  Column 列位置，范围：1~16
// * @retval 无
// */
//void LCD_SetCursor(unsigned char Line, unsigned char Column)
//{
//    if (Line == 1)
//    {
//        LCD_WriteCommand(0x80 | (Column - 1));
//    }
//    else if (Line == 2)
//    {
//        LCD_WriteCommand(0x80 | (Column - 1 + 0x40));
//    }
//}

///**
// * @brief  LCD1602初始化函数
// * @param  无
// * @retval 无
// */
//void LCD_Init()
//{
//    LCD_WriteCommand(0x38); //八位数据接口，两行显示，5*7点阵
//    LCD_WriteCommand(0x0c); //显示开，光标关，闪烁关
//    LCD_WriteCommand(0x06); //数据读写操作后，光标自动加一，画面不动
//    LCD_WriteCommand(0x01); //光标复位，清屏
//}

///**
// * @brief  在LCD1602指定位置上显示一个字符
// * @param  Line 行位置，范围：1~2
// * @param  Column 列位置，范围：1~16
// * @param  Char 要显示的字符
// * @retval 无
// */
//void LCD_ShowChar(unsigned char Line, unsigned char Column, char Char)
//{
//    LCD_SetCursor(Line, Column);
//    LCD_WriteData(Char);
//}

///**
// * @brief  在LCD1602指定位置开始显示所给字符串
// * @param  Line 起始行位置，范围：1~2
// * @param  Column 起始列位置，范围：1~16
// * @param  String 要显示的字符串
// * @retval 无
// */
//void LCD_ShowString(unsigned char Line, unsigned char Column, char *String)
//{
//    unsigned char i;
//    LCD_SetCursor(Line, Column);
//    for (i = 0; String[i] != '\0'; i++)
//    {
//        LCD_WriteData(String[i]);
//    }
//}

///**
// * @brief  返回值=X的Y次方
// */
//int LCD_Pow(int X, int Y)
//{
//    unsigned char i;
//    int Result = 1;
//    for (i = 0; i < Y; i++)
//    {
//        Result *= X;
//    }
//    return Result;
//}

///**
// * @brief  在LCD1602指定位置开始显示所给数字
// * @param  Line 起始行位置，范围：1~2
// * @param  Column 起始列位置，范围：1~16
// * @param  Number 要显示的数字，范围：0~65535
// * @param  Length 要显示数字的长度，范围：1~5
// * @retval 无
// */
//void LCD_ShowNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length)
//{
//    unsigned char i;
//    LCD_SetCursor(Line, Column);
//    for (i = Length; i > 0; i--)
//    {
//        LCD_WriteData(Number / LCD_Pow(10, i - 1) % 10 + '0');
//    }
//}

///**
// * @brief  在LCD1602指定位置开始以有符号十进制显示所给数字
// * @param  Line 起始行位置，范围：1~2
// * @param  Column 起始列位置，范围：1~16
// * @param  Number 要显示的数字，范围：-32768~32767
// * @param  Length 要显示数字的长度，范围：1~5
// * @retval 无
// */
//void LCD_ShowSignedNum(unsigned char Line, unsigned char Column, int Number, unsigned char Length)
//{
//    unsigned char i;
//    unsigned int Number1;
//    LCD_SetCursor(Line, Column);
//    if (Number >= 0)
//    {
//        LCD_WriteData('+');
//        Number1 = Number;
//    }
//    else
//    {
//        LCD_WriteData('-');
//        Number1 = -Number;
//    }
//    for (i = Length; i > 0; i--)
//    {
//        LCD_WriteData(Number1 / LCD_Pow(10, i - 1) % 10 + '0');
//    }
//}

///**
// * @brief  在LCD1602指定位置开始以十六进制显示所给数字
// * @param  Line 起始行位置，范围：1~2
// * @param  Column 起始列位置，范围：1~16
// * @param  Number 要显示的数字，范围：0~0xFFFF
// * @param  Length 要显示数字的长度，范围：1~4
// * @retval 无
// */
//void LCD_ShowHexNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length)
//{
//    unsigned char i, SingleNumber;
//    LCD_SetCursor(Line, Column);
//    for (i = Length; i > 0; i--)
//    {
//        SingleNumber = Number / LCD_Pow(16, i - 1) % 16;
//        if (SingleNumber < 10)
//        {
//            LCD_WriteData(SingleNumber + '0');
//        }
//        else
//        {
//            LCD_WriteData(SingleNumber - 10 + 'A');
//        }
//    }
//}

///**
// * @brief  在LCD1602指定位置开始以二进制显示所给数字
// * @param  Line 起始行位置，范围：1~2
// * @param  Column 起始列位置，范围：1~16
// * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
// * @param  Length 要显示数字的长度，范围：1~16
// * @retval 无
// */
//void LCD_ShowBinNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length)
//{
//    unsigned char i;
//    LCD_SetCursor(Line, Column);
//    for (i = Length; i > 0; i--)
//    {
//        LCD_WriteData(Number / LCD_Pow(2, i - 1) % 2 + '0');
//    }
//}

/*---------------------------------------
Matrix Key
---------------------------------------*/

/**
 * @brief 检测矩阵键盘按键
 * 
 * @return unsigned char 
 * @retval KeyNum 键盘号
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

/*---------------------------------------
Key
---------------------------------------*/

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

/*---------------------------------------
Timer0 Init
---------------------------------------*/
/**
 * @brief 定时器器0函数初始化函数，12MHz
 * @retval 无
 */
void Timer0_Init(void)		//1毫秒@12.000MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
    ET0 = 1;        //设置中断标志
    EA = 1;
    PT0 = 0;
}
/*
1s计时器中断函数模板

void Timer0_Routine() interrupt 1
{
    static unsigned int T0_Count = 0;
    TL0 = 0x18;
    TH0 = 0xFC;
    T0_Count++;
    if (T0_Count >= 1000)
    {
        T0_Count = 0;
        P0_0 = ~P0_0;
    }
}
*/

/*---------------------------------------
Serial Ports
---------------------------------------*/

/**
 * @brief 初始化串口(4800bps)
 * 
 */

void Uart_Init(void)		//4800bps@12.000MHz
{
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xF3;		//设置定时初始值
	TH1 = 0xF3;		//设置定时重载值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//定时器1开始计时
    EA = 1;         //中断设置
    ES = 1;
}

// /**
//  * @brief 初始化串口(可接收)
//  * 
//  * @param MODE 重载时标志参数
//  */
// void Uart_Init(unsigned int MODE)
// {
//     PCON |= 0x80;		//使能波特率倍速位SMOD
// 	SCON = 0x50;		//8位数据,可变波特率
// 	TMOD &= 0x0F;		//设置定时器模式
// 	TMOD |= 0x20;		//设置定时器模式
// 	TL1 = 0xF3;		//设置定时初始值
// 	TH1 = 0xF3;		//设置定时重载值
// 	ET1 = 0;		//禁止定时器1中断
// 	TR1 = 1;		//定时器1开始计时
//     EA = 1;
//     ES = 1;
// }

/**
 * @brief 向电脑发送数据
 * 
 * @param Byte 所发送的1Byte数据
 * @retval 无
 */
void Uart_SendByte(unsigned char Byte)
{
    SBUF = Byte;
    while (TI == 0);
    TI = 0;     //TI软件复位
}

/*串口控制LED样例
void UART_Routine() interrupt 4
{
    if(RI == 1)
    {
        P0 = ~SBUF;
        Uart_SendByte(SBUF);
        RI = 0;         //RI软件复位
    }
}
*/