/**
 * @file BasicalFunc.h
 * @author WuChengpei_Sky (w2563070231@outlook.com)
 * @brief 人人卓越工程51单片机常用函数库
 * @version 1.1
 * @date 2022-01-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __BASICALFUNC_H__
#define __BASICALFUNC_H__

extern void Delay(unsigned int X_ms); //@12.000MHz Delay(X_ms)

extern void SEG(unsigned char Location, unsigned char Number); //SEG View (Location, Number)

/*LCD1602*/
extern void LCD_Init();
extern void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char);
extern void LCD_ShowString(unsigned char Line,unsigned char Column,char *String);
extern void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
extern void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length);
extern void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
extern void LCD_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);

/*Martix Key*/
extern unsigned char MatrixKey();

/*Key*/
extern unsigned char Key();

/*Timer0_Init*/
extern void Timer0_Init(void);

#endif