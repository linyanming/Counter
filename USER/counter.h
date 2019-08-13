
#ifndef __COUNTER_H__
#define __COUNTER_H__

#include "key.h"
#include "lcd.h"
#include "tim.h"
#include "led.h"

//板子运行状态
#define BOOT_INIT    0
#define BOOT_RUN     1
#define BOOT_STOP    2

//开关机时间
#define START_TIME   2000
#define CLOSE_TIME   2000

#define RSSTATUS1  3  //两个干簧管都断开
#define RSSTATUS2  2  //RSB导通RSF断开
#define RSSTATUS3  1  //其他状态

#define PI 3.14

#define LINEDIA 0.29      //线直径     单位：mm
#define SLOTWD  330       //线槽宽度
#define LINEWHDIA 20.28   //线轮直径
#define SUBNUM 50         //每当圈数计数该数值线轮直径需要减一次线直径

typedef struct
{
	uint8_t linenumber;  //线号
	float linedia;       //线直径
	float slotwidth;     //线槽宽度
	float linewheeldia;  //线轮直径
	uint32_t linelen;    //线总长
	float length;        //当前长度
}Line;


extern uint8_t BoardSt;

void TimeCounter(void);
void dspnum(void);
void DisplayData(uint8_t num);
void KeyHandler(void);
void ControlHandler(void);
void StatusInit(void);
void CounterNumHandler(void);
void LineInit(void);

#endif

