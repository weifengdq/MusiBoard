#include "adc.h"
#include "delay.h"
#include <intrins.h>

/*----------------------------
��ʼ��ADC
----------------------------*/
void ADC_Init()
{
    P1ASF = 0x1f;                   //����P1.0 1 2 3 4��ΪAD��
    ADC_RES = 0;                    //�������Ĵ���
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    Delay_ms(2);                     //ADC�ϵ���ʱ
}

/*----------------------------
��ȡADC���
----------------------------*/
unsigned char ADC_GetResult(unsigned char ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //�ȴ�4��NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//�ȴ�ADCת�����
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    return ADC_RES;                 //����ADC���
}

