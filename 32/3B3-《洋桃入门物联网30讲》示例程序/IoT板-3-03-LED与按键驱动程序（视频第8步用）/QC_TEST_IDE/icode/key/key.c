/*
 * key.c
 *
 *  Created on: Jan 18, 2022
 *      Author: Administrator
 */

#include "key.h"

uint8_t KEY_1(void)
{
	uint8_t a;
	a=0;//���δ���밴�������򷵻�0
	if(HAL_GPIO_ReadPin(GPIOA,KEY1_Pin)==GPIO_PIN_RESET){//�������ӿڵĵ�ƽ
		HAL_Delay(20);//��ʱȥ����
		if(HAL_GPIO_ReadPin(GPIOA,KEY1_Pin)==GPIO_PIN_RESET){ //�������ӿڵĵ�ƽ
			a=1;//���밴����������1
		}
	}
	while(HAL_GPIO_ReadPin(GPIOA,KEY1_Pin)==GPIO_PIN_RESET); //�ȴ������ɿ�
	return a;
}

uint8_t KEY_2(void)
{
	uint8_t a;
	a=0;//���δ���밴�������򷵻�0
	if(HAL_GPIO_ReadPin(GPIOA,KEY2_Pin)==GPIO_PIN_RESET){//�������ӿڵĵ�ƽ
		HAL_Delay(20);//��ʱȥ����
		if(HAL_GPIO_ReadPin(GPIOA,KEY2_Pin)==GPIO_PIN_RESET){ //�������ӿڵĵ�ƽ
			a=1;//���밴����������1
		}
	}
	while(HAL_GPIO_ReadPin(GPIOA,KEY2_Pin)==GPIO_PIN_RESET); //�ȴ������ɿ�
	return a;
}


