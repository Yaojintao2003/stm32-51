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
* 备  注：
*******************************************************************************
*/

#define  _LED_BUZZER_C
#include "config.h"
#include "LedBuzzer.h"

uint8 code LedChar[] = {  //数码管显示字符转换表
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};

uint8 staBuzzer = 0; //蜂鸣器状态控制，非0-鸣叫、0-关闭
struct sLedBuff ledBuff; //LED显示缓冲区，默认初值全0，正好达到上电全亮的效果

/* LED初始化函数，初始化IO */
void InitLed()
{
    //初始化IO口
    P0 = 0xFF;
    ENLED = 0;
}
/* 流水灯实现函数，间隔调用实现流动效果 */
void FlowingLight()
{
    static bit dir = 0;   //定义移位方向变量dir，用于控制移位的方向
    static uint8 shift = 0x01;  //定义循环移位变量shift，并赋初值0x01
    
    ledBuff.alone = ~shift;   //循环移位变量取反送到独立LED的显示缓冲区
    if (dir == 0)             //移位方向变量为0时，左移
    {
        shift = shift << 1;   //循环移位变量左移1位
        if (shift == 0x80)    //左移到最左端后，改变移位方向
        {
            dir = 1;
        }
    }
    else                      //移位方向变量不为0时，右移
    {
        shift = shift >> 1;   //循环移位变量右移1位
        if (shift == 0x01)    //右移到最右端后，改变移位方向
        {
            dir = 0;
        }
    }
}
/* 数码管测试函数，间隔调用以实现测试效果 */
void TestLedNumber()
{
    uint8 i;
    static int8 cnt = 0;
    static int8 dot = sizeof(ledBuff.number)-1;
    
    for (i=0; i<sizeof(ledBuff.number); i++)  //测试数字填充数码管缓冲区
    {
        ledBuff.number[i] = LedChar[cnt];
    }
    cnt++;                                    //实现测试数字的0～9循环
    if (cnt >= 10)
    {
        cnt = 0;
    }
    ledBuff.number[dot] &= 0x7F;  //点亮对应位的小数点
    dot--;                        //实现小数点位置循环
    if (dot < 0)
    {
        dot = sizeof(ledBuff.number)-1;
    }
}
/* 矩阵LED螺旋曲线动画演示函数，间隔调用以实现动画 */
void TestLedArray()
{
    uint8 i;
    static uint8 frame=0;
    static const uint8 code spiral[][8] = {
        {0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,},
        {0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00,},
        {0x00, 0x00, 0x00, 0x18, 0x10, 0x00, 0x00, 0x00,},
        {0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,},
        {0x00, 0x00, 0x00, 0x18, 0x1C, 0x00, 0x00, 0x00,},
        {0x00, 0x00, 0x00, 0x1C, 0x1C, 0x00, 0x00, 0x00,},
        {0x00, 0x00, 0x04, 0x1C, 0x1C, 0x00, 0x00, 0x00,},
        {0x00, 0x00, 0x0C, 0x1C, 0x1C, 0x00, 0x00, 0x00,},

        {0x00, 0x00, 0x1C, 0x1C, 0x1C, 0x00, 0x00, 0x00,},
        {0x00, 0x00, 0x3C, 0x1C, 0x1C, 0x00, 0x00, 0x00,},
        {0x00, 0x00, 0x3C, 0x3C, 0x1C, 0x00, 0x00, 0x00,},
        {0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x00, 0x00, 0x00,},
        {0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x20, 0x00, 0x00,},
        {0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x30, 0x00, 0x00,},
        {0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x38, 0x00, 0x00,},
        {0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x00, 0x00,},

        {0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x3E, 0x00, 0x00,},
        {0x00, 0x00, 0x3C, 0x3C, 0x3E, 0x3E, 0x00, 0x00,},
        {0x00, 0x00, 0x3C, 0x3E, 0x3E, 0x3E, 0x00, 0x00,},
        {0x00, 0x00, 0x3E, 0x3E, 0x3E, 0x3E, 0x00, 0x00,},
        {0x00, 0x02, 0x3E, 0x3E, 0x3E, 0x3E, 0x00, 0x00,},
        {0x00, 0x06, 0x3E, 0x3E, 0x3E, 0x3E, 0x00, 0x00,},
        {0x00, 0x0E, 0x3E, 0x3E, 0x3E, 0x3E, 0x00, 0x00,},
        {0x00, 0x1E, 0x3E, 0x3E, 0x3E, 0x3E, 0x00, 0x00,},

        {0x00, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x00, 0x00,},
        {0x00, 0x7E, 0x3E, 0x3E, 0x3E, 0x3E, 0x00, 0x00,},
        {0x00, 0x7E, 0x7E, 0x3E, 0x3E, 0x3E, 0x00, 0x00,},
        {0x00, 0x7E, 0x7E, 0x7E, 0x3E, 0x3E, 0x00, 0x00,},
        {0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x3E, 0x00, 0x00,},
        {0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x00, 0x00,},
        {0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x40, 0x00,},
        {0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x60, 0x00,},

        {0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x70, 0x00,},
        {0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x78, 0x00,},
        {0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7C, 0x00,},
        {0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x00,},
        {0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7F, 0x00,},
        {0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x7F, 0x7F, 0x00,},
        {0x00, 0x7E, 0x7E, 0x7E, 0x7F, 0x7F, 0x7F, 0x00,},
        {0x00, 0x7E, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x00,},

        {0x00, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00,},
        {0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00,},
        {0x01, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00,},
        {0x03, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00,},
        {0x07, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00,},
        {0x0F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00,},
        {0x1F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00,},
        {0x3F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00,},

        {0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00,},
        {0xFF, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00,},
        {0xFF, 0xFF, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00,},
        {0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x7F, 0x7F, 0x00,},
        {0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x7F, 0x00,},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x00,},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x00,},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,},

        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80,},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0,},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0,},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8,},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC,},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,},
    };

    if (frame < sizeof(spiral)/sizeof(spiral[0]))  //逐个点点亮
    {
        for (i=0; i<8; i++)
        {
            ledBuff.array[i] = ~spiral[frame][i];
        }
        frame++;
    }
    else if (frame < (sizeof(spiral)/sizeof(spiral[0]))*2)  //逐个点熄灭
    {
        for (i=0; i<8; i++)
        {
            ledBuff.array[i] = spiral[frame-sizeof(spiral)/sizeof(spiral[0])][i];
        }
        frame++;
    }
    else  //执行完一遍后再循环
    {
        frame = 0;
    }
}
/* LED动态显示扫描，需在定时中断中调用 */
void LedScan()
{
    static uint8 i = 0;  //LED位选索引
    
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
}
/* 蜂鸣器“嘀”一声 */
void BuzzBeep()
{
    staBuzzer = 80;
}
/* 蜂鸣器发声控制，需在定时中断中调用 */
void BuzzCtrl()
{
    if (staBuzzer != 0)
    {
        BUZZER = ~BUZZER;  //蜂鸣器鸣叫
        staBuzzer--;
    }
    else
    {
        BUZZER = 1;        //蜂鸣器静音
    }
}
