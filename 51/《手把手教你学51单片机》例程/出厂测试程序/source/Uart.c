/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2014 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.kingst.org
*
* 文件名：Uart.c
* 描  述：基于帧模式的UART串口驱动模块
* 版本号：v1.0.0
* 备  注：通信波特率配置为由T2产生
*******************************************************************************
*/

#define  _UART_C
#include "config.h"
#include "Uart.h"
#include "main.h"

bit flagFrame = 0;  //帧接收完成标志，即接收到一帧新数据
bit flagTxd = 0;    //单字节发送完成标志，用来替代TXD中断标志位
uint8 cntRxd = 0;   //接收字节计数器
uint8 pdata bufRxd[50];  //接收字节缓冲区

extern void UartAction(uint8 *buf, uint8 len);

/* 串口配置函数，baud-通信波特率 */
void ConfigUART(uint16 baud)
{
    SCON  = 0x50;  //配置串口为模式1
    T2CON = 0x30;  //配置T2为重载模式提供波特率
    RCAP2H = (65536-(OSC_FREQ/baud/32)) >> 8;  //计算波特率对应重载值
    RCAP2L = (65536-(OSC_FREQ/baud/32))&0xFF;
    TH2 = RCAP2H;  //初值等于重载值
    TL2 = RCAP2L;
    ET2 = 0;       //禁止T2中断
    ES  = 1;       //使能串口中断
    TR2 = 1;       //启动T2
}
/* 串口数据写入，即串口发送函数，buf-待发送数据的指针，len-指定的发送长度 */
void UartWrite(uint8 *buf, uint8 len)
{
    while (len--)  //循环发送所有字节
    {
        flagTxd = 0;      //清零发送标志
        SBUF = *buf++;    //发送一个字节数据
        while (!flagTxd); //等待该字节发送完成
    }
}
/* 串口数据读取函数，buf-接收指针，len-指定的读取长度，返回值-实际读到的长度 */
uint8 UartRead(uint8 *buf, uint8 len)
{
    uint8 i;
    
    if (len > cntRxd)  //指定读取长度大于实际接收到的数据长度时，
    {                  //读取长度设置为实际接收到的数据长度
        len = cntRxd;
    }
    for (i=0; i<len; i++)  //拷贝接收到的数据到接收指针上
    {
        *buf++ = bufRxd[i];
    }
    cntRxd = 0;  //接收计数器清零
    
    return len;  //返回实际读取长度
}
/* 串口接收监控，由空闲时间判定帧结束，需在定时中断中调用，ms-定时间隔 */
void UartRxMonitor(uint8 ms)
{
    static uint8 cntbkp = 0;
    static uint8 idletmr = 0;

    if (cntRxd > 0)  //接收计数器大于零时，监控总线空闲时间
    {
        if (cntbkp != cntRxd)  //接收计数器改变，即刚接收到数据时，清零空闲计时
        {
            cntbkp = cntRxd;
            idletmr = 0;
        }
        else                   //接收计数器未改变，即总线空闲时，累积空闲时间
        {
            if (idletmr < 30)  //空闲计时小于30ms时，持续累加
            {
                idletmr += ms;
                if (idletmr >= 30)  //空闲时间达到30ms时，即判定为一帧接收完毕
                {
                    flagFrame = 1;  //设置帧接收完成标志
                }
            }
        }
    }
    else
    {
        cntbkp = 0;
    }
}
/* 串口驱动函数，监测数据帧的接收，调度功能函数，需在主循环中调用 */
void UartDriver()
{
    uint8 len;
    uint8 pdata buf[40];

    if (flagFrame) //有命令到达时，读取处理该命令
    {
        flagFrame = 0;
        len = UartRead(buf, sizeof(buf));  //将接收到的命令读取到缓冲区中
        UartAction(buf, len);  //传递数据帧，调用动作执行函数
    }
}
/* 串口中断服务函数 */
void InterruptUART() interrupt 4
{
    if (RI)  //接收到新字节
    {
        RI = 0;  //清零接收中断标志位
        if (cntRxd < sizeof(bufRxd)) //接收缓冲区尚未用完时，
        {                            //保存接收字节，并递增计数器
            bufRxd[cntRxd++] = SBUF;
        }
    }
    if (TI)  //字节发送完毕
    {
        TI = 0;   //清零发送中断标志位
        flagTxd = 1;  //设置字节发送完成标志
    }
}
