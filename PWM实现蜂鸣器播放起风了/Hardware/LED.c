#include "LED.h"

void LED_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruc;
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB , ENABLE);
	
	GPIO_InitStruc.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruc.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruc.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStruc);
}

void LED_OFF(void) {
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
}

void LED_ON(void) {
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
}