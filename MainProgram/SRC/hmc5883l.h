#ifndef __HMC5883L_H__
#define __HMC5883L_H__

#include "config0.h"

#define	SlaveAddress1   0x3C	  //����������IIC�����еĴӵ�ַ

extern float HMC5883_Angle;

void HMC5883_Init();            //��ʼ��5883
void  Single_Write_HMC5883(unsigned char REG_Address,unsigned char REG_data);   //����д������
//unsigned char Single_Read_HMC5883(unsigned char REG_Address);                   //������ȡ�ڲ��Ĵ�������
void  Multiple_Read_HMC5883();                                  //�����Ķ�ȡ�ڲ��Ĵ�������
//������ģ��iicʹ�ú���-------------
void HMC5883_Start();
void HMC5883_Stop();
void HMC5883_SendACK(bit ack);
unsigned char  HMC5883_RecvACK();
unsigned char HMC5883_SendChar(unsigned char dat);
unsigned char HMC5883_RecvChar();
void HMC5883_ReadPage();
void HMC5883_WritePage();

#endif
