

#include "tim.h"

#define TIM_COUNT_NUM (125 - 1)


void Tim4_Config(void)
{
	/* Enable TIM4 clock */
	CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
									  
	/* Time Base configuration */
	TIM4_TimeBaseInit(TIM4_Prescaler_8, TIM_COUNT_NUM);
	
	/* Clear TIM4 update flag */
	TIM4_ClearFlag(TIM4_FLAG_Update);
	
	/* Enable update interrupt */
	TIM4_ITConfig(TIM4_IT_Update, ENABLE);

	ITC_SetSoftwarePriority(TIM4_UPD_OVF_TRG_IRQn, ITC_PriorityLevel_2);
	TIM4_Cmd(ENABLE);
}


