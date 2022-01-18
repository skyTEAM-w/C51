#include <REGX52.H>

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