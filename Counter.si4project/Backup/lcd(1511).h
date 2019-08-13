
#ifndef __LCD_H__
#define __LCD_H__

#include "stm8l15x_lcd.h"
#include "stm8l15x_clk.h"

void LCD_Init(void);
void LCD_WriteBaterry(u8 bt);
void LCD_WriteNum(u8 bNum,u8 pos,bool dot);

#endif
