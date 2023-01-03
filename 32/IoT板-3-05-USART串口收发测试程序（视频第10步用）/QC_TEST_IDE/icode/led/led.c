/*
 * led.c
 *
 *  Created on: 2022年1月18日
 *      Author: Administrator
 */

#include "led.h"

void LED_1(uint8_t a)//LED1独立控制函数（0为熄灭，其他值为点亮）
{
	if(a)HAL_GPIO_WritePin(GPIOB,LED1_Pin,GPIO_PIN_SET);
	else HAL_GPIO_WritePin(GPIOB,LED1_Pin,GPIO_PIN_RESET);
}
void LED_2(uint8_t a)//LED2独立控制函数（0为熄灭，其他值为点亮）
{
	if(a)HAL_GPIO_WritePin(GPIOB,LED2_Pin,GPIO_PIN_SET);
	else HAL_GPIO_WritePin(GPIOB,LED2_Pin,GPIO_PIN_RESET);
}
void LED_ALL(uint8_t a)//LED1~2整组操作函数（低2位的1/0状态对应2个LED亮灭，最低位对应LED1）
{
	if(a&0x01)HAL_GPIO_WritePin(GPIOB,LED1_Pin,GPIO_PIN_SET);
	else HAL_GPIO_WritePin(GPIOB,LED1_Pin,GPIO_PIN_RESET);
	if(a&0x02)HAL_GPIO_WritePin(GPIOB,LED2_Pin,GPIO_PIN_SET);
	else HAL_GPIO_WritePin(GPIOB,LED2_Pin,GPIO_PIN_RESET);
}
void LED_1_Contrary(void){
	HAL_GPIO_WritePin(GPIOB,LED1_Pin,1-HAL_GPIO_ReadPin(GPIOB,LED1_Pin));
}
void LED_2_Contrary(void){
	HAL_GPIO_WritePin(GPIOB,LED2_Pin,1-HAL_GPIO_ReadPin(GPIOB,LED2_Pin));
}
