/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2014 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.kingst.org
*
* 文件名：main.c
* 描  述：全板测试程序 工程主文件
* 版本号：v1.0.0
* 备  注：测试开发板上的各个部件
*******************************************************************************
*/

#define  _MAIN_C
#include "config.h"
#include "Lcd1602.h"
#include "LedBuzzer.h"
#include "keyboard.h"
#include "DS1302.h"
#include "DS18B20.h"
#include "Infrared.h"
#include "Uart.h"
#include "I2C.h"
#include "eeprom.h"
#include "PCF8591.h"
#include "main.h"


bit flag100ms = 0; //100ms定时标志
uint8 T0RH = 0;    //T0重载值的高字节
uint8 T0RL = 0;    //T0重载值的低字节

void main()
{
    uint8 tmr1s = 10;
    uint8 delay = 20;
    
    EA = 1;           //开总中断
    ConfigTimer0(1);  //配置T0定时1ms
    ConfigUART(9600); //配置串口通信
    InitLed();        //初始化LED模块
    InitInfrared();   //初始化红外接收模块
    InitLcd1602();    //初始化液晶模块
    Start18B20();     //启动首次温度转换
    SetDACOut(0);     //设置DAC输出0V
    BuzzBeep();       //蜂鸣器提示
    
    while (1)  //进入主循环
    {
        KeyDriver();      //执行按键驱动
        UartDriver();     //执行串口通信驱动
        InfraredDriver(); //执行红外接收驱动
        if (flag100ms)    //每隔100ms执行以下分支
        {
            flag100ms = 0;
            if (delay > 0)
            {
                delay--;
                if (delay == 0)
                {   //本段代码仅在上电延时后执行一次
                    InitDS1302();  //初始化实时时钟模块
                    CheckE2rom();  //执行EEPROM的检测
                }
                continue;
            }            
            FlowingLight();  //流水灯效果实现
            TestLedArray();  //LED矩阵测试实现
            ShowRealTime();  //时间显示刷新
            tmr1s++;         //每隔1s执行以下分支
            if (tmr1s >= 10)
            {
                tmr1s = 0;
                TestLedNumber(); //数码管测试显示
                RefreshTemp();   //温度刷新显示
                RefreshADC();    //刷新ADC测量值
            }
        }
    }
}
/* EEPROM检测函数，检查其是否工作正常 */
void CheckE2rom()
{
    uint8 buf[2];
    
    buf[0] = 0x33;  //设置检测数据
    buf[1] = 0xAA;
    E2Write(buf, 0x00, 2);  //由地址0x00写入
    buf[0] = 0x00;  //清零检测数据
    buf[1] = 0x00;
    E2Read(buf, 0x00, 2);  //由地址0x00读出并校验
    if ((buf[0]!=0x33) || (buf[1]!=0xAA))
    {
        LcdShowStr(3, 1, "N");  //数据错误时显示“N”
        return;
    }
    
    buf[0] = 0x55;  //设置检测数据
    buf[1] = 0xCC;
    E2Write(buf, 0x7F, 2);  //由地址0x7F写入
    buf[0] = 0x00;  //清零检测数据
    buf[1] = 0x00;
    E2Read(buf, 0x7F, 2);  //由地址0x7F读出并校验
    if ((buf[0]!=0x55) || (buf[1]!=0xCC))
    {
        LcdShowStr(3, 1, "N");  //数据错误时显示“N”
    }
    else
    {
        LcdShowStr(3, 1, "Y");  //数据正确时显示“Y”
    }
}
/* 将一个字节按十六进制格式显示到屏幕上，(x,y)-屏幕起始坐标，byt-待显示字节 */
void ShowHexByte(uint8 x, uint8 y, uint8 byt)
{
    uint8 tmp;
    uint8 str[4];
    
    //高四位转为十六进制字符
    tmp = byt >> 4;
    if (tmp <= 9)
        str[0] = tmp + '0';
    else
        str[0] = tmp - 10 + 'A';
    //低四位转为十六进制字符
    tmp = byt & 0x0F;
    if (tmp <= 9)
        str[1] = tmp + '0';
    else
        str[1] = tmp - 10 + 'A';
    //添加结束符并显示到液晶
    str[2] = '\0';
    LcdShowStr(x, y, str);
}
/* 按键动作函数，根据键码执行相应的操作，keycode-按键键码 */
void KeyAction(uint8 keycode)
{
	uint8 str[4];
    
    BuzzBeep();  //蜂鸣器“嘀”
    ShowHexByte(14, 0, keycode);  //显示键码
	//将键码值按十六进制形式转换为字符串
	str[0] = keycode >> 4;
	if (str[0] <= 9)
		str[0] += '0';
	else 
		str[0] += ('A'-10);
	str[1] = keycode & 0xF;
	if (str[1] <= 9)
		str[1] += '0';
	else
		str[1] += ('A'-10);
	str[2] = '\r';
	str[3] = '\n';
	//键码由串口发送
	UartWrite("key: 0x", sizeof("key: 0x")-1);
	UartWrite(str, 4);
}
/* 串口动作函数，根据接收到的命令帧执行响应的动作
   buf-接收到的命令帧指针，len-命令帧长度 */
