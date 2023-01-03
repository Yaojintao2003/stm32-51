/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2016 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.qdkingst.com
*
* 文件名：Ultrasonic.h
* 描  述：超声波模块驱动头文件
* 版本号：v1.0.0
* 备  注：
*******************************************************************************
*/

#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H


#include <reg52.h>
#include <intrins.h>


sbit Trig = P2^7;      //超声波控制端引脚
sbit Echo = P3^3;      //超声波接收端引脚

void InitUltrasonic();
unsigned int GetDistance();

#endif
