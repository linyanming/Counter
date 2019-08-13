
#include "counter.h"

uint32_t SysTime = 0; //系统时间

uint8_t num = 0;
uint8_t tnum = 0;
//时间计数1ms计一次
void TimeCounter(void)
{
	SysTime++;
	if(SysTime % 1000 == 0)
	{
		num++;
	}
}

void dspnum(void)
{
	if(tnum != num)
	{
		DisplayData(num);
		tnum = num;
	}
}

void DisplayData(uint8_t num)
{
	uint8_t hun = 0;
	uint8_t ten = 0;
	uint8_t unit = 0;

	if(num >= 100)
	{
		hun = num / 100;
		ten = (num % 100) / 10;
		unit = num	% 10;
	}
	else if(num >= 10)
	{
		ten = num / 10;
		unit = num % 10;
	}
	else
	{
		unit = num;
	}
	
	LCD_WriteNum(hun, DSP2, 0);
	LCD_WriteNum(ten, DSP3, 0);
	LCD_WriteNum(unit, DSP4, 0);
}

void KeyHandler(void)
{
	if(KeyRSTst == KEYPRESS)
	{
		num = 0;
		
//		LCD_WriteNum(1, DSP2, 0);
	}
	else if(KeyRSTst == KEYRELEASE)
	{
//		LCD_WriteNum(0, DSP3, 0);
		KeyRSTst = KEYINIT;
	}
}

