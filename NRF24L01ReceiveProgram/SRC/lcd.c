#include "lcd.h"
#include "delay.h"
#include "ascii.h"
#include "gb1616.h"
#include "gb3232.h"
#include "image0.h"

unsigned int BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ 

void SPI_Init()
{
//    ACC = P_SW1;
//    ACC &= ~(SPI_S0 | SPI_S1);    
//    P_SW1 = ACC;                
	    
	ACC = P_SW1;
	ACC &= ~(SPI_S0 | SPI_S1);  
	ACC |= SPI_S0;               
	P_SW1 = ACC;  
//  
//  ACC = P_SW1;
//  ACC &= ~(SPI_S0 | SPI_S1);    //S1_S0=0 S1_S1=1
//  ACC |= SPI_S1;               //(P1.6/RxD_3, P1.7/TxD_3)
//  P_SW1 = ACC;  

	//SPI��ʼ��
	SPDAT = 0;
	SPCTL = 0xd0;	//����1101,0000	  //SS��LCD_CS
	SPSTAT = 0xc0;	//��־λд1����
	//LCD Reset
	LCD_RST=1;
	Delay_ms(1);
	LCD_RST=0;
	Delay_ms(10);
	LCD_RST=1;
	Delay_ms(120);
}

void LCD_Writ_Bus(char a){   //����д�뺯�� 8λ
      LCD_CS = 0;
      SPDAT = a;//��ʼ����
	  while(!(SPSTAT&0x80));  // �ȴ�SPIF = 1,�������  
	  SPSTAT = 0xc0;    
      LCD_CS = 1;
}	

void LCD_WRITE_CMD(char cmd){//8 bit
      LCD_DC = 0;
      LCD_Writ_Bus(cmd);
}

void LCD_WRITE_COM_DATA(char com_data){//8 bit
      LCD_DC = 1;
      LCD_Writ_Bus(com_data);
}

void LCD_WRITE_DATA(int a){//16λ����,�����Σ�ÿ����8λ
      LCD_DC = 1;
      LCD_Writ_Bus(a>>8);
	  LCD_Writ_Bus(a);
}

//���û�㴰�ڣ�x=0~239,y=0~319
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
     	LCD_WRITE_CMD(0x2a); 	//x��   
     	LCD_WRITE_DATA(x1);
     	LCD_WRITE_DATA(x2);
	    LCD_WRITE_CMD(0x2b); 	//y��   
	    LCD_WRITE_DATA(y1);
     	LCD_WRITE_DATA(y2);
     	LCD_WRITE_CMD(0x2c);
}

//��������
//Color:Ҫ���������ɫ
void LCD_Clear(unsigned int Color)
{
	unsigned int i,j;	
	Address_set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	{
	  for (j=0;j<LCD_H;j++)
	  {
      	LCD_WRITE_DATA(Color);
	  }
	}
}

//����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(unsigned int x,unsigned int y)
{
	Address_set(x,y,x,y);//���ù��λ�� 
	LCD_WRITE_DATA(POINT_COLOR); 	    
} 

//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	unsigned int t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}

//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶 (Һ�����ֱ���320*240�����ᳬ��255�������unsigned char����)
void LCD_DrawCircle(unsigned int x0,unsigned int y0,unsigned char r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}     

//��ָ�����������ָ����ɫ
//�����С:
//  (xend-xsta)*(yend-ysta)
void LCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color)
{          
	unsigned int i,j; 
	Address_set(xsta,ysta,xend,yend);      //���ù��λ�� 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)
			LCD_WRITE_DATA(color);//���ù��λ�� 	    
	} 					  	    
}

