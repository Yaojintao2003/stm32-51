/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2016 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.qdkingst.com
*
* 文件名：main.c
* 描  述：超声波测距，定时器中断0计时，数码管显示距离
* 版本号：v1.0.0
* 备  注：
*******************************************************************************
*/

#include "config.h"
#include "Ultrasonic.h"

uint16 flag = 0;      //定义定时标志
uint8 LedBuff[7] = {  //数码管和8个独立LED的显示缓冲区，初值0xFF确保启动时都不亮
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
const int8 code LedChar[] = { //数码管显示字符转换表
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};
const uint8 code ledtab[8] = { //led显示转换表
    0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00
};

void ShowDistance();

/* 主函数 */
void main()
{
    EA = 1;            //使能总中断
    ENLED = 0;         //使能U3，选择控制数码管
    ADDR3 = 1;         //因为需要动态改变ADDR0-2的值，所以不需要再初始化了
    TMOD &= 0x0f;
    TMOD |= 0x10;      //设置T1为模式1
    TH1 = 0xFC;        //为T1赋初值0xFC67，定时1ms
    TL1 = 0x67;
    ET1 = 1;           //使能T1中断
    TR1 = 1;           //启动T1
    InitUltrasonic();  //初始化超声波模块

    while (1)
    {
        if(flag == 1)       //每间隔200ms启动一次测距
        {
            flag = 0;
            ShowDistance();
        }
    }
}

/* 执行测距并显示结果 */
void ShowDistance()
{
    int8 i;
    uint8 buff[5];
    uint16 distance ;   //暂存距离值

    distance = GetDistance();
    buff[0] = distance%10;
    buff[1] = distance/10%10;
    buff[2] = distance/100%10;
    buff[3] = distance/1000%10;
    buff[4] = distance/10000%10;
    for(i = 4; i >= 1;i--)  //整数部分高位的0转换为空白符
    {
        if(buff[i]==0)
            LedBuff[i] = 0xff;
        else
            break;
    }
    for(; i >= 0; i--)      //有效数字转化为显示字符
    {
        LedBuff[i] = LedChar[ buff[i] ];
    }
    LedBuff[1] &= 0x7f;     //在从低起第2位添加小数点，表示显示数值的单位为厘米
    LedBuff[6] = ledtab[(distance-1)/500 ]; //将4m的距离距离平均分为8个区间，用LED灯直观反应出来
}

/* 定时器1中断服务函数 */
void InterruptTimer1() interrupt 3
{
    static uint8 i = 0;     //LED位选索引
    static uint16 cnt = 0;  //定义计数变量

    TH1 = 0xFC;                   //重新加载初值
    TL1 = 0x67;
    P0 = 0xFF;                    //关闭所有段选位，显示消隐
    P1 = (P1 & 0xF8) | i;         //位选索引值赋值到P1口低3位
    P0 = LedBuff[i];              //缓冲区中索引位置的数据送到P0口
    if (i < (sizeof(LedBuff)-1))  //索引递增循环，遍历整个缓冲区
    {
        i++;
    }
    else
    {
        i = 0 ;
    }
    cnt++;
	if(cnt >= 200)   //定时200ms
    {
        cnt = 0;     //清零计数值
        flag = 1;    //设置定时标志为1
    }
}
