#include "base.h"


/*****���������ʼ�����������а�����led�ͼ̵������������ĳ�ʼ������******/
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

//����led(x,1)������x��led��
//	  led(x,0)Ϩ���x��led��
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