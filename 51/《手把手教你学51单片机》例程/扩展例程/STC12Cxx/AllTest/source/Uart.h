/*
*******************************************************************************
*                     ���ְ��ֽ���ѧ51��Ƭ��(C���԰�)��
*                    ���� KST-51 ��Ƭ�������� ʾ��Դ����
*
*         (c) ��Ȩ���� 2014 ��ɳ̲������/�廪��ѧ������ ��������Ȩ��
*                 ��ȡ������������ʣ�http://www.kingst.org
*
* �ļ�����Uart.c
* ��  ��������֡ģʽ��UART��������ģ���ͷ�ļ�
* �汾�ţ�v1.0.0
* ��  ע��
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
