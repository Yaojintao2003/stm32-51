/******************************************************************
 * 文件：LED.c
 * 功能：实现LED驱动功能函数
 * 日期：2018-02-16
 * 作者：zx
 * 版本：Ver.1.0 | 最初版本
 * 
 * Copyright (C) 2018 zx. All rights reserved.
*******************************************************************/
#include "LED/LED.h"

/**
 * 功能：初始化LED
 * 参数：None
 * 返回值：None
 */
void initLED(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 				  //定义GPIO初始化结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能GPIO时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		      //设置对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //设置推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //工作速度50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);			      //设置生效
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);				      //默认低电平，LED不亮
}
  

/**
 * 功能：点亮LED
 * 参数：None
 * 返回值：None
 */
void openLED(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

/**
 * 功能：熄灭LED
 * 参数：None
 * 返回值：None
 */
void closeLED(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

/**
 * 功能：翻转LED
 * 参数：None
 * 返回值：None
 */
void toggleLED(void)
{
	GPIOB->ODR ^= GPIO_Pin_5;
}
