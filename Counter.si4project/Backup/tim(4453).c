

#include "tim.h"


void Tim2_Config(void)
{
	/* Enable TIM2 clock */
	CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
	
	/* Remap TIM2 ETR to LSE: TIM2 external trigger becomes controlled by LSE clock */
//	SYSCFG_REMAPPinConfig(REMAP_Pin_TIM2TRIGLSE, ENABLE);
	
	/* Enable LSE clock */
//	CLK_LSEConfig(CLK_LSE_ON);
	/* Wait for LSERDY flag to be reset */
//	while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
	
	/* TIM2 configuration:
	   - TIM2 ETR is mapped to LSE
	   - TIM2 counter is clocked by LSE div 4
		so the TIM2 counter clock used is LSE / 4 = 32.768 / 4 = 8.192 KHz
	   TIM2 Channel1 output frequency = TIM2CLK / (TIM2 Prescaler * (TIM2_PERIOD + 1))
									  = 8192 / (1 * 8) = 1024 Hz				*/
									  
	/* Time Base configuration */
	TIM2_TimeBaseInit(TIM2_Prescaler_1, TIM2_CounterMode_Up, TIM2_PERIOD);
	TIM2_ETRClockMode2Config(TIM2_ExtTRGPSC_DIV4, TIM2_ExtTRGPolarity_NonInverted, 0);
	
	TIM2_UpdateRequestConfig(TIM2_UpdateSource_Global);
	
	/* Clear TIM2 update flag */
	TIM2_ClearFlag(TIM2_FLAG_Update);
	
	/* Enable update interrupt */
	TIM2_ITConfig(TIM2_IT_Update, ENABLE);
	
	TIM2_Cmd(ENABLE);
}


