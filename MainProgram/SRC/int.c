#include "int.h"

void INT_Init()
{
//	INT_CLKO |= 0x10;               //(EX2 = 1)使能INT2中断	（下降沿）
//	EA = 1;

	//INT0
    A_Phase = 1;
    IT0 = 1;                    //设置INT0的中断类型 (1:仅下降沿 0:上升沿和下降沿)
    EX0 = 1;                    //使能INT0中断
    EA = 1;
    
}