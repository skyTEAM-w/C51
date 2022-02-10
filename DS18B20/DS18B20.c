#include <REGX52.H>
#include "OneWire.h"

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