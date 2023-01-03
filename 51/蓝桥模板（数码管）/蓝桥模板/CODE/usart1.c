#include "usart1.h"
uchar usart1_r[NUM];
uchar pc;
uchar usart1_ms;
bit u_f;

void usart1_init()
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x8F;		//设置定时初始值
	T2H = 0xFD;		//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
  ES=1;
}



void usart1_send(uchar *p,int leng)
{
ET1=0;	
while(leng--)
{
SBUF=*p++;
while(!TI);
TI=0;
}	
ET1=1;
}


void usart1_handle()interrupt 4
{
if(RI)
{
usart1_ms=0;
	u_f=1;
usart1_r[pc]=SBUF;
	pc++;	if(pc>NUM-1){pc=0;}
RI=0;
}
}

