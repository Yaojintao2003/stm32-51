/*
 * buzzer.c
 *
 *  Created on: 2022��1��22��
 *      Author: Administrator
 */

#include "buzzer.h"
#define time1 50 //������ʱ��
#define hz1 1 //��������������λ���룩
void BUZZER_SOLO1(void){//��������������ı���������ʽ1��HAL��ľ�׼��ʱ������
    uint16_t i;
    for(i=0;i<time1;i++){//ѭ����������������ʱ��
       HAL_GPIO_WritePin(BEEP1_GPIO_Port,BEEP1_Pin,GPIO_PIN_RESET); //�������ӿ�����͵�ƽ0
       HAL_Delay(hz1); //��ʱ�����뼶��ʱ��С1΢�룬ʵ�ֵĵ����ϵͣ�����Ҫ�����д΢�뼶��ʱ�����������ʵ�ã�
       HAL_GPIO_WritePin(BEEP1_GPIO_Port,BEEP1_Pin,GPIO_PIN_SET); //�������ӿ�����ߵ�ƽ1
       HAL_Delay(hz1); //��ʱ
    }
}
#define time2 200 //������ʱ��
#define hz2 500 //��������������λ΢�룩
void BUZZER_SOLO2(void){//��������������ı���������ʽ2��CPU΢�뼶��ʱ��
    uint16_t i;
    for(i=0;i<time2;i++){//ѭ����������������ʱ��
       HAL_GPIO_WritePin(BEEP1_GPIO_Port,BEEP1_Pin,GPIO_PIN_RESET); //�������ӿ�����͵�ƽ0
       delay_us(hz2); //��ʱ
       HAL_GPIO_WritePin(BEEP1_GPIO_Port,BEEP1_Pin,GPIO_PIN_SET); //�������ӿ�����ߵ�ƽ1
       delay_us(hz2); //��ʱ
    }
}
