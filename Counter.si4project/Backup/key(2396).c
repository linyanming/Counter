
#include "key.h"


void KeyInit(void)
{
	GPIO_Init(GPIOC, GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_In_FL_IT);
	EXTI_SetPinSensitivity(EXTI_Pin_5, EXTI_Trigger_Rising_Falling);
	EXTI_SetPinSensitivity(EXTI_Pin_6, EXTI_Trigger_Rising_Falling);
}

