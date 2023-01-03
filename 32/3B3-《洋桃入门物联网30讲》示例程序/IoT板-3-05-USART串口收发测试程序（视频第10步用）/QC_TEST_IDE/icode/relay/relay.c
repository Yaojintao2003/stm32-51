/*
 * relay.c
 *
 *  Created on: 2021年10月20日
 *      Author: Administrator
 */

#include "relay.h"

void RELAY_1(uint8_t c){ //继电器的控制程序（c=0继电器放开，c=1继电器吸合）
    if(c)HAL_GPIO_WritePin(GPIOA,RELAY1_Pin,GPIO_PIN_RESET); //继电器吸
    else  HAL_GPIO_WritePin(GPIOA,RELAY1_Pin,GPIO_PIN_SET); //继电器松
}
