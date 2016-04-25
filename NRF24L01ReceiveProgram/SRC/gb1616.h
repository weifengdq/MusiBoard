#ifndef __GB1616_H__
#define __GB1616_H__

#include "config0.h"

/* 汉字字模的数据结构定义 */
struct  typFNT_GB16                 // 汉字字模数据结构 
{
       unsigned char  Index[3];               // 汉字内码索引	
       unsigned char   Msk[32];                        // 点阵码数据 
}; 

extern code struct typFNT_GB16 codeGB_16[];

#endif

