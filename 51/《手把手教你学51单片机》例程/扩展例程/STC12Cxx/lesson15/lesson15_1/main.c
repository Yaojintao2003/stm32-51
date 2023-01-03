/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2014 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.kingst.org
*
* 文件名：main.c
* 描  述：第15章 DS1302单次读写模式示例
* 版本号：v1.0.0
* 备  注：详情见第15章15.3.4节，针对STC12系列单片机修改中断延时补偿与总线时序
*         用单次读写模式访问DS1302，并将日期时间显示在液晶上
*******************************************************************************
*/

#include <reg52.h>
#include <intrins.h>

sbit DS1302_CE = P1^7;
sbit DS1302_CK = P3^5;
sbit DS1302_IO = P3^4;

bit flag200ms = 0;       //200ms定时标志
unsigned char T0RH = 0;  //T0重载值的高字节
unsigned char T0RL = 0;  //T0重载值的低字节

void ConfigTimer0(unsigned int ms);
void InitDS1302();
unsigned char DS1302SingleRead(unsigned char reg);
extern void InitLcd1602();
extern void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);

void main()
{
    unsigned char i;
    unsigned char psec=0xAA;  //秒备份，初值AA确保首次读取时间后会刷新显示
    unsigned char time[8];    //当前时间数组
    unsigned char str[12];    //字符串转换缓冲区

    EA = 1;           //开总中断
    ConfigTimer0(1);  //T0定时1ms
    InitDS1302();     //初始化实时时钟
    InitLcd1602();    //初始化液晶
    
    while (1)
    {
        if (flag200ms)  //每200ms读取依次时间
        {
            flag200ms = 0;
            for (i=0; i<7; i++)  //读取DS1302当前时间
            {
                time[i] = DS1302SingleRead(i);
            }
            if (psec != time[0]) //检测到时间有变化时刷新显示
            {
                str[0] = '2';  //添加年份的高2位：20
                str[1] = '0';
                str[2] = (time[6] >> 4) + '0';  //“年”高位数字转换为ASCII码
                str[3] = (time[6]&0x0F) + '0';  //“年”低位数字转换为ASCII码
                str[4] = '-';  //添加日期分隔符
                str[5] = (time[4] >> 4) + '0';  //“月”
                str[6] = (time[4]&0x0F) + '0';
                str[7] = '-';
                str[8] = (time[3] >> 4) + '0';  //“日”
                str[9] = (time[3]&0x0F) + '0';
                str[10] = '\0';
                LcdShowStr(0, 0, str);  //显示到液晶的第一行
                
                str[0] = (time[5]&0x0F) + '0';  //“星期”
                str[1] = '\0';
                LcdShowStr(11, 0, "week");
                LcdShowStr(15, 0, str);  //显示到液晶的第一行
                
                str[0] = (time[2] >> 4) + '0';  //“时”
                str[1] = (time[2]&0x0F) + '0';
                str[2] = ':';  //添加时间分隔符
                str[3] = (time[1] >> 4) + '0';  //“分”
                str[4] = (time[1]&0x0F) + '0';
                str[5] = ':';
                str[6] = (time[0] >> 4) + '0';  //“秒”
                str[7] = (time[0]&0x0F) + '0';
                str[8] = '\0';
                LcdShowStr(4, 1, str);  //显示到液晶的第二行
                
                psec = time[0];  //用当前值更新上次秒数
            }
        }
    }
}
/* DS1302总线附加延时函数 */
void BusDelay()
{
    _nop_();
    _nop_();
}
/* 发送一个字节到DS1302通信总线上 */
void DS1302ByteWrite(unsigned char dat)
{
    unsigned char mask;
    
    for (mask=0x01; mask!=0; mask<<=1)  //低位在前，逐位移出
    {
        if ((mask&dat) != 0) //首先输出该位数据
            DS1302_IO = 1;
        else
            DS1302_IO = 0;
        DS1302_CK = 1;       //然后拉高时钟
        BusDelay();
        DS1302_CK = 0;       //再拉低时钟，完成一个位的操作
    }
    DS1302_IO = 1;           //最后确保释放IO引脚
}
/* 由DS1302通信总线上读取一个字节 */
unsigned char DS1302ByteRead()
{
    unsigned char mask;
    unsigned char dat = 0;
    
    for (mask=0x01; mask!=0; mask<<=1)  //低位在前，逐位读取
    {
        if (DS1302_IO != 0)  //首先读取此时的IO引脚，并设置dat中的对应位
        {
            dat |= mask;
        }
        DS1302_CK = 1;       //然后拉高时钟
        BusDelay();
        DS1302_CK = 0;       //再拉低时钟，完成一个位的操作
    }
    return dat;              //最后返回读到的字节数据
}
/* 用单次写操作向某一寄存器写入一个字节，reg-寄存器地址，dat-待写入字节 */
void DS1302SingleWrite(unsigned char reg, unsigned char dat)
{
    DS1302_CE = 1;                   //使能片选信号
    DS1302ByteWrite((reg<<1)|0x80);  //发送写寄存器指令
    DS1302ByteWrite(dat);            //写入字节数据
    DS1302_CE = 0;                   //除能片选信号
}
/* 用单次读操作从某一寄存器读取一个字节，reg-寄存器地址，返回值-读到的字节 */
unsigned char DS1302SingleRead(unsigned char reg)
{
    unsigned char dat;
    
    DS1302_CE = 1;                   //使能片选信号
    DS1302ByteWrite((reg<<1)|0x81);  //发送读寄存器指令
    dat = DS1302ByteRead();          //读取字节数据
    DS1302_CE = 0;                   //除能片选信号
    
    return dat;
}
/* DS1302初始化，如发生掉电则重新设置初始时间 */
void InitDS1302()
{
    unsigned char i;
    unsigned char code InitTime[] = {  //2013年10月8日 星期二 12:30:00
        0x00,0x30,0x12, 0x08, 0x10, 0x02, 0x13
    };
    
    DS1302_CE = 0;  //初始化DS1302通信引脚
    DS1302_CK = 0;
    i = DS1302SingleRead(0);  //读取秒寄存器
    if ((i & 0x80) != 0)      //由秒寄存器最高位CH的值判断DS1302是否已停止
    {
        DS1302SingleWrite(7, 0x00);  //撤销写保护以允许写入数据
        for (i=0; i<7; i++)          //设置DS1302为默认的初始时间
        {
            DS1302SingleWrite(i, InitTime[i]);
        }
    }
}
/* 配置并启动T0，ms-T0定时时间 */
void ConfigTimer0(unsigned int ms)
{
    unsigned long tmp;  //临时变量
    
    tmp = 11059200 / 12;      //定时器计数频率
    tmp = (tmp * ms) / 1000;  //计算所需的计数值
    tmp = 65536 - tmp;        //计算定时器重载值
    tmp = tmp + 2;            //补偿中断响应延时造成的误差
    T0RH = (unsigned char)(tmp>>8);  //定时器重载值拆分为高低字节
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;   //清零T0的控制位
    TMOD |= 0x01;   //配置T0为模式1
    TH0 = T0RH;     //加载T0重载值
    TL0 = T0RL;
    ET0 = 1;        //使能T0中断
    TR0 = 1;        //启动T0
}
/* T0中断服务函数，执行200ms定时 */
void InterruptTimer0() interrupt 1
{
    static unsigned char tmr200ms = 0;
    
    TH0 = T0RH;  //重新加载重载值
    TL0 = T0RL;
    tmr200ms++;
    if (tmr200ms >= 200)  //定时200ms
    {
        tmr200ms = 0;
        flag200ms = 1;
    }
}
