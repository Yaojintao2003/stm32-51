/*
 * key.h
 *
 *  Created on: Jan 18, 2022
 *      Author: Administrator
 */

#ifndef KEY_KEY_H_
#define KEY_KEY_H_

#include "stm32f1xx_hal.h" //HAL库文件声明
#include "main.h" //IO定义与初始化函数在main.c文件中，必须引用

uint8_t KEY_1(void);//按键1
uint8_t KEY_2(void);//按键2

#endif /* KEY_KEY_H_ */
