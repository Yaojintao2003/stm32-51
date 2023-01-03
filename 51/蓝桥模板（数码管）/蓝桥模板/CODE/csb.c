#include "csb.h"

void csb_init()
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xF4;		//���ö�ʱ��ʼֵ
	TH0 = 0xFF;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 0;		//��ʱ��0��ʼ��ʱ
}



uchar csb_get()
{
uchar num=2,ds;
csb_tx=0;
TL0 = 0xF4;		//���ö�ʱ��ʼֵ
TH0 = 0xFF;		//���ö�ʱ��ʼֵ	
TR0 = 1;		//��ʱ��0��ʼ��ʱ
while(num--)
{
while(!TF0);
csb_tx^=1;
	TF0=0;
}	
TR0 = 0;
TL0 = 0x00;		//���ö�ʱ��ʼֵ
TH0 = 0x00;		//���ö�ʱ��ʼֵ	
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

