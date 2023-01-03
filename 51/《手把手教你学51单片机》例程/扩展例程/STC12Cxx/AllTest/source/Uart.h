/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2014 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.kingst.org
*
* 文件名：Uart.c
* 描  述：基于帧模式的UART串口驱动模块的头文件
* 版本号：v1.0.0
* 备  注：
*******************************************************************************
*/

#ifndef _UART_H
#define _UART_H


#ifndef _UART_C

#endif

void ConfigUART(uint16 baud);
void UartDriver();
void UartRxMonitor(uint8 ms);
void UartWrite(uint8 *buf, uint8 len);
uint8 UartRead(uint8 *buf, uint8 len);


#endif
