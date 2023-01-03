/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2016 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.qdkingst.com
*
* 文件名：main.c
* 描  述：九路抢答器
* 版本号：v1.1.0
* 备  注：1~9为抢答按钮，Ese为清除按钮，Enter为允许抢答按钮
          先按 ENter，再按抢答按钮，最后按Ese 清除数据，重新开始
          先按 抢答按钮 视为违规，蜂鸣器报警，退出请按Ese
*******************************************************************************
*/
#include "config.h"
#include "keyboard.h"
#include "Lcd1602.h"

sbit BUZZ = P1^6;   //蜂鸣器控制引脚
uint8 T0RH = 0;
uint8 T0RL = 0;
uint8 T1RH = 0;
uint8 T1RL = 0;

void ConfigTimer0(unsigned int ms);
void OpenBuzz(unsigned int frequ);
void StopBuzz();

void main()
{
    EA = 1;                    //开总中断
    ConfigTimer0(1);           //配置定时器0 1ms进一次中断
    InitLcd1602();
    LcdShowStr(0,0,"State :"); //1602显示按键名称
    LcdShowStr(0,1,"Number:"); //1602显示抢答组号

    while(1)
    {
        KeyDriver();           //按键驱动程序
    }
}

void KeyAction(uint8 keycode)
{
    uint8 buf[1];   //用于存放按键号码
    static uint8 permit = 0;   //允许抢答标志
    static uint8 done = 0;     //抢答完成标志

    if((keycode>='1') && (keycode<='9'))  //数字键1-9执行抢答
    {
        if(done == 0)
        {
            if(permit == 0)
            {
                buf[0] = keycode;
                LcdShowStr(8,0,"Warning");   //显示警告
                LcdShowStr(8,1,buf);         //显示按键号码
                done = 1;
                OpenBuzz(4000);
            }
            else
            {
                buf[0] = keycode;
                LcdShowStr(8,1,buf);         //显示按键号码
                done = 1;
            }
        }
    }
    else if (keycode == 0x0D)  //回车键，开始抢答
    {
        if(done == 0)
        {
            LcdAreaClear(8,0,8);
            LcdShowStr(8,0,"Permit");
            permit = 1;
        }
    }
    else if (keycode == 0x1B)  //Esc键，清除
    {
        LcdFullClear();
        LcdShowStr(0,0,"State :");
        LcdShowStr(0,1,"Number:");
        permit = 0;
        done = 0;
        StopBuzz();
    }
}

/* 配置并启动T0，ms-T0定时时间 */
void ConfigTimer0(unsigned int ms)
{
    uint32 tmp;  //临时变量

    tmp = 11059200 / 12;      //定时器计数频率
    tmp = (tmp * ms) / 1000;  //计算所需的计数值
    tmp = 65536 - tmp;        //计算定时器重载值
    tmp = tmp + 13;           //补偿中断响应延时造成的误差
    T0RH = (uint8)(tmp>>8);  //定时器重载值拆分为高低字节
    T0RL = (uint8)tmp;
    TMOD &= 0x00;   //清零T0的控制位
    TMOD |= 0x11;   //配置T0为模式1
    TH0 = T0RH;     //加载T0重载值
    TL0 = T0RL;
    ET0 = 1;        //使能T0中断
    TR0 = 1;        //启动T0
}
/* 蜂鸣器启动函数，frequ-工作频率 */
void OpenBuzz(unsigned int frequ)
{
    uint16 reload;    //计算所需的定时器重载值

    reload = 65536 - (11059200/12)/(frequ*2);  //由给定频率计算定时器重载值
    T1RH = (uint8)(reload >> 8);  //16位重载值分解为高低两个字节
    T1RL = (uint8)reload;
    TH1  = 0xFF;  //设定一个接近溢出的初值，以使定时器马上投入工作
    TL1  = 0xFE;
    ET1  = 1;     //使能T1中断
    TR1  = 1;     //启动T1
}
/* 蜂鸣器停止函数 */
void StopBuzz()
{
    ET1 = 0;   //禁用T1中断
    TR1 = 0;   //停止T1
}

/* T0中断服务函数，完成按键扫描 */
void InterruptTimer0() interrupt 1
{
    TH0 = T0RH;  //重新加载重载值
    TL0 = T0RL;
    KeyScan();   //键盘扫描
}

/* T1中断服务函数，用于控制蜂鸣器发声 */
void InterruptTimer1() interrupt 3
{
    TH1 = T1RH;   //重新加载重载值
    TL1 = T1RL;
    BUZZ = ~BUZZ; //反转蜂鸣器控制电平
}
