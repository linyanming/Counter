
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

extern uint8_t BoardSt;

void TimeCounter(void);
void dspnum(void);
void DisplayData(uint8_t num);
void KeyHandler(void);
void ControlHandler(void);
void StatusInit(void);
void CounterNumHandler(void);


#endif

