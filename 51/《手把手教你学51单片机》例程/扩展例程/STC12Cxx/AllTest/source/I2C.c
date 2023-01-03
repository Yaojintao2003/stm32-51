/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2014 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.kingst.org
*
* 文件名：I2C.c
* 描  述：I2C总线驱动模块
* 版本号：v1.0.0
* 备  注：基于IO口模拟实现，总线时序延时等皆由软件方式实现
*         软件延时函数的准确度基于STC12系列单片机在12M晶振的情况
*******************************************************************************
*/

#define  _I2C_C
#include "config.h"
#include "I2C.h"

/* STC12系列单片机在12M晶振下产生5us的软件延时 */
void I2CDelay()
{
    uint8 us = 5;
    
    do {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    } while (--us);
}
/* 产生总线起始信号 */
void I2CStart()
{
    I2C_SDA = 1; //首先确保SDA、SCL都是高电平
    I2C_SCL = 1;
    I2CDelay();
    I2C_SDA = 0; //先拉低SDA
    I2CDelay();
    I2C_SCL = 0; //再拉低SCL
}
/* 产生总线停止信号 */
void I2CStop()
{
    I2C_SCL = 0; //首先确保SDA、SCL都是低电平
    I2C_SDA = 0;
    I2CDelay();
    I2C_SCL = 1; //先拉高SCL
    I2CDelay();
    I2C_SDA = 1; //再拉高SDA
    I2CDelay();
}
/* I2C总线写操作，dat-待写入字节，返回值-从机应答位的值 */
bit I2CWrite(uint8 dat)
{
    bit ack;  //用于暂存应答位的值
    uint8 mask;  //用于探测字节内某一位值的掩码变量

    for (mask=0x80; mask!=0; mask>>=1) //从高位到低位依次进行
    {
        if ((mask&dat) == 0)  //该位的值输出到SDA上
            I2C_SDA = 0;
        else
            I2C_SDA = 1;
        I2CDelay();
        I2C_SCL = 1;          //拉高SCL
        I2CDelay();
        I2C_SCL = 0;          //再拉低SCL，完成一个位周期
    }
    I2C_SDA = 1;   //8位数据发送完后，主机释放SDA，以检测从机应答
    I2CDelay();
    I2C_SCL = 1;   //拉高SCL
    ack = I2C_SDA; //读取此时的SDA值，即为从机的应答值
    I2CDelay();
    I2C_SCL = 0;   //再拉低SCL完成应答位，并保持住总线

    return (~ack); //应答值取反以符合通常的逻辑：
                   //0=不存在或忙或写入失败，1=存在且空闲或写入成功
}
/* I2C总线读操作，并发送非应答信号，返回值-读到的字节 */
uint8 I2CReadNAK()
{
    uint8 mask;
    uint8 dat;

    I2C_SDA = 1;  //首先确保主机释放SDA
    for (mask=0x80; mask!=0; mask>>=1) //从高位到低位依次进行
    {
        I2CDelay();
        I2C_SCL = 1;      //拉高SCL
        if(I2C_SDA == 0)  //读取SDA的值
            dat &= ~mask; //为0时，dat中对应位清零
        else
            dat |= mask;  //为1时，dat中对应位置1
        I2CDelay();
        I2C_SCL = 0;      //再拉低SCL，以使从机发送出下一位
    }
    I2C_SDA = 1;   //8位数据发送完后，拉高SDA，发送非应答信号
    I2CDelay();
    I2C_SCL = 1;   //拉高SCL
    I2CDelay();
    I2C_SCL = 0;   //再拉低SCL完成非应答位，并保持住总线

    return dat;
}
/* I2C总线读操作，并发送应答信号，返回值-读到的字节 */
uint8 I2CReadACK()
{
    uint8 mask;
    uint8 dat;

    I2C_SDA = 1;  //首先确保主机释放SDA
    for (mask=0x80; mask!=0; mask>>=1) //从高位到低位依次进行
    {
        I2CDelay();
        I2C_SCL = 1;      //拉高SCL
        if(I2C_SDA == 0)  //读取SDA的值
            dat &= ~mask; //为0时，dat中对应位清零
        else
            dat |= mask;  //为1时，dat中对应位置1
        I2CDelay();
        I2C_SCL = 0;      //再拉低SCL，以使从机发送出下一位
    }
    I2C_SDA = 0;   //8位数据发送完后，拉低SDA，发送应答信号
    I2CDelay();
    I2C_SCL = 1;   //拉高SCL
    I2CDelay();
    I2C_SCL = 0;   //再拉低SCL完成应答位，并保持住总线

    return dat;
}
