
#include "lcd.h"

/****************************************************************

Duty：1/COM的数量   
Bias：偏压
LCD frequency = (clock source * Duty) / (Prescaler * Divider)
LCD频率 = 38000 * 1 / 4 / (2 * 18) = 264Hz
帧频 = 264 / 4 = 66 Hz
**********************************************************************/
void LCD_Init(void)
{
	CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);
	CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);

	LCD_Init(LCD_Prescaler_2, LCD_Divider_18, LCD_Duty_1_4, LCD_Bias_1_3, LCD_VoltageSource_Internal);
	
}

