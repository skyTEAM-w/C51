#include <REGX52.H>

void Delay(unsigned int ms) //@12.000MHz
{
    unsigned char i, j;
    while (ms--)
    {
        /* code */
        i = 2;
        j = 239;
        do
        {
            while (--j);
        } while (--i);
    }
}

void main()
{
    while (1)
    {
        /* code */
        // P0_0 = 0;
        // Delay(500);
        // P0_0 = 1;
        // Delay(500);
        if(P2_3 == 0){
            Delay(15);
            while (P2_3 == 0);
            Delay(15);
            P0_0 = ~ P0_0;
        }
    }
}