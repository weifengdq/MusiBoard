#include "lkb.h"
#include "delay.h"
#include <stdio.h>

bit key_up=1;	//�����ɿ���־λ��ʹ����������
unsigned int Key_Num=0;

void sendMsg(KEYMSG* keymsg, key, msg)
{
	keymsg->key = key;
	keymsg->msg = msg;
	printf("%d,%d\n", keymsg->key, keymsg->msg);
}

/****************************************
���÷������˿�Ϊ�������
****************************************/
void LKB_Init()
{
	LED1 = 1;		//�ر�LED
	BEEP = 0;		//�رշ�����(��������������������,�ɷ�ֹ�ϵ�����)
	//��Ƭ����λ�󣬹ܽ����Ϊ�ߵ�ƽ�����԰���Ҳ���Բ�����1��
//	KEY1 = 1;
//	KEY2 = 1;
//	KEY3 = 1;
//	KEY4 = 1;
//	KEY5 = 1;
//	KEY6 = 1;
//	KEY7 = 1;
//	KEY8 = 1;
//	KEY9 = 1;
//	KEY10= 1;

	P2M1 &= 0xdf;	//P25��0��1101 1111		
	P2M0 |= 0x20;	//P25��1��0010 0000
}

/****************************************
*��Դ������Ƶ�ʡ�ʱ����ƺ���
*Freq��������Ƶ��
*time: ���������ʱ�䣺ms
****************************************/
void Buzzer_Func(unsigned int Freq,unsigned int time)
{
	unsigned int i;
	for(i=0;i<time*Freq/500;i++)
	{
		BEEP=~BEEP;
		Delay_us(500000L/Freq);	//���500000L/65536=7.6HzԼ8Hz ���500 000Hz
	}
	BEEP=0;				//�رշ�����
}

/*****************************************
*����ɨ�躯��V2
*xiaodou:
*һ��10ms�����������ʱ��1ms�ж�һ�Σ�
*���������Ϊ10����
*mode:
*0 ��֧������
*1 ֧������
*****************************************/
void Key_Scan(unsigned int xiaodou,unsigned char mode)
{
	static unsigned char i;
    if(mode)	/*key_up=1*/;  //֧������
	if(/*key_up &&*/ (KEY1==0 || KEY2==0 || KEY3==0 || KEY4==0 || KEY5==0
				||KEY6==0 || KEY7==0 || KEY8==0 || KEY9==0 || KEY10==0) )	//�а�������
	{
		i++;			//ȥ����
		if(i>=(xiaodou+1)) 		//�涨ʱ���ж϶��ı�
		{
			i=0; 
			key_up=0;
			if(KEY1==0)		 Key_Num = 1;
			else if(KEY2==0) Key_Num = 2;
			else if(KEY3==0) Key_Num = 3;
			else if(KEY4==0) Key_Num = 4;
			else if(KEY5==0) Key_Num = 5;
			else if(KEY6==0) Key_Num = 6;
			else if(KEY7==0) Key_Num = 7;
			else if(KEY8==0) Key_Num = 8;
			else if(KEY9==0) Key_Num = 9;
			else if(KEY10==0) Key_Num = 10;
			else Key_Num=0;
		}
	}
	else if( (KEY1==1) && (KEY2==1) && (KEY3==1) && (KEY4==1) && (KEY5==1)
		   &&(KEY6==1) && (KEY7==1) && (KEY8==1) && (KEY9==1) && (KEY10==1)) 	// �ް�������
	{
		key_up=1; 
		Key_Num=0;	     
	}
	else Key_Num=0;
}