void UartAction(uint8 *buf, uint8 len)
{
    BuzzBeep();  //蜂鸣器“嘀”
    UartWrite(buf, len);  //将接收到的数据原样送回
}
/* 读取DS1302时间，显示在液晶屏上 */
void ShowRealTime()
{
    struct sTime buf;
    static struct sTime pdata bkp = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};

    GetRealTime(&buf);
    if (bkp.hour != buf.hour)  //小时变化时刷新显示
    {
        ShowHexByte(0, 0, buf.hour);
        bkp.hour = buf.hour;
    }
    if (bkp.min != buf.min)  //分钟变化时刷新显示
    {
        ShowHexByte(3, 0, buf.min);
        bkp.min = buf.min;
    }
    if (bkp.sec != buf.sec)  //秒变化时刷新显示
    {
        ShowHexByte(6, 0, buf.sec);
        bkp.sec = buf.sec;
    }
}
/* 温度刷新函数，读取当前温度并根据需要刷新液晶显示 */
void RefreshTemp()
{
    int16 temp;
    uint8 pdata str[4];
    static int16 backup = 32767;  //保证首次执行时必定刷新
    
    Get18B20Temp(&temp); //获取当前温度值
    Start18B20();        //启动下一次转换
    temp >>= 4;          //舍弃4bit小数位
    if (backup != temp)  //温度变化时刷新液晶显示
    {
        str[0] = (temp/10) + '0';  //十位转为ASCII码
        str[1] = (temp%10) + '0';  //个位转为ASCII码
        str[2] = '\0';             //字符串结束符
        LcdShowStr(7, 1, str);    //显示到液晶上
        backup = temp;             //刷新上次温度值
    }
}
/* 获取ADC转换结果，并刷新显示 */
void RefreshADC()
{
    uint8 val;
    uint8 str[4];
    
    val = GetADCValue(0);  //获取当前转换值
    str[0] = (val/100) + '0';  //转换为3位十进制数字
    str[1] = (val%100)/10 + '0';
    str[2] = (val%10) + '0';
    str[3] = '\0';
    LcdShowStr(13, 1, str);  //显示到液晶上
}
/* 配置并启动T0，ms-T0定时时间 */
void ConfigTimer0(uint16 ms)
{
    uint32 tmp;
    
    tmp = (SYS_MCLK*ms)/1000; //计算所需的计数值
    tmp = 65536 - tmp;        //计算定时器重载值
    tmp = tmp + 33;           //补偿中断响应延时造成的误差   
    T0RH = (uint8)(tmp>>8);   //定时器重载值拆分为高低字节
    T0RL = (uint8)tmp;
    TMOD &= 0xF0;   //清零T0的控制位
    TMOD |= 0x01;   //配置T0为模式1
    TH0 = T0RH;     //加载T0重载值
    TL0 = T0RL;
    ET0 = 1;        //使能T0中断
    TR0 = 1;        //启动T0
}
/* T0中断服务函数，实现系统定时和按键扫描 */
void InterruptTimer0() interrupt 1
{
    static uint8 tmr100ms = 0;
    
    TH0 = T0RH;  //重新加载重载值
    TL0 = T0RL;
    BuzzCtrl();  //蜂鸣器发声控制
    LedScan();   //执行LED扫描
    KeyScan();   //执行按键扫描
    UartRxMonitor(1);  //执行串口接收监控
    tmr100ms++;  //定时100ms
    if (tmr100ms >= 100)
    {
        tmr100ms = 0;
        flag100ms = 1;
    }
}
