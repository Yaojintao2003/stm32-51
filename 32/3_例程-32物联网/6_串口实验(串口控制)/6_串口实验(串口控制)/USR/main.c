/******************************************************************
 * 文件：main.c
 * 功能：主函数入口
 * 日期：2018-02-16
 * 作者：zx
 * 版本：Ver.1.0 | 最初版本
 * 官博：http://fengmeitech.club
 * Copyright (C) 2017 zx. All rights reserved.
*******************************************************************/
#include "LED/LED.h"
#include "DELAY/Delay.h"
#include "UART/uart.h"
#include "NVIC/NVIC.h"
#include "EXTI/EXTI.h"
#include "KEY/key.h"
#include "devtype.h"

DevStatusStructure DevStatus = {OFF,KEY_NO}; //默认LED不亮，键值为空

int main(void)
{
	/*各外设初始化*/
    initLED();
    initNVIC(NVIC_PriorityGroup_2);
    initEXTI_0_1();
    initKey();
    initUART();
	initSysTick();

    while (1)
    {
        if(DevStatus.LEDStatus!=0) //LED开判断
        {
            openLED();
        }else //LED关判断
        {
            closeLED();
        }

        switch(DevStatus.KeyValue)
        {
            case KEY_UP:
                        sendString(USART1,"KEY UP pressed!\n");
                        DevStatus.KeyValue = KEY_NO;
                                                                     break;
            case KEY_DOWN: 
                        sendString(USART1,"KEY DOWN pressed!\n");
                        DevStatus.KeyValue = KEY_NO;
                                                                     break;

            default : DevStatus.KeyValue = KEY_NO;                   break;            
        }
    }
}

