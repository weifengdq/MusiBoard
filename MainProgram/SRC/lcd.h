#ifndef __LCD_H__
#define __LCD_H__

#include "config0.h"
//定义LCD的尺寸	
//初始化中可修改横屏竖屏 竖屏时坐标左上角为(0,0)
#define LCD_W 240	
#define LCD_H 320

#define SPI_S0 0x04              //P_SW1.2
#define SPI_S1 0x08              //P_SW1.3

//颜色定义
#define Red			0xf800	//FF0000:11111 000000 00000
#define Orange      0xfd20  //FFA500:11111 101001 00000？？？有点问题
#define Yellow		0xffe0	//FFFF00:11111 111111 00000
#define Green 	 	0x07e0	//008000:00000 100010 00000
#define Cyan        0x07ff	//00FFFF:00000 111111 11111
#define Blue 		0x001f	//0000FF:00000 000000 11111
#define Purple		0xf81f	//800080:				   ？？？
#define Black		0x0000
#define White		0xffff

extern unsigned int BACK_COLOR, POINT_COLOR;   //背景色，画笔色

void SPI_Init();
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);//先设置绘点窗口才写数据么？
void LCD_Writ_Bus(char a);
void LCD_WRITE_DATA(int a);
void LCD_Clear(unsigned int Color);
void LCD_DrawPoint(unsigned int x,unsigned int y);
void LCD_DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_DrawCircle(unsigned int x0,unsigned int y0,unsigned char r);
void LCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);
void LCD_Init();
void LCD_Sleep();
void LCD_ExitSleep();

void LCD_ShowChar(unsigned int x,unsigned int  y,unsigned char num,unsigned char mode);
unsigned long mypow(unsigned char m,unsigned char n);
void LCD_ShowNum(unsigned int  x,unsigned int  y,unsigned long num,unsigned char len);
void LCD_Show2Num(unsigned int  x,unsigned int  y,unsigned int  num,unsigned char len);
void LCD_ShowString(unsigned int  x,unsigned int  y,const unsigned char *s);
void LCD_ShowGB3232(unsigned short x, unsigned short y, unsigned char *s);
void LCD_ShowImage(unsigned char *p,unsigned int x_axis,unsigned int y_axis,unsigned int length,unsigned int width);

#endif
