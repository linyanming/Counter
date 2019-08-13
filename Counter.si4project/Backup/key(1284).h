

#ifndef __KEY_H__
#define __KEY_H__

#include "stm8l15x_gpio.h"

#define KEYINIT    0  //初始状态
#define KEYPRESS   1  //按键按下
#define KEYRELEASE 2  //按键释放

extern uint8_t KeyPWRst;
extern uint8_t KeyRSTst;

#define KEYPORT GPIOC
#define KEYRST_PIN GPIO_Pin_5
#define KEYPWR_PIN GPIO_Pin_6

#define KEYRST 0
#define KEYPWR 1

void KeyInit(void);
void KeyShakeCheck(void);
uint8_t ReadKeyValue(uint8_t key);




#endif

