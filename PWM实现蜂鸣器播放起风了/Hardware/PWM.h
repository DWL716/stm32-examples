#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

void PWM_Init(void);
void PWM_SetComparel(uint16_t comparel);
void startTone(float freq);
void stopTone();
void musicalPWM(void);

#endif
