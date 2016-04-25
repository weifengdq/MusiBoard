#include "math.h"
#include "mpu6050.h"
#include "delay.h"
#include <intrins.h>

float Gyro_y;        //Y轴陀螺仪数据暂存					//由角速度计算的倾斜角度
float Angle_ax;      //由加速度计算的倾斜角度
float Angle;         //小车最终倾斜角度

//**************************************
//I2C起始信号
//**************************************
void I2C_Start()
{
    SDA2 = 1;                    //拉高数据线
    SCL2 = 1;                    //拉高时钟线
    Delay_us(5);                 //延时
    SDA2 = 0;                    //产生下降沿
    Delay_us(5);                 //延时
    SCL2 = 0;                    //拉低时钟线
}
//**************************************
//I2C停止信号
//**************************************
void I2C_Stop()
{
    SDA2 = 0;                    //拉低数据线
    SCL2 = 1;                    //拉高时钟线
    Delay_us(5);                 //延时
    SDA2 = 1;                    //产生上升沿
    Delay_us(5);                 //延时
}
//**************************************
//I2C发送应答信号
//入口参数:ack (0:ACK 1:NAK)
//**************************************
void I2C_SendACK(unsigned char ack)
{
    SDA2 = ack;                  //写应答信号
    SCL2 = 1;                    //拉高时钟线
    Delay_us(5);                 //延时
    SCL2 = 0;                    //拉低时钟线
    Delay_us(5);                 //延时
}
//**************************************
//I2C接收应答信号
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

//    SCL2 = 1;                    //拉高时钟线
//    Delay_us(5);                 //延时
//    CY = SDA2;                   //读应答信号
//    SCL2 = 0;                    //拉低时钟线
//    Delay_us(5);                 //延时
//    return CY;
}
//**************************************
//向I2C总线发送一个字节数据
//**************************************
unsigned char I2C_SendByte(unsigned char dat)
{
    unsigned char i;
    for (i=0; i<8; i++)         //8位计数器
    {
		SCL2 = 0;
		Delay_us(5);
		if(dat & 0x80) SDA2=1;
		else SDA2=0;		
        dat <<= 1;              //移出数据的最高位
        //SDA2 = CY;               //送数据口
        SCL2 = 1;                //拉高时钟线
        Delay_us(5);             //延时
    }
	SCL2 = 0;                //拉低时钟线
    return I2C_RecvACK();
}
//**************************************
//从I2C总线接收一个字节数据
//**************************************
unsigned char I2C_RecvByte()
{
    unsigned char i;
    unsigned char dat = 0;
    SDA2 = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        SCL2 = 1;                //拉高时钟线
        Delay_us(5);             //延时
        dat |= SDA2;             //读数据               
        SCL2 = 0;                //拉低时钟线
        Delay_us(5);             //延时
    }
    return dat;
}
//**************************************
//向I2C设备写入一个字节数据
//**************************************
void Single_WriteI2C(unsigned char REG_Address,unsigned char REG_data)
{
    I2C_Start();                  //起始信号
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
    I2C_SendByte(REG_Address);    //内部寄存器地址，
    I2C_SendByte(REG_data);       //内部寄存器数据，
    I2C_Stop();                   //发送停止信号
}
//**************************************
//从I2C设备读取一个字节数据
//**************************************
unsigned char Single_ReadI2C(unsigned char REG_Address)
{
	unsigned char REG_data;
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress);    //发送设备地址+写信号
	I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始	
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号
	REG_data=I2C_RecvByte();       //读出寄存器数据
	I2C_SendACK(1);                //接收应答信号
	I2C_Stop();                    //停止信号
	return REG_data;
}


//**************************************
//初始化MPU6050
//**************************************
void MPU6050_Init(void)
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG_M, 0x06);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x01);
}
//**************************************
//合成数据
//**************************************
int GetData(unsigned char REG_Address_1)
{
	unsigned char H,L;
	H=Single_ReadI2C(REG_Address_1);
	L=Single_ReadI2C(REG_Address_1+1);	//地址+1  变为低地址eg：ACCEL_XOUT_H	0x3B    ACCEL_XOUT_L	0x3C   “GetData（ACCEL_XOUT_H）”代表合成的数据
	return (H<<8)+L;   //合成数据
}

