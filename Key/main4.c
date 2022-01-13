#include <REGX52.H>

void Delay(unsigned int ms) //@12.000MHz
{
    unsigned char i, j;
    while (ms--)
    {
        i = 2;
        j = 239;
        do
        {
            while (--j);
        } while (--i);
    }
}

void main(){
    unsigned char LEDNum = 0;
    while(1){
        if(P2_3 == 0){
            Delay(15);
            while(P2_3 == 0);
            Delay(15);
            
            LEDNum ++;
			if(LEDNum >= 8)
                LEDNum = 0;
            
            P0 = ~(0x01 << LEDNum);
        }
        if(P2_2 == 0){
            Delay(15);
            while(P2_2 == 0);
            Delay(15);

            if (LEDNum == 0)
            {
                LEDNum = 7;
            }
            
            LEDNum --;
            P0 = ~(0x01 << LEDNum);
        }
    }
}