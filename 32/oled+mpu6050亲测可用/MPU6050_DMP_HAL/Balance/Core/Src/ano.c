#include "ano.h"
#include "usart.h"

/*������λ���汾 �����ƴ�����վV4.22 Э��汾4.01*/

//����1����1���ַ� 
//c:Ҫ���͵��ַ�
void usart2_send_char(u8 c)
{
	while((USART1->SR&0X40)==0);//�ȴ���һ�η������   
	USART1->DR=c;   	
} 

//fun:������. 0XA0~0XAF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart2_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[33];
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+4]=0;	//У��������
	send_buf[0]=0XAA;	//֡ͷ
	send_buf[1] =0xAA;
	send_buf[2]=fun;	//������
	send_buf[3]=len;	//���ݳ���
	for(i=0;i<len;i++)
		send_buf[4+i]=data[i];			//��������
	for(i=0;i<len+4;i++)
		send_buf[len+4]+=send_buf[i];	//����У���	
	for(i=0;i<len+5;i++)
		usart2_send_char(send_buf[i]);	//�������ݵ�����1

}
//���ͼ��ٶȴ��������ݺ�����������
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 i=0;
	u8 tbuf[18]; 
	for(i=0;i<18;i++)
	{tbuf[i] = 0;}
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	usart2_niming_report(0X02,tbuf,18);
}	

//�ϱ��������̬����
void mpu6050_send_status_data(short roll,short pitch,short yaw)
{
	u8 i=0;
	u8 tbuf[12]; 
	for(i=0;i<12;i++)
	{tbuf[i] = 0;}
	tbuf[0]=(roll>>8)&0XFF;
	tbuf[1]=roll&0XFF;
	tbuf[2]=(pitch>>8)&0XFF;
	tbuf[3]=pitch&0XFF;
	tbuf[4]=(yaw>>8)&0XFF;
	tbuf[5]=yaw&0XFF; 
//	tbuf[6]=(gyrox>>8)&0XFF;
//	tbuf[7]=gyrox&0XFF;
//	tbuf[8]=(gyroy>>8)&0XFF;
//	tbuf[9]=gyroy&0XFF;
//	tbuf[10]=(gyroz>>8)&0XFF;
//	tbuf[11]=gyroz&0XFF;
	usart2_niming_report(0X01,tbuf,12);
}	




