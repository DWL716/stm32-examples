#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "beep2.h"

uint8_t i;

int main(void) {
	OLED_Init();
	//PWM_Init();
	Music_init2();
	OLED_ShowString(1,1, "-----");
	while(1){
		//musicalPWM();
		B_Music();
	}
}
