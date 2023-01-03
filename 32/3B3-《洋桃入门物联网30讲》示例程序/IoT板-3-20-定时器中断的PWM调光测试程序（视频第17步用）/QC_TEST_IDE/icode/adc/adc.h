/*
 * adc.h
 *
 *  Created on: Oct 21, 2021
 *      Author: Administrator
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

#include "stm32f1xx_hal.h" //HAL¿âÎÄ¼şÉùÃ÷
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

uint16_t ADC_IN_1(void);
uint16_t ADC_IN_2(void);

#endif /* ADC_ADC_H_ */
