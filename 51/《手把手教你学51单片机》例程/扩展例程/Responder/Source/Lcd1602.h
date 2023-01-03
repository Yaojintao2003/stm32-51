/*
*******************************************************************************
*                     ���ְ��ֽ���ѧ51��Ƭ��(C���԰�)��
*                    ���� KST-51 ��Ƭ�������� ʾ��Դ����
*
*         (c) ��Ȩ���� 2016 ��ɳ̲������/�廪��ѧ������ ��������Ȩ��
*                 ��ȡ������������ʣ�http://www.qdkingst.com
*
* �ļ�����Lcd1602.h
* ��  ����1602�ַ�Һ������ģ���ͷ�ļ�
* �汾�ţ�v1.0.0
* ��  ע����Lcd1602.c�ļ�һͬʹ��
*******************************************************************************
*/

#ifndef _LCD1602_H
#define _LCD1602_H

#include <reg52.h>

#define LCD1602_DB  P0
sbit LCD1602_RS = P1^0;
sbit LCD1602_RW = P1^1;
sbit LCD1602_E  = P1^5;

void InitLcd1602();
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);
void LcdAreaClear(unsigned char x, unsigned char y, unsigned char len);
void LcdFullClear();

#endif
