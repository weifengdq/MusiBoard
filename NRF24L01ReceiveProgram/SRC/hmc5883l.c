#include "hmc5883l.h"
#include "delay.h"
#include <math.h>

float HMC5883_Angle=0;
	
/**************************************
��ʼ�ź�
**************************************/
void HMC5883_Start()
{
    SDA1 = 1;                    //����������
    SCL1 = 1;                    //����ʱ����
    Delay_us(5);                 //��ʱ
    SDA1 = 0;                    //�����½���
    Delay_us(5);                 //��ʱ
    SCL1 = 0;                    //����ʱ����
}

/**************************************
ֹͣ�ź�
**************************************/
void HMC5883_Stop()
{
    SDA1 = 0;                    //����������
    SCL1 = 1;                    //����ʱ����
    Delay_us(5);                 //��ʱ
    SDA1 = 1;                    //����������
    Delay_us(5);                 //��ʱ
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void HMC5883_SendACK(bit ack)
{
    SDA1 = ack;                  //дӦ���ź�
    SCL1 = 1;                    //����ʱ����
    Delay_us(5);                 //��ʱ
    SCL1 = 0;                    //����ʱ����
    Delay_us(5);                 //��ʱ
}

/**************************************
����Ӧ���ź�
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

//    SCL1 = 1;                    //����ʱ����
//    Delay_us(5);                 //��ʱ
//    CY = SDA1;                   //��Ӧ���ź�
//    SCL1 = 0;                    //����ʱ����
//    Delay_us(5);                 //��ʱ
//
//    return CY;
}

/**************************************
��IIC���߷���һ���ֽ�����
**************************************/
unsigned char HMC5883_SendChar(unsigned char dat)
{
    unsigned char i;
    for (i=0; i<8; i++)         //8λ������
    {
		SCL1 = 0;
		Delay_us(5);
		if(dat & 0x80) SDA1=1;
		else SDA1=0;		
        dat <<= 1;              //�Ƴ����ݵ����λ
        SCL1 = 1;                //����ʱ����
        Delay_us(5);             //��ʱ
    }
	SCL1 = 0;                //����ʱ����
    return HMC5883_RecvACK();
}

/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
unsigned char HMC5883_RecvChar()
{
    unsigned char i;
    unsigned char dat = 0;

    SDA1 = 1;                    //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        SCL1 = 1;                //����ʱ����
        Delay_us(5);             //��ʱ
        dat |= SDA1;             //������               
        SCL1 = 0;                //����ʱ����
        Delay_us(5);             //��ʱ
    }
    return dat;
}

//***************************************************

void Single_Write_HMC5883(unsigned char REG_Address,unsigned char REG_data)
{
    HMC5883_Start();                  //��ʼ�ź�
    HMC5883_SendChar(SlaveAddress1);   //�����豸��ַ+д�ź�
    HMC5883_SendChar(REG_Address);    //�ڲ��Ĵ�����ַ����ο�����pdf 
    HMC5883_SendChar(REG_data);       //�ڲ��Ĵ������ݣ���ο�����pdf
    HMC5883_Stop();                   //����ֹͣ�ź�
}

////********���ֽڶ�ȡ�ڲ��Ĵ���*************************
//unsigned char Single_Read_HMC5883(unsigned char REG_Address)
//{  unsigned char REG_data;
//    HMC5883_Start();                          //��ʼ�ź�
//    HMC5883_SendChar(SlaveAddress1);           //�����豸��ַ+д�ź�
//    HMC5883_SendChar(REG_Address);                   //���ʹ洢��Ԫ��ַ����0��ʼ	
//    HMC5883_Start();                          //��ʼ�ź�
//    HMC5883_SendChar(SlaveAddress1+1);         //�����豸��ַ+���ź�
//    REG_data=HMC5883_RecvChar();              //�����Ĵ�������
//	HMC5883_SendACK(1);   
//	HMC5883_Stop();                           //ֹͣ�ź�
//    return REG_data; 
//}
//******************************************************
//��������HMC5883�ڲ��Ƕ����ݣ���ַ��Χ0x3~0x5
//******************************************************
void Multiple_Read_HMC5883(void)
{
    unsigned char i;
	unsigned char BUF[8];                     //�������ݻ����� 
	int x,y,z;
    HMC5883_Start();                          //��ʼ�ź�
    HMC5883_SendChar(SlaveAddress1);           //�����豸��ַ+д�ź�
    HMC5883_SendChar(0x03);                   //���ʹ洢��Ԫ��ַ����0x3��ʼ	
    HMC5883_Start();                          //��ʼ�ź�
    HMC5883_SendChar(SlaveAddress1+1);         //�����豸��ַ+���ź�
	for (i=0; i<7; i++)                      //������ȡ6����ַ���ݣ��洢��BUF
    {
        BUF[i] = HMC5883_RecvChar();          //BUF[0]�洢����
        if (i == 6)
        {
           HMC5883_SendACK(1);                //���һ��������Ҫ��NOACK
        }
        else
        {
          HMC5883_SendACK(0);                //��ӦACK
       }
   	}
    HMC5883_Stop();                          //ֹͣ�ź�
    Delay_us(5);

	x=BUF[0] << 8 | BUF[1]; //Combine MSB and LSB of X Data output register
    z=BUF[2] << 8 | BUF[3]; //Combine MSB and LSB of Z Data output register
    y=BUF[4] << 8 | BUF[5]; //Combine MSB and LSB of Y Data output register

    HMC5883_Angle= (atan2((float)y,(float)x))*180.0/3.14+180.0; // angle in degrees
}

//��ʼ��HMC5883��������Ҫ��ο�pdf�����޸�****
void HMC5883_Init()
{
     Single_Write_HMC5883(0x02,0x00);  //
}
