/******************************************************************
 * 文件：EXTI.h
 * 功能：声明外部中断相关函数
 * 日期：2018-02-25
 * 作者：zx
 * 版本：Ver.1.0 | 最初版本
 * 
 * Copyright (C) 2018 zx. All rights reserved.
*******************************************************************/
#ifndef __EXTI_H
#define __EXTI_H

#include "stm32f10x.h"
#include "KEY/key.h"


void initEXTI_0_1(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);

#endif
