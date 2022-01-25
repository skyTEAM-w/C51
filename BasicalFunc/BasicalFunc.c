/**
 * @file BasicalFunc.c
 * @author WuChengpei_Sky (w2563070231@outlook.com)
 * @brief 人人卓越工程51单片机常用函数库的实现
 * @version 1.3
 * @date 2022-01-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <REGX52.H>
#include <INTRINS.H>

#define DS1302_SECOND   0x80
#define DS1302_MINUTE   0x82
#define DS1302_HOUR     0x84
#define DS1302_DATE     0x86
#define DS1302_MONTH    0x88
#define DS1302_DAY      0x8A
#define DS1302_YEAR     0x8C
#define DS1302_WP       0x8E
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
        TempData[0] = SEGNum[Data / 1000];
        TempData[1] = SEGNum[(Data % 1000) / 100];
        TempData[2] = SEGNum[((Data % 1000) % 100) / 10];
        TempData[3] = SEGNum[((Data % 1000) % 100) % 10];
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
        TempData[4] = SEGNum[Data / 1000];
        TempData[5] = SEGNum[(Data % 1000) / 100];
        TempData[6] = SEGNum[((Data % 1000) % 100) / 10];
        TempData[7] = SEGNum[((Data % 1000) % 100) % 10];
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
            TempData[count] = SEGNum[Data % Mode];
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

/*---------------------------------------
DS1302
---------------------------------------*/
sbit DS1302_SCLK = P3 ^ 4;
sbit DS1302_IO = P3 ^ 5;
sbit DS1302_CE = P3 ^ 6;

unsigned char DS1302_Time[] = {22, 1, 18, 15, 29, 15, 3};

/**
 * @brief DS1302初始化函数
 * 
 */
void DS1302_Init()
{
    DS1302_CE = 0;
    DS1302_SCLK = 0;
}

/**
 * @brief 十进制转BCD码函数
 * 
 * @param Dec 需要转换的十进制数
 * @return unsigned char 
 * @retval BCD码
 */
unsigned char DS1302_toBCD(unsigned char Dec)
{
    return Dec / 10 * 16 + Dec % 10;
}

/**
 * @brief BCD转十进制函数
 * 
 * @param Hex 需要转换的BCD码
 * @return unsigned char 
 * @retval 十进制数
 */
unsigned char DS1302_toDec(unsigned char Hex)
{
    return Hex / 16 * 10 + Hex % 16;
}

/**
 * @brief 向DS1302写入数据
 * 
 * @param Command 写入指令
 * @param Data 写入的数据
 * @retval 无
 */
void DS1302_WriteByte(unsigned char Command, unsigned char Data)
{
    unsigned char i;
    DS1302_CE = 1;

    for (i = 0; i < 8; i++) //写入指令
    {
        DS1302_IO = Command & (0x01 << i);
        DS1302_SCLK = 1;
        DS1302_SCLK = 0;
    }

    for (i = 0; i < 8; i++) //写入数据
    {
        DS1302_IO = Data & (0x01 << i);
        DS1302_SCLK = 1;
        DS1302_SCLK = 0;
    }

    DS1302_CE = 0;
}

/**
 * @brief 从DS1302读入数据
 * 
 * @param Command 读出命令
 * @return unsigned char 
 * @retval 无
 */
unsigned char DS1302_ReadByte(unsigned char Command)
{
    unsigned char i, Data = 0x00;
    Command |= 0x01;    //将设置的命令转为读取命令
    DS1302_CE = 1;

    for (i = 0; i < 8; i++) //写入指令
    {
        DS1302_IO = Command & (0x01 << i);
        DS1302_SCLK = 0;
        DS1302_SCLK = 1;
    }

    for (i = 0; i < 8; i++)
    {
        DS1302_SCLK = 1;
        DS1302_SCLK = 0;
        if (DS1302_IO)
        {
            Data |= 0x01 << i;
        }
    }
    DS1302_CE = 0;
    return Data;
}

/**
 * @brief 设置时间函数
 * 
 */
void DS1302_SetTime()
{
    DS1302_WriteByte(DS1302_WP, 0x00);
    DS1302_WriteByte(DS1302_YEAR, DS1302_toBCD(DS1302_Time[0]));
    DS1302_WriteByte(DS1302_MONTH, DS1302_toBCD(DS1302_Time[1]));
    DS1302_WriteByte(DS1302_DATE, DS1302_toBCD(DS1302_Time[2]));
    DS1302_WriteByte(DS1302_HOUR, DS1302_toBCD(DS1302_Time[3]));
    DS1302_WriteByte(DS1302_MINUTE, DS1302_toBCD(DS1302_Time[4]));
    DS1302_WriteByte(DS1302_SECOND, DS1302_toBCD(DS1302_Time[5]));
    DS1302_WriteByte(DS1302_DATE, DS1302_toBCD(DS1302_Time[6]));
    DS1302_WriteByte(DS1302_WP, 0x80);
}

/**
 * @brief 读取时间函数
 * 
 */
void DS1302_ReadTime()
{
    DS1302_Time[0] = DS1302_toDec(DS1302_ReadByte(DS1302_YEAR));
    DS1302_Time[1] = DS1302_toDec(DS1302_ReadByte(DS1302_MONTH));
    DS1302_Time[2] = DS1302_toDec(DS1302_ReadByte(DS1302_DATE));
    DS1302_Time[3] = DS1302_toDec(DS1302_ReadByte(DS1302_HOUR));
    DS1302_Time[4] = DS1302_toDec(DS1302_ReadByte(DS1302_MINUTE));
    DS1302_Time[5] = DS1302_toDec(DS1302_ReadByte(DS1302_SECOND));
    DS1302_Time[6] = DS1302_toDec(DS1302_ReadByte(DS1302_DAY));
}

/**
 * @brief 某年某月的天数
 * 
 * @param Year 
 * @param Month 
 * @return unsigned char 
 */

unsigned char DS1302_MonthTable[] = {31, 28, 31, 30, 51, 30, 31, 31, 30, 31, 30, 31};
unsigned char DS1302_DayOf(unsigned char Year, unsigned char Month)
{
    unsigned char year = Year + 2000;
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
    {
        return Month == 2 ? 29 : DS1302_MonthTable[Month - 1];
    }
    return DS1302_MonthTable[Month - 1];
}

/*---------------------------------------
Buzzer
---------------------------------------*/

sbit Buzzer = P3 ^ 7;

/**
 * @brief 蜂鸣器延时函数
 * 
 */
void Buzzer_Delay500us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	i = 247;
	while (--i);
}


/**
 * @brief 蜂鸣器响X毫秒
 * 
 * @param _ms 毫秒数
 */
void Buzzer_Time(unsigned int _ms)
{
    unsigned char i = 0;
    for (i = 0; i < _ms; i++)
    {
        Buzzer = !Buzzer;
        Buzzer_Delay500us();
    }
}