//*********************************************************
// 卡尔曼滤波
//摘抄网上流行的代码
//*********************************************************
void Kalman_Filter(float Accel,float Gyro)		//加速度偏角 角速度
{
	
	//******卡尔曼参数************
//	float  Q_angle=0.001;  
//	float  Q_gyro=0.003;
//	float  R_angle=0.5;

	float  Q_bias, Angle_err;
	float  PCt_0, PCt_1, E;
	float  K_0, K_1, t_0, t_1;
	float  Pdot[4] ={0.0,0.0,0.0,0.0};
	float  PP[2][2] = { { 1.0, 0.0 },{ 0.0, 1.0 } };	
	
	Angle+=(Gyro - Q_bias)*dt; //先验估计

	
//	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差微分
	Pdot[0]=0.001 - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差微分

	Pdot[1]=- PP[1][1];
	Pdot[2]=- PP[1][1];
//	Pdot[3]=Q_gyro;
	Pdot[3]=0.003;
	
	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - Angle;	//zk-先验估计
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
//	E = R_angle + C_0 * PCt_0;
	E = 0.5 + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

//	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
//	PP[0][1] -= K_0 * t_1;
//	PP[1][0] -= K_1 * t_0;
//	PP[1][1] -= K_1 * t_1;
		
	Angle	+= K_0 * Angle_err;	 //后验估计
	Q_bias	+= K_1 * Angle_err;	 //后验估计
	Gyro_y   = Gyro - Q_bias;	 //输出值（后验估计）的微分=角速度

}


//*********************************************************
// 倾角计算（卡尔曼滤波）
//*********************************************************

void Angle_Calcu(void)	 
{
	float Accel_x;	     					//X轴加速度值暂存
	float Accel_y;	     					//Y轴加速度值暂存
	float Accel_z;       					//由加速度计计算的倾斜角度
	//------加速度-------------------------
	//范围为2g时，换算关系：16384 LSB/G
	//角度较小时，x=sinx得到角度（弧度），deg=rad*180/3.14
	//因为x>=sinx,故乘以1.3适当放大
	//注意小车前进方向在XY平面
	Accel_x  = (float)GetData(ACCEL_XOUT_H)/286.0f;	  //读取X轴加速度
	Accel_y  = (float)GetData(ACCEL_ZOUT_H)/286.0f;	  //读取Z轴加速度
	Accel_z  = sqrt(Accel_x*Accel_x+Accel_y*Accel_y);	//开方
	//Angle_ax = (Accel_x) /16384;   //去除零点偏移，计算得到角度（弧度）
	//Angle_ax = Angle_ax*1.2*180/3.14;     //弧度转换为度
	Angle_ax = 57.3*asin(Accel_x/Accel_z);	//计算加速度偏离x方向的角度，asinf(弧度)，两个16384约去了
	/*		
	Accel_x  = GetData(ACCEL_XOUT_H);	  //读取X轴加速度
	Angle_ax = (Accel_x - 1100) /16384;   //去除零点偏移，计算得到角度（弧度）
	Angle_ax = Angle_ax*1.2*180/3.14;     //弧度转换为度
	UART1PutChar((char)Angle_ax);
	*/

  //------角速度--------------------
	//范围为2000deg/s时，换算关系：16.4 LSB/(deg/s)
	//注意小车前进方向为X方向
	Gyro_y = (float)GetData(GYRO_YOUT_H);	 //静止时Y轴输出为-30左右
	Gyro_y = -(Gyro_y)/16.4f;         //去除零点便偏移，计算角速度，负号为方向处理
	//Angle_gy = Angle_gy + Gyro_y*0.01;  //角速度积分的倾斜角度
	
	//-------卡尔曼滤波融合-------------------

	Kalman_Filter(Angle_ax,Gyro_y);  //卡尔曼滤波计算倾角，Angle,Angle_ax,Gyro_y均为全局变量

	//互补滤波
	//补偿原理是取当前倾角和加速度获得倾角差值进行放大，然后
	//与陀螺仪角速度叠加后再积分，从而使倾角最跟踪为加速度获得的角度
	//0.5为放大倍数，可调节补偿度；0.01为系统周期10ms
	//Angle = Angle + (((Angle_ax-Angle)*0.5 + Gyro_y)*0.01);*/
															  
}  

