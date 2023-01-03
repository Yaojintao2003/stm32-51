/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2014 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.kingst.org
*
* 文件名：LedBuzzer.c
* 描  述：点阵LED、数码管、独立LED和无源蜂鸣器的驱动模块
* 版本号：v1.0.0
* 备  注：所需的定时中断由STC12系列的PCA模块实现
*******************************************************************************
*/

#define  _LED_BUZZER_C
#include "config.h"
#include "LedBuzzer.h"


//定义STC12系列PCA模块的寄存器
sfr CMOD   = 0xD9;  //PCA 工作模式寄存器
sfr CCON   = 0xD8;  //PCA 控制寄存器
sfr CL     = 0xE9;  //PCA 计数器低位
sfr CH     = 0xF9;  //PCA 计数器高位
sfr CCAPM0 = 0xDA; //PCA 模块0 PWM 寄存器
sfr CCAPM1 = 0xDB; //PCA 模块1 PWM 寄存器
sfr CCAP0L = 0xEA; //PCA 模块 0 的捕捉/比较寄存器低 8 位
sfr CCAP0H = 0xFA; //PCA 模块 0 的捕捉/比较寄存器高 8 位
sfr CCAP1L = 0xEB; //PCA 模块 1 的捕捉/比较寄存器低 8 位
sfr CCAP1H = 0xFB; //PCA 模块 1 的捕捉/比较寄存器高 8 位
sfr PCA_PWM0 = 0xF2; //PCA 模块0 PWM 寄存器
sfr PCA_PWM1 = 0xF3; //PCA 模块1 PWM 寄存器
sbit PPCA = IP^7;    //PCA 模块中断优先级
sbit CF   = CCON^7;  //PCA计数器溢出标志,由硬件或软件置位,必须由软件清0。
sbit CR   = CCON^6;  //1:允许 PCA 计数器计数, 必须由软件清0。
sbit CCF1 = CCON^1;  //PCA 模块1 中断标志, 由硬件置位, 必须由软件清0。
sbit CCF0 = CCON^0;  //PCA 模块0 中断标志, 由硬件置位, 必须由软件清0。


uint8 code LedChar[] = {  //数码管显示字符转换表
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};

uint16 reload = 0;  //定时所需的重载值
uint16 tmrcmp = 0;  //PCA0软件定时比较值
bit staBuzzer = 0;  //蜂鸣器状态控制位，1-鸣叫、0-关闭
struct sLedBuff ledBuff; //LED显示缓冲区，默认初值全0，正好达到上电全亮的效果

/* LED初始化函数，初始化IO、配置定时器 */
void InitLed()
{
    //初始化IO口
    P0 = 0xFF;
    ENLED = 0;
    //配置PCA作为动态扫描定时
    reload = SYS_MCLK/1500;  //计算所需重载值，每秒产生1500次中断
    tmrcmp = reload;  //初始比较值即为重载值
    CMOD   = 0x88;    //配置PCA使用主时钟，禁止计数中断
    CCAPM0 = 0x49;    //配置PCA0为16位软件定时器模式
    CCAP0L = tmrcmp;  //软件定时比较值送到比较寄存器
    CCAP0H = tmrcmp>>8;
    PPCA   = 1;       //中断优先级设置为高
	CR     = 1;       //启动PCA计数器
}
/* 流水灯实现函数，间隔调用实现流动效果 */
void FlowingLight()
{
    static uint8 i = 0;
    const uint8 code tab[] = {  //流动表
        0x7F, 0x3F, 0x1F, 0x0F, 0x87, 0xC3, 0xE1, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF
    };
    
    ledBuff.alone = tab[i];   //表中对应值送到独立LED的显示缓冲区
    if (i < (sizeof(tab)-1))  //索引递增循环，遍历整个流动表
        i++;
    else
        i = 0;
}
/* 数码管上显示一位数字，index-数码管位索引(从右到左对应0～5)，
**     num-待显示的数字，point-代表是否显示此位上的小数点 */
void ShowLedNumber(uint8 index, uint8 num, uint8 point)
{
    ledBuff.number[index] = LedChar[num];  //输入数字转换为数码管字符0～F
    if (point != 0)
    {
        ledBuff.number[index] &= 0x7F;  //point不为0时点亮当前位的小数点
    }
}
/* 点阵上显示一帧图片，ptr-待显示图片指针 */
void ShowLedArray(uint8 *ptr)
{
    uint8 i;
    
    for (i=0; i<sizeof(ledBuff.array); i++)
    {
        ledBuff.array[i] = *ptr++;
    }
}
/* PCA中断服务函数，LED动态扫描、蜂鸣器控制 */
void InterruptPCA() interrupt 7
{
    static uint8 i = 0;  //LED位选索引
    
    CCF0 = 0;  //清零PCA0中断标志
    tmrcmp += reload;  //重载值累加到比较值上
    CCAP0L = tmrcmp;   //比较值送到比较寄存器
    CCAP0H = tmrcmp>>8;
    //全部LED动态扫描显示
    if (ENLED == 0)  //LED使能时才进行动态扫描
    {
        P0 = 0xFF;                       //关闭所有段选位，显示消隐
        P1 = (P1 & 0xF0) | i;            //位选索引值赋值到P1口低4位
        P0 = *((uint8 data*)&ledBuff+i); //缓冲区中索引位置的数据送到P0口
        if (i < (sizeof(ledBuff)-1))     //索引递增循环，遍历整个缓冲区
            i++;
        else
            i = 0;
    }
    //由蜂鸣器状态位控制蜂鸣器
    if (staBuzzer == 1)
        BUZZER = ~BUZZER;  //蜂鸣器鸣叫
    else
        BUZZER = 1;        //蜂鸣器静音
}
