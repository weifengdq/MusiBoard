#include "uart.h"

void UART_Init(unsigned long Baud)
{
    ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
//	    
//	  ACC = P_SW1;
//	  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=1 S1_S1=0
//	  ACC |= S1_S0;               //(P3.6/RxD_2, P3.7/TxD_2)
//	  P_SW1 = ACC;  
//  
//  ACC = P_SW1;
//  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=1
//  ACC |= S1_S1;               //(P1.6/RxD_3, P1.7/TxD_3)
//  P_SW1 = ACC;  

#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;                //8位可变波特率
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;                //9位可变波特率,校验位初始为1
#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;                //9位可变波特率,校验位初始为0
#endif

    T2L = (65536 - (FOSC/4/Baud));   //设置波特率重装值
    T2H = (65536 - (FOSC/4/Baud))>>8;
    AUXR |= 0x14;                //T2为1T模式, 并启动定时器2
    AUXR |= 0x01;               //选择定时器2为串口1的波特率发生器
    //ES = 1;                     //使能串口1中断
    //EA = 1;
}
