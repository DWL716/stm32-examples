#include "beep2.h"

// 音调 --- pwm占空比
int a[] = {
	8, 7, 8, 7, 8, 9, 11, 9,
	8, 7, 8, 7, 8, 9, 8, 7, 4,
	8, 7, 8, 7, 8, 9, 11, 9,
	8, 9, 8, 7, 8, -1,
	8, 7, 8, 7, 8, 9, 11, 9,
	8, 9, 8, 7, 5, 9, 8, 7, 8,
	7, 9, 8, 7, 8, 7, 4, 9, 8, 7, 8,
	7, 7, 8, 9, 7,
	12, 11, 12, 12, 8, 13, 12, 13, 13, -1, // 0
	13, 12, 13, 13, 9, 14, 15, 14, 13, 12, 11,
	12, 11, 12, 12, 11, 12, 11, 12, 11, 8, 8, 11,
	9, 7, 8, 9, 7,
	12, 11, 12, 12, 7, 13, 12, 13, 13, -1, // 0
	13, 12, 13, 13, 9, 14, 15, 14, 13, 12, 11,
	12, 16, 16, 16, 11, 12, 16, 16, 16, 11, 12,
	9, // 颜。
};
// 时间
int tm[] = {
	3, 1, 3, 1, 2, 2, 2, 2,
	3, 1, 3, 1, 1, 1, 1, 1, 4,
	3, 1, 3, 1, 2, 2, 2, 2,
	3, 1, 2, 2, 4, 2, // 8
	3, 1, 3, 1, 2, 2, 2, 2,
	3, 1, 2, 2, 4, 1, 1, 1, 1,
	4, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1,
	8, 2, 2, 2, 2,
	2, 1, 1, 3, 1, 2, 1, 1, 4, 1, // 0
	2, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2,
	2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 3,
	8, 2, 2, 2, 2,
	2, 1, 1, 3, 1, 2, 1, 1, 4, 1, // 0
	2, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2,
	2, 1, 1, 2, 2, 2, 1, 1, 1, 2, 1,
	4 // 12
};

// 低音 0 - 6
// 中音 7-13
// 高音 14- 21
uint16_t tone[21] = {
	3817, 3401, 3030, 2865, 2551, 2272, 2024,
	1912, 1703, 1517, 1432, 1275, 1136, 1012,
	956, 851, 758, 715, 637, 568, 506};

void Music_init2(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_InternalClockConfig(TIM2);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 3816 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 3816 / 2; // CCR
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_Cmd(TIM2, ENABLE);
};

void Sound_SetHZ(int16_t a)
{
	if (a == -1)
	{
		TIM_SetAutoreload(TIM2, 3817);
		TIM_SetCompare3(TIM2, 3817);
	}
	else
	{
		TIM_SetAutoreload(TIM2, a);
		TIM_SetCompare3(TIM2, a / 2);
	}
}

void B_Music(void)
{
	int i = 0;
	// OLED_Init();

	OLED_ShowString(2, 1, "tone:");
	OLED_ShowString(1, 1, "Start");

	for (i = 0; i < 132; i++)
	{

		if (a[i] == -1)
		{
			Sound_SetHZ(-1);
			Delay_ms(800 * tm[i] / 4);
		}
		else
		{
			OLED_ShowNum(2, 6, tone[a[i]], 4);
			Sound_SetHZ(tone[a[i]]);
			Delay_ms(800 * tm[i] / 4);
		}
	}
	OLED_ShowString(1, 1, "end");
	TIM_SetCompare3(TIM2, 3817);
	Delay_ms(1000);
};
