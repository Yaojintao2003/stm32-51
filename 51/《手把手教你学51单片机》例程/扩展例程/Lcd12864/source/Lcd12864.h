/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2014 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.kingst.org
*
* 文件名：Lcd12864.c
* 描  述：12864点阵液晶驱动模块的头文件
* 版本号：v1.0.0
* 备  注：
*******************************************************************************
*/

#ifndef _LCD_12864_H
#define _LCD_12864_H


//12864液晶口线与板载1602液晶相同
#define LCD12864_DB   LCD1602_DB
#define LCD12864_RS   LCD1602_RS
#define LCD12864_RW   LCD1602_RW
#define LCD12864_E    LCD1602_E


#ifndef _LCD_12864_C

#endif


void InitLcd12864();
void LcdShowString(uint8 x, uint8 y, uint8 *str);
void LcdShowImage(uint8 x, uint8 y, uint8 w, uint8 h, uint8 *img);
void LcdClearArea(uint8 x, uint8 y, uint8 w, uint8 h);


#endif
