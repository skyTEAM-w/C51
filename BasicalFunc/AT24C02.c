#include <REGX52.H>
#include "I2C.h"

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