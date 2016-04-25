#include "math.h"
#include "mpu6050.h"
#include "delay.h"
#include <intrins.h>

float Gyro_y;        //Y�������������ݴ�					//�ɽ��ٶȼ������б�Ƕ�
float Angle_ax;      //�ɼ��ٶȼ������б�Ƕ�
float Angle;         //С��������б�Ƕ�

//**************************************
//I2C��ʼ�ź�
//**************************************
void I2C_Start()
{
    SDA2 = 1;                    //����������
    SCL2 = 1;                    //����ʱ����
    Delay_us(5);                 //��ʱ
    SDA2 = 0;                    //�����½���
    Delay_us(5);                 //��ʱ
    SCL2 = 0;                    //����ʱ����
}
//**************************************
//I2Cֹͣ�ź�
//**************************************
void I2C_Stop()
{
    SDA2 = 0;                    //����������
    SCL2 = 1;                    //����ʱ����
    Delay_us(5);                 //��ʱ
    SDA2 = 1;                    //����������
    Delay_us(5);                 //��ʱ
}
//**************************************
//I2C����Ӧ���ź�
//��ڲ���:ack (0:ACK 1:NAK)
//**************************************
void I2C_SendACK(unsigned char ack)
{
    SDA2 = ack;                  //дӦ���ź�
    SCL2 = 1;                    //����ʱ����
    Delay_us(5);                 //��ʱ
    SCL2 = 0;                    //����ʱ����
    Delay_us(5);                 //��ʱ
}
//**************************************
//I2C����Ӧ���ź�
//**************************************
unsigned char I2C_RecvACK(void)
{
	SCL2=0;
	Delay_us(5);
	SDA2=1;
	Delay_us(5);
	SCL2=1;
	Delay_us(5);
	if(SDA2==1)
	{
		SCL2=0;
		Delay_us(5);
		return 0;
	}
	SCL2=0;
	Delay_us(5);
	return 1;

//    SCL2 = 1;                    //����ʱ����
//    Delay_us(5);                 //��ʱ
//    CY = SDA2;                   //��Ӧ���ź�
//    SCL2 = 0;                    //����ʱ����
//    Delay_us(5);                 //��ʱ
//    return CY;
}
//**************************************
//��I2C���߷���һ���ֽ�����
//**************************************
unsigned char I2C_SendByte(unsigned char dat)
{
    unsigned char i;
    for (i=0; i<8; i++)         //8λ������
    {
		SCL2 = 0;
		Delay_us(5);
		if(dat & 0x80) SDA2=1;
		else SDA2=0;		
        dat <<= 1;              //�Ƴ����ݵ����λ
        //SDA2 = CY;               //�����ݿ�
        SCL2 = 1;                //����ʱ����
        Delay_us(5);             //��ʱ
    }
	SCL2 = 0;                //����ʱ����
    return I2C_RecvACK();
}
//**************************************
//��I2C���߽���һ���ֽ�����
//**************************************
unsigned char I2C_RecvByte()
{
    unsigned char i;
    unsigned char dat = 0;
    SDA2 = 1;                    //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        SCL2 = 1;                //����ʱ����
        Delay_us(5);             //��ʱ
        dat |= SDA2;             //������               
        SCL2 = 0;                //����ʱ����
        Delay_us(5);             //��ʱ
    }
    return dat;
}
//**************************************
//��I2C�豸д��һ���ֽ�����
//**************************************
void Single_WriteI2C(unsigned char REG_Address,unsigned char REG_data)
{
    I2C_Start();                  //��ʼ�ź�
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
    I2C_SendByte(REG_data);       //�ڲ��Ĵ������ݣ�
    I2C_Stop();                   //����ֹͣ�ź�
}
//**************************************
//��I2C�豸��ȡһ���ֽ�����
//**************************************
unsigned char Single_ReadI2C(unsigned char REG_Address)
{
	unsigned char REG_data;
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress);    //�����豸��ַ+д�ź�
	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ����0��ʼ	
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress+1);  //�����豸��ַ+���ź�
	REG_data=I2C_RecvByte();       //�����Ĵ�������
	I2C_SendACK(1);                //����Ӧ���ź�
	I2C_Stop();                    //ֹͣ�ź�
	return REG_data;
}


//**************************************
//��ʼ��MPU6050
//**************************************
void MPU6050_Init(void)
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//�������״̬
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG_M, 0x06);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x01);
}
//**************************************
//�ϳ�����
//**************************************
int GetData(unsigned char REG_Address_1)
{
	unsigned char H,L;
	H=Single_ReadI2C(REG_Address_1);
	L=Single_ReadI2C(REG_Address_1+1);	//��ַ+1  ��Ϊ�͵�ַeg��ACCEL_XOUT_H	0x3B    ACCEL_XOUT_L	0x3C   ��GetData��ACCEL_XOUT_H��������ϳɵ�����
	return (H<<8)+L;   //�ϳ�����
}

