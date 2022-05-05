#include <REGX52.H>

void Int0_Init(void)
{
    IT0 = 1; //下降沿触发
    IE0 = 0;
    EX0 = 1;
    EA = 1;
    PX0 = 1; //防止解码误判
}

/*
中断模板
void Int0_Routine(void) interrupt 0
{
    Num++;
}
*/