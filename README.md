# 51单片机

This is a project created to learn 51.

~~就不放洋屁了~~

此项目共有1个主要文件夹“BasicalFunc”，以及多个存放项目的文件夹。

*仓库内所有代码均不能直接放入普中科技的开发板中，请使用者自行对照开发板原理图进行更改！*

## 更新

### 2022 -5-1

AD数模转换。

详见：[BasicalFunc->AD数模转换](#ad-xpt2016-数模转换函数)

### 2022-4-30

PWM，LED与电机调速。

代码与`BasicalFunc`方案独立，单独存在于`PWM`文件夹内，包含定时器PWM与软件PWM。

详见：[PWM脉冲宽度调制](#pwm脉冲宽度调制)

*此后更新方案将不再适用于人人卓越单片机开发板*

### 2022-2-10

DS18B20温度传感器，实现温度的读取。

详见：[BasicalFunc->DS18B20](#ds18b20-温度传感相关函数).

*2022-2-16新增：*

优化了当使用定时器扫描数码管与按键时单总线操作出错的问题。

此时，不建议使用定时器进行精密操作。

### 2022-2-2

新的定时器扫描按键与数码管函数。

详见：[BasicalFunc->SEG](#seg)与[BasicalFunc->Key](#key).

## BasicalFunc

此文件夹中`BasicalFunc.h`与`BasicalFunc.c`最为重要，包含C51学习过程中个人构建的大多数函数模板，2022-1-29之后停止更新。

函数的具体操作，在`README.md`中有详细的展示。

*注意：`BasicalFunc.h`与`BasicalFunc.c`文件在2022-1-29之后不再作为主要函数库使用，仅作为集成参考，具体使用以各类函数库为准*

目前已将各方法分到各同名文件中，方便查找，使用方法与前面一样。

### 结构

目前，基础库一共有7类函数：

1. Delay MCU延时函数
2. SEG 数码管显示函数
3. LCD1602 初始化以及各类显示函数
4. MatrixKey 矩阵键盘按键扫描函数
5. Key 独立按键键码获取函数
6. Timer0 定时器0初始化函数以及其定时中断模板
7. SerialPorts 串口通信初始化与传输函数
8. DS1302 时钟相关函数(部分)
9. Buzzer 蜂鸣器相关函数
10. AT24C02 存储相关函数
11. DS18B20 温度传感相关函数
12. AD XPT2046 数模转换函数

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

显示函数分定时器扫描与`Delay`扫描两种。

**数组定义：**

```c
// Make an Arry to save the one's complement of SEG
unsigned char code SEG_Table[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
    0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,
    0x00, 0x40}; 

//未使用数组
unsigned char code SEGSign[] = {
    0x7F, 0xBF};

//数码管缓存区表
unsigned char SEG_Buf[9] = {0, 16, 16, 16, 16, 16, 16, 16, 16};

```

**函数声明：**

```c
void SEG(unsigned char Location, unsigned char Number); //SEG View (Location, Number)
void SEG_Loop();
void SEG_SetBuf(unsigned char Location, unsigned char Number);
```

**定时器扫描函数实现：**

```c
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
```

数码管的具体显示，需要通过扫描实现。

**`Delay`扫描函数**

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
extern void Key_Loop();
```

**实现：**

```c
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
void Timer0_Init(void)  //1毫秒@12.000MHz
{
    TMOD &= 0xF0;  //设置定时器模式
    TMOD |= 0x01;  //设置定时器模式
    TL0 = 0x18;  //设置定时初始值
    TH0 = 0xFC;  //设置定时初始值
    TF0 = 0;  //清除TF0标志
    TR0 = 1;  //定时器0开始计时
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
void Uart_Init(void)  //4800bps@12.000MHz
{
    PCON |= 0x80;  //使能波特率倍速位SMOD
    SCON = 0x50;  //8位数据,可变波特率
    TMOD &= 0x0F;  //设置定时器模式
    TMOD |= 0x20;  //设置定时器模式
    TL1 = 0xF3;  //设置定时初始值
    TH1 = 0xF3;  //设置定时重载值
    ET1 = 0;  //禁止定时器1中断
    TR1 = 1;  //定时器1开始计时
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

#### DS1302

DS1302有关函数。

**预处理部分：**

```c
#define DS1302_SECOND   0x80
#define DS1302_MINUTE   0x82
#define DS1302_HOUR     0x84
#define DS1302_DATE     0x86
#define DS1302_MONTH    0x88
#define DS1302_DAY      0x8A
#define DS1302_YEAR     0x8C
#define DS1302_WP       0x8E
```

根据DS1302手册定义的所有数据的读入指令表。

**数据存储部分：**

```c
unsigned char DS1302_Time[] = {22, 1, 18, 15, 29, 15, 3};
```

根据编写时间设置的初始时间表。

**函数集体声明部分：**

```c
extern void DS1302_Init();  //初始化
extern unsigned char DS1302_ReadByte(unsigned char Command);    //从DS1302读入数据
extern void DS1302_WriteByte(unsigned char Command, unsigned char Data);    //向DS1302写入数据
extern void DS1302_SetTime();   //设置时间
extern void DS1302_ReadTime();  //读取时间
extern unsigned char DS1302_DayOf(unsigned char Year, unsigned char Month); //某年某月日期数的判定
```

**函数的集体实现：**

包含了两个BCD与十进制互化的函数，不能外部调用。

```c
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
 * @param Year 年
 * @param Month 月
 * @return unsigned char 
 */
unsigned char DS1302_DayOf(unsigned char Year, unsigned char Month)
{
    unsigned char year = Year + 2000;
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
    {
        return Month == 2 ? 29 : DS1302_MonthTable[Month - 1];
    }
    return DS1302_MonthTable[Month - 1];
}
```

#### Buzzer

此类函数用于控制蜂鸣器发声

**可用的外部函数：**

```c
void Buzzer_Time(unsigned int _ms);
```

**内部实现：**

```c
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
```

#### AT24C02 存储相关函数

用于数据在AT24C02上的读写。

**I2C总线部分：**

定义：

```c
void I2C_Start();
void I2C_Stop();
void I2C_SendByte(unsigned char Byte);
unsigned char I2C_ReceiveByte();
void I2C_SendAck(unsigned char AckBit);
unsigned char I2C_ReceiveAck();

```

根据时序图，进行各函数的编写。

实现：

```c
sbit I2C_SCL = P3 ^ 2;
sbit I2C_SDA = P3 ^ 3;

/**
 * @brief  I2C开始
 * @param  无
 * @retval 无
 */
void I2C_Start(void)
{
    I2C_SDA = 1;
    I2C_SCL = 1;
    I2C_SDA = 0;
    I2C_SCL = 0;
}

/**
 * @brief  I2C停止
 * @param  无
 * @retval 无
 */
void I2C_Stop(void)
{
    I2C_SDA = 0;
    I2C_SCL = 1;
    I2C_SDA = 1;
}

/**
 * @brief  I2C发送一个字节
 * @param  Byte 要发送的字节
 * @retval 无
 */
void I2C_SendByte(unsigned char Byte)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        I2C_SDA = Byte & (0x80 >> i);
        I2C_SCL = 1;
        I2C_SCL = 0;
    }
}

/**
 * @brief  I2C接收一个字节
 * @param  无
 * @retval 接收到的一个字节数据
 */
unsigned char I2C_ReceiveByte(void)
{
    unsigned char i, Byte = 0x00;
    I2C_SDA = 1;
    for (i = 0; i < 8; i++)
    {
        I2C_SCL = 1;
        if (I2C_SDA)
        {
            Byte |= (0x80 >> i);
        }
        I2C_SCL = 0;
    }
    return Byte;
}

/**
 * @brief  I2C发送应答
 * @param  AckBit 应答位，0为应答，1为非应答
 * @retval 无
 */
void I2C_SendAck(unsigned char AckBit)
{
    I2C_SDA = AckBit;
    I2C_SCL = 1;
    I2C_SCL = 0;
}

/**
 * @brief  I2C接收应答位
 * @param  无
 * @retval 接收到的应答位，0为应答，1为非应答
 */
unsigned char I2C_ReceiveAck(void)
{
    unsigned char AckBit;
    I2C_SDA = 1;
    I2C_SCL = 1;
    AckBit = I2C_SDA;
    I2C_SCL = 0;
    return AckBit;
}

```

**AT24C02部分：**

此部分相当于对I2C部分的继承，可用C++进行实现。

定义：

```c
void AT24C02_WriteByte(unsigned char WordAddress, unsigned char Data);
unsigned char AT24C02_ReadByte(unsigned char WordAddress);
```

根据I2C与时序图，编排成读写函数。

实现：

```c
#define AT24C02_ADDRESS 0xA0

/**
 * @brief AT24C02写入一个字节
 * 
 * @param WordAddress 地址
 * @param Data 数据
 */
void AT24C02_WriteByte(unsigned char WordAddress, unsigned char Data)
{
    I2C_Start();
    I2C_SendByte(AT24C02_ADDRESS);
    I2C_ReceiveAck();
    I2C_SendByte(WordAddress);
    I2C_ReceiveAck();
    I2C_SendByte(Data);
    I2C_ReceiveAck();
    I2C_Stop();
}

/**
 * @brief AT24C02读取一个字节
 * 
 * @param WordAddress 地址
 * @return unsigned char 所读取的数据
 */
unsigned char AT24C02_ReadByte(unsigned char WordAddress)
{
    unsigned char Data;
    I2C_Start();
    I2C_SendByte(AT24C02_ADDRESS);
    I2C_ReceiveAck();
    I2C_SendByte(WordAddress);
    I2C_ReceiveAck();
    I2C_Start();
    I2C_SendByte(AT24C02_ADDRESS | 0x01);
    I2C_ReceiveAck();
    Data = I2C_ReceiveByte();
    I2C_SendAck(1);
    I2C_Stop();
    return Data;
}
```

#### DS18B20 温度传感相关函数

**单总线 1-Wire 部分：**

根据时序要求编写基本的初始化与接收发送函数

定义：

```c
unsigned char OneWire_Init();
void OneWire_SendBit(unsigned char Bit);
unsigned char OneWire_ReceiveBit();
void OneWire_SendByte(unsigned char Byte);
unsigned char OneWire_ReceiveByte();
```

实现：

```c
sbit OneWire_DQ = P3 ^ 7;

/**
 * @brief 单总线初始化函数
 * 
 * @return unsigned char 
 * @retval 0为初始化成功，1为失败
 */
unsigned char OneWire_Init()
{
    unsigned char i;
    unsigned char AckBit;
    OneWire_DQ = 1;
    OneWire_DQ = 0;

    //Delay500us
	i = 247;
	while (--i);

    OneWire_DQ = 1;

    //Delay70us
    i = 32;
	while (--i);

    AckBit = OneWire_DQ;

    //Delay500us
	i = 247;
	while (--i);

    return AckBit;
}

/**
 * @brief 单总线发送一位
 * 
 * @param Bit 发送的位
 */
void OneWire_SendBit(unsigned char Bit)
{
    unsigned char i = 0;
    OneWire_DQ = 0;

    //Delay10us
    i = 4;
	while (--i);

    OneWire_DQ = Bit;

    //Delay50us
    i = 24;
	while (--i);

    OneWire_DQ = 1;
}

/**
 * @brief 单总线接收一位
 * 
 * @return unsigned char 接收的一位 
 */
unsigned char OneWire_ReceiveBit()
{
    unsigned char Bit;
    unsigned char i = 0;

    OneWire_DQ = 0;
    i = 2; // Delay5us
    while (--i);
    OneWire_DQ = 1;
    i = 2; // Delay5us
    while (--i);
    Bit = OneWire_DQ;
    i = 22; //Delay50us
	while (--i);

    return Bit;
}

/**
 * @brief 单总线发送一个字节
 * 
 * @param Byte 发送的一个字节
 */
void OneWire_SendByte(unsigned char Byte)
{
    unsigned char i;
    for(i = 0; i < 8; i++)
    {
        OneWire_SendBit(Byte & (0x01 << i));
    }
}

/**
 * @brief 单总线接收一个字节
 * 
 * @return unsigned char 接收的一个字节
 */
unsigned char OneWire_ReceiveByte()
{
    unsigned char Byte = 0x00;
    unsigned char i;

    for(i = 0; i < 8; i++)
    {
        if(OneWire_ReceiveBit())
        {
            Byte |= 0x01 << i;
        }
    }

    return Byte;
}
```

**DS18B20部分：**

根据时序进行组装。

定义：

```c
void DS18B20_ConvertT();
float DS18B20_ReadT();
```

实现：

```c
#define DS18B20_SKIP_ROM        0xCC
#define DS18B20_CONVERT_T       0x44
#define DS18B20_READ_SCRATCHPAD 0xBE

/**
 * @brief DS18B20温度变换函数
 * 
 */
void DS18B20_ConvertT()
{
    OneWire_Init();
    OneWire_SendByte(DS18B20_SKIP_ROM);
    OneWire_SendByte(DS18B20_CONVERT_T);
}

/**
 * @brief DS18B20温度读取函数
 * 
 * @return float 温度
 */
float DS18B20_ReadT()
{
    unsigned char TLSB, TMSB;
    int Temp;
    float T;
    OneWire_Init();
    OneWire_SendByte(DS18B20_SKIP_ROM);
    OneWire_SendByte(DS18B20_READ_SCRATCHPAD);
    TLSB = OneWire_ReceiveByte();
    TMSB = OneWire_ReceiveByte();

    Temp = (TMSB << 8) | TLSB;

    T = Temp / 16.0;

    return T;
}
```

#### AD XPT2016 数模转换函数

**声明：**

```c
#define XPT2046_XP_8      0x9C
#define XPT2046_YP_8      0xDC
#define XPT2046_VBAT_8    0xAC
#define XPT2046_AUX_8     0xEC

#define XPT2046_XP_12      0x94
#define XPT2046_YP_12      0xD4
#define XPT2046_VBAT_12    0xA4
#define XPT2046_AUX_12     0xE4

unsigned int XPT2046_READ_AD(unsigned char Command);
```

**实现：**

```c
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
```

## 关于定时器扫描数码管与按键

**优势：** 使用定时器扫描，可以实现更加复杂的任务，不用占用MCU资源。

### 方式

定义好循环函数，在定时器内进行执行，可以自动设定循环时间。

**例如：**

```c
void Timer0_Routine() interrupt 1
{
    static unsigned int T0_Count1 = 0, T0_Count2 = 0, T0_Count3 = 0;
    TL0 = 0x18;
    TH0 = 0xFC;
    T0_Count1++;

    if (T0_Count1 >= 20)
    {
        T0_Count1 = 0;
        Key_Loop();
    }

    T0_Count2++;
    if (T0_Count2 >= 2)
    {
        T0_Count2 = 0;
        SEG_Loop();
    }

    T0_Count3++;
    if (T0_Count3 >= 10)
    {
        T0_Count3 = 0;
        Sec_Loop();
    }
}
```

## 关于单总线与DS18B20温度传感器

用于DS18B20温度传感器与STC89C52的通信。

## PWM脉冲宽度调制


**软件调制：**

```c
for (Time = 0; Time < 100; Time++)
        {
            for (i = 0; i < 20; i++)
            {
                LED = 0;
                Delay(Time);
                LED = 1;
                Delay(100 - Time);
            }
        }
        for (Time = 100; Time > 0; Time--)
        {
            for (i = 0; i < 20; i++)
            {
                LED = 0;
                Delay(Time);
                LED = 1;
                Delay(100 - Time);
            }
        }
```

**硬件调制：**

```c
/*
定时器中断PWM
@12MHz 100us
*/
void Timer0_Routine() interrupt 1
{
    static unsigned int T0Count;
    TL0 = 0x9C; //设置定时初值
    TH0 = 0xFF; //设置定时初值
    T0Count++;
    Counter++;
    Counter %= 100;
    if (Counter < Compare)
    {
        LED = 0;
        Motor = 1;
    }
    else
    {
        LED = 1;
        Motor = 0;
    }
}
```
