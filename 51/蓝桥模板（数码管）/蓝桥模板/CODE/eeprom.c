#include "eeprom.h"

void Delay10ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 117;
	j = 184;
	do
	{
		while (--j);
	} while (--i);
}



void write_eeprom(uchar *p,uchar add,int leng)
{
IIC_Start(); 
IIC_SendByte(0xa0);
IIC_WaitAck();	
IIC_SendByte(add);
IIC_WaitAck();	
while(leng--)
{
IIC_SendByte(*p++);
IIC_WaitAck();	
IIC_Delay(200);
}
IIC_Stop();
Delay10ms();
}

void read_eeprom(uchar *p,uchar add,int leng )
{
EA=0;
IIC_Start(); 
IIC_SendByte(0xa0);
IIC_WaitAck();	
IIC_SendByte(add);
IIC_WaitAck();	
IIC_Start(); 
IIC_SendByte(0xa1);
IIC_WaitAck();	
while(leng--)
{
*p++=IIC_RecByte();
if(leng) 	IIC_SendAck(0); 
else      IIC_SendAck(1); 
}
IIC_Stop();
EA=1;
}
