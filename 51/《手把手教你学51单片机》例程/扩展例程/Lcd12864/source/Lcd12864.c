/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2014 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.kingst.org
*
* 文件名：Lcd12864.c
* 描  述：12864点阵液晶驱动模块
* 版本号：v1.0.0
* 备  注：适用于KST-51开发板配套的12864液晶模块
*******************************************************************************
*/

#define _LCD_12864_C
#include "config.h"
#include "Lcd12864.h"


/* 等待液晶准备好 */
void LcdWaitReady()
{
    uint8 sta;
    
    LCD12864_DB = 0xFF;
    LCD12864_RS = 0;
    LCD12864_RW = 1;
    do {
        LCD12864_E = 1;
        sta = LCD12864_DB;
        LCD12864_E = 0;
    } while (sta & 0x80); //bit7等于1表示液晶正忙，重复检测直到其等于0为止
}
/* 向液晶写入一字节命令，cmd-待写入命令值 */
void LcdWriteCmd(uint8 cmd)
{
    LcdWaitReady();
    LCD12864_RS = 0;
    LCD12864_RW = 0;
    LCD12864_DB = cmd;
    LCD12864_E  = 1;
    LCD12864_E  = 0;
}
/* 向液晶写入一字节数据，dat-待写入数据值 */
void LcdWriteDat(uint8 dat)
{
    LcdWaitReady();
    LCD12864_RS = 1;
    LCD12864_RW = 0;
    LCD12864_DB = dat;
    LCD12864_E  = 1;
    LCD12864_E  = 0;
}
/*
* 函数名：LcdShowString
* 描  述：在显示屏上显示一串字符串
* 输  入：str - 待显示字符串指针
*         x - 屏幕显示横坐标(以像素为单位)
*         y - 屏幕显示纵坐标(以像素为单位)
* 输  出：无
* 备  注：输入的字符串必须符合C语言规范，即以'\0'-NULL为结束标识；
*         x、y坐标必须是16的整数倍，因DDRAM地址以全角字符(16*16pixel)为单位。
*/
void LcdShowString(uint8 x, uint8 y, uint8 *str)
{
    uint8 addr;
    
    //由输入的显示坐标计算DDRAM的地址
    x >>= 4;
    y >>= 4;
    if (y >= 2)
    {
        y -= 2;
        x += 8;
    }
    addr = y*16 + x;
    //由起始DDRAM地址连续写入字符串
    LcdWriteCmd(0x30); //启动DDRAM操作
    LcdWriteCmd(0x80|addr);
    while (*str != '\0')
    {
        LcdWriteDat(*str);
        str++;
    }
}
/*
* 函数名：LcdShowImage
* 描  述：在显示屏上显示一幅图像
* 输  入：img - 待显示图像指针
*         x - 屏幕显示横坐标(以像素为单位)
*         y - 屏幕显示纵坐标(以像素为单位)
*         w - 图像宽度(以像素为单位)
*         h - 图像高度(以像素为单位)
* 输  出：无
* 备  注：x与w必须是16的整数倍，因CGRAM最小寻址单位为2字节；y与h可为0-63的任意值。
*/
void LcdShowImage(uint8 x, uint8 y, uint8 w, uint8 h, uint8 *img)
{
    int16 i;
    uint8 xi,yi;
    uint8 xt,yt;

    x >>= 4;
    w >>= 3;
    i = 0;
    LcdWriteCmd(0x36); //启动CGRAM操作
    for (yi=0; yi<h; yi++)
    {
        yt = y+yi;
        xt = x;
        if (yt >= 32)
        {
            yt -= 32;
            xt += 8;
        }
        LcdWriteCmd(0x80|yt);
        LcdWriteCmd(0x80|xt);
        for (xi=0; xi<w; xi++)
        {
            LcdWriteDat(img[i++]);
        }
    }
}

/*
* 函数名：LcdClearArea
* 描  述：清除屏幕上的一块图形显示区域
* 输  入：x - 区域起始横坐标(以像素为单位)
*         y - 区域起始纵坐标(以像素为单位)
*         w - 区域宽度(以像素为单位)
*         h - 区域高度(以像素为单位)
* 输  出：无
* 备  注：x与w必须是16的整数倍，因CGRAM最小寻址单位为2字节；y与h可为0-63的任意值。
*/
void LcdClearArea(uint8 x, uint8 y, uint8 w, uint8 h)
{
    uint8 xi,yi;
    uint8 xt,yt;

    x >>= 4;
    w >>= 3;
    LcdWriteCmd(0x36); //启动CGRAM操作
    for (yi=0; yi<h; yi++)
    {
        yt = y+yi;
        xt = x;
        if (yt >= 32)
        {
            yt -= 32;
            xt += 8;
        }
        LcdWriteCmd(0x80|yt);
        LcdWriteCmd(0x80|xt);
        for (xi=0; xi<w; xi++)
        {
            LcdWriteDat(0x00); //写入0x00以清除显示
        }
    }
}

/* 12864液晶初始化函数 */
void InitLcd12864()
{
    uint8 x, y;

    //字符模式初始化
    LcdWriteCmd(0x30);  //基本指令集
    LcdWriteCmd(0x01);  //清零字符显示
    LcdWriteCmd(0x02);  //地址归位
    LcdWriteCmd(0x0C);  //开显示
    
    //图形模式初始化
    LcdWriteCmd(0x34);    //启动扩展指令，关闭图形显示
    for (y=0; y<32; y++)  //清零图形显示缓冲区
    {
        LcdWriteCmd(0x80|y);
        LcdWriteCmd(0x80|0);
        for (x=0; x<32; x++)
        {
            LcdWriteDat(0x00);
        }
    }
    LcdWriteCmd(0x36);  //开启图形模式显示
}
