/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2014 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.kingst.org
*
* 文件名：Lcd1602.c
* 描  述：1602字符液晶驱动模块
* 版本号：v1.0.0
* 备  注：因液晶的控制与LED显示共用P0口和P1口的部分资源，所以本例中在访问液晶时
*         都采取措施避免对P0和P1口线的访问冲突。
*******************************************************************************
*/

#define  _LCD1602_C
#include "config.h"
#include "Lcd1602.h"

uint8 tmpP0;   //暂存P0口的值
bit tmpADDR0;  //暂存LED位选译码地址0的值
bit tmpADDR1;  //暂存LED位选译码地址1的值

/* 暂停LED动态扫描，暂存相关引脚的值 */
void LedScanPause()
{
    ENLED = 1;
    tmpP0 = P0;
    tmpADDR0 = ADDR0;
    tmpADDR1 = ADDR1;
}
/* 恢复LED动态扫描，恢复相关引脚的值 */
void LedScanContinue()
{
    ADDR0 = tmpADDR0;
    ADDR1 = tmpADDR1;
    P0 = tmpP0;
    ENLED = 0;
}
/* 等待液晶准备好 */
void LcdWaitReady()
{
    uint8 sta;
    
    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do {
        LCD1602_E = 1;
        sta = LCD1602_DB; //读取状态字
        LCD1602_E = 0;
    } while (sta & 0x80); //bit7等于1表示液晶正忙，重复检测直到其等于0为止
}
/* 向LCD1602液晶写入一字节命令，cmd-待写入命令值 */
void LcdWriteCmd(uint8 cmd)
{
    LedScanPause();
    LcdWaitReady();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_E  = 1;
    LCD1602_E  = 0;
    LedScanContinue();
}
/* 向LCD1602液晶写入一字节数据，dat-待写入数据值 */
void LcdWriteDat(uint8 dat)
{
    LedScanPause();
    LcdWaitReady();
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_DB = dat;
    LCD1602_E  = 1;
    LCD1602_E  = 0;
    LedScanContinue();
}
/* 清屏 */
void LcdClearScreen()
{
	LcdWriteCmd(0x01);
}
/* 设置显示RAM起始地址，亦即光标位置，(x,y)-对应屏幕上的字符坐标 */
void LcdSetCursor(uint8 x, uint8 y)
{
    uint8 addr;
    
    if (y == 0)  //由输入的屏幕坐标计算显示RAM的地址
        addr = 0x00 + x;  //第一行字符地址从0x00起始
    else
        addr = 0x40 + x;  //第二行字符地址从0x40起始
    LcdWriteCmd(addr | 0x80);  //设置RAM地址
}
/* 在液晶上显示字符串，(x,y)-对应屏幕上的起始坐标，str-字符串指针 */
void LcdShowStr(uint8 x, uint8 y, uint8 *str)
{
    LcdSetCursor(x, y);   //设置起始地址
    while (*str != '\0')  //连续写入字符串数据，直到检测到结束符
    {
        LcdWriteDat(*str++);
    }
}
/* 初始化1602液晶 */
void InitLcd1602()
{
    LcdWriteCmd(0x38);  //16*2显示，5*7点阵，8位数据接口
    LcdWriteCmd(0x0C);  //显示器开，光标关闭
    LcdWriteCmd(0x06);  //文字不动，地址自动+1
    LcdClearScreen();   //清屏
    //显示初始信息
    LcdShowStr(0, 0, "--:--:--  K:0x00");
    LcdShowStr(0, 1, "E2:- T:-- AD:---");
}
