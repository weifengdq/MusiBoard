#ifndef __HMC5883L_H__
#define __HMC5883L_H__

#include "config0.h"

#define	SlaveAddress1   0x3C	  //定义器件在IIC总线中的从地址

extern float HMC5883_Angle;

void HMC5883_Init();            //初始化5883
void  Single_Write_HMC5883(unsigned char REG_Address,unsigned char REG_data);   //单个写入数据
//unsigned char Single_Read_HMC5883(unsigned char REG_Address);                   //单个读取内部寄存器数据
void  Multiple_Read_HMC5883();                                  //连续的读取内部寄存器数据
//以下是模拟iic使用函数-------------
void HMC5883_Start();
void HMC5883_Stop();
void HMC5883_SendACK(bit ack);
unsigned char  HMC5883_RecvACK();
unsigned char HMC5883_SendChar(unsigned char dat);
unsigned char HMC5883_RecvChar();
void HMC5883_ReadPage();
void HMC5883_WritePage();

#endif
