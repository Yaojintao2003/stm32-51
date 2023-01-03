/*
 * usart1.h
 *
 *  Created on: Oct 20, 2021
 *      Author: Administrator
 */

#ifndef INC_USART_H_
#define INC_USART_H_

#include "stm32f1xx_hal.h" //HAL���ļ�����
#include <string.h>//�����ַ�������Ŀ�
#include "../inc/retarget.h"//����printf����������ӳ��

extern UART_HandleTypeDef huart1;//����USART1��HAL��ṹ��
extern UART_HandleTypeDef huart2;//����USART2��HAL��ṹ��
extern UART_HandleTypeDef huart3;//����USART2��HAL��ṹ��

#define USART1_REC_LEN  200//����USART1�������ֽ���
#define USART2_REC_LEN  200//����USART1�������ֽ���
#define USART3_REC_LEN  200//����USART1�������ֽ���

extern uint8_t  USART1_RX_BUF[USART1_REC_LEN];//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern uint16_t USART1_RX_STA;//����״̬���
extern uint8_t USART1_NewData;//��ǰ�����жϽ��յ�1���ֽ����ݵĻ���

extern uint8_t  USART2_RX_BUF[USART2_REC_LEN];//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern uint16_t USART2_RX_STA;//����״̬���
extern uint8_t USART2_NewData;//��ǰ�����жϽ��յ�1���ֽ����ݵĻ���
extern uint8_t RS485orBT;//��RS485orBT��־λΪ1ʱ��RS485ģʽ��Ϊ0ʱ������ģʽ

extern uint8_t  USART3_RX_BUF[USART3_REC_LEN];//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern uint16_t USART3_RX_STA;//����״̬���
extern uint8_t USART3_NewData;//��ǰ�����жϽ��յ�1���ֽ����ݵĻ���

void  HAL_UART_RxCpltCallback(UART_HandleTypeDef  *huart);//�����жϻص���������

#endif /* INC_USART_H_ */
