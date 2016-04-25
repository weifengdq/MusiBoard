#include "config0.h"
#include "lkb.h"

bit LED_flag=0;
bit BEEP_flag=0;
bit KEY_flag=0;
bit ADC_flag=0;
bit MPU6050_flag=0;
bit HMC5883_flag=0;

/*********************************************
* Timer0中断服务程序
*********************************************/
void tm0_isr() interrupt 1 using 1	   //10ms中断一次
{
	static unsigned int i=0;	//用于LED
	static unsigned int j=0;	//用于蜂鸣器
	static unsigned int k=0;	//用于MPU6050
//	static unsigned int m=0;	//用于HMC5883L
	static unsigned int n=0;	//用于ADC
	
	i++;
	if(i>=100)
	{
		i=0;
		LED_flag=1;
	}

	Key_Scan(1,1);	//支持连按；按键消抖时间，10ms
   	if(Key_Num)
	{
		//if(Key_Num==10) BEEP_flag=1;  //按键10按下，蜂鸣器鸣响
		KEY_flag=1;
	}	
	if(BEEP_flag)
	{
		j++;
		BEEP=~BEEP;	//10ms翻转一次，50Hz
		if(j>10)	//响100ms
		{
			j=0;
			BEEP_flag=0;
			BEEP=0;		   //关闭蜂鸣器
		}
	}

	k++;
	if(k%2)	//10ms采样一次
	{
		//k=0;
		MPU6050_flag=1;
	}
	else
		HMC5883_flag=1;

//	m++;
//	if(m>=1)
//	{
//		m=0;
//		HMC5883_flag=1;
//	}

	n++;
	if(n>=1)	//ad: 10ms采样一次
	{
		n=0;
		ADC_flag=1;
	}

}
