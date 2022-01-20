#ifndef __DS1302_H__
#define __DS1302_H__

extern unsigned char DS1302_Time[];

extern void DS1302_Init();
extern unsigned char DS1302_ReadByte(unsigned char Command);
extern void DS1302_WriteByte(unsigned char Command, unsigned char Data);
extern void DS1302_SetTime();
extern void DS1302_ReadTime();
extern void DS1302_DayOf();

#endif