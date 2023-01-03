/******************************************************************
 * 文件：main.c
 * 功能：主函数入口
 * 日期：2018-02-16
 * 作者：zx
 * 版本：Ver.1.0 | 最初版本
 * 官博：http://fengmeitech.club
 * Copyright (C) 2018 zx. All rights reserved.
*******************************************************************/
#include "LED/LED.h"
#include "stm32f10x.h"
#include "DELAY/Delay.h"

int main(void)
{
	
	initSysTick();
	initLED(); //初始化LED	
	while (1)
	{
		toggleLED();//翻转LED
		Delay_ms(500);
	}
}

