
#ifndef __LCD_H__
#define __LCD_H__

#include "stm8l15x_lcd.h"
#include "stm8l15x_clk.h"

#define DSP1  0   //数码管1
#define DSP2  1   //数码管2
#define DSP3  2   //数码管3
#define DSP4  3   //数码管4


void LCD_Config(void);
void LCD_WriteBaterry(uint8_t bt);
void LCD_WriteNum(uint8_t bNum,uint8_t pos,uint8_t dot);
void LCD_ClearAll(void);
void LCD_Deconfig(void);

#endif
