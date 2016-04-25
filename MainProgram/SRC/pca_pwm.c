#include "pca_pwm.h"

void PCA_PWM_Init()
{
//	  ACC = P_SW1;
//    ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=0
//    P_SW1 = ACC;                    //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
    
//	 ACC = P_SW1;
//	 ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=1 CCP_S1=0
//	 ACC |= CCP_S0;                  //(P3.4/ECI_2, P3.5/CCP0_2, P3.6/CCP1_2, P3.7/CCP2_2)
//	 P_SW1 = ACC;  
  
	  ACC = P_SW1;
	  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=1
	  ACC |= CCP_S1;                  //(P2.4/ECI_3, P2.5/CCP0_3, P2.6/CCP1_3, P2.7/CCP2_3)
	  P_SW1 = ACC;  

    CCON = 0;                       //��ʼ��PCA���ƼĴ���
                                    //PCA��ʱ��ֹͣ
                                    //���CF��־
                                    //���ģ���жϱ�־
    CL = 0;                         //��λPCA�Ĵ���
    CH = 0;
    CMOD = 0x02;                    //����PCAʱ��Դ
									//��ֹPCA��ʱ������ж�
	//ע�⣬3��PWMƵ����һ����

    PCA_PWM1 = 0x00;                //PCAģ��1������8λPWM
    CCAP1H = CCAP1L = 192;         //PWM1��ռ�ձ�Ϊ25%	(256-192)/256=25% 
    CCAPM1 = 0x42;  
	
	//����P2^7Ϊ�������
	P2M1 &= 0x7f;	//P25��0��0111 1111		
	P2M0 |= 0x80;	//P25��1��1000 0000

	PCA_PWM2 = 0x00;                //PCAģ��2������8λPWM
    CCAP2H = CCAP2L = 0xff;         //PWM2��ռ�ձ�Ϊ1/256 ((256-255)/256)
    CCAPM2 = 0x42;                                
                                    
//    PCA_PWM0 = 0x00;                //PCAģ��0������8λPWM
//    CCAP0H = CCAP0L = 0x20;         //PWM0��ռ�ձ�Ϊ87.5% ((100H-20H)/100H)
//    CCAPM0 = 0x42;                  //PCAģ��0Ϊ8λPWMģʽ
//
//    PCA_PWM1 = 0x40;                //PCAģ��1������7λPWM
//    CCAP1H = CCAP1L = 0x20;         //PWM1��ռ�ձ�Ϊ75% ((80H-20H)/80H)
//    CCAPM1 = 0x42;                  //PCAģ��1Ϊ7λPWMģʽ
//
//    PCA_PWM2 = 0x80;                //PCAģ��2������6λPWM
//    CCAP2H = CCAP2L = 0x20;         //PWM2��ռ�ձ�Ϊ50% ((40H-20H)/40H)
//    CCAPM2 = 0x42;                  //PCAģ��2Ϊ6λPWMģʽ

    CR = 1;                         //PCA��ʱ����ʼ����
}

