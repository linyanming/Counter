
#include "counter.h"

uint16_t SysTime = 0; //系统时间 变量数值超过65535计算速度会变慢

uint8_t BoardSt = BOOT_STOP;  //启动状态变量
uint8_t InitFlag = 0;         //初始化变量，指示是否初始化
uint8_t KR_flag = 0;          //右边按键按下后是否弹起变量
uint8_t KP_flag = 0;		  //电源按键按下后是否弹起变量

uint16_t KP_PressTime = 0;    //按键按下时间

uint8_t preStatus;   //干簧管上一个状态
uint8_t nowStatus;	 //干簧管当前状态

uint16_t CounterNum = 0;   //计数器圈数

Line LengthLine;
float prelinelen;
//uint16_t CounterNum_t = 0; //计数器圈数缓存 用于对比当前圈数
//时间计数1ms计一次
void TimeCounter(void)
{
	if(KP_PressTime > 0)
		KP_PressTime++;
}


void LineInit(void)
{
	LengthLine.linenumber = 8;
	LengthLine.linedia = LINEDIA;
	LengthLine.linelen = 300000;   
	LengthLine.linewheeldia = LINEWHDIA;
	LengthLine.slotwidth = 330;
	LengthLine.length = 0;
	prelinelen = 0;
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

void LenAdd(void)
{
	LengthLine.length += LengthLine.linedia * PI;  //计算长度
}

void LenSub(void)
{
	LengthLine.length -= LengthLine.linedia * PI;  //计算长度
	if(LengthLine.length < 0)
	{
		LengthLine.length = 0;
	}
}

void DisplayLen(uint16_t num)
{
	float templen = len / 1000; //毫米转化为米
	templen = ((float)((int)(len * 10))) / 10;
	if(num > 50)
	{
		
	}
	LengthLine.length = num * LengthLine.linedia * PI;  //计算长度
}

void BoardClose(void)
{
	LCD_ClearAll();
	LED_Off();
	if(InitFlag == 1)
	{
		LCD_Deconfig();
		Tim4_Deconfig();
		InitFlag = 0;
	}
	PWR_FastWakeUpCmd(ENABLE);
	PWR_UltraLowPowerCmd(ENABLE);
	halt();
}

void BoardInit(void)
{
	if(InitFlag == 0)
	{
		Tim4_Config();
		LCD_Config();
		InitFlag = 1;
	}
	KeyShakeCheck();
	KeyHandler();
	if(KP_PressTime > START_TIME)
	{
		PWR_UltraLowPowerCmd(DISABLE);
		BoardSt = BOOT_RUN;
		KP_PressTime = 0;
		CounterNum = 0;
		LCD_WriteBaterry(3);
		DisplayData(CounterNum);
		StatusInit();
	}

}

void StatusInit(void)
{
	if(ReadKeyValue(KEYRSB) == 1 && ReadKeyValue(KEYRSF) == 1)
	{
		nowStatus = RSSTATUS1;
		preStatus = RSSTATUS1;
	}
	else if(ReadKeyValue(KEYRSB) == 0 && ReadKeyValue(KEYRSF) == 1)
	{
		nowStatus = RSSTATUS2;
		preStatus = RSSTATUS2;
	}
	else
	{
		nowStatus = RSSTATUS3;
		preStatus = RSSTATUS3;
	}
}

void CounterNumHandler(void)
{
	if(ReadKeyValue(KEYRSB) == 1 && ReadKeyValue(KEYRSF) == 1)
	{
		nowStatus = RSSTATUS1;
	}
	else if(ReadKeyValue(KEYRSB) == 0 && ReadKeyValue(KEYRSF) == 1)
	{
		nowStatus = RSSTATUS2;
	}
	else
	{
		nowStatus = RSSTATUS3;
	}

	if(nowStatus == RSSTATUS1 && preStatus == RSSTATUS2)
	{
		if(CounterNum > 0)
		{
			CounterNum--;
			DisplayData(CounterNum);
		}
		preStatus = nowStatus;
	}
	else if(nowStatus == RSSTATUS2 && preStatus == RSSTATUS1)
	{
		CounterNum++;
		DisplayData(CounterNum);
		preStatus = nowStatus;
	}
	else
	{
		preStatus = nowStatus;
	}
}

void BoardStart(void)
{

	CounterNumHandler();
	KeyShakeCheck();
	KeyHandler();
	
	if(KP_PressTime > CLOSE_TIME)
	{
		BoardSt = BOOT_STOP;
		KeyPWRst = KEYINIT;
		KP_flag = 0;
		KP_PressTime = 0;
	}
}

void KeyHandler(void)
{
	if(BoardSt == BOOT_RUN)
	{
		if(KeyRSTst == KEYPRESS)
		{
			if(KR_flag == 0)
			{
				CounterNum = 0;
				DisplayData(CounterNum);
				LED_Toggle();
				KR_flag = 1;
			}
		}
		else if(KeyRSTst == KEYRELEASE)
		{
			KeyRSTst = KEYINIT;
			KR_flag = 0;
		}
	}
	
	if(KeyPWRst == KEYPRESS)
	{
		if(KP_flag == 0)
		{
			KP_PressTime = 1;
			KP_flag = 1;
		}
	}
	else if(KeyPWRst == KEYRELEASE)
	{
		KeyPWRst = KEYINIT;
		KP_flag = 0;
		KP_PressTime = 0;
		if(BoardSt == BOOT_INIT)
		{
			BoardSt = BOOT_STOP;
		}
	}
}



void ControlHandler(void)
{
	if(BoardSt == BOOT_INIT)
	{
		BoardInit();
	}
	else if(BoardSt == BOOT_RUN)
	{
		BoardStart();
	}
	else
	{
		BoardClose();
	}
}
