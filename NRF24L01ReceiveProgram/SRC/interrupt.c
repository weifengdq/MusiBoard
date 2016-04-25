#include "config0.h"
#include "lkb.h"

bit LED_flag=0;
bit BEEP_flag=0;
bit KEY_flag=0;
bit ADC_flag=0;
bit MPU6050_flag=0;
bit HMC5883_flag=0;

/*********************************************
* Timer0�жϷ������
*********************************************/
void tm0_isr() interrupt 1 using 1	   //10ms�ж�һ��
{
	static unsigned int i=0;	//����LED
	static unsigned int j=0;	//���ڷ�����
	static unsigned int k=0;	//����MPU6050
//	static unsigned int m=0;	//����HMC5883L
	static unsigned int n=0;	//����ADC
	
	i++;
	if(i>=100)
	{
		i=0;
		LED_flag=1;
	}

	Key_Scan(1,1);	//֧����������������ʱ�䣬10ms
   	if(Key_Num)
	{
		//if(Key_Num==10) BEEP_flag=1;  //����10���£�����������
		KEY_flag=1;
	}	
	if(BEEP_flag)
	{
		j++;
		BEEP=~BEEP;	//10ms��תһ�Σ�50Hz
		if(j>10)	//��100ms
		{
			j=0;
			BEEP_flag=0;
			BEEP=0;		   //�رշ�����
		}
	}

	k++;
	if(k%2)	//10ms����һ��
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
	if(n>=1)	//ad: 10ms����һ��
	{
		n=0;
		ADC_flag=1;
	}

}
