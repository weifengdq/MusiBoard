#ifndef __GB3232_H__
#define __GB3232_H__

/* ������ģ�����ݽṹ����*/
struct  typFNT_GB32                 // ������ģ���ݽṹ 
{
       unsigned char  Index[3];               // ������������	
       unsigned char   Msk[128];                        // ���������� 
};

extern code struct  typFNT_GB32 codeGB_32[]; 

#endif