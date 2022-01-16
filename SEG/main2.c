#include <REGX52.H>
//#include <NomalFunc.h>
#include "Delay.h"
#include "SEG.h"
//#include "BasicalFunc.h"

//unsigned char code SEGNum[] = {
//    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
//    0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,
//    0x00
//};

//void Delay(unsigned int X_ms) //@12.000MHz
//{
//    unsigned char i, j;
//    while (X_ms--)
//    {
//        /* code */
//        i = 2;
//        j = 239;
//        do
//        {
//            while (--j);
//        } while (--i);
//    }
//}

//void SEG(unsigned char Location, unsigned char Number) {
//    P1 = ~(0x01 << (8-Location));
//    P0 = ~SEGNum[Number];
//    Delay(1);
//    P0 = 0xFF;
//}

void main(){
    while(1){
        SEGScan(12000, 'n');
    }
}