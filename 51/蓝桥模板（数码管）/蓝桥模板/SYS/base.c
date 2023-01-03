#include "base.h"


/*****基本外设初始化函数，其中包括了led和继电器，蜂鸣器的初始化配置******/
void base_init()
{
P0=0xff;
P2=(P2&0x1f)|0x80;	
P2=(P2&0x1f);	
P0=0x00;
P2=(P2&0x1f)|0xa0;	
P2=(P2&0x1f);	
}

void jidian(bit flag)
{
if(flag)
{
P0|=0x01<<4;
}else 
{
P0&=~(0x01<<4);
}
P2=(P2&0x1f)|0xa0;	
P2=(P2&0x1f);	
}

//例：led(x,1)点亮第x个led灯
//	  led(x,0)熄灭第x个led灯
void led(uchar x,bit flag)
{
static uchar led_temp=0xff;
	P0=led_temp;
if(flag)	
{
P0&=~(0x01<<(x-1));
}else
{
P0|=(0x01<<(x-1));
}
led_temp=P0;
P2=(P2&0x1f)|0x80;	
P2=(P2&0x1f);
}