/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2016 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.qdkingst.com
*
* 文件名：Lcd1602.h
* 描  述：1602字符液晶驱动模块的头文件
* 版本号：v1.0.0
* 备  注：与Lcd1602.c文件一同使用
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
