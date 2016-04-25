#include "gb3232.h"
#include "lcd.h"

/////////////////////////////////////////////////////////////////////////
// 汉字字模表(超过16个汉字，请修改PutGB3232函数)                                                          //
// 汉字库: 楷体32.dot,横向取模左高位,数据排列:从左到右从上到下         //
/////////////////////////////////////////////////////////////////////////
code struct  typFNT_GB32 codeGB_32[] =          // 数据表 
{
"青",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,
0x00,0x01,0x80,0x00,0x00,0x01,0x98,0x00,0x00,0x03,0xFC,0x00,0x00,0x7F,0x00,0x00,
0x00,0x01,0x00,0x00,0x00,0x01,0xF8,0x00,0x00,0x3F,0x80,0x00,0x00,0x01,0x00,0x00,
0x00,0x01,0x0F,0xF8,0x00,0x0F,0xFE,0x18,0x07,0xFC,0x00,0x00,0x1F,0x00,0x30,0x00,
0x00,0x03,0xFC,0x00,0x00,0x1C,0x1C,0x00,0x00,0x10,0x18,0x00,0x00,0x11,0xC8,0x00,
0x00,0x1E,0x08,0x00,0x00,0x10,0x0C,0x00,0x00,0x11,0xCC,0x00,0x00,0x1F,0x0C,0x00,
0x00,0x30,0x0C,0x00,0x00,0x30,0x0C,0x00,0x00,0x30,0x0C,0x00,0x00,0x60,0x7C,0x00,
0x00,0x60,0x38,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"青",0*/

"春",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x03,0x00,0x00,
0x00,0x03,0x00,0x00,0x00,0x03,0x7E,0x00,0x00,0x07,0xF0,0x00,0x00,0x1E,0x00,0x00,
0x00,0x02,0x00,0x00,0x00,0x06,0xF8,0x00,0x00,0x7F,0xC0,0x00,0x00,0x36,0x00,0x00,
0x00,0x04,0x0E,0x00,0x00,0x0D,0xFF,0x00,0x01,0xFF,0xC0,0x00,0x01,0xC8,0x20,0x00,
0x00,0x18,0x10,0x00,0x00,0x30,0x0C,0x00,0x00,0x30,0x76,0x00,0x00,0x7F,0xFB,0x80,
0x00,0xD8,0x19,0xC0,0x01,0x98,0x18,0xF8,0x03,0x19,0x98,0x7E,0x0C,0x1F,0x98,0x00,
0x18,0x18,0x18,0x00,0x00,0x18,0x18,0x00,0x00,0x18,0xD8,0x00,0x00,0x1F,0xF8,0x00,
0x00,0x18,0x38,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,/*"春",1*/
	
};
		