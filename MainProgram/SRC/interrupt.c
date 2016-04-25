#include "config0.h"
#include "lkb.h"

bit LED_flag=0;
bit BEEP_flag=0;
bit KEY_flag=0;
bit ADC_flag=0;
bit MPU6050_flag=0;
bit HMC5883_flag=0;
bit Encoder_flag=0;

bit Rotate_Dir=0;	  //�����жϱ�������ת����
int Cnt_Last;                      //��¼��һ�εĲ���ֵ
int Cnt_Now=500;                       //��¼���εĲ���ֵ
unsigned int Freq=0;

//�����ο�����δʹ��
void exint0() interrupt 0  using 2     //INT0�ж����
{
	//���ж�ת��
	if(B_Phase)	//���B������Ϊ����ѹ����ʾ��ת
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


//�ⲿ�ж�2�������
//void exint2() interrupt 10          //INT2�ж����	���½��أ�
//{
//	//���ж�ת��
//	if(B_Phase)	//���B������Ϊ����ѹ����ʾ��ת
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
////  INT_CLKO &= 0xEF;               //����Ҫ�ֶ�����жϱ�־,���ȹر��ж�,��ʱϵͳ���Զ�����ڲ����жϱ�־
////  INT_CLKO |= 0x10;               //Ȼ���ٿ��жϼ���
//}

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
	static unsigned int p=0;	//���ڱ�����

	//ʹ��Aslong��JGA25-371ֱ�����ٵ����334�߱����������ٱ�Ϊ 21.3��12V���ѹ���ת��201rpm
	//��ô�ת����10ms�����������201*21.3*334/60/100=238.3257������
	unsigned char ch,cl;
	static unsigned int temp=0;
	static unsigned int temp_1=0;	//�ϴε�ֵ
	cl=TL1;	//�ȶ���λ����λ���û��ô�죩
	ch=TH1;
	temp_1=temp;
	temp=ch*256+cl;	//��������ôʵ�֣� ch<<8+cl
	//if(temp>=temp_1) Freq=(temp-temp_1)/5;			// *200/1000 kHz			  //20kHz ÿ5ms ��100����
	//else Freq=(65536-temp_1 + temp)/5;
	if(temp>=temp_1) Freq=(temp-temp_1)*100;			//1s������������Ƶ��
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
