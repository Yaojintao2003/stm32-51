#include "ds18b20.h"

//��������ʱ����
void Delay_OneWire(unsigned int t)  //STC89C52RC
{
	while(t--);
}

//ͨ����������DS18B20дһ���ֽ�
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(60);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(60);
}

//��DS18B20��ȡһ���ֽ�
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(60);
	}
	return dat;
}

//DS18B20�豸��ʼ��
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(144);
  	DQ = 0;
  	Delay_OneWire(960);
  	DQ = 1;
  	Delay_OneWire(120); 
    initflag = DQ;     
  	Delay_OneWire(60);
  	return initflag;
}

float wendu_get()
{
uchar high,low;
float wd;	
init_ds18b20();
Write_DS18B20(0xcc);
Write_DS18B20(0x44);	
  Delay_OneWire(60);
EA=0;
init_ds18b20();	
Write_DS18B20(0xcc);
Write_DS18B20(0xbe);	
low=Read_DS18B20();
high=Read_DS18B20();
wd=(high<<4)+(low>>4)+(low&0x0f)*0.0625;	
EA=1;	
return wd;
}




