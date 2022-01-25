#include <REGX52.H>
#include <INTRINS.H>

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