#include <REGX52.H>
#include <INTRINS.H>

void Delay500ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 4;
	j = 205;
	k = 187;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main()
{
	/*
	1111 1111 = ~0000 0000
	1111 1110 = ~0000 0001
	1111 1101 = ~0000 0010 = 0000 0001 << 1
	1111 1011
	1111 0111
	1110 1111
	1101 1111
	1011 1111
	0111 1111
	1111 1110
	*/
	int cnt=0;
	
	while(1){
		//if(P2==0x80){
		//	P2=~(0x01);
		//}
		if(cnt>=8){
			cnt=0;
		}
		P0=~(0x01<<cnt++);
		//P0 ~= P0+1;
		Delay500ms();
	}
}