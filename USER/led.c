

#include "led.h"

void LED_Config(void)
{
	GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Slow);
}

void LED_Off(void)
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_4);
}

void LED_Toggle(void)
{
	GPIO_ToggleBits(GPIOD,GPIO_Pin_4);
}
