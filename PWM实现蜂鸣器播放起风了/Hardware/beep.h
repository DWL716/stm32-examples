#ifndef __BEEP__H
#define __BEEP__H

#include "stdlib.h"	  
#include "Delay.h"

//定义GPIOB的位地址变量宏，位输入宏，输出宏
#define PBeep PBout(5)

void BEEP_Init(void);
void Sound(uint16_t frq);
void Sound2(u16 time);
void play_music(void);
void play_successful(void);
void play_failed(void);

#endif


