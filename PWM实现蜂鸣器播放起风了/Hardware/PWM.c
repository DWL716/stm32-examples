#include "PWM.h"
#include <stdbool.h>
#include <math.h>


/** 步骤
1. RCC开启时钟
2. 配置时基单元，包括时钟源选择
3. 配置输出比较单元，包括CCR值、输出比较模式、极性选择、输出使能
4. 配置GPIO，需参考时钟的引脚定义表
5. 运行控制
*/ 
// Taken from http://www.phy.mtu.edu/~suits/notefreqs.html
const float ToneFreq[] = {
    1046.50, // C6
    1174.66, // D6
    1318.51, // E6
    1396.91, // F6
    1567.98, // G6
    1760.00, // A6
    1975.53, // B6
    2093.00, // C7
    -1 // End of array (if -1, then restart array index)
};

#define __TIM_USEC_TICK(PERIOD) ((SystemCoreClock / 1000000UL) * PERIOD)
#define __PSC(PERIOD)           (((__TIM_USEC_TICK(PERIOD) + 49999UL) / 50000UL) - 1)
#define __ARR(PERIOD)           ((__TIM_USEC_TICK(PERIOD) / (__PSC(PERIOD) + 1)) - 1)

uint16_t ButtonPressed = 0;

void PWM_Init(void) {

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 初始化GPIOA pin2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_InternalClockConfig(TIM2);
		long one_sec = 1 * 1000 * 1000;
    // 初始化时基单元
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = __ARR(one_sec); // arr
    TIM_TimeBaseInitStructure.TIM_Prescaler = __PSC(one_sec); // psc
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    // TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    // TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE)

    // 初始化输出比较单元
    TIM_OCStructInit(&TIM_OCInitStruct); // 结构体初始值
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // 输出比较模式
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; // 输出比较极性
    TIM_OCInitStruct.TIM_Pulse = 0; // 脉冲，时钟CCR值
    TIM_OCInitStruct.TIM_OutputState = ENABLE; // 输出状态
    TIM_OC3Init(TIM2, &TIM_OCInitStruct);

    TIM_Cmd(TIM2, ENABLE);
}


void PWM_SetComparel(uint16_t comparel) {
    TIM_SetCompare3(TIM2,comparel);
}

uint16_t index1 = 0;

float freq = 0;


void musicalPWM(void) {
	freq = ToneFreq[index1++];
	if (freq == -1) {
    index1 = 0;
  }
	
	float usec = 1000000 / freq;
	
	TIM2->PSC = round(__PSC(usec));
  TIM2->ARR = round(__ARR(usec));
	
	TIM_SetCompare3(TIM2,freq / 2.0);
}

void startTone(float freq) {
    // 1 cycle == 2 toggles
	TIM_SetCompare3(TIM2,freq / 2.0);
    //TIM2_StartFrequency(freq / 2.0);
    //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

void stopTone() {
    // Stop Tone Gen
    TIM2->CR1 &= ~TIM_CR1_CEN;
    // Turn off buzzer
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	
}

void TIM2_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF;
        stopTone();
    }
}
