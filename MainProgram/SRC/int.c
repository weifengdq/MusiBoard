#include "int.h"

void INT_Init()
{
//	INT_CLKO |= 0x10;               //(EX2 = 1)ʹ��INT2�ж�	���½��أ�
//	EA = 1;

	//INT0
    A_Phase = 1;
    IT0 = 1;                    //����INT0���ж����� (1:���½��� 0:�����غ��½���)
    EX0 = 1;                    //ʹ��INT0�ж�
    EA = 1;
    
}