//Һ�����س�ʼ��
void LCD_Init()	//Initial_ILI9340C(void)
{
        LCD_WRITE_CMD(0xCB);  
        LCD_WRITE_COM_DATA(0x39); 
        LCD_WRITE_COM_DATA(0x2C); 
        LCD_WRITE_COM_DATA(0x00); 
        LCD_WRITE_COM_DATA(0x34); 
        LCD_WRITE_COM_DATA(0x02); 

        LCD_WRITE_CMD(0xCF);  
        LCD_WRITE_COM_DATA(0x00); 
        LCD_WRITE_COM_DATA(0XC1); 
        LCD_WRITE_COM_DATA(0X30); 
 
        LCD_WRITE_CMD(0xE8);  
        LCD_WRITE_COM_DATA(0x85); 
        LCD_WRITE_COM_DATA(0x00); 
        LCD_WRITE_COM_DATA(0x78); 
 
        LCD_WRITE_CMD(0xEA);  
        LCD_WRITE_COM_DATA(0x00); 
        LCD_WRITE_COM_DATA(0x00); 
 
        LCD_WRITE_CMD(0xED);  
        LCD_WRITE_COM_DATA(0x64); 
        LCD_WRITE_COM_DATA(0x03); 
        LCD_WRITE_COM_DATA(0X12); 
        LCD_WRITE_COM_DATA(0X81); 

        LCD_WRITE_CMD(0xF7);  
        LCD_WRITE_COM_DATA(0x20); 
  
        LCD_WRITE_CMD(0xC0);    //Power control 
        LCD_WRITE_COM_DATA(0x23);   //VRH[5:0] 
 
        LCD_WRITE_CMD(0xC1);    //Power control 
        LCD_WRITE_COM_DATA(0x10);   //SAP[2:0];BT[3:0] 
 
        LCD_WRITE_CMD(0xC5);    //VCM control 
        LCD_WRITE_COM_DATA(0x3e); //�Աȶȵ���
        LCD_WRITE_COM_DATA(0x28); 
 
        LCD_WRITE_CMD(0xC7);    //VCM control2 
        LCD_WRITE_COM_DATA(0x86);  //--
 
        LCD_WRITE_CMD(0x36);    // Memory Access Control 
        LCD_WRITE_COM_DATA(0x48);  //C8	   //48 68����//28 E8 ����

        LCD_WRITE_CMD(0x3A);    
        LCD_WRITE_COM_DATA(0x55); 

        LCD_WRITE_CMD(0xB1);    
        LCD_WRITE_COM_DATA(0x00);  
        LCD_WRITE_COM_DATA(0x18); 
 
        LCD_WRITE_CMD(0xB6);    // Display Function Control 
        LCD_WRITE_COM_DATA(0x08); 
        LCD_WRITE_COM_DATA(0x82);
        LCD_WRITE_COM_DATA(0x27);  
 
        LCD_WRITE_CMD(0xF2);    // 3Gamma Function Disable 
        LCD_WRITE_COM_DATA(0x00); 
 
        LCD_WRITE_CMD(0x26);    //Gamma curve selected 
        LCD_WRITE_COM_DATA(0x01); 
 
        LCD_WRITE_CMD(0xE0);    //Set Gamma 
        LCD_WRITE_COM_DATA(0x0F); 
        LCD_WRITE_COM_DATA(0x31); 
        LCD_WRITE_COM_DATA(0x2B); 
        LCD_WRITE_COM_DATA(0x0C); 
        LCD_WRITE_COM_DATA(0x0E); 
        LCD_WRITE_COM_DATA(0x08); 
        LCD_WRITE_COM_DATA(0x4E); 
        LCD_WRITE_COM_DATA(0xF1); 
        LCD_WRITE_COM_DATA(0x37); 
        LCD_WRITE_COM_DATA(0x07); 
        LCD_WRITE_COM_DATA(0x10); 
        LCD_WRITE_COM_DATA(0x03); 
        LCD_WRITE_COM_DATA(0x0E); 
        LCD_WRITE_COM_DATA(0x09); 
        LCD_WRITE_COM_DATA(0x00); 

        LCD_WRITE_CMD(0XE1);    //Set Gamma 
        LCD_WRITE_COM_DATA(0x00); 
        LCD_WRITE_COM_DATA(0x0E); 
        LCD_WRITE_COM_DATA(0x14); 
        LCD_WRITE_COM_DATA(0x03); 
        LCD_WRITE_COM_DATA(0x11); 
        LCD_WRITE_COM_DATA(0x07); 
        LCD_WRITE_COM_DATA(0x31); 
        LCD_WRITE_COM_DATA(0xC1); 
        LCD_WRITE_COM_DATA(0x48); 
        LCD_WRITE_COM_DATA(0x08); 
        LCD_WRITE_COM_DATA(0x0F); 
        LCD_WRITE_COM_DATA(0x0C); 
        LCD_WRITE_COM_DATA(0x31); 
        LCD_WRITE_COM_DATA(0x36); 
        LCD_WRITE_COM_DATA(0x0F); 
 
        LCD_WRITE_CMD(0x11);    //Exit Sleep 
        Delay_ms(120); 
				
        LCD_WRITE_CMD(0x29);    //Display on 
        LCD_WRITE_CMD(0x2c); 

}

//LCD��������״̬
void LCD_Sleep()	//_ILI9340C(void)
{
  LCD_WRITE_CMD(0x28);//Display off
  Delay_ms(20);
  LCD_WRITE_CMD(0x10);//Enter Sleep mode
}
//LCD�˳�����״̬
void LCD_ExitSleep()	//_ILI9340C(void)
{
  LCD_WRITE_CMD(0x11);//Exit Sleep
  Delay_ms(120);
  LCD_WRITE_CMD(0x29);//Display on
  LCD_WRITE_CMD(0x2c); 
}

