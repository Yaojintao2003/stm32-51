/*
 * relay.c
 *
 *  Created on: 2021��10��20��
 *      Author: Administrator
 */

#include "relay.h"

void RELAY_1(uint8_t c){ //�̵����Ŀ��Ƴ���c=0�̵����ſ���c=1�̵������ϣ�
    if(c)HAL_GPIO_WritePin(GPIOA,RELAY1_Pin,GPIO_PIN_RESET); //�̵�����
    else  HAL_GPIO_WritePin(GPIOA,RELAY1_Pin,GPIO_PIN_SET); //�̵�����
}
