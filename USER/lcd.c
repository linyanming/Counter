
#include "lcd.h"


/****************************************************************
LCD初始化函数

Duty：1/COM的数量   
Bias：偏压
LCD frequency = (clock source * Duty) / (Prescaler * Divider)
LCD频率 = 38000 * 1 / 4 / (2 * 18) = 264Hz
帧频 = 264 / 4 = 66 Hz
**********************************************************************/
void LCD_Config(void)
{
	CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);
	CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);

	LCD_Init(LCD_Prescaler_1, LCD_Divider_16, LCD_Duty_1_4, LCD_Bias_1_3, LCD_VoltageSource_External);

	LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0x00);
	LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0xff);
	LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0x01);
	LCD_PortMaskConfig(LCD_PortMaskRegister_3, 0x00);

	LCD_ContrastConfig(LCD_Contrast_Level_4);
	LCD_DeadTimeConfig(LCD_DeadTime_0);
	LCD_PulseOnDurationConfig(LCD_PulseOnDuration_1);

	LCD_Cmd(ENABLE);
}

void LCD_Deconfig(void)
{
	LCD_DeInit();
	LCD_Cmd(DISABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_LCD, DISABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_RTC, DISABLE);
}

void LCD_ClearAll(void)
{
	LCD->RAM[LCD_RAMRegister_1] = 0x00;
	LCD->RAM[LCD_RAMRegister_4] = 0x00;
	LCD->RAM[LCD_RAMRegister_8] = 0x00;
	LCD->RAM[LCD_RAMRegister_11] = 0x00;
	LCD->RAM[LCD_RAMRegister_2] = 0x00;
	LCD->RAM[LCD_RAMRegister_5] = 0x00;
	LCD->RAM[LCD_RAMRegister_9] = 0x00;
	LCD->RAM[LCD_RAMRegister_12] = 0x00;
}

/***********************************************************
电池显示函数
功能：
	显示电池电量
参数：
	bt：电池电量数量 最大为3

返回值：
	无
************************************************************/

void LCD_WriteBaterry(uint8_t bt)
{
	switch(bt)
	{
		case 0:
			LCD->RAM[LCD_RAMRegister_2] &= (~0x01);
			LCD->RAM[LCD_RAMRegister_2] |= 0x01;
			LCD->RAM[LCD_RAMRegister_12] &= (~0x10);
			LCD->RAM[LCD_RAMRegister_5] &= (~0x10);
			LCD->RAM[LCD_RAMRegister_9] &= (~0x01);
			break;
		case 1:
			LCD->RAM[LCD_RAMRegister_2] &= (~0x01);
			LCD->RAM[LCD_RAMRegister_2] |= 0x01;
			LCD->RAM[LCD_RAMRegister_9] &= (~0x01);
			LCD->RAM[LCD_RAMRegister_9] |= 0x01;
			LCD->RAM[LCD_RAMRegister_12] &= (~0x10);
			LCD->RAM[LCD_RAMRegister_5] &= (~0x10);
			break;
		case 2:
			LCD->RAM[LCD_RAMRegister_2] &= (~0x01);
			LCD->RAM[LCD_RAMRegister_2] |= 0x01;
			LCD->RAM[LCD_RAMRegister_9] &= (~0x01);
			LCD->RAM[LCD_RAMRegister_9] |= 0x01;
			LCD->RAM[LCD_RAMRegister_12] &= (~0x10);
			LCD->RAM[LCD_RAMRegister_12] |= 0x10;
			LCD->RAM[LCD_RAMRegister_5] &= (~0x10);
			break;
		case 3:
			LCD->RAM[LCD_RAMRegister_12] &= (~0x10);
			LCD->RAM[LCD_RAMRegister_12] |= 0x10;
			LCD->RAM[LCD_RAMRegister_5] &= (~0x10);
			LCD->RAM[LCD_RAMRegister_5] |= 0x10;
			LCD->RAM[LCD_RAMRegister_2] &= (~0x01);
			LCD->RAM[LCD_RAMRegister_2] |= 0x01;
			LCD->RAM[LCD_RAMRegister_9] &= (~0x01);
			LCD->RAM[LCD_RAMRegister_9] |= 0x01;
			break;
		default:
			break;
	}
}

