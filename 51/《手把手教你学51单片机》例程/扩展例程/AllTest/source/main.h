/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2014 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.kingst.org
*
* 文件名：main.c
* 描  述：全板测试程序 工程主文件的头文件
* 版本号：v1.0.0
* 备  注：
*******************************************************************************
*/

#ifndef _MAIN_H
#define _MAIN_H


#ifndef _MAIN_C

#endif

void KeyAction(uint8 keycode);
void ShowRealTime();
void RefreshTemp();
void CheckE2rom();
void RefreshADC();
void ConfigTimer0(uint16 ms);

#endif
