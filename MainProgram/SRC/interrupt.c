#include "config0.h"
#include "lkb.h"

bit LED_flag=0;
bit BEEP_flag=0;
bit KEY_flag=0;
bit ADC_flag=0;
bit MPU6050_flag=0;
bit HMC5883_flag=0;
bit Encoder_flag=0;

bit Rotate_Dir=0;	  //用于判断编码器旋转方向
int Cnt_Last;                      //记录上一次的捕获值
int Cnt_Now=500;                       //记录本次的捕获值
unsigned int Freq=0;

//仅作参考，并未使用
void exint0() interrupt 0  using 2     //INT0中断入口
{
	//先判断转向
	if(B_Phase)	//如果B相脉冲为正电压，表示正转
	{
		Rotate_Dir=1;
		Cnt_Now++;
		if(Cnt_Now>=1000)	Cnt_Now=0;
		Cnt_Last=Cnt_Now;
	}
	else
	{
		Rotate_Dir=0;
		Cnt_Now--;
		if(Cnt_Now<=0)	Cnt_Now=1000;
	}
}


//外部中断2服务程序
//void exint2() interrupt 10          //INT2中断入口	（下降沿）
//{
//	//先判断转向
//	if(B_Phase)	//如果B相脉冲为正电压，表示正转
//	{
//		Rotate_Dir=1;
//		Cnt_Now++;
//		if(Cnt_Now>=1000)	Cnt_Now=0;
//		Cnt_Last=Cnt_Now;
//	}
//	else
//	{
//		Rotate_Dir=0;
//		Cnt_Now--;
//		if(Cnt_Now<=0)	Cnt_Now=1000;
//	}	
//	    
////  INT_CLKO &= 0xEF;               //若需要手动清除中断标志,可先关闭中断,此时系统会自动清除内部的中断标志
////  INT_CLKO |= 0x10;               //然后再开中断即可
//}

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
	static unsigned int p=0;	//用于编码器

	//使用Aslong的JGA25-371直流减速电机：334线编码器，减速比为 21.3，12V额定电压，额定转速201rpm
	//那么额定转速下10ms输出脉冲数：201*21.3*334/60/100=238.3257个脉冲
	unsigned char ch,cl;
	static unsigned int temp=0;
	static unsigned int temp_1=0;	//上次的值
	cl=TL1;	//先读低位（高位变得没那么快）
	ch=TH1;
	temp_1=temp;
	temp=ch*256+cl;	//用左移怎么实现？ ch<<8+cl
	//if(temp>=temp_1) Freq=(temp-temp_1)/5;			// *200/1000 kHz			  //20kHz 每5ms 计100个数
	//else Freq=(65536-temp_1 + temp)/5;
	if(temp>=temp_1) Freq=(temp-temp_1)*100;			//1s的脉冲数，即频率
	else Freq=(65536-temp_1 + temp)*100;

	p++;
	if(p>=1)
	{
		p=0;
		Encoder_flag=1;
	}
	
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
