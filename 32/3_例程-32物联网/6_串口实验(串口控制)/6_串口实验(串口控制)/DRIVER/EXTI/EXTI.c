/******************************************************************
 * 文件：EXTI.c
 * 功能：实现外部中断相关函数
 * 日期：2018-02-25
 * 作者：zx
 * 版本：Ver.1.0 | 最初版本
 * 
 * Copyright (C) 2018 zx. All rights reserved.
*******************************************************************/
#include "EXTI/EXTI.h"
#include "devtype.h"
#include "KEY/key.h"

extern DevStatusStructure DevStatus;
/**
 * 功能：初始化外部中断0/1
 * 参数：None
 * 返回值：None
 */
void initEXTI_0_1(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;                        //定义外部中断初始化结构体

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);        //开启外设复用时钟

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); //使用PA0作为EXTI0信号输入源
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;                  //设置外部中断线0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;         //外部中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     //下降沿触发中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                   //使能外部中断屏蔽寄存器
    EXTI_Init(&EXTI_InitStructure);                             //设置生效

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1); //使用PA1作为EXTI1信号输入源
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;                  //设置外部中断线1
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;         //外部中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     //下降沿触发中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                   //使能外部中断屏蔽寄存器
    EXTI_Init(&EXTI_InitStructure);                             //设置生效
}

/**
 * 功能：外部中断0中断服务函数
 * 参数：None
 * 返回值：None
 */
void EXTI0_IRQHandler(void)
{
    u8 keyvalue;
    keyvalue = getKeyValue(KEY_PRESS); //获取键值
	
    switch(keyvalue)
    {
        case KEY_UP: DevStatus.KeyValue = KEY_UP;     break;
        case KEY_DOWN: DevStatus.KeyValue = KEY_DOWN; break;

        default : DevStatus.KeyValue = KEY_NO;        break;
    }
    EXTI_ClearFlag(EXTI_Line0);        //清除外部中断挂起位
    
}

/**
 * 功能：外部中断1中断服务函数
 * 参数：None
 * 返回值：None
 */
void EXTI1_IRQHandler(void)
{
    u8 keyvalue;
    keyvalue = getKeyValue(KEY_PRESS); //获取键值
	
    switch(keyvalue)
    {
        case KEY_UP: DevStatus.KeyValue = KEY_UP;     break;
        case KEY_DOWN: DevStatus.KeyValue = KEY_DOWN; break;

        default : DevStatus.KeyValue = KEY_NO;        break;
    }
    EXTI_ClearFlag(EXTI_Line1);        //清除外部中断挂起位
}
