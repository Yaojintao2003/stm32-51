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

int main(void)
{
	u16 i,j;

	initLED(); //初始化LED	
	while (1)
	{
        toggleLED();//翻转LED

        for(i=0;i<10000;++i) //阻塞单片机产生延时
        {
              for(j=0;j<100;++j);
        }
	}
}

