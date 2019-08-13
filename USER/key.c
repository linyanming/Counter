
#include "key.h"
#include "lcd.h"

#define NO_ACTION     0
#define PRESS_CHECK   1
#define RELEASE_CHECK 2

#define KCHECK_MAXTIMES 8

typedef struct
{
	u8 times;
	u8 k_status;
}KSCheck;

KSCheck KPCheck;  //电源按键状态确认
KSCheck KRCheck;  //复位按键状态确认

uint8_t KeyPWRst = KEYINIT;  //电源按键状态
uint8_t KeyRSTst = KEYINIT;  //复位按键状态


uint8_t ReadKeyValue(uint8_t key)
{
	if(key == KEYRST)
	{
		return (GPIOC->IDR & KEYRST_PIN) >> 5;
	}
	else if(key == KEYPWR)
	{
		return (GPIOC->IDR & KEYPWR_PIN) >> 6;
	}
	else if(key == KEYRSB)
	{
		return (GPIOD->IDR & KEYRSB_PIN) >> 6;
	}
	else if(key == KEYRSF)
	{
		return (GPIOD->IDR & KEYRSF_PIN) >> 7;
	}
}

void KeyInit(void)
{
	GPIO_Init(KEYPORT, KEYPWR_PIN, GPIO_Mode_In_PU_IT);
	GPIO_Init(KEYPORT, KEYRST_PIN, GPIO_Mode_In_PU_No_IT);
	GPIO_Init(KEYRSPORT, KEYRSB_PIN, GPIO_Mode_In_PU_No_IT);
	GPIO_Init(KEYRSPORT, KEYRSF_PIN, GPIO_Mode_In_PU_No_IT);
	GPIO_Init(KEYRSPORT, KEYRS_PIN, GPIO_Mode_Out_PP_Low_Slow);
	
	EXTI_SetPinSensitivity(EXTI_Pin_6, EXTI_Trigger_Falling);
	ITC_SetSoftwarePriority(EXTI6_IRQn, ITC_PriorityLevel_3);
}

void KeyShakeCheck(void)
{
	if(KeyRSTst == KEYINIT && ReadKeyValue(KEYRST) == 0 && KRCheck.k_status == NO_ACTION)
	{
		KRCheck.k_status = PRESS_CHECK;
		KRCheck.times = 0;
	}
	else if(KeyRSTst == KEYPRESS && ReadKeyValue(KEYRST) == 1 && KRCheck.k_status == NO_ACTION)
	{
		KRCheck.k_status = RELEASE_CHECK;
		KRCheck.times = 0;
	}

	if(KeyPWRst == KEYINIT && ReadKeyValue(KEYPWR) == 0 && KPCheck.k_status == NO_ACTION)
	{
		KPCheck.k_status = PRESS_CHECK;
		KPCheck.times = 0;
	}
	else if(KeyPWRst == KEYPRESS && ReadKeyValue(KEYPWR) == 1 && KPCheck.k_status == NO_ACTION)
	{
		KPCheck.k_status = RELEASE_CHECK;
		KPCheck.times = 0;
	}
	
	if(KRCheck.k_status == PRESS_CHECK)
	{
		if(KRCheck.times < KCHECK_MAXTIMES)
		{
			if(ReadKeyValue(KEYRST) == 0)
			{
				KRCheck.times++;
			}
			else
			{
				KRCheck.times = 0;
				KRCheck.k_status = NO_ACTION;
			}
		}
		else
		{
			KRCheck.times = 0;
			KRCheck.k_status = NO_ACTION;
			KeyRSTst = KEYPRESS;
		}
	}
	else if(KRCheck.k_status == RELEASE_CHECK)
	{
		if(KRCheck.times < KCHECK_MAXTIMES)
		{
			if(ReadKeyValue(KEYPWR) == 1)
			{
				KRCheck.times++;
			}
			else
			{
				KRCheck.times = 0;
				KRCheck.k_status = NO_ACTION;
			}
		}
		else
		{
			KRCheck.times = 0;
			KRCheck.k_status = NO_ACTION;
			KeyRSTst = KEYRELEASE;
		}
	}

	if(KPCheck.k_status == PRESS_CHECK)
	{
		if(KPCheck.times < KCHECK_MAXTIMES)
		{
			if(ReadKeyValue(KEYPWR) == 0)
			{
				KPCheck.times++;
			}
			else
			{
				KPCheck.times = 0;
				KPCheck.k_status = NO_ACTION;
			}
		}
		else
		{
			KPCheck.times = 0;
			KPCheck.k_status = NO_ACTION;
			KeyPWRst = KEYPRESS;
		}
	}
	else if(KPCheck.k_status == RELEASE_CHECK)
	{
		if(KPCheck.times < KCHECK_MAXTIMES)
		{
			if(ReadKeyValue(KEYRST) == 1)
			{
				KPCheck.times++;
			}
			else
			{
				KPCheck.times = 0;
				KPCheck.k_status = NO_ACTION;
			}
		}
		else
		{
			KPCheck.times = 0;
			KPCheck.k_status = NO_ACTION;
			KeyPWRst = KEYRELEASE;
		}
	}
}

/************************************************
按键状态确认函数

参数：
	按键标识

返回值：
	无
*************************************************/
#if 0
void KeyStatusCheck(uint8_t keyst)
{
	switch(keyst)
	{
		case KEYPRESS:
			if(KeyPWRst == KEYINIT)
			{
				KPCheck.k_status = PRESS_CHECK;
				KPCheck.times = 0;
			}
			break;
		case KEYRELEASE:
			if(KeyPWRst == KEYPRESS)
			{
				KPCheck.k_status = RELEASE_CHECK;
				KPCheck.times = 0;
			}
			break;
		default:
			break;
	}

}
#endif
