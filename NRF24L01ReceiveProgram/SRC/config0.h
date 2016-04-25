#ifndef __CONFIG0_H__
#define __CONFIG0_H__

/*************************************
*����1ʹ�ö�ʱ��2��Ϊ�����ʷ�����
*************************************/

#include "stc15f2k60s2.h"

#define FOSC 33177600L
//#define FOSC 11059200L

sbit LED1 = P2^0;
sbit BEEP = P2^5;

sbit KEY1 = P1^5;
sbit KEY2 = P1^6;
sbit KEY3 = P1^7;
sbit KEY4 = P3^3;
sbit KEY5 = P4^6;
sbit KEY6 = P3^2;
sbit KEY7 = P3^5;
sbit KEY8 = P4^2;
sbit KEY9 = P3^4;
sbit KEY10= P4^7;

//LCD���Ŷ��壺ע��LCD���ӵ���SPI�ӿڵĵڶ����ţ���lcd.c�������л�
sbit    LCD_LED = 	P2^6; //һ��ֱ�ӽӵأ�ʹҺ��������������Ҫ���ȵ��ڣ����ӵ�P26�ڣ�ʹ��PWM���Ե���Һ��������
sbit	LCD_RST =	P0^0; //Һ��RESET����,�ӵ�Ƭ��IO����
sbit	LCD_DC	=	P0^1;//Һ��D/CX���ţ�����/������ƣ��ӵ�Ƭ��IO����
sbit	LCD_CS	=	P2^4;//Һ��ƬѡCS,P1.4Ϊ��Ƭ��SPI���ߵ�SS����,ͨ��MSTRλ����ΪIOģʽ
//����3��SPI���ŵ����ӡ���ӵ�Ƭ��Ĭ��Ӳ��SPI����,ֻ��Ҫ��SPCTL��ʹ�ܼ���,��������ж���
//��ΪP1��SPI�ڣ�lcd.c��SPI_Init()���л�����P2��SPI������
//sbit	SDI	=	P1^3; Һ��SDI����,����д��Һ��,�ӵ�Ƭ��MOSI����P1.5
//sbit	SDO	=	P1^4; Һ��SDO����,��Һ��������,�ӵ�Ƭ��MISO����P1.6
//sbit	SCL =   P1^5; Һ��SCLʱ������,�ӵ�Ƭ��SCLKʱ������P1.7

//MPU6050���Ŷ���
sbit SCL2 = P0^5;
sbit SDA2 = P0^6;

//HMC5883L
sbit SCL1 = P0^7;    
sbit SDA1 = P5^5;

//NRF24L01���Ŷ���
sbit MISO=P4^1;
sbit SCK =P4^3;
sbit CE  =P0^3;
sbit IRQ =P0^2;
sbit MOSI=P4^0;
sbit CSN =P5^4; 

extern bit LED_flag;
extern bit BEEP_flag;
extern bit KEY_flag;
extern bit ADC_flag;
extern bit MPU6050_flag;
extern bit HMC5883_flag;

extern unsigned int Key_Num;

extern unsigned int adc0,adc1,adc2,adc3,adc4;
extern float li_voltage;

extern float Gyro_y;        //Y�������������ݴ�					//�ɽ��ٶȼ������б�Ƕ�
extern float Angle_ax;      //�ɼ��ٶȼ������б�Ƕ�
extern float Angle;         //������б�Ƕ�

#endif

