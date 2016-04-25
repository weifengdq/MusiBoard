#include "timer.h"

void Timer_Init(unsigned int T_N100us)	//百微秒
{
	unsigned int T_100us;

//  AUXR |= 0x80;                   	//定时器0为1T模式  //33.1776MHz下最多1.97ms
//	T_100us = 65536-FOSC/10000*T_N100us; 	//1T模式
  	AUXR &= 0x7f;                   	//定时器0为12T模式 //33.1776MHz下最多23.7ms
	T_100us = 65536-FOSC/12/10000*T_N100us; 	//12T模式
    TMOD = 0x00;                    	//设置定时器为模式0(16位自动重装载)
    TL0 = T_100us;                     	//初始化计时值
    TH0 = T_100us >> 8;
    TR0 = 1;                        	//定时器0开始计时
    ET0 = 1;                        	//使能定时器0中断
    EA = 1;		
}
