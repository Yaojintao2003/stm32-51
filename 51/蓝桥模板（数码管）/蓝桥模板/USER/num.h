/*********该头文件包含了四个头文件，
分别是单片机的头文件(STC15F2K60S2.h)，
机器延时(-nop-)需要用到的头文件(intrins.h)
sptintf()函数需要用的头文件(stdio.h)
涉及字符串操作的相关头文件(string.h)
************/
#ifndef __num_H_
#define __num_H_
#include "STC15F2K60S2.h"
#include "intrins.h"
#include "stdio.h"
#include "string.h"

/******对无符号字符型和无符号整型进行宏定义，简化表达方式********/
#define uchar unsigned char
#define uint  unsigned  int


/****函数声明区*****/
void Timer1Init(void);
#endif