#include "hmc5883l.h"
#include "delay.h"
#include <math.h>

float HMC5883_Angle=0;
	
/**************************************
起始信号
**************************************/
void HMC5883_Start()
{
    SDA1 = 1;                    //拉高数据线
    SCL1 = 1;                    //拉高时钟线
    Delay_us(5);                 //延时
    SDA1 = 0;                    //产生下降沿
    Delay_us(5);                 //延时
    SCL1 = 0;                    //拉低时钟线
}

/**************************************
停止信号
**************************************/
void HMC5883_Stop()
{
    SDA1 = 0;                    //拉低数据线
    SCL1 = 1;                    //拉高时钟线
    Delay_us(5);                 //延时
    SDA1 = 1;                    //产生上升沿
    Delay_us(5);                 //延时
}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void HMC5883_SendACK(bit ack)
{
    SDA1 = ack;                  //写应答信号
    SCL1 = 1;                    //拉高时钟线
    Delay_us(5);                 //延时
    SCL1 = 0;                    //拉低时钟线
    Delay_us(5);                 //延时
}

/**************************************
接收应答信号
**************************************/
unsigned char HMC5883_RecvACK()
{
	SCL1=0;
	Delay_us(5);
	SDA1=1;
	Delay_us(5);
	SCL1=1;
	Delay_us(5);
	if(SDA1==1)
	{
		SCL1=0;
		Delay_us(5);
		return 0;
	}
	SCL1=0;
	Delay_us(5);
	return 1;

//    SCL1 = 1;                    //拉高时钟线
//    Delay_us(5);                 //延时
//    CY = SDA1;                   //读应答信号
//    SCL1 = 0;                    //拉低时钟线
//    Delay_us(5);                 //延时
//
//    return CY;
}

/**************************************
向IIC总线发送一个字节数据
**************************************/
unsigned char HMC5883_SendChar(unsigned char dat)
{
    unsigned char i;
    for (i=0; i<8; i++)         //8位计数器
    {
		SCL1 = 0;
		Delay_us(5);
		if(dat & 0x80) SDA1=1;
		else SDA1=0;		
        dat <<= 1;              //移出数据的最高位
        SCL1 = 1;                //拉高时钟线
        Delay_us(5);             //延时
    }
	SCL1 = 0;                //拉低时钟线
    return HMC5883_RecvACK();
}

/**************************************
从IIC总线接收一个字节数据
**************************************/
unsigned char HMC5883_RecvChar()
{
    unsigned char i;
    unsigned char dat = 0;

    SDA1 = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        SCL1 = 1;                //拉高时钟线
        Delay_us(5);             //延时
        dat |= SDA1;             //读数据               
        SCL1 = 0;                //拉低时钟线
        Delay_us(5);             //延时
    }
    return dat;
}

//***************************************************

void Single_Write_HMC5883(unsigned char REG_Address,unsigned char REG_data)
{
    HMC5883_Start();                  //起始信号
    HMC5883_SendChar(SlaveAddress1);   //发送设备地址+写信号
    HMC5883_SendChar(REG_Address);    //内部寄存器地址，请参考中文pdf 
    HMC5883_SendChar(REG_data);       //内部寄存器数据，请参考中文pdf
    HMC5883_Stop();                   //发送停止信号
}

////********单字节读取内部寄存器*************************
//unsigned char Single_Read_HMC5883(unsigned char REG_Address)
//{  unsigned char REG_data;
//    HMC5883_Start();                          //起始信号
//    HMC5883_SendChar(SlaveAddress1);           //发送设备地址+写信号
//    HMC5883_SendChar(REG_Address);                   //发送存储单元地址，从0开始	
//    HMC5883_Start();                          //起始信号
//    HMC5883_SendChar(SlaveAddress1+1);         //发送设备地址+读信号
//    REG_data=HMC5883_RecvChar();              //读出寄存器数据
//	HMC5883_SendACK(1);   
//	HMC5883_Stop();                           //停止信号
//    return REG_data; 
//}
//******************************************************
//连续读出HMC5883内部角度数据，地址范围0x3~0x5
//******************************************************
void Multiple_Read_HMC5883(void)
{
    unsigned char i;
	unsigned char BUF[8];                     //接收数据缓存区 
	int x,y,z;
    HMC5883_Start();                          //起始信号
    HMC5883_SendChar(SlaveAddress1);           //发送设备地址+写信号
    HMC5883_SendChar(0x03);                   //发送存储单元地址，从0x3开始	
    HMC5883_Start();                          //起始信号
    HMC5883_SendChar(SlaveAddress1+1);         //发送设备地址+读信号
	for (i=0; i<7; i++)                      //连续读取6个地址数据，存储中BUF
    {
        BUF[i] = HMC5883_RecvChar();          //BUF[0]存储数据
        if (i == 6)
        {
           HMC5883_SendACK(1);                //最后一个数据需要回NOACK
        }
        else
        {
          HMC5883_SendACK(0);                //回应ACK
       }
   	}
    HMC5883_Stop();                          //停止信号
    Delay_us(5);

	x=BUF[0] << 8 | BUF[1]; //Combine MSB and LSB of X Data output register
    z=BUF[2] << 8 | BUF[3]; //Combine MSB and LSB of Z Data output register
    y=BUF[4] << 8 | BUF[5]; //Combine MSB and LSB of Y Data output register

    HMC5883_Angle= (atan2((float)y,(float)x))*180.0/3.14+180.0; // angle in degrees
}

//初始化HMC5883，根据需要请参考pdf进行修改****
void HMC5883_Init()
{
     Single_Write_HMC5883(0x02,0x00);  //
}
