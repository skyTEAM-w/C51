#ifndef __TIMER0_H__
#define __TIMER0_H__

extern void Timer0_Init(void);

extern void Timer0_SetCounter(unsigned int Value);
extern unsigned int Timer0_GetCounter(void);
extern void Timer0_Run(unsigned char Flag);

#endif