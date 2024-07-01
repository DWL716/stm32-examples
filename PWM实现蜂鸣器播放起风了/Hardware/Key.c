#include "Key.h"


void Key_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	// GPIO_InitStructure.GPIO_Speed = 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


uint8_t Key_GetNum(void) {
		uint8_t KeyNum = 0;
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) {
			Delay_ms(20);
			KeyNum = 1;
		}
	
		return KeyNum;
}
