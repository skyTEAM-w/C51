#ifndef __NOMALFUNC_H__
#define __NOMALFUNC_H__

void Delay(unsigned int X_ms); //@12.000MHz Delay(X_ms)

void SEG(unsigned char Location, unsigned char Number); //SEG View (Location, Number)

/*LCD1602*/
extern void LCD_Init();
extern void LCD_ShowChar(unsigned char LineC,unsigned char ColumnC,char Char);
extern void LCD_ShowString(unsigned char LineS,unsigned char ColumnS,char *String);
extern void LCD_ShowNum(unsigned char LineN,unsigned char ColumnN,unsigned int NumberN,unsigned char LengthN);
extern void LCD_ShowSignedNum(unsigned char LineSI,unsigned char ColumnSI,int NumberSI,unsigned char LengthSI);
extern void LCD_ShowHexNum(unsigned char LineH,unsigned char ColumnH,unsigned int NumberH,unsigned char LengthH);
extern void LCD_ShowBinNum(unsigned char LineB,unsigned char ColumnB,unsigned int NumberB,unsigned char LengthB);

#endif