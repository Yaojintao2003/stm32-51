/*
 * buzzer.c
 *
 *  Created on: 2022年1月22日
 *      Author: Administrator
 */

#include "buzzer.h"
#define time1 50 //单音的时长
#define hz1 1 //单音的音调（单位毫秒）
void BUZZER_SOLO1(void){//蜂鸣器输出单音的报警音（样式1：HAL库的精准延时函数）
    uint16_t i;
    for(i=0;i<time1;i++){//循环次数决定单音的时长
       HAL_GPIO_WritePin(BEEP1_GPIO_Port,BEEP1_Pin,GPIO_PIN_RESET); //蜂鸣器接口输出低电平0
       HAL_Delay(hz1); //延时（毫秒级延时最小1微秒，实现的单调较低，因不需要额外编写微秒级延时函数所以最简单实用）
       HAL_GPIO_WritePin(BEEP1_GPIO_Port,BEEP1_Pin,GPIO_PIN_SET); //蜂鸣器接口输出高电平1
       HAL_Delay(hz1); //延时
    }
}
#define time2 200 //单音的时长
#define hz2 500 //单音的音调（单位微秒）
void BUZZER_SOLO2(void){//蜂鸣器输出单音的报警音（样式2：CPU微秒级延时）
    uint16_t i;
    for(i=0;i<time2;i++){//循环次数决定单音的时长
       HAL_GPIO_WritePin(BEEP1_GPIO_Port,BEEP1_Pin,GPIO_PIN_RESET); //蜂鸣器接口输出低电平0
       delay_us(hz2); //延时
       HAL_GPIO_WritePin(BEEP1_GPIO_Port,BEEP1_Pin,GPIO_PIN_SET); //蜂鸣器接口输出高电平1
       delay_us(hz2); //延时
    }
}
