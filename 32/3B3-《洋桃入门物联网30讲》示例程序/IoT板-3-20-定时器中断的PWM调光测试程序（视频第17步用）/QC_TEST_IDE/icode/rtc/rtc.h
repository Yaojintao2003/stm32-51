/*
 * rtc.h
 *
 *  Created on: 2021��10��20��
 *      Author: Administrator
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "stm32f1xx_hal.h" //HAL���ļ�����
#include "main.h" //IO�������ʼ��������main.c�ļ��У���������

/*
//ʱ���д������˵��//
1����mani.c�ļ�����ѭ��֮ǰ����RTC_Init();��ʹ��RTCʱ�ӡ�
	RTC_Init�����Դ��ж��״��ϵ繦��
2��ʹ��RTC_Get();����ʱ�䡣���������ݴ���ڣ�
�� ryear	��16λ��
�� rmon	�����¶���8λ��
�� rday
ʱ rhour
�� rmin
�� rsec
�� rweek
3��ʹ��RTC_Set(4λ��,2λ��,2λ��,2λʱ,2λ��,2λ��); д��ʱ�䡣
���磺RTC_Set(2022,8,6,21,34,0);

�����������ǰ�������3�������ģ�����Ҫ���á�
ע��Ҫʹ��RTC_Get��RTC_Set�ķ���ֵ��Ϊ0ʱ��ʾ��д��ȷ��
*/

extern RTC_HandleTypeDef hrtc;

//����rtc.c�ļ��ж����ȫ�ֱ�����ע�⣺���ﲻ�ܸ�������ֵ��
extern uint16_t ryear;
extern uint8_t rmon,rday,rhour,rmin,rsec,rweek;

void RTC_Init(void); //�û��Խ��Ĵ����ϵ�BPK�жϵ�RTC��ʼ��������ѭ��ǰ���á�
uint8_t Is_Leap_Year(uint16_t year);//�ж��Ƿ������꺯��
uint8_t RTC_Get(void);//������ǰʱ��ֵ������������Ҫ��RTCʱ���á�
uint8_t RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec);//д�뵱ǰʱ�䡾����������Ҫд��RTCʱ���á�
uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day);//�������ռ�������

#endif

