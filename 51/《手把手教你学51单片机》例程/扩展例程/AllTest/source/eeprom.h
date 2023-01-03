/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2014 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.kingst.org
*
* 文件名：eeprom.c
* 描  述：EEPROM芯片24C02读写驱动模块的头文件
* 版本号：v1.0.0
* 备  注：
*******************************************************************************
*/

#ifndef _EEPROM_H
#define _EEPROM_H


#ifndef _EEPROM_C

#endif

void E2Read(uint8 *buf, uint8 addr, uint8 len);
void E2Write(uint8 *buf, uint8 addr, uint8 len);


#endif
