/*
 * led.h
 *
 *  Created on: 2022��1��18��
 *      Author: Administrator
 */

#ifndef LED_LED_H_
#define LED_LED_H_

#include "stm32f1xx_hal.h" //HAL���ļ�����
#include "main.h" //IO�������ʼ��������main.c�ļ��У���������

void LED_1(uint8_t a);//LED1�������ƺ�����0ΪϨ������ֵΪ������
void LED_2(uint8_t a);//LED2�������ƺ�����0ΪϨ������ֵΪ������
void LED_ALL(uint8_t a);//LED1~4���������������4λ��1/0״̬��Ӧ4��LED�������λ��ӦLED1��
void LED_1_Contrary(void);//LED1״̬ȡ��
void LED_2_Contrary(void);//LED2״̬ȡ��

#endif /* LED_LED_H_ */
