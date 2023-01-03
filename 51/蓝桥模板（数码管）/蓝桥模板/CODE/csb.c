#include "csb.h"

void csb_init()
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xF4;		//设置定时初始值
	TH0 = 0xFF;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0开始计时
}



uchar csb_get()
{
uchar num=2,ds;
csb_tx=0;
TL0 = 0xF4;		//设置定时初始值
TH0 = 0xFF;		//设置定时初始值	
TR0 = 1;		//定时器0开始计时
while(num--)
{
while(!TF0);
csb_tx^=1;
	TF0=0;
}	
TR0 = 0;
TL0 = 0x00;		//设置定时初始值
TH0 = 0x00;		//设置定时初始值	
TR0 = 1;
while(csb_rx&&!TF0);
TR0 = 0;
if(TF0)
{
ds=99;
TF0=0;
}else
{
ds=((TH0<<8)|TL0)*0.017;
}
		
return ds;
}