//��ָ��λ����ʾһ���ַ�
//num:Ҫ��ʾ���ַ�:" "--->"~"
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//��ָ��λ����ʾһ���ַ�
//num:Ҫ��ʾ���ַ�:" "--->"~"
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(unsigned int x,unsigned int  y,unsigned char num,unsigned char mode)
{
    unsigned char temp;
    unsigned char pos,t;
	unsigned int  x0=x;
	unsigned int  colortemp=POINT_COLOR;      
    if(x>LCD_W-16||y>LCD_H-16)return;	    
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	Address_set(x,y,x+8-1,y+16-1);      //���ù��λ�� 
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(unsigned int )num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_WRITE_DATA(POINT_COLOR);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(unsigned int )num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   

//m^n����
unsigned long mypow(unsigned char m,unsigned char n)
{
	unsigned long result=1;	 
	while(n--)result*=m;    
	return result;
}
			 
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//num:��ֵ(0~4294967295);	
void LCD_ShowNum(unsigned int  x,unsigned int  y,unsigned long num,unsigned char len)
{         	
	unsigned char t,temp;
	unsigned char enshow=0;
	num=(unsigned int )num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ',0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0); 
	}
} 

//��ʾ2������
//x,y:�������
//num:��ֵ(0~65535);	 
void LCD_Show2Num(unsigned int  x,unsigned int  y,unsigned int  num,unsigned char len)
{         	
	unsigned char t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+8*t,y,temp+'0',0); 
	}
}

/*��ʾһ��GB1616����*/
void LCD_PutGB1616(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int fColor,unsigned int bColor)
{
	unsigned int i,j,k;

	Address_set(x,y,x+16-1,y+16-1);
	for (k=0;k<64;k++) 
	{ //64��ʾ�Խ����ֿ��е���������ѭ����ѯ����(���������30�����֣��˴�д30Ҳ����)
		if ((codeGB_16[k].Index[0]==c[0])&&(codeGB_16[k].Index[1]==c[1]))
		{ 
    		for(i=0;i<32;i++) 
			{
		  		unsigned short m=codeGB_16[k].Msk[i];
		  		for(j=0;j<8;j++) 
				{
					if((m&0x80)==0x80) 
					{
						LCD_WRITE_DATA(fColor);
					}
					else 
					{
						LCD_WRITE_DATA(bColor);
					}
					m<<=1;
				} 
	  		}
		}  
  	}	
}

//��ʾ�ַ���
//x,y:�������  
//*s:�ַ�����ʼ��ַ
//��16����
void LCD_ShowString(unsigned short x, unsigned short y, unsigned char *s)
{
	unsigned char i=0;
	while(*s) 
	{
		if( *s < 0x80) 	   //��ʾASCII�ַ�(Ӣ�ļ����ţ�
		{
			//һ��д�����Զ�����
			if(x>LCD_W-16){x=0;y+=16;}
		    if(y>LCD_H-16){y=x=0;}
	        LCD_ShowChar(x,y,*s,0);
	        x+=8;
	        s++;
		}
		else			   //��ʾ����
		{	
			if(x>LCD_W-32){x=0;y+=32;}	  //����ò�����ﲻ�ԣ������Զ����У�
	    	if(y>LCD_H-32){y=x=0;}							
			LCD_PutGB1616(x+i*8,y,(unsigned char*)s,POINT_COLOR,BACK_COLOR);
			s+=2;i+=2;
		}
	}
}

/*��ʾһ��GB3232����*/
void LCD_PutGB3232(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int fColor,unsigned int bColor)
{
	unsigned int i,j,k;

	Address_set(x, y,  x+32-1, y+32-1);
	for (k=0;k<15;k++) 
	{ //15��ʾ�Խ����ֿ��еĸ�����ѭ����ѯ����(����ֻ����2���֣�д2Ҳ��)
		if ((codeGB_32[k].Index[0]==c[0])&&(codeGB_32[k].Index[1]==c[1]))
		{ 
    		for(i=0;i<128;i++) 
			{
		  		unsigned short m=codeGB_32[k].Msk[i];
		  		for(j=0;j<8;j++) 
				{
					if((m&0x80)==0x80) 
					{
						LCD_WRITE_DATA(fColor);
					}
					else 
					{
						LCD_WRITE_DATA(bColor);
					}
					m<<=1;
				} 
		  	}
		}  
	 }	
}

void LCD_ShowGB3232(unsigned short x, unsigned short y, unsigned char *s)
{
	unsigned char i=0;
	while(*s)
	{
		LCD_PutGB3232(x+i*16,y,(unsigned char*)s,POINT_COLOR,BACK_COLOR);
		s+=2;i+=2;
	}
}


void LCD_ShowImage(unsigned char *p,unsigned int x_axis,unsigned int y_axis,unsigned int length,unsigned int width)
{
	unsigned int i;
	Address_set(x_axis, y_axis , x_axis+length-1 , y_axis+width-1);		//��������
	for(i=0;i<length*width;i++)
	{
		LCD_DC=1;
		LCD_Writ_Bus( *(p+i*2+1) );
		LCD_Writ_Bus( *(p+i*2)   );
	}
}
