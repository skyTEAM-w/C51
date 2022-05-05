#include <REGX52.H>
#include "Timer0.h"
#include "Int0.h"

unsigned int IR_Time;   //时间
unsigned char IR_State; //状态, 0->空闲, 1->等待, 2->接收与解码

unsigned char IR_Data[4]; //数据缓存
unsigned char IR_pData;

unsigned char IR_DataFlag;   //数据结束标志
unsigned char IR_RepeatFlag; //重复标志
unsigned char IR_Address;    //地址码
unsigned char IR_Command;    //命令

/**
 * @brief 红外接收初始化函数
 * 
 */
void IR_Init(void)
{
    Timer0_Init();
    Int0_Init();
}

/**
 * @brief get方法区
 * 
 */

/**
 * @brief 获取DataFlag
 * 
 * @return unsigned char 
 * @retval IR_DataFlag
 */
unsigned char IR_GetDataFlag(void)
{
    if (IR_DataFlag)
    {
        IR_DataFlag = 0;
        return 1;
    }
    return 0;
}

/**
 * @brief 获取RepeatFlag
 * 
 * @return unsigned char 
 * @retval IR_RepeatFlag
 */
unsigned char IR_GetRepeatFlag(void)
{
    if(IR_RepeatFlag)
    {
        IR_RepeatFlag = 0;
        return 1;
    }
    return 0;
}

/**
 * @brief 获取红外命令
 * 
 * @return unsigned char 
 * @retval IR_Command
 */
unsigned char IR_GetCommand(void)
{
    return IR_Command;
}

/**
 * @brief 获取地址
 * 
 * @return unsigned char 
 * @retval IR_Address
 */
unsigned char IR_GetAddress(void)
{
    return IR_Address;
}

/**
 * @brief 红外外部中断中断
 * 
 */
void Int0_Rountine(void) interrupt 0
{
    if (IR_State == 0)
    {
        Timer0_SetCounter(0);
        Timer0_Run(1);
        IR_State = 1;
    }
    else if (IR_State == 1)
    {
        IR_Time = Timer0_GetCounter();
        Timer0_SetCounter(0);
        if (IR_Time > 12442 - 500 && IR_Time < 12442 + 500)
        {
            IR_State = 2; //解码数据状态
        }
        else if (IR_Time > 10368 - 500 && IR_Time < 10368 + 500)
        {
            IR_RepeatFlag = 1;
            Timer0_Run(0); //计算器停止
            IR_State = 0;  //状态回0
        }
        else
        {
            IR_State = 1;
        }
    }
    else if (IR_State == 2)
    {
        IR_Time = Timer0_GetCounter();
        Timer0_SetCounter(0);
        if (IR_Time > 1032 - 500 && IR_Time < 1032 + 500)
        {
            IR_Data[IR_pData / 8] &= ~(0x01 << (IR_pData % 8));
            IR_pData++;
        }
        else if (IR_Time > 2074 - 500 && IR_Time < 2074 + 500)
        {
            IR_Data[IR_pData / 8] |= (0x01 << (IR_pData % 8));
            IR_pData++;
        }
        else
        {
            //接收错误
            IR_pData = 0;
            IR_State = 1;
        }
        //数据验证
        if (IR_pData >= 32)
        {
            IR_pData = 0;
            if ((IR_Data[0] == ~IR_Data[1]) && (IR_Data[2] == ~IR_Data[3]))
            {
                IR_Address = IR_Data[0];
                IR_Command = IR_Data[2];
                IR_DataFlag = 1;
            }
            Timer0_Run(0);
            IR_State = 0;
        }
    }
}