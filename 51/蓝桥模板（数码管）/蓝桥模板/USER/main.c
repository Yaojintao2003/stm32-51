#include "num.h"
#include "base.h"
#include "shuma.h" 
uchar shuma[10];//������ʾ���ݸ������ܳ����Լ����õ��������


void main()
{
 Timer1Init(); //��ʱ����ʼ��
	base_init();//���������ʼ���磺�رշ��������ر�led
while(1)
{
shuma_trans(shuma,sprintf(shuma,"2018-117"));//˫������Ϊ�Լ�����ʾ���κ����ݰ�����ĸ��
}

}

/*****��ʱ��1��ʼ������*******/
void Timer1Init(void)		//1����@12.000MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x20;		//���ö�ʱ��ʼֵ
	TH1 = 0xD1;		//���ö�ʱ��ʼֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	EA=1;       //�����ж�
	ET1=1;      //�򿪶�ʱ���ж�
}



/*******��ʱ��1�жϷ�����***********/
void time1_handle() interrupt 3
{
static uint cent_ms;
	cent_ms++;                     
shuma_dis();	//ÿ��1ms�Զ�������ʾ����������
}





