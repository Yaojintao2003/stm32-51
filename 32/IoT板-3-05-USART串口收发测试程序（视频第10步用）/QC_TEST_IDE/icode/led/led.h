/*
 * led.h
 *
 *  Created on: 2022年1月18日
 *      Author: Administrator
 */

#ifndef LED_LED_H_
#define LED_LED_H_

#include "stm32f1xx_hal.h" //HAL库文件声明
#include "main.h" //IO定义与初始化函数在main.c文件中，必须引用

void LED_1(uint8_t a);//LED1独立控制函数（0为熄灭，其他值为点亮）
void LED_2(uint8_t a);//LED2独立控制函数（0为熄灭，其他值为点亮）
void LED_ALL(uint8_t a);//LED1~4整组操作函数（低4位的1/0状态对应4个LED亮灭，最低位对应LED1）
void LED_1_Contrary(void);//LED1状态取反
void LED_2_Contrary(void);//LED2状态取反

#endif /* LED_LED_H_ */
