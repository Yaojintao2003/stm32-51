/*
 * adc.c
 *
 *  Created on: Oct 21, 2021
 *      Author: Administrator
 */

#include "adc.h"

uint16_t ADC_IN_1(void) //ADC�ɼ�����
{
	HAL_ADC_Start(&hadc1);//��ʼADC�ɼ�
	HAL_ADC_PollForConversion(&hadc1,500);//�ȴ��ɼ�����
	if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))//��ȡADC��ɱ�־λ
	{
		return HAL_ADC_GetValue(&hadc1);//����ADC��ֵ
	}
	return 0;
}

uint16_t ADC_IN_2(void) //ADC�ɼ�����
{
	HAL_ADC_Start(&hadc2);//��ʼADC�ɼ�
	HAL_ADC_PollForConversion(&hadc2,500);//�ȴ��ɼ�����
	if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc2), HAL_ADC_STATE_REG_EOC))//��ȡADC��ɱ�־λ
	{
		return HAL_ADC_GetValue(&hadc2);//����ADC��ֵ
	}
	return 0;
}
