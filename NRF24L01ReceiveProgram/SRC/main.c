/**************************************************************************************
��    �ߣ�weifengdq������΢����weifengdq��
ע�������ALT+F7 ����->Target->Memory Mode->Large:variables in XDATA,�����Ҫ�õ�����,
		  Debug�����ú÷���ʹ��ںţ�
		  ��ʼ����Һ�����������ʾ����Ҫ��P19������ñ�嵽���棬ʹLCD����������P26����,ʹ
		  ��PWM1������Һ����������;
		  ʹ�ô���1��1ͨ���������ݴ���Ϸ����lkb.h��������KEYMSG�ṹ��
�޸����ڣ�2014/10/17
*************************************************************************************/
#include "config0.h"
#include "delay.h"
#include "lkb.h"
#include "timer.h"
#include "uart.h"
#include <stdio.h>										 
#include "lcd.h"
#include "adc.h"
#include "mpu6050.h"
#include "image0.h"
#include "pca_pwm.h"
#include "hmc5883l.h"
#include "nrf24l01.h"
#include <math.h>

unsigned int adc0,adc1,adc2,adc3,adc4;
float li_voltage=0;

KEYMSG left;
KEYMSG right;
KEYMSG up;
KEYMSG down;
KEYMSG kb;

/*����ʹ�õ�������һ�����鿴Ч��*/
void main()
{	
	unsigned int i;
	unsigned int j;
	unsigned int k;
	unsigned char r_Angle=0;
//	unsigned int m;
	float Last_HMC5883_Angle=0;
//	float Avg_HMC5883_Angle=0;
//	float Last_Avg_HMC5883_Angle=0;
//	float temp_HMC5883_Angle=0;
	Delay_ms(10);	
	Delay_us(10);

	/*2.2�硢320*240�ֱ��ʡ�SPI�ӿڵ�LCD��ʼ��,ʹ��STC��Ӳ��SPI,LCD����оƬΪILI9340C*/
	LCD_LED=0;		//LCD��������
	SPI_Init();		//IAP15Ӳ��SPI��ʼ��,�л�����SPI�ĵڶ�ͨ��:SS/P24��MOSI/P23��MISO/P22��SCK/P21
	LCD_Init();		//LCD��ʼ���������ṩ

	/*LCD������ʾ*/
	LCD_Sleep();	//LCD��������״̬
	Delay_ms(100);	
	LCD_ExitSleep();//LCD�˳�����״̬
	/*LCDˢ����ʾ����Ȼ��������� �׺�*/
	LCD_Clear(Red);	
	LCD_Clear(Orange);
	LCD_Clear(Yellow);
	LCD_Clear(Green);
	LCD_Clear(Cyan);
	LCD_Clear(Blue);
	LCD_Clear(Purple);
	LCD_Clear(White);
	LCD_Clear(Black);
	/*LCD���ߡ�Ӣ�ġ����֡�������ʾ����*/
	/*Һ�������Ͻ�����Ϊ(0,0),���½�����Ϊ(239,319)*/
	POINT_COLOR=Green; //ȫ�ֱ���
	BACK_COLOR=Black; 	//ǰ��ɫ�������ںڰ�д�ֵĸ�����ɫ�ķ۱ʣ��ڰ����ɫ���Ǳ���ɫ	
	LCD_ShowString(0,0,"Hello,world!");	//��(0,0)���꿪ʼ��ʾ�ַ�����Hello,World!��
	POINT_COLOR=Blue;					//������ʱ���Ļ�����ɫ����ʾ��ͬ��ɫ������
	LCD_DrawLine(100,0,240,15);			//��һ���߶Σ���ʼ����(100,0),�յ�����(240,15)
	LCD_Show2Num(0,16,65535,5);			//��(0,16)���꿪ʼ��ʾ0~65535�����֣�5Ϊ����λ��������ʾ5λ����
	LCD_ShowNum(100,16,987654321,9);	//��(100,16)���꿪ʼ��ʾunsigned long������(0~4294967295),10Ϊ����λ������10λ����
	POINT_COLOR=Yellow;
	for(i=32;i<320;i+=16)
	{
		LCD_ShowString(0,i,"abcdefghijklmnopqrstuvwxyz!");
	}
	LCD_LED=1;		//LCD����ر�
	Delay_ms(500);
	LCD_LED=0;		//LCD��������
	Delay_ms(500);
	LCD_Clear(Black);					//��������ɫ
	//LCD_Fill(120,120,160,160,Yellow);	//�ھ����������ɫ�飬�ں�������F12���Բ鿴����ԭ��
	LCD_ShowImage(gImage_musi,80,120,78,80);//��ʾ��ɫͼ�� Ľ˾LOGO
	Delay_ms(2000);
	LCD_Clear(Black);
	POINT_COLOR=Red;
	LCD_DrawRectangle(0,0,239,60);	 //�����Σ��Խ�����(���ϡ�����)Ϊ(0,0),(239,60)
	POINT_COLOR=Purple;
	LCD_ShowString(80,2,"Ľ˾"); 	 //��ʾ16*16����
	LCD_ShowString(16,22,"Ϊһ��΢ã��һ����У�");
	LCD_ShowString(8,42,"������ܿܿ˭����������ԣ�");
	POINT_COLOR=Cyan;
	LCD_ShowGB3232(80,160,"�ഺ");
	POINT_COLOR=Green;
	LCD_DrawCircle(112,176,48);
	Delay_ms(500);

	PCA_PWM_Init();		//PWM��ʼ�����л���PWM�ĵ���ͨ��P25/P26/P27�������ʹ����PWM1(P26,��LCD_LED����)
	LCD_LED=0;			//LCD��������
	for(i=255;i<=0;i--)	//Һ������256�����ȵ�����ʾ:��������
	{
		CCAP1H = CCAP1L = i; 	  //ͨ������ռ�ձ���������Ļ����
		Delay_ms(20);
	}
	LCD_LED=1;			//LCD����ر�
	for(i=0;i<255;i++)	//Һ������256�����ȵ�����ʾ:�ɰ�����
	{
		CCAP1H = CCAP1L = i; 	  //ͨ������ռ�ձ���������Ļ����
		Delay_ms(20);
	}
	Delay_ms(1000);
	LCD_Clear(Black);

	LKB_Init();				//LED Key Buzzer��LED�������������� ��ʼ��(���÷������˿�Ϊ�������)
	Buzzer_Func(500,1000);	//������500Hz@1000ms

	ADC_Init();			   //AD��ʼ��������P1��0 1 2 3(ҡ��)4(﮵�ص�ѹ)��ΪAD�ڣ�
	POINT_COLOR = Green; 	   //Һ����������ɫ
	BACK_COLOR=Black;	   //����ɫ
	LCD_ShowString(0,0 ,"adc0:     ");
	LCD_ShowString(0,16,"adc1:     ");
	LCD_ShowString(120,0,"adc2:     ");
	LCD_ShowString(120,16,"adc3:     ");
	LCD_ShowString(0,32,"li_voltage:      V");
	LCD_ShowString(104,32,".");

	NRF24L01_Init();
	RX_Mode();		     //����ģʽ
	LCD_ShowString(0,48,"NRF24L01_State: Rx_Mode");

	MPU6050_Init();
	Delay_ms(100);
	POINT_COLOR = Red;
	LCD_DrawLine(0,319,239,319);
	LCD_DrawLine(0,139,239,139);
	LCD_DrawLine(0,229,239,229);

	HMC5883_Init();
	Delay_ms(100);
	POINT_COLOR = Green;
	LCD_DrawCircle(120,100,31);
	LCD_ShowString(0,64,"direction��");



	Timer_Init(100);	 //ÿ10ms�ж�һ��,����Ϊ��us��100*100us = 10ms

	UART_Init(115200);	 //���ڳ�ʼ����ʹ���˴���1�ĵ�һͨ�������ò�����115200�����ڷ���ʱ�����л����ڶ�ͨ��P36/P37
	SBUF=0;				 //��һ��ʹ��printf��ʱ�Ǳ����

	i=0;
	j=0;
	k=0;
//	m=0;
	while(1)
	{
		if(nRF24L01_RxPacket(Rx_Buf))
  		{
			if( (Rx_Buf[0]==1) && (Rx_Buf[1]==2))	//У��ǰ�����ֽ�
			{
				r_Angle=Rx_Buf[3];
				j++;
				if(j>=240)
				{
					j=0;
					LCD_Fill(0,140,239,318,Black);
					POINT_COLOR = Red;
					BACK_COLOR=Black;
					LCD_DrawLine(0,229,239,229);
					POINT_COLOR = Green;
				}
				if(Rx_Buf[2]=='+')
				{
					LCD_DrawPoint(j,229-r_Angle);
				}
				else
				{
					LCD_DrawPoint(j,229+r_Angle);	
				}
//				POINT_COLOR = Black;
//				LCD_DrawLine(j+1,140,j+1,238);	//Ҳ���������
//				LCD_DrawLine(j+2,140,j+2,238);
//				LCD_DrawLine(j+3,140,j+3,238);
//				POINT_COLOR = Red;
//				LCD_DrawPoint(j+1,229);
//				LCD_DrawPoint(j+2,229);
//				LCD_DrawPoint(j+3,229);
//				POINT_COLOR = Green;
			}
		}		




		if(LED_flag)		 //ÿ1s�л�һ��LED��״̬
		{
			LED_flag=0;
			LED1 = ~LED1;
		}

		//MPU6050�ļ�������ʹ����Delay_us(5);
		if(MPU6050_flag)
		{
			MPU6050_flag=0;
			//Angle_Calcu();		//��Ǽ��� ���õ���������ȫ�ֱ���,ʹ��������ժ���Ŀ������˲�
								//float Angle;   //������б�Ƕ�
								//float Gyro_y;  //Y����ٶ�
			//printf("%.2f,%.2f\n",Angle,Gyro_y);
//			j++;
//			if(j>=240)
//			{
//				j=0;
//				LCD_Fill(0,140,239,318,Black);
//				POINT_COLOR = Red;
//				BACK_COLOR=Black;
//				LCD_DrawLine(0,229,239,229);
//				POINT_COLOR = Green;
//			}
//			if((int)Angle>-90 && (int)Angle<90 )	  //�ų���ʼ���ȶ��Ӳ�
//			{
//				LCD_DrawPoint(j,229-(int)Angle);
//				//ͨ��NRF24L01���ͳ�ȥ
//				Tx_Buf[2] 	=(unsigned char)Angle;	
//				//Tx_Buf[3]	=(int)Angle>>8;		
//		  		Transmit(Tx_Buf);			//24L01���ͻ�������
//		  		sta=SPI_Read(READ_REG +  STATUS);	//��24L01��״̬�Ĵ���
//		  		if(TX_DS)	//sbit TX_DS =sta^5;���ݷ�������жϣ����յ�Ӧ���ź�ʱ��λ��1��д1����ж�
//		  		{  
//		   			//Delay_ms(5); 
//		   			SPI_RW_Reg(WRITE_REG + STATUS,sta);  //д״̬�Ĵ���������ж�
//		  		}
//	  			if(MAX_RT)  //����Ƿ��ͳ�ʱ  sbit MAX_RT =sta^4;
//		  		{			//�ﵽ�����ط��жϣ�MAX_RT�жϲ�������������ϵͳ���ܽ���ͨѶ��д1����ж�
//		   			//Delay_ms(5);  
//		   			SPI_RW_Reg(WRITE_REG + STATUS,sta);
//		  		}
//			}
		}

		if(HMC5883_flag)
		{
			HMC5883_flag=0;
//			Multiple_Read_HMC5883();				  //�Ƕȵı仯�ƺ����������Ե�
//			LCD_Show2Num(32,80,(int)HMC5883_Angle,3); 
//			POINT_COLOR=BACK_COLOR;					   //���Ǻ���ʹ�û��ȶ����Ƕ�!
//			LCD_DrawLine(120,100,120+(int)(28.0*cos((Last_HMC5883_Angle-180.0)/57.3)),100+(int)(28.0*sin((Last_HMC5883_Angle-180.0)/57.3)));
//			POINT_COLOR=Yellow;
//			LCD_DrawLine(120,100,120+(int)(28.0*cos((HMC5883_Angle-180.0)/57.3)),100+(int)(28.0*sin((HMC5883_Angle-180.0)/57.3)));
//			Last_HMC5883_Angle=HMC5883_Angle;
//			//ƽ��ֵ�˲�
//			m++;
//			temp_HMC5883_Angle+=HMC5883_Angle;	
//			if(m==5)
//			{
//				m=0;
//				Avg_HMC5883_Angle=temp_HMC5883_Angle/5.0;	 //��ǰ���ĸ�ֵ�����
//				//printf("\n\n%.2f\n\n",HMC5883_Angle);	  //HMC5883_Angle: float�ͣ�0~360
//				LCD_Show2Num(32,80,(int)Avg_HMC5883_Angle,3); 
//				POINT_COLOR=BACK_COLOR;
//				LCD_DrawLine(120,100,120+28*cos((Last_Avg_HMC5883_Angle-180.0)/57.3),100+28*sin((Last_Avg_HMC5883_Angle-180.0)/57.3));
//				POINT_COLOR=Yellow;
//				LCD_DrawLine(120,100,120+28*cos((Avg_HMC5883_Angle-180.0)/57.3),100+28*sin((Avg_HMC5883_Angle-180.0)/57.3));
//				Last_Avg_HMC5883_Angle=Avg_HMC5883_Angle;
//			} 
		}
		
		if(ADC_flag)
		{
			ADC_flag=0;
			POINT_COLOR= Green;
			i++;
			switch(i)
			{
				case 1: adc0=(int)ADC_GetResult(0);LCD_Show2Num(48,0,adc0,3); break;
				case 2:	adc1=(int)ADC_GetResult(1);LCD_Show2Num(48,16,adc1,3); break;
				case 3:	adc2=(int)ADC_GetResult(2);LCD_Show2Num(168,0,adc2,3); break;
				case 4:	adc3=(int)ADC_GetResult(3);LCD_Show2Num(168,16,adc3,3); break;
				case 5:	li_voltage=(float)ADC_GetResult(4)*0.019f; break; //adc4*3.3/256*(6.8+3.3)/6.8=adc4*0.019
				default:
					i=0;
					LCD_Show2Num(96,32,(int)li_voltage,1); 				//��ʾ��������
					LCD_Show2Num(112,32,((int)(li_voltage*100))%100,2); //��ʾС������
					//printf("%d,%d,%d,%d,%.2f\n",adc0,adc1,adc2,adc3,li_voltage);
			 }//switch ����

			k++;
			//printf("%d,%d,", adc1, adc0);
			if(k%2)
			{
				if(adc1>=0 && adc1<=80)	
				{
					sendMsg(&up, 1, 1);
				}
				else if(adc1>=170 && adc1<=255)	
				{
					sendMsg(&down, 4, 1);					 
				}
				else 
				{
					if(up.msg == 1)
					{
						sendMsg(&up, 1, 0);
					}
					else if(down.msg == 1)
					{
						sendMsg(&down, 4, 0);
					}
				}
			}
			else
			{
				if(adc0>=0 && adc0<=80)	
				{
					sendMsg(&left, 3, 1);
				}
				else if(adc0>=170 && adc0<=255)	
				{
					sendMsg(&right, 2, 1);					 
				}
				else 
				{
					if(left.msg == 1)
					{
						sendMsg(&left, 3, 0);
					}
					else if(right.msg == 1)
					{
						sendMsg(&right, 2, 0);
					}
				}
			}	
		}

		//interrupt.c�а�Key_Scan()�ĳ�֧������
		//uart.c�аѴ���1�л���P30/P31
		if(KEY_flag)
		{	
			if (key_up==0)
			{
				sendMsg(&kb, Key_Num, 1);	 //sendMsg()����������printf
			}
			else  //if(key_up == 1) 
			{
				KEY_flag=0;
				if (kb.msg == 1) ;
					sendMsg(&kb, kb.key, 0);
			}
			//printf("%d\n",Key_Num);
		}
	}		
}
