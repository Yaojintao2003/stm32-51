#include "ano.h"
#include "usart.h"

/*适用上位机版本 匿名科创地面站V4.22 协议版本4.01*/

//串口1发送1个字符 
//c:要发送的字符
void usart2_send_char(u8 c)
{
	while((USART1->SR&0X40)==0);//等待上一次发送完毕   
	USART1->DR=c;   	
} 

//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart2_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[33];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+4]=0;	//校验数置零
	send_buf[0]=0XAA;	//帧头
	send_buf[1] =0xAA;
	send_buf[2]=fun;	//功能字
	send_buf[3]=len;	//数据长度
	for(i=0;i<len;i++)
		send_buf[4+i]=data[i];			//复制数据
	for(i=0;i<len+4;i++)
		send_buf[len+4]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+5;i++)
		usart2_send_char(send_buf[i]);	//发送数据到串口1

}
//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
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

//上报解算的姿态数据
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




