#include <REGX52.H>

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
    EA = 0;
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

    EA = 1;

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
    EA = 0;

    //Delay10us
    i = 4;
	while (--i);

    OneWire_DQ = Bit;

    //Delay50us
    i = 24;
	while (--i);

    OneWire_DQ = 1;

    EA = 1;
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
    EA = 0;

    OneWire_DQ = 0;
    i = 2; // Delay5us
    while (--i);
    OneWire_DQ = 1;
    i = 2; // Delay5us
    while (--i);
    Bit = OneWire_DQ;
    i = 22; //Delay50us
	while (--i);

    EA = 1;

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