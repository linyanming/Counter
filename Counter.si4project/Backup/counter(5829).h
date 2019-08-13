
#ifndef __COUNTER_H__
#define __COUNTER_H__

#include "key.h"
#include "lcd.h"
#include "tim.h"


void TimeCounter(void);
void dspnum(void);
void DisplayData(uint8_t num);
void KeyHandler(void);


#endif

