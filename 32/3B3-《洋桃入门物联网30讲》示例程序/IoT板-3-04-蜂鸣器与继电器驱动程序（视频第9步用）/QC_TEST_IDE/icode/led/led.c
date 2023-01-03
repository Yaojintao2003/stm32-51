/*
 * led.c
 *
 *  Created on: 2022��1��18��
 *      Author: Administrator
 */

#include "led.h"

void LED_1(uint8_t a)//LED1�������ƺ�����0ΪϨ������ֵΪ������
{
	if(a)HAL_GPIO_WritePin(GPIOB,LED1_Pin,GPIO_PIN_SET);
	else HAL_GPIO_WritePin(GPIOB,LED1_Pin,GPIO_PIN_RESET);
}
void LED_2(uint8_t a)//LED2�������ƺ�����0ΪϨ������ֵΪ������
{
	if(a)HAL_GPIO_WritePin(GPIOB,LED2_Pin,GPIO_PIN_SET);
	else HAL_GPIO_WritePin(GPIOB,LED2_Pin,GPIO_PIN_RESET);
}
void LED_ALL(uint8_t a)//LED1~2���������������2λ��1/0״̬��Ӧ2��LED�������λ��ӦLED1��
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
