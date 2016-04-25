#ifndef __LKB_H__
#define __LKB_H__

#include "config0.h"

typedef struct{
	unsigned int key;
	unsigned int msg;
}KEYMSG;

extern bit key_up;

void sendMsg(KEYMSG* keymsg, key, msg);

void LKB_Init();
void Buzzer_Func(unsigned int Freq,unsigned int time);
void Key_Scan(unsigned int xiaodou,unsigned char mode);

#endif
