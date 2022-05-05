#include <REGX52.H>

/**
 * @brief 定时器器0初始化函数
 * @retval 无
 */
void Timer0_Init(void)		
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		
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

/**
 * @brief 设置计算器
 * 
 * @param Value 计数器参数
 */
void Timer0_SetCounter(unsigned int Value)
{
    TH0 = Value >> 8;
    TL0 = Value % 256;
}

/**
 * @brief 获取计数器
 * 
 * @return unsigned int 计数器数值
 */
unsigned int Timer0_GetCounter(void)
{
    return (TH0 << 8) | TL0;
}

/**
 * @brief 定时器运行控制
 * 
 * @param Flag 1为运行，0为停止
 */
void Timer0_Run(unsigned char Flag)
{
    TR0 = Flag;
}