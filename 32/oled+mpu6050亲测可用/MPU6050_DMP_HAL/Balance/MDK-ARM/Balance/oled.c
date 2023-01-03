//              说明: 
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PD6（SCL）
//              SDA   接PD7（SDA）            
//              ----------------------------------------------------------------

#include "oled.h"
#include "oledfont.h"  	
#include "stm32f1xx_hal.h"
#include "ioiic.h"
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
/**********************************************
//IIC Start
**********************************************/


void WriteDat(unsigned char I2C_Data)//写数据
{
	IIC_Start();
	IIC_Send_Byte(0x78); //oled的地址
	IIC_Wait_Ack();
	IIC_Send_Byte(0x40); //寄存器地址
	IIC_Wait_Ack();
	IIC_Send_Byte(I2C_Data);
	IIC_Wait_Ack();
	IIC_Stop();
}



void WriteCmd(unsigned char I2C_Command)//写命令
{
	IIC_Start();
	IIC_Send_Byte(0x78); //oled的地址
	IIC_Wait_Ack();
	IIC_Send_Byte(0x00); //寄存器地址
	IIC_Wait_Ack();
	IIC_Send_Byte(I2C_Command);
	IIC_Wait_Ack();
	IIC_Stop();
}


/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
		{
			WriteDat(fill_Data);
		}
	}
}

//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	WriteCmd(0X8D);  //SET DCDC命令
	WriteCmd(0X14);  //DCDC ON
	WriteCmd(0XAF);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	WriteCmd(0X8D);  //SET DCDC命令
	WriteCmd(0X10);  //DCDC OFF
	WriteCmd(0XAE);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		WriteCmd (0xb0+i);    //设置页地址（0~7）
		WriteCmd (0x00);      //设置显示位置—列低地址
		WriteCmd (0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++) WriteDat(0); 
	} //更新显示
}
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		WriteCmd (0xb0+i);    //设置页地址（0~7）
		WriteCmd (0x00);      //设置显示位置—列低地址
		WriteCmd (0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)WriteDat(1); 
	} //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
		{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			WriteDat(F8X16[c*16+i]);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			WriteDat(F8X16[c*16+i+8]);
		}
		else 
		{	
			OLED_Set_Pos(x,y);
			for(i=0;i<6;i++)
			WriteDat(F6x8[c][i]);
		}
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				WriteDat(Hzk[2*no][t]);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
		{	
			WriteDat(Hzk[2*no+1][t]);
			adder+=1;
		}					
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	WriteDat(BMP[j++]);	    	
	    }
	}
} 

//初始化SSD1306					    
void OLED_Init(void)
{ 	
 
	HAL_Delay(100);
	
	WriteCmd(0xAE);//--display off
	WriteCmd(0x00);//---set low column address
	WriteCmd(0x10);//---set high column address
	WriteCmd(0x40);//--set start line address  
	WriteCmd(0xB0);//--set page address
	WriteCmd(0x81); // contract control
	WriteCmd(0xFF);//--128   
	WriteCmd(0xA1);//set segment remap 
	WriteCmd(0xA6);//--normal / reverse
	WriteCmd(0xA8);//--set multiplex ratio(1 to 64)
	WriteCmd(0x3F);//--1/32 duty
	WriteCmd(0xC8);//Com scan direction
	WriteCmd(0xD3);//-set display offset
	WriteCmd(0x00);//
	
	WriteCmd(0xD5);//set osc division
	WriteCmd(0x80);//
	
	WriteCmd(0xD8);//set area color mode off
	WriteCmd(0x05);//
	
	WriteCmd(0xD9);//Set Pre-Charge Period
	WriteCmd(0xF1);//
	
	WriteCmd(0xDA);//set com pin configuartion
	WriteCmd(0x12);//
	
	WriteCmd(0xDB);//set Vcomh
	WriteCmd(0x30);//
	
	WriteCmd(0x8D);//set charge pump enable
	WriteCmd(0x14);//
	
	WriteCmd(0xAF);//--turn on oled panel
	
} 


