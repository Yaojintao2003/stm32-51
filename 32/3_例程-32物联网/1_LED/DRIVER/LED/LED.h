/******************************************************************
 * 文件：LED.h
 * 功能：实现LED驱动功能函数
 * 日期：2018-02-16
 * 作者：zx
 * 版本：Ver.1.0 | 最初版本
 * 
 * Copyright (C) 2018 zx. All rights reserved.
*******************************************************************/
#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"

void initLED(void);
void openLED(void);
void closeLED(void);
void toggleLED(void);

#endif
