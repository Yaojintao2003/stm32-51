/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2014 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.kingst.org
*
* 文件名：main.c
* 描  述：第17章 作业题2 将模拟输入通道0、1的电压值显示到数码管上
* 版本号：v1.0.0
* 备  注：通道0电压值显示在左侧，通道1电压值显示在右侧，中间隔两位不显示。
*******************************************************************************
*/

#include <reg52.h>

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

bit flag300ms = 1;       //300ms定时标志
unsigned char T0RH = 0;  //T0重载值的高字节
unsigned char T0RL = 0;  //T0重载值的低字节
unsigned char code LedChar[] = {  //数码管显示字符转换表
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};
unsigned char LedBuff[6] = {  //数码管显示缓冲区，初值0xFF确保启动时都不亮
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

void ConfigTimer0(unsigned int ms);
unsigned char GetADCValue(unsigned char chn);
extern void I2CStart();
extern void I2CStop();
extern unsigned char I2CReadACK();
extern unsigned char I2CReadNAK();
extern bit I2CWrite(unsigned char dat);
extern void InitLcd1602();
extern void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);

void main()
{
    unsigned char val;
    
    EA = 1;           //开总中断
    ENLED = 0;        //使能U3，选择控制数码管
    ADDR3 = 1;        //因为需要动态改变ADDR0-2的值，所以0-2不需要初始化了
    ConfigTimer0(2);  //配置T0定时2ms
    
    while (1)
    {
        if (flag300ms)
        {
            flag300ms = 0;
            //显示通道0的电压
            val = GetADCValue(0);   //获取ADC通道0的转换值
            val = (val*25) / 255;   //电压值=转换结果*2.5V/255，式中的25隐含了一位十进制小数
            LedBuff[5] = LedChar[val/10] & 0x7F;  //通道0的整数位显示到数码管第5位，并点亮第5位的小数点
            LedBuff[4] = LedChar[val%10];         //通道0的小数位显示到数码管第4位
             //显示通道1的电压
            val = GetADCValue(1);   //获取ADC通道1的转换值
            val = (val*25) / 255;   //电压值=转换结果*2.5V/255，式中的25隐含了一位十进制小数
            LedBuff[1] = LedChar[val/10] & 0x7F;  //通道1的整数位显示到数码管第1位，并点亮第1位的小数点
            LedBuff[0] = LedChar[val%10];         //通道1的小数位显示到数码管第0位
        }
    }
}
/* 读取当前的ADC转换值，chn-ADC通道号0~3 */
unsigned char GetADCValue(unsigned char chn)
{
    unsigned char val;
    
    I2CStart();
    if (!I2CWrite(0x48<<1))  //寻址PCF8591，如未应答，则停止操作并返回0
    {
        I2CStop();
        return 0;
    }
    I2CWrite(0x40|chn);        //写入控制字节，选择转换通道
    I2CStart();
    I2CWrite((0x48<<1)|0x01);  //寻址PCF8591，指定后续为读操作    
    I2CReadACK();              //先空读一个字节，提供采样转换时间
    val = I2CReadNAK();        //读取刚刚转换完的值
    I2CStop();
    
    return val;
}
/* 配置并启动T0，ms-T0定时时间 */
void ConfigTimer0(unsigned int ms)
{
    unsigned long tmp;  //临时变量
    
    tmp = 11059200 / 12;      //定时器计数频率
    tmp = (tmp * ms) / 1000;  //计算所需的计数值
    tmp = 65536 - tmp;        //计算定时器重载值
    tmp = tmp + 12;           //补偿中断响应延时造成的误差
    T0RH = (unsigned char)(tmp>>8);  //定时器重载值拆分为高低字节
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;   //清零T0的控制位
    TMOD |= 0x01;   //配置T0为模式1
    TH0 = T0RH;     //加载T0重载值
    TL0 = T0RL;
    ET0 = 1;        //使能T0中断
    TR0 = 1;        //启动T0
}
/* T0中断服务函数，执行数码管动态显示、300ms定时 */
void InterruptTimer0() interrupt 1
{
    static unsigned char i = 0;  //动态扫描的索引
    static unsigned char tmr300ms = 0;  //300ms软件定时计数器
    
    TH0 = T0RH;  //重新加载重载值
    TL0 = T0RL;
    tmr300ms++;
    if (tmr300ms >= 150)  //定时300ms
    {
        tmr300ms = 0;
        flag300ms = 1;
    }
    //以下代码完成数码管动态扫描刷新
    P0 = 0xFF;   //显示消隐
    switch (i)
    {
        case 0: ADDR2=0; ADDR1=0; ADDR0=0; i++; P0=LedBuff[0]; break;
        case 1: ADDR2=0; ADDR1=0; ADDR0=1; i++; P0=LedBuff[1]; break;
        case 2: ADDR2=0; ADDR1=1; ADDR0=0; i++; P0=LedBuff[2]; break;
        case 3: ADDR2=0; ADDR1=1; ADDR0=1; i++; P0=LedBuff[3]; break;
        case 4: ADDR2=1; ADDR1=0; ADDR0=0; i++; P0=LedBuff[4]; break;
        case 5: ADDR2=1; ADDR1=0; ADDR0=1; i=0; P0=LedBuff[5]; break;
        default: break;
    }
}
