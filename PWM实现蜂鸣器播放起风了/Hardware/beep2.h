#ifndef __BEEP2_H__
#define __BEEP2_H__
#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"

void Music_init2(void);
void Sound_SetHZ(int16_t a);
void Play_Music(int a,int b,int c);
void B_Music(void);

#endif
