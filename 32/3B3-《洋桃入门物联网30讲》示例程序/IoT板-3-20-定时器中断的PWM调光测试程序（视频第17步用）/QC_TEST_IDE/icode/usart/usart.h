/*
 * usart1.h
 *
 *  Created on: Oct 20, 2021
 *      Author: Administrator
 */

#ifndef INC_USART_H_
#define INC_USART_H_

#include "stm32f1xx_hal.h" //HAL库文件声明
#include <string.h>//用于字符串处理的库
#include "../inc/retarget.h"//用于printf函数串口重映射

extern UART_HandleTypeDef huart1;//声明USART1的HAL库结构体
extern UART_HandleTypeDef huart2;//声明USART2的HAL库结构体
extern UART_HandleTypeDef huart3;//声明USART2的HAL库结构体

#define USART1_REC_LEN  200//定义USART1最大接收字节数
#define USART2_REC_LEN  200//定义USART1最大接收字节数
#define USART3_REC_LEN  200//定义USART1最大接收字节数

extern uint8_t  USART1_RX_BUF[USART1_REC_LEN];//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern uint16_t USART1_RX_STA;//接收状态标记
extern uint8_t USART1_NewData;//当前串口中断接收的1个字节数据的缓存

extern uint8_t  USART2_RX_BUF[USART2_REC_LEN];//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern uint16_t USART2_RX_STA;//接收状态标记
extern uint8_t USART2_NewData;//当前串口中断接收的1个字节数据的缓存
extern uint8_t RS485orBT;//当RS485orBT标志位为1时是RS485模式，为0时是蓝牙模式

extern uint8_t  USART3_RX_BUF[USART3_REC_LEN];//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern uint16_t USART3_RX_STA;//接收状态标记
extern uint8_t USART3_NewData;//当前串口中断接收的1个字节数据的缓存

void  HAL_UART_RxCpltCallback(UART_HandleTypeDef  *huart);//串口中断回调函数声明

#endif /* INC_USART_H_ */
