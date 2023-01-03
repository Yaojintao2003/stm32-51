/*
*******************************************************************************
*                     《手把手教你学51单片机(C语言版)》
*                    配套 KST-51 单片机开发板 示例源代码
*
*         (c) 版权所有 2016 金沙滩工作室/清华大学出版社 保留所有权利
*                 获取更多资料请访问：http://www.qdkingst.com
*
* 文件名：main.c
* 描  述：蜂鸣器演奏简单乐谱——“生日歌”
* 版本号：v1.0.0
* 备  注：参考曲谱在project目录下
*******************************************************************************
*/
#include "reg52.h"
#include "config.h"

sbit BUZZ = P1^6;  //蜂鸣器控制引脚

uint16 code NoteFrequ[]={
    262, 294, 330, 349, 392, 440, 494,      //低音1~7
    523, 578, 659, 698, 784, 880, 988,      //中音1~7
    1046,1175,1318,1397,1568,1760,1976      //高音1~7
};

uint16 code NoteReload[]={           //低音、中音、高音对应的定时器重装载值
    65535-(11059200/12)/( 262*2),    //低音1~7
    65535-(11059200/12)/( 294*2),
    65535-(11059200/12)/( 330*2),
    65535-(11059200/12)/( 349*2),
    65535-(11059200/12)/( 392*2),
    65535-(11059200/12)/( 440*2),
    65535-(11059200/12)/( 494*2),

    65535-(11059200/12)/( 523*2),    //中音1~7
    65535-(11059200/12)/( 578*2),
    65535-(11059200/12)/( 659*2),
    65535-(11059200/12)/( 698*2),
    65535-(11059200/12)/( 784*2),
    65535-(11059200/12)/( 880*2),
    65535-(11059200/12)/( 988*2),

    65535-(11059200/12)/(1046*2),    //高音1~7
    65535-(11059200/12)/(1175*2),
    65535-(11059200/12)/(1318*2),
    65535-(11059200/12)/(1397*2),
    65535-(11059200/12)/(1568*2),
    65535-(11059200/12)/(1760*2),
    65535-(11059200/12)/(1976*2),
};

uint16 T0RH = 0xFF;    //T0高位
uint16 T0RL = 0x00;    //T0低位
bit timeflag = 0;      //定时器中断完成标志
bit enable = 1;        //蜂鸣器使能标志；

void PlayHappyBrithday();

void main()
{
    uint16 i;

    EA = 1;            //使能全局中断
    TMOD = 0x01;       //配置T0工作在模式1
    TH0 = T0RH;
    TL0 = T0RL;
    ET0 = 1;           //使能T0中断
    TR0 = 1;           //启动T0

    while(1)
    {
        PlayHappyBrithday();       //播放乐曲--生日歌
        for (i=0; i<40000; i++);   //停止一段时间
    }
}

void PlayHappyBrithday()
{
    uint8 beat;                        //当前节拍索引
    uint8 note;                        //当前音符索引
    uint16 time = 0;                   //当前节拍计时
    uint16 beatTime = 0;               //当前节拍总时间
    uint16 soundTime = 0;              //当前节拍发声时间
    const uint8 code HappyBrithdayNote[] = {   //生日歌音符表：1~7表示低音1~7、8~14表示中音1~7、15~21表示高音1~7、255表示空音符
        5,  5,  6,  5,  8,  7,         //第1、2小节
        5,  5,  6,  5,  9,  8,         //第3、4小节
        5,  5,  12, 10, 8,  7,  6,     //第5、6小节
        255,255,11, 11, 10, 8,  9,     //第7、8小节
        8,  255,255                        //第9小节
    };
    const uint8 code HappyBrithdayBeat[] = {   //节拍表：一拍用4表示、1/4拍=1、2拍=8
        2,  2,  4,  4,  4,  8,         //第1、2小节
        2,  2,  4,  4,  4,  8,         //第3、4小节
        2,  2,  4,  4,  4,  4,  4,     //第5、6小节
        4,  4,  2,  2,  4,  4,  4,     //第7、8小节
        4,  4,  4                      //第9小节
    };

    for (beat=0; beat<sizeof(HappyBrithdayNote); )  //用节拍索引作为循环变量
    {
        while(!timeflag);    //每次定时器中断完成后，检测处理节拍
        timeflag = 0;
        if(time == 0)        //当前节拍播放完，启动新节拍
        {
            note = HappyBrithdayNote[beat] - 1;
            if(note >= 100)  //检测到空音符时，只启动节拍而不发声
            {
                T0RH = NoteReload[0] >> 8;
                T0RL = NoteReload[0];
                beatTime = (HappyBrithdayBeat[beat] * NoteFrequ[0]) >> 2;
                soundTime = beatTime - (beatTime >> 2);
                enable = 0;  //指示蜂鸣器不发声
                time++;
            }
            else
            {
                T0RH = NoteReload[note] >> 8;
                T0RL = NoteReload[note];
                //计算节拍总时间，右移2位相当于除4，移位代替除法可以加快执行速度
                beatTime = (HappyBrithdayBeat[beat] * NoteFrequ[note]) >> 2;
                //计算发声时间，为总时间的0.75，移位原理同上
                soundTime = beatTime - (beatTime >> 2);
                enable = 1;  //指示蜂鸣器开始发声
                time++;
            }
        }
        else  //当前节拍未播完则处理当前节拍
        {
            if (time >= beatTime)  //当前持续时间到达节拍总时间时归零，
            {                      //并递增节拍索引，以准备启动新节拍
                time = 0;
                beat++;
            }
            else  //当前持续时间未达到总时间时，
            {
                time++;   //累加时间计数
                if (time == soundTime)  //到达发声时间后，指示关闭蜂鸣器，
                {                       //插入0.25*总时间的静音间隔，
                    enable = 0;         //用以区分连续的两个节拍
                }
            }
        }
    }
}

/* T0中断服务函数，用于控制蜂鸣器发声 */
void InterruptTimer0() interrupt 1
{
    TH0 = T0RH;   //重新加载重载值
    TL0 = T0RL;
    timeflag = 1;
    if (enable)   //使能时反转蜂鸣器控制电平
        BUZZ = ~BUZZ;
    else          //未使能时关闭蜂鸣器
        BUZZ = 1;
}
