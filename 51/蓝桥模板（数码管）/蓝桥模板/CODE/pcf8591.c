#include "pcf8591.h"



float AD_get(uchar add)
{
uchar temp;
float ad;
EA=0;
IIC_Start(); 
IIC_SendByte(0x90);
IIC_WaitAck();	
IIC_SendByte(add);
IIC_WaitAck();	
IIC_Start(); 
IIC_SendByte(0x91);
IIC_WaitAck();	
temp=IIC_RecByte();
IIC_SendAck(1); 
IIC_Stop();
ad=temp/51.0;	
EA=1;	
return ad;
}

void DA_convent(float dat)
{
uchar temp;
	temp=dat*51;
IIC_Start(); 
IIC_SendByte(0x90);
IIC_WaitAck();	
IIC_SendByte(0x40);
IIC_WaitAck();	
IIC_SendByte(temp);
IIC_WaitAck();
IIC_Stop();
}