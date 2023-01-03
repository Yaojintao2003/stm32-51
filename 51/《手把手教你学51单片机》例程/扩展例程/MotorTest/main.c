/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2014 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.kingst.org
*
* 文件名：main.c
* 描  述：步进电机测试程序
* 版本号：v1.0.0
* 备  注：
*******************************************************************************
*/

#include <reg52.h>


void main()
{
    EA = 1;       //使能总中断
    TMOD = 0x01;  //设置T0为模式1
    TH0  = 0xF8;  //为T0赋初值0xF8CD，定时2ms
    TL0  = 0xCD;
    ET0  = 1;     //使能T0中断
    TR0  = 1;     //启动T0
    
	while (1);
}

/* T0中断服务函数，用于驱动步进电机旋转 */
void InterruptTimer0() interrupt 1
{
    unsigned char tmp;  //临时变量
    static unsigned char index = 0;  //节拍输出索引
    unsigned char code BeatCode[8] = {  //步进电机节拍对应的IO控制代码
        0xE, 0xC, 0xD, 0x9, 0xB, 0x3, 0x7, 0x6
    };
    
    TH0 = 0xF8;  //重新加载初值
    TL0 = 0xCD;
    tmp = P1;                    //用tmp把P1口当前值暂存
    tmp = tmp & 0xF0;            //用&操作清零低4位
    tmp = tmp | BeatCode[index]; //用|操作把节拍代码写到低4位
    P1  = tmp;                   //把低4位的节拍代码和高4位的原值送回P1
    index++;                     //节拍输出索引递增
    index = index & 0x07;        //用&操作实现到8归零
}
