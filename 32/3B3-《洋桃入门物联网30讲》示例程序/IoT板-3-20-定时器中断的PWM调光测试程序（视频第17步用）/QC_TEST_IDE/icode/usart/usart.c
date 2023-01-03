/*
 * usart1.c
 *
 *  Created on: Oct 20, 2021
 *      Author: Administrator
 */

#include "usart.h"

uint8_t USART1_RX_BUF[USART1_REC_LEN];//接收缓冲,最大USART_REC_LEN个字节.
uint16_t USART1_RX_STA=0;//接收状态标记//bit15：接收完成标志，bit14：接收到0x0d，bit13~0：接收到的有效字节数目
uint8_t USART1_NewData;//当前串口中断接收的1个字节数据的缓存

uint8_t USART2_RX_BUF[USART2_REC_LEN];//接收缓冲,最大USART_REC_LEN个字节.
uint16_t USART2_RX_STA=0;//接收状态标记//bit15：接收完成标志，bit14：接收到0x0d，bit13~0：接收到的有效字节数目
uint8_t USART2_NewData;//当前串口中断接收的1个字节数据的缓存
uint8_t RS485orBT;//当RS485orBT标志位为1时是RS485模式，为0时是蓝牙模式

uint8_t USART3_RX_BUF[USART3_REC_LEN];//接收缓冲,最大USART_REC_LEN个字节.
uint16_t USART3_RX_STA=0;//接收状态标记//bit15：接收完成标志，bit14：接收到0x0d，bit13~0：接收到的有效字节数目
uint8_t USART3_NewData;//当前串口中断接收的1个字节数据的缓存

void  HAL_UART_RxCpltCallback(UART_HandleTypeDef  *huart)//串口中断回调函数
{
	if(huart ==&huart1)//判断中断来源（串口1：USB转串口）
    {
       printf("%c",USART1_NewData); //把收到的数据以 a符号变量 发送回电脑
       if((USART1_RX_STA&0x8000)==0){//接收未完成
           if(USART1_RX_STA&0x4000){//接收到了0x0d
               if(USART1_NewData!=0x0a)USART1_RX_STA=0;//接收错误,重新开始
               else USART1_RX_STA|=0x8000;   //接收完成了
           }else{ //还没收到0X0D
               if(USART1_NewData==0x0d)USART1_RX_STA|=0x4000;
               else{
                  USART1_RX_BUF[USART1_RX_STA&0X3FFF]=USART1_NewData; //将收到的数据放入数组
                  USART1_RX_STA++;  //数据长度计数加1
                  if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//接收数据错误,重新开始接收
               }
           }
       }
       HAL_UART_Receive_IT(&huart1,(uint8_t *)&USART1_NewData,1); //再开启接收中断
    }
    if(huart ==&huart2)//判断中断来源（RS485/蓝牙模块）
    {
       if(RS485orBT){//当RS485orBT标志位为1时是RS485模式，为0时是蓝牙模式
    	   USART2_RX_BUF[0]=USART2_NewData;//将接收到的数据放入缓存数组（因只用到1个数据，所以只存放在数据[0]位置）
    	   USART2_RX_STA++;//数据接收标志位加1
       }else{
    	   printf("%c",USART2_NewData); //把收到的数据以 a符号变量 发送回电脑
       }
       HAL_UART_Receive_IT(&huart2,(uint8_t *)&USART2_NewData, 1); //再开启接收中断
    }
	if(huart ==&huart3)//判断中断来源（串口3：WIFI模块）
	{
		printf("%c",USART3_NewData); //把收到的数据以 a符号变量 发送回电脑
		HAL_UART_Receive_IT(&huart3,(uint8_t *)&USART3_NewData,1); //再开启接收中断
	}
}
