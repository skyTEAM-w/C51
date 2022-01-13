#include <REGX52.H>

void main(){
    P0 = 0xFE;
    while (1)
    {
        if (P2_3 == 0 && P2_2 == 0)
        {
            P0_0 = 0;
        }
        else P0_0 = 1;
        
    }
    
}