#include "adc.h"
#include "delay.h"
#include <intrins.h>

/*----------------------------
初始化ADC
----------------------------*/
void ADC_Init()
{
    P1ASF = 0x1f;                   //设置P1.0 1 2 3 4口为AD口
    ADC_RES = 0;                    //清除结果寄存器
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    Delay_ms(2);                     //ADC上电延时
}

/*----------------------------
读取ADC结果
----------------------------*/
unsigned char ADC_GetResult(unsigned char ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //等待4个NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//等待ADC转换完成
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    return ADC_RES;                 //返回ADC结果
}

