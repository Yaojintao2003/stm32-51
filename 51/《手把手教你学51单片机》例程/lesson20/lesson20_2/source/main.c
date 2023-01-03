/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2016 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.qdkingst.com
*
* 文件名：main.c
* 描  述：超声波测距，在1602液晶上显示
* 版本号：v1.0.0
* 备  注：
*******************************************************************************
*/

#include "config.h"
#include "Lcd1602.h"
#include "Ultrasonic.h"

uint16 flag;   //定义定时标志

void ShowDistance();

/* 主函数 */
void main()
{
    EA = 1;            //开启总中断
    TMOD &= 0Xf0;
    TMOD |= 0X01;      //设置T0为方式1
    TH0 = 0xFC;        //为T0赋初值0xFC67，定时1ms
    TL0 = 0x67;
    ET0 = 1;           //使能T0中断
    TR0 = 1;           //启动T0
    InitUltrasonic();  //超声波模块初始化
    InitLcd1602();     //1602初始化

    while(1)
    {
        if(flag == 1)
        {
            flag = 0;
            ShowDistance();
        }
    }
}

/* 执行测距并显示结果 */
void ShowDistance()
{
    uint8 i;
    uint8 buff[6];
    uint16 Distance ;   //暂存距离值

    Distance = GetDistance();
    buff[5] = Distance%10 + '0';   //将数据转换为字符形式
    buff[4] ='.';
    buff[3] = (Distance/10)%10 + '0';
    buff[2] = (Distance/100)%10 + '0';
    buff[1] = (Distance/1000)%10 + '0';
    buff[0] = (Distance/10000)%10 + '0';
    for(i = 0; i <= 3; i++)  //整数部分高位的0转换为空白符
    {
        if(buff[i] == '0')   //判断是不是字符0
            buff[i] = ' ';   //将字符0 转换为空格 即不显示
        else
            break;
    }
    LcdShowStr(0, 1, &buff); //显示距离值
    LcdShowStr(6, 1, "cm");  //显示距离单位
}

/* 定时器0中断服务函数 */
void InterruptTimer0() interrupt 1 using 3
{
    static uint16 cnt = 0;   //定义计数变量

    TH0 = 0xFC;      //重新加载初值
    TL0 = 0x67;
    cnt++;
    if(cnt >= 330)   //定时330ms
    {
        cnt = 0;     //清零计数值
        flag = 1;    //设置定时标志为1
    }
}
