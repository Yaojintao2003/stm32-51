/*
 * usart1.c
 *
 *  Created on: Oct 20, 2021
 *      Author: Administrator
 */

#include "usart.h"

uint8_t USART1_RX_BUF[USART1_REC_LEN];//���ջ���,���USART_REC_LEN���ֽ�.
uint16_t USART1_RX_STA=0;//����״̬���//bit15��������ɱ�־��bit14�����յ�0x0d��bit13~0�����յ�����Ч�ֽ���Ŀ
uint8_t USART1_NewData;//��ǰ�����жϽ��յ�1���ֽ����ݵĻ���

uint8_t USART2_RX_BUF[USART2_REC_LEN];//���ջ���,���USART_REC_LEN���ֽ�.
uint16_t USART2_RX_STA=0;//����״̬���//bit15��������ɱ�־��bit14�����յ�0x0d��bit13~0�����յ�����Ч�ֽ���Ŀ
uint8_t USART2_NewData;//��ǰ�����жϽ��յ�1���ֽ����ݵĻ���
uint8_t RS485orBT;//��RS485orBT��־λΪ1ʱ��RS485ģʽ��Ϊ0ʱ������ģʽ

uint8_t USART3_RX_BUF[USART3_REC_LEN];//���ջ���,���USART_REC_LEN���ֽ�.
uint16_t USART3_RX_STA=0;//����״̬���//bit15��������ɱ�־��bit14�����յ�0x0d��bit13~0�����յ�����Ч�ֽ���Ŀ
uint8_t USART3_NewData;//��ǰ�����жϽ��յ�1���ֽ����ݵĻ���

void  HAL_UART_RxCpltCallback(UART_HandleTypeDef  *huart)//�����жϻص�����
{
	if(huart ==&huart1)//�ж��ж���Դ������1��USBת���ڣ�
    {
       printf("%c",USART1_NewData); //���յ��������� a���ű��� ���ͻص���
       if((USART1_RX_STA&0x8000)==0){//����δ���
           if(USART1_RX_STA&0x4000){//���յ���0x0d
               if(USART1_NewData!=0x0a)USART1_RX_STA=0;//���մ���,���¿�ʼ
               else USART1_RX_STA|=0x8000;   //���������
           }else{ //��û�յ�0X0D
               if(USART1_NewData==0x0d)USART1_RX_STA|=0x4000;
               else{
                  USART1_RX_BUF[USART1_RX_STA&0X3FFF]=USART1_NewData; //���յ������ݷ�������
                  USART1_RX_STA++;  //���ݳ��ȼ�����1
                  if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����
               }
           }
       }
       HAL_UART_Receive_IT(&huart1,(uint8_t *)&USART1_NewData,1); //�ٿ��������ж�
    }
    if(huart ==&huart2)//�ж��ж���Դ��RS485/����ģ�飩
    {
       if(RS485orBT){//��RS485orBT��־λΪ1ʱ��RS485ģʽ��Ϊ0ʱ������ģʽ
    	   USART2_RX_BUF[0]=USART2_NewData;//�����յ������ݷ��뻺�����飨��ֻ�õ�1�����ݣ�����ֻ���������[0]λ�ã�
    	   USART2_RX_STA++;//���ݽ��ձ�־λ��1
       }else{
    	   printf("%c",USART2_NewData); //���յ��������� a���ű��� ���ͻص���
       }
       HAL_UART_Receive_IT(&huart2,(uint8_t *)&USART2_NewData, 1); //�ٿ��������ж�
    }
	if(huart ==&huart3)//�ж��ж���Դ������3��WIFIģ�飩
	{
		printf("%c",USART3_NewData); //���յ��������� a���ű��� ���ͻص���
		HAL_UART_Receive_IT(&huart3,(uint8_t *)&USART3_NewData,1); //�ٿ��������ж�
	}
}
