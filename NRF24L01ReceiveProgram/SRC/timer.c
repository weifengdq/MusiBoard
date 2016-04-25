#include "timer.h"

void Timer_Init(unsigned int T_N100us)	//��΢��
{
	unsigned int T_100us;

//  AUXR |= 0x80;                   	//��ʱ��0Ϊ1Tģʽ  //33.1776MHz�����1.97ms
//	T_100us = 65536-FOSC/10000*T_N100us; 	//1Tģʽ
  	AUXR &= 0x7f;                   	//��ʱ��0Ϊ12Tģʽ //33.1776MHz�����23.7ms
	T_100us = 65536-FOSC/12/10000*T_N100us; 	//12Tģʽ
    TMOD = 0x00;                    	//���ö�ʱ��Ϊģʽ0(16λ�Զ���װ��)
    TL0 = T_100us;                     	//��ʼ����ʱֵ
    TH0 = T_100us >> 8;
    TR0 = 1;                        	//��ʱ��0��ʼ��ʱ
    ET0 = 1;                        	//ʹ�ܶ�ʱ��0�ж�
    EA = 1;		
}
