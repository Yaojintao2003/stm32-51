#include "usart1.h"
uchar usart1_r[NUM];
uchar pc;
uchar usart1_ms;
bit u_f;

void usart1_init()
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0x8F;		//���ö�ʱ��ʼֵ
	T2H = 0xFD;		//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
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

