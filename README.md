# C51
This is a project created to learn C51.

~~就不放洋屁了~~

此项目共有1个主要文件夹“BasicalFunc”，以及多个存放项目的文件夹。

*仓库内所有代码均不能直接放入普中科技的开发板中，请使用者自行对照开发板原理图进行更改！*

## BasicalFunc

此文件夹中`BasicalFunc.h`与`BasicalFunc.c`最为重要，包含C51学习过程中个人构建的大多数函数模板，随时更新。
### 结构
目前，基础库一共有7类函数：
1. Delay MCU延时函数
2. SEG 数码管显示函数
3. LCD1602 初始化以及各类显示函数
4. MatrixKey 矩阵键盘按键扫描函数
5. Key 独立按键键码获取函数
6. Timer0 定时器0初始化函数以及其定时中断模板
7. SerialPorts 串口通信初始化与传输函数(部分)
### 使用
使用此库时，需要将`BasicalFunc.h`与`BasicalFunc.c`，两个文件复制到与Keil5项目源代码相同的文件夹中。之后，在Keil5中“Add existing Files……”加入项目。最后在main文件添加`#include "BasicalFunc.h"`即可。
### 函数介绍
#### Delay
通过MCU自身运算，进行的延时，此过程中无法使用MCU进行其他操作。
**定义：**
```C
extern void Delay(unsigned int X_ms); //@12.000MHz Delay(X_ms)
```
**实现：**
```c
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
            while (--j);
        } while (--i);
    }
}

```
#### SEG
分为数码管数字显示数组与显示函数两部分。

**数组定义：**
```c
unsigned char code SEGNum[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
    0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,
    0x00}; // Make an Arry to save the one's complement of SEG
```
**函数声明：**
```c
extern void SEG(unsigned char Location, unsigned char Number); //SEG View (Location, Number)
```
**函数实现：**
```c
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
```
数码管的具体显示，需要通过扫描实现。

##### 更新
全新的数码管扫描函数，可以实现0~65535范围数据在数码管的显示。
**定义：**

```c
extern void SEGScan(unsigned int Data, char ScanMode)
```
**实现：**

```c
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
    {R_MODE
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

```

#### LCD1602
非自编函数，不予以解释。具体见BasicalFunc文件夹内的LCD1602文件。
#### Matrix Key
矩阵键盘扫描程序，获取按键键码。
顺序如下：
|     |     |     |     |
| --- | --- | --- | --- |
| 1   | 2   | 3   | 4   |
| 5   | 6   | 7   | 8   |
| 9   | 10  | 11  | 12  |
| 13  | 14  | 15  | 16  |

**声明：**
```c
extern unsigned char MatrixKey();
```
**实现：**
```c
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
```
#### Key
获取独立按键键码。键码与PCB板标号相同。

**声明：**
```c
extern unsigned char Key();
```
**实现：**
```c
unsigned char Key()
{
    unsigned char KeyNum = 0;

    if(P2_3 == 0)   {Delay(20); while(P2_3 == 0); Delay(20); KeyNum = 1;}
    if(P2_2 == 0)   {Delay(20); while(P2_2 == 0); Delay(20); KeyNum = 2;}
    if(P2_1 == 0)   {Delay(20); while(P2_1 == 0); Delay(20); KeyNum = 3;}
    if(P2_0 == 0)   {Delay(20); while(P2_0 == 0); Delay(20); KeyNum = 4;}

    return KeyNum;
}
```
#### 定时器初始化及其计时模板
通过中断，在MCU外执行计时操作。

**初始化声明：**
```c
extern void Timer0_Init(void);
```
**初始化实现：**
```c
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
```
**1S计时器模板：**
```c
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
```
#### SerialPorts
串口通信设置部分，仅包含初始化与向电脑发送部分，后续将会添加。

**初始化声明：**
```c
extern void Uart_Init(void);
```
**发送声明：**
```c
extern void Uart_SendByte(unsigned char Byte);
```
**初始化实现：**
```c
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
```
此初始化仅针对单片机与电脑相互的串口通信，不保证单向传输时不会发送异常中断，若遇到，删除中断设置即可。
**发送实现：**
```c
void Uart_SendByte(unsigned char Byte)
{
    SBUF = Byte;
    while (TI == 0);
    TI = 0;     //TI软件复位
}
```
**接收模板：**
```c
/*串口控制LED样例*/
void UART_Routine() interrupt 4
{
    if(RI == 1)
    {
        P0 = ~SBUF;
        Uart_SendByte(SBUF);
        RI = 0;         //RI软件复位
    }
}
```