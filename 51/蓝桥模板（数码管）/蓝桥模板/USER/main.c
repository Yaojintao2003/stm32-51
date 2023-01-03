#include "num.h"
#include "base.h"
#include "shuma.h" 
uchar shuma[10];//数码显示内容个数不能超过自己设置的最大容量


void main()
{
 Timer1Init(); //定时器初始化
	base_init();//基本外设初始化如：关闭蜂鸣器，关闭led
while(1)
{
shuma_trans(shuma,sprintf(shuma,"2018-117"));//双引号内为自己想显示的任何内容包括字母。
}

}

/*****定时器1初始化函数*******/
void Timer1Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x20;		//设置定时初始值
	TH1 = 0xD1;		//设置定时初始值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	EA=1;       //打开总中断
	ET1=1;      //打开定时器中断
}



/*******定时器1中断服务函数***********/
void time1_handle() interrupt 3
{
static uint cent_ms;
	cent_ms++;                     
shuma_dis();	//每隔1ms自动更新显示段码表的内容
}