const u8 LCM_Num[10]={0xdd,0x88,0xe5,0xe9,0xb8,0x79,0x7d,0xc8,0xfd,0xf9};

/* 0    1   2  3   4  5   6  7 8  9 */

/***********************************************************
数字显示函数
功能：
	显示对应数码管的数字
参数：
	bNum:想要显示的数字
	pos：第几个数码管
	dot：是否显示点号

返回值：
	无
************************************************************/
#if 1
void LCD_WriteNum(uint8_t bNum,uint8_t pos,uint8_t dot)
{
	uint8_t number;
	if(bNum > 9)
	{
		bNum = 9;
	}
	if(dot && (pos != DSP4))
	{
		number = LCM_Num[bNum] + 0x02;
	}
	else
	{
		number = LCM_Num[bNum];
	}
	
	switch(pos)
	{
		case DSP1:
			//COM0      
			LCD->RAM[LCD_RAMRegister_1] &= (~0x03);
			LCD->RAM[LCD_RAMRegister_1] |= (u8)((number & 0xC0) >> 6);
			//COM1  
			LCD->RAM[LCD_RAMRegister_4] &= (~0x30);
			LCD->RAM[LCD_RAMRegister_4] |= (u8)((number & 0x30));
			//COM2  
			LCD->RAM[LCD_RAMRegister_8] &= (~0x03);
			LCD->RAM[LCD_RAMRegister_8] |= (u8)((number & 0x0C) >> 2);
			//COM3  
			LCD->RAM[LCD_RAMRegister_11] &= (~0x30);
			LCD->RAM[LCD_RAMRegister_11] |= (u8)((number & 0x03) << 4);
			break;
		case DSP2:
			//COM0      
			LCD->RAM[LCD_RAMRegister_1] &= (~0x0C);
			LCD->RAM[LCD_RAMRegister_1] |= (u8)((number & 0xC0) >> 4);
			//COM1  
			LCD->RAM[LCD_RAMRegister_4] &= (~0xC0);
			LCD->RAM[LCD_RAMRegister_4] |= (u8)((number & 0x30) << 2);
			//COM2  
			LCD->RAM[LCD_RAMRegister_8] &= (~0x0C);
			LCD->RAM[LCD_RAMRegister_8] |= (u8)((number & 0x0C));
			//COM3  
			LCD->RAM[LCD_RAMRegister_11] &= (~0xC0);
			LCD->RAM[LCD_RAMRegister_11] |= (u8)((number & 0x03) << 6);
			break;
		case DSP3:
			//COM0      
			LCD->RAM[LCD_RAMRegister_1] &= (~0x30);
			LCD->RAM[LCD_RAMRegister_1] |= (u8)((number & 0xC0) >> 2);
			//COM1  
			LCD->RAM[LCD_RAMRegister_5] &= (~0x03);
			LCD->RAM[LCD_RAMRegister_5] |= (u8)((number & 0x30) >> 4);
			//COM2  
			LCD->RAM[LCD_RAMRegister_8] &= (~0x30);
			LCD->RAM[LCD_RAMRegister_8] |= (u8)((number & 0x0C) << 2);
			//COM3  
			LCD->RAM[LCD_RAMRegister_12] &= (~0x03);
			LCD->RAM[LCD_RAMRegister_12] |= (u8)((number & 0x03));
			break;
		case DSP4:
			//COM0      
			LCD->RAM[LCD_RAMRegister_1] &= (~0xC0);
			LCD->RAM[LCD_RAMRegister_1] |= (u8)((number & 0xC0));
			//COM1  
			LCD->RAM[LCD_RAMRegister_5] &= (~0x0C);
			LCD->RAM[LCD_RAMRegister_5] |= (u8)((number & 0x30) >> 2);
			//COM2  
			LCD->RAM[LCD_RAMRegister_8] &= (~0xC0);
			LCD->RAM[LCD_RAMRegister_8] |= (u8)((number & 0x0C) << 4);
			//COM3  
			LCD->RAM[LCD_RAMRegister_12] &= (~0x0C);
			LCD->RAM[LCD_RAMRegister_12] |= (u8)((number & 0x03) << 2);
			break;
		default:
			break;
	}
}
#else
void LCD_WriteNum(uint8_t bNum,uint8_t pos,uint8_t dot)
{
	uint8_t number;
	if(dot && (pos != DSP4))
	{
		number = LCM_Num[bNum] + 0x02;
	}
	else
	{
		number = LCM_Num[bNum];
	}
	switch(pos)
	{
		case DSP1:
			//COM0      
			LCD->RAM[LCD_RAMRegister_1] &= (~0x03);
			LCD->RAM[LCD_RAMRegister_1] |= (u8)((number & 0x03));
			//COM1  
			LCD->RAM[LCD_RAMRegister_4] &= (~0x30);
			LCD->RAM[LCD_RAMRegister_4] |= (u8)((number & 0x0C) << 2);
			//COM2  
			LCD->RAM[LCD_RAMRegister_8] &= (~0x03);
			LCD->RAM[LCD_RAMRegister_8] |= (u8)((number & 0x30) >> 4);
			//COM3  
			LCD->RAM[LCD_RAMRegister_11] &= (~0x30);
			LCD->RAM[LCD_RAMRegister_11] |= (u8)((number & 0xC0) >> 2);
			break;
		case DSP2:
			//COM0      
			LCD->RAM[LCD_RAMRegister_1] &= (~0x0C);
			LCD->RAM[LCD_RAMRegister_1] |= (u8)((number & 0x03) << 2);
			//COM1  
			LCD->RAM[LCD_RAMRegister_4] &= (~0xC0);
			LCD->RAM[LCD_RAMRegister_4] |= (u8)((number & 0x0C) << 4);
			//COM2  
			LCD->RAM[LCD_RAMRegister_8] &= (~0x0C);
			LCD->RAM[LCD_RAMRegister_8] |= (u8)((number & 0x30) >> 2);
			//COM3  
			LCD->RAM[LCD_RAMRegister_11] &= (~0xC0);
			LCD->RAM[LCD_RAMRegister_11] |= (u8)((number & 0xC0));
			break;
		case DSP3:
			//COM0      
			LCD->RAM[LCD_RAMRegister_1] &= (~0x30);
			LCD->RAM[LCD_RAMRegister_1] |= (u8)((number & 0x03) << 4);
			//COM1  
			LCD->RAM[LCD_RAMRegister_5] &= (~0x03);
			LCD->RAM[LCD_RAMRegister_5] |= (u8)((number & 0x0C) >> 2);
			//COM2  
			LCD->RAM[LCD_RAMRegister_8] &= (~0x30);
			LCD->RAM[LCD_RAMRegister_8] |= (u8)((number & 0x30));
			//COM3  
			LCD->RAM[LCD_RAMRegister_12] &= (~0x03);
			LCD->RAM[LCD_RAMRegister_12] |= (u8)((number & 0xC0) >> 6);
			break;
		case DSP4:
			//COM0      
			LCD->RAM[LCD_RAMRegister_1] &= (~0xC0);
			LCD->RAM[LCD_RAMRegister_1] |= (u8)((number & 0x03) << 6);
			//COM1  
			LCD->RAM[LCD_RAMRegister_5] &= (~0x0C);
			LCD->RAM[LCD_RAMRegister_5] |= (u8)((number & 0x0C));
			//COM2  
			LCD->RAM[LCD_RAMRegister_8] &= (~0xC0);
			LCD->RAM[LCD_RAMRegister_8] |= (u8)((number & 0x30) << 2);
			//COM3  
			LCD->RAM[LCD_RAMRegister_12] &= (~0x0C);
			LCD->RAM[LCD_RAMRegister_12] |= (u8)((number & 0xC0) >> 4);
			break;
		default:
			break;
	}
}

#endif

