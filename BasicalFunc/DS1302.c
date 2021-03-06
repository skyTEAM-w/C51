#include <REGX52.H>

#define DS1302_SECOND 0x80
#define DS1302_MINUTE 0x82
#define DS1302_HOUR 0x84
#define DS1302_DATE 0x86
#define DS1302_MONTH 0x88
#define DS1302_DAY 0x8A
#define DS1302_YEAR 0x8C
#define DS1302_WP 0x8E

sbit DS1302_SCLK = P3 ^ 4;
sbit DS1302_IO = P3 ^ 5;
sbit DS1302_CE = P3 ^ 6;

unsigned char DS1302_Time[] = {22, 1, 18, 15, 29, 15, 3};
unsigned char DS1302_MonthTable[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
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
    Command |= 0x01; //将设置的命令转为读取命令
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