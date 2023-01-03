/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2014 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.kingst.org
*
* 文件名：PCF8591.c
* 描  述：AD/DA芯片PCF8591驱动模块
* 版本号：v1.0.0
* 备  注：
*******************************************************************************
*/

#define  _PCF8591_C
#include "config.h"
#include "I2C.h"
#include "PCF8591.h"

/* 读取当前的ADC转换值，chn-ADC通道号0~3 */
uint8 GetADCValue(uint8 chn)
{
    uint8 val;
    
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

/* 设置DAC输出值，val-设定值 */
void SetDACOut(uint8 val)
{
    I2CStart();
    if (!I2CWrite(0x48<<1)) //寻址PCF8591，如未应答，则停止操作并返回
    {
        I2CStop();
        return;
    }
    I2CWrite(0x40);         //写入控制字节
    I2CWrite(val);          //写入DA值  
    I2CStop();
}
