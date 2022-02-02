#ifndef __SEG_H__
#define __SEG_H__

void SEG(unsigned char Location, unsigned char Number); //SEG View (Location, Number)
void SEGScan(unsigned int Data, char ScanMode);
void SEG_Loop();
void SEG_SetBuf(unsigned char Location, unsigned char Number);

#endif