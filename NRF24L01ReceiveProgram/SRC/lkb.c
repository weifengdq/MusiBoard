#include "lkb.h"
#include "delay.h"
#include <stdio.h>

bit key_up=1;	//按键松开标志位，使连按或不连按
unsigned int Key_Num=0;

void sendMsg(KEYMSG* keymsg, key, msg)
{
	keymsg->key = key;
	keymsg->msg = msg;
	printf("%d,%d\n", keymsg->key, keymsg->msg);
}

/****************************************
设置蜂鸣器端口为推挽输出
****************************************/
void LKB_Init()
{
	LED1 = 1;		//关闭LED
	BEEP = 0;		//关闭蜂鸣器(蜂鸣器驱动有下拉电阻,可防止上电鸣响)
	//单片机复位后，管脚输出为高电平，所以按键也可以不用置1？
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

	P2M1 &= 0xdf;	//P25清0：1101 1111		
	P2M0 |= 0x20;	//P25置1：0010 0000
}

/****************************************
*无源蜂鸣器频率、时间控制函数
*Freq：蜂鸣器频率
*time: 蜂鸣器响的时间：ms
****************************************/
void Buzzer_Func(unsigned int Freq,unsigned int time)
{
	unsigned int i;
	for(i=0;i<time*Freq/500;i++)
	{
		BEEP=~BEEP;
		Delay_us(500000L/Freq);	//最低500000L/65536=7.6Hz约8Hz 最高500 000Hz
	}
	BEEP=0;				//关闭蜂鸣器
}

/*****************************************
*按键扫描函数V2
*xiaodou:
*一般10ms消抖，如果定时器1ms中断一次，
*这个参数设为10即可
*mode:
*0 不支持连按
*1 支持连按
*****************************************/
void Key_Scan(unsigned int xiaodou,unsigned char mode)
{
	static unsigned char i;
    if(mode)	/*key_up=1*/;  //支持连按
	if(/*key_up &&*/ (KEY1==0 || KEY2==0 || KEY3==0 || KEY4==0 || KEY5==0
				||KEY6==0 || KEY7==0 || KEY8==0 || KEY9==0 || KEY10==0) )	//有按键按下
	{
		i++;			//去抖动
		if(i>=(xiaodou+1)) 		//随定时器中断而改变
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
		   &&(KEY6==1) && (KEY7==1) && (KEY8==1) && (KEY9==1) && (KEY10==1)) 	// 无按键按下
	{
		key_up=1; 
		Key_Num=0;	     
	}
	else Key_Num=0;
}
