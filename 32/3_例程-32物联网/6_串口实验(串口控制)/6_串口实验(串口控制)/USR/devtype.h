/******************************************************************
 * 文件：devtype.h
 * 功能：抽象定义项目属性
 * 日期：2018-02-28
 * 作者：zx
 * 版本：Ver.1.0 | 最初版本
 * 
 * Copyright (C) 2018 zx. All rights reserved.
*******************************************************************/
#include "stm32f10x.h"

//用结构体来描述此时设备的状态
typedef struct
{
    u8 LEDStatus; //此时LED状态 ON为亮 OFF为灭
    u8 KeyValue;//当前键值
}DevStatusStructure;

