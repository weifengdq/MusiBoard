#ifndef __GB3232_H__
#define __GB3232_H__

/* 汉字字模的数据结构定义*/
struct  typFNT_GB32                 // 汉字字模数据结构 
{
       unsigned char  Index[3];               // 汉字内码索引	
       unsigned char   Msk[128];                        // 点阵码数据 
};

extern code struct  typFNT_GB32 codeGB_32[]; 

#endif