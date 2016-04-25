#include "delay.h"
#include <intrins.h>

void Delay_us_1()
{
#if FOSC==11059200	
	_nop_();_nop_();_nop_();
	
#elif FOSC==12000000
	_nop_();_nop_();_nop_();_nop_();
	
#elif FOSC==22118400
	unsigned char i;
	i = 3;
	while (--i);
	
#elif FOSC==24000000
	unsigned char j;
	_nop_();
	_nop_();
	j = 3;
	while (--j);
	
#else
	unsigned char k;
	_nop_();
	_nop_();
	k = 3;
	while (--k);
	
#endif
}

void Delay_us(unsigned int i)
{
	while(i--)
	{
		Delay_us_1();
	}
}

//========================================================================
// ����: void  delay_ms(unsigned int ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��, �Զ���Ӧ��ʱ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void  Delay_ms(unsigned int ms)
{
     unsigned int i;
	 do{
	      //i = FOSC / 13000;
		  i = FOSC / 10000;
		  while(--i);    //14T per loop
     }while(--ms);
}