//*********************************************************
// �������˲�
//ժ���������еĴ���
//*********************************************************
void Kalman_Filter(float Accel,float Gyro)		//���ٶ�ƫ�� ���ٶ�
{
	
	//******����������************
//	float  Q_angle=0.001;  
//	float  Q_gyro=0.003;
//	float  R_angle=0.5;

	float  Q_bias, Angle_err;
	float  PCt_0, PCt_1, E;
	float  K_0, K_1, t_0, t_1;
	float  Pdot[4] ={0.0,0.0,0.0,0.0};
	float  PP[2][2] = { { 1.0, 0.0 },{ 0.0, 1.0 } };	
	
	Angle+=(Gyro - Q_bias)*dt; //�������

	
//	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э����΢��
	Pdot[0]=0.001 - PP[0][1] - PP[1][0]; // Pk-����������Э����΢��

	Pdot[1]=- PP[1][1];
	Pdot[2]=- PP[1][1];
//	Pdot[3]=Q_gyro;
	Pdot[3]=0.003;
	
	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   // =����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - Angle;	//zk-�������
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
//	E = R_angle + C_0 * PCt_0;
	E = 0.5 + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

//	PP[0][0] -= K_0 * t_0;		 //����������Э����
//	PP[0][1] -= K_0 * t_1;
//	PP[1][0] -= K_1 * t_0;
//	PP[1][1] -= K_1 * t_1;
		
	Angle	+= K_0 * Angle_err;	 //�������
	Q_bias	+= K_1 * Angle_err;	 //�������
	Gyro_y   = Gyro - Q_bias;	 //���ֵ��������ƣ���΢��=���ٶ�

}


//*********************************************************
// ��Ǽ��㣨�������˲���
//*********************************************************

void Angle_Calcu(void)	 
{
	float Accel_x;	     					//X����ٶ�ֵ�ݴ�
	float Accel_y;	     					//Y����ٶ�ֵ�ݴ�
	float Accel_z;       					//�ɼ��ٶȼƼ������б�Ƕ�
	//------���ٶ�-------------------------
	//��ΧΪ2gʱ�������ϵ��16384 LSB/G
	//�ǶȽ�Сʱ��x=sinx�õ��Ƕȣ����ȣ���deg=rad*180/3.14
	//��Ϊx>=sinx,�ʳ���1.3�ʵ��Ŵ�
	//ע��С��ǰ��������XYƽ��
	Accel_x  = (float)GetData(ACCEL_XOUT_H)/286.0f;	  //��ȡX����ٶ�
	Accel_y  = (float)GetData(ACCEL_ZOUT_H)/286.0f;	  //��ȡZ����ٶ�
	Accel_z  = sqrt(Accel_x*Accel_x+Accel_y*Accel_y);	//����
	//Angle_ax = (Accel_x) /16384;   //ȥ�����ƫ�ƣ�����õ��Ƕȣ����ȣ�
	//Angle_ax = Angle_ax*1.2*180/3.14;     //����ת��Ϊ��
	Angle_ax = 57.3*asin(Accel_x/Accel_z);	//������ٶ�ƫ��x����ĽǶȣ�asinf(����)������16384Լȥ��
	/*		
	Accel_x  = GetData(ACCEL_XOUT_H);	  //��ȡX����ٶ�
	Angle_ax = (Accel_x - 1100) /16384;   //ȥ�����ƫ�ƣ�����õ��Ƕȣ����ȣ�
	Angle_ax = Angle_ax*1.2*180/3.14;     //����ת��Ϊ��
	UART1PutChar((char)Angle_ax);
	*/

  //------���ٶ�--------------------
	//��ΧΪ2000deg/sʱ�������ϵ��16.4 LSB/(deg/s)
	//ע��С��ǰ������ΪX����
	Gyro_y = (float)GetData(GYRO_YOUT_H);	 //��ֹʱY�����Ϊ-30����
	Gyro_y = -(Gyro_y)/16.4f;         //ȥ������ƫ�ƣ�������ٶȣ�����Ϊ������
	//Angle_gy = Angle_gy + Gyro_y*0.01;  //���ٶȻ��ֵ���б�Ƕ�
	
	//-------�������˲��ں�-------------------

	Kalman_Filter(Angle_ax,Gyro_y);  //�������˲�������ǣ�Angle,Angle_ax,Gyro_y��Ϊȫ�ֱ���

	//�����˲�
	//����ԭ����ȡ��ǰ��Ǻͼ��ٶȻ����ǲ�ֵ���зŴ�Ȼ��
	//�������ǽ��ٶȵ��Ӻ��ٻ��֣��Ӷ�ʹ��������Ϊ���ٶȻ�õĽǶ�
	//0.5Ϊ�Ŵ������ɵ��ڲ����ȣ�0.01Ϊϵͳ����10ms
	//Angle = Angle + (((Angle_ax-Angle)*0.5 + Gyro_y)*0.01);*/
															  
}  

