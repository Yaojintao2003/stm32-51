#include "i2c.h"
#include "oledfont.h"
#include "oled.h"
#include "math.h"

uint8_t oled[1024];

/*********************OLED写数据************************************/ 
void OLED_WrDat(unsigned char IIC_Data)
{
	I2C_Send_Data(0x78,0x40,IIC_Data);
}
/*********************OLED写命令************************************/
void OLED_WrCmd(unsigned char IIC_Command)
{
	I2C_Send_Data(0x78,0x00,IIC_Command);
}
/*********************OLED 设置坐标************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
//  OLED_WrCmd(0xb0+y);
//  OLED_WrCmd(((x&0xf0)>>4)|0x10);
//  OLED_WrCmd((x&0x0f)|0x01);
} 
/*********************OLED全屏************************************/
void OLED_Fill(unsigned char bmp_dat) 
{
  unsigned char y,x;
  for(y=0;y<8;y++)
  {
    for(x=0;x<X_WIDTH;x++)
      oled[y*128+x] = bmp_dat;
  }
}
/*********************OLED复位************************************/
void OLED_CLS(void)
{
  unsigned char y,x;
  for(y=0;y<8;y++)
  {
    for(x=0;x<X_WIDTH;x++)
       oled[y*128+x] = 0;
  }
}
/*********************OLED初始化************************************/
void OLED_Init_I2C(void)
{
  HAL_Delay(100);//初始化之前的延时很重要！
  OLED_WrCmd(0xae);//--turn off oled panel
  OLED_WrCmd(0x00);//---set low column address
  OLED_WrCmd(0x10);//---set high column address
  OLED_WrCmd(0x40);//--set start line address  Set oledping RAM Display Start Line (0x00~0x3F)
  OLED_WrCmd(0x81);//--set contrast control register
  OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
  OLED_WrCmd(0xa1);//--Set SEG/Column oledping     0xa0左右反置 0xa1正常
  OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
  OLED_WrCmd(0xa6);//--set normal display
  OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
  OLED_WrCmd(0x3f);//--1/64 duty
  OLED_WrCmd(0xd3);//-set display offset	Shift oledping RAM Counter (0x00~0x3F)
  OLED_WrCmd(0x00);//-not offset
  OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
  OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
  OLED_WrCmd(0xd9);//--set pre-charge period
  OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  OLED_WrCmd(0xda);//--set com pins hardware configuration
  OLED_WrCmd(0x12);
  OLED_WrCmd(0xdb);//--set vcomh
  OLED_WrCmd(0x40);//Set VCOM Deselect Level
	
  OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02) //寻址模式选择
  OLED_WrCmd(0x00);//
	
  OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
  OLED_WrCmd(0x14);//--set(0x10) disable
  OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
  OLED_WrCmd(0xaf);//--turn on oled panel
} 


/*---------------LCD复位-----------------*/
void LCD_CLS(void)
{
  unsigned char y,x;
  for(y=0;y<8;y++)
  {
    for(x=0;x<X_WIDTH;x++)
      oled[y*128+x] = 0;
  }
}

/*------显示6X8一组标准的ASCII字符串，显示坐标为（x，y）------*/
void LCD_P6x8Str(unsigned char x,unsigned char  y,unsigned char ch[])
{
  unsigned char c=0,i=0,j=0;
  while (ch[j]!='\0')
  {
    c =ch[j]-32;
		i=128/6;
    if(x>=i*6){x=0;y++;}
    for(i=0;i<6;i++)
			oled[y*128+x+i]=F6x8[c][i];
    x+=6;
    j++;
  }
}
//显示一个6X8的字符
void LCD_P6x8Char(unsigned char x,unsigned char  y,unsigned char ucData)
{
  unsigned char i, ucDataTmp;
  ucDataTmp = ucData-32;
  if(x > 126)
  {
    x= 0;
    y++;
  }
  for(i = 0; i < 6; i++)
  {
    oled[y*128+x+i]=F6x8[ucDataTmp][i];
  }
}

/*--------------显示6X8的浮点数--------------*/
void write_6_8_number(unsigned char x,unsigned char y, float number)
{
  unsigned char i=0;
  unsigned char temp[16];
  unsigned char *point=temp;
  float decimal;
  int data;
  if(number<0)
  {
    temp[0]='-';
    LCD_P6x8Char(x,y,temp[0]);
    x+=6;
    number=-number;
  }
  data=(int)number;
  decimal=number-data;					//小数部分
  
  if(data>=1000000000)           //是否能被10^9整除
  {
    temp[i]=48+data/1000000000;
    data=data%1000000000;
    i++;
  }
  if(data>=100000000)           //是否能被10^8整除
  {
    temp[i]=48+data/100000000;
    data=data%100000000;
    i++;
  }
  else
    if(data<100000000&&i!=0)
    {
      temp[i]=0+48;
      i++;
    }
  if(data>=10000000)           //是否能被10^7整除
  {
    temp[i]=48+data/10000000;
    data=data%10000000;
    i++;
  }
  else
    if(data<10000000&&i!=0)
    {
      temp[i]=0+48;
      i++;
    }
  if(data>=1000000)           //是否能被10^6整除
  {
    temp[i]=48+data/1000000;
    data=data%1000000;
    i++;
  }
  else
    if(data<1000000&&i!=0)
    {
      temp[i]=0+48;
      i++;
    }
  if(data>=100000)           //是否能被100000整除
  {
    temp[i]=48+data/100000;
    data=data%100000;
    i++;
  }
  else
    if(data<100000&&i!=0)
    {
      temp[i]=0+48;
      i++;
    }
  if(data>=10000)           //是否能被10000整除
  {
    temp[i]=48+data/10000;
    data=data%10000;
    i++;
  }
  else
    if(data<10000&&i!=0)
    {
      temp[i]=0+48;
      i++;
    }
  if(data>=1000)           //是否能被1000整除
  {
    temp[i]=48+data/1000;
    data=data%1000;
    i++;
  }
  else
    if(data<1000&&i!=0)
    {
      temp[i]=0+48;
      i++;
    }
  if(data>=100)              //是否能被100整除
  {
    temp[i]=48+data/100;
    data=data%100;
    i++;
  }
  else
    if(data<100&&i!=0)
    {
      temp[i]=0+48;
      i++;
    }
  if(data>=10)                  //是否能被10整除
  {
    temp[i]=48+data/10;
    data=data%10;
    i++;
  }
  else
    if(data<10&&i!=0)
    {
      temp[i]=48;
      i++;
    }
  temp[i]=48+data;
  if(decimal>=0.0001f)           //判断是否有小数部分
  {
    i++;
    temp[i]='.';                //显示小数点
    i++;
    data=(int)(decimal*1000);
    temp[i]=48+data/100;
    data=data%100;
    i++;
    if(data>0)
    {
      temp[i]=48+data/10;
      data=data%10;
    }
    if(data>=0)
    {
      i++;
      temp[i]=data+48;
    }
  }
  i++;
  temp[i]='\0';
  LCD_P6x8Str(x,y,point);
}
/*------显示8X16一组标准的ASCII字符串，显示坐标为（x，y）------*/
void LCD_P8x16Str(unsigned char x,unsigned char  y,unsigned char ch[])
{
  unsigned char c=0,i=0,j=0;
  while (ch[j]!='\0')
  {
    c =ch[j]-32;
    if(x>120){x=0;y+=2;}
    for(i=0;i<8;i++)
      oled[y*128+x+i]=F8X16[c*16+i];
    for(i=0;i<8;i++)
			oled[y*128+128+x+i]=F8X16[c*16+i+8];
    x+=8;
    j++;
  }
}
//显示一个8X16的字符
void LCD_P8x16Char(unsigned char x,unsigned char  y,unsigned char ch)
{
  unsigned char c=0,i=0,j=0;
  c =ch-32;
  if(x>120){x=0;y+=2;} 
  for(i=0;i<8;i++)
		oled[y*128+x+i]=F8X16[c*16+i];
  for(i=0;i<8;i++)
		oled[y*128+128+x+i]=F8X16[c*16+i+8];
  x+=8;
  j++;
}
/*---------------------显示8X16的浮点数--------------------*/
void write_8_16_number(unsigned char x,unsigned char y, float number)
{
  unsigned char i=0;
  unsigned char temp[16];
  unsigned char *point=temp;
  float decimal;
  int data;
  
  if(number<0)
  {
    temp[0]='-';
    LCD_P8x16Char(x,y,temp[0]);
    x+=1;
    number=-number;
  }
  data=(int)number;
  decimal=number-data;     //小数部分
  if(data>=1000)           //是否可被1000整除
  {
    temp[i]=48+data/1000;
    data=data%1000;
    i++;
  }
  if(data>=100)              //可否被100整除
  {
    temp[i]=48+data/100;
    data=data%100;
    i++;
  }
  else
    if(data<100&&i!=0)
    {
      temp[i]=0+48;
      i++;
    }
  if(data>=10)                  //可否被10整除
  {
    temp[i]=48+data/10;
    data=data%10;
    i++;
  }
  else
    if(data<10&&i!=0)
    {
      temp[i]=48;
      i++;
    }
  temp[i]=48+data;
  if(decimal>=0.0001f)           //判断是够为小数
  {
    i++;
    temp[i]='.';                //显示小数点
    i++;
    data=(int)(decimal*1000);
    temp[i]=48+data/100;
    data=data%100;
    i++;
    if(data>0)
    {
      temp[i]=48+data/10;
      data=data%10;
    }
    if(data>=0)
    {
      i++;
      temp[i]=data+48;
    }
  }
  i++;
  temp[i]='\0';
  LCD_P8x16Str(x,y,point);
}


void LCD_clear_L(unsigned char x,unsigned char y)
{
  for(;x<X_WIDTH;x++)
  {
    oled[128*y+x]=0;
  }
}

void Draw_Logo(uint8_t *LOGO128x64)
{
  unsigned int ii=0;
  unsigned char x,y;
  for(y=0;y<8;y++)
  {
    for(x=0;x<128;x++)
    {
     oled[128*y+x]=LOGO128x64[ii++];
    }
  }   
}

/***************************************************
函数名: void OLED_Init(void)
说明:	OLED显示屏初始化
入口:	无
出口:	无
备注:	上电初始化，运行一次
****************************************************/
void OLED_Init(void)
{
#if I2C_SOFT<=1
	I2C_init();
#elif I2C_SOFT==2
	I2C_soft_init();
#endif
  OLED_Init_I2C();  
#if I2C_SOFT==2
	I2C_hard_init();
#endif
}

void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y1 = y1/8;
  else
		y1 = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
    for(x=x0;x<x1;x++)
		{
			oled[y*128+x] = BMP[j++];
		}
	}
}

void show_oled(){
	I2C_Send_nData(0x78,0x40,oled,1024);
}

void draw_point(int x,int y){
	int i,j;
	if(x<128&&x>=0&&y>=0&&y<64);
	else return;
	i=y/8;
	j=y%8;
	oled[i*128+x]=(0xff&(0x01<<j))|oled[i*128+x];
}

void draw_line(double x0,double y0,double x1,double y1){
	double i,j,k,n,y;
	if((y0-y1)==0||(x0-x1)==0)
	{
		if((y0-y1)==0){
			j=max(x0,x1);
			k=min(x0,x1);
			for(n=k;n<j;n=n+1){
				draw_point(n,y0);
			}
		}
		else {
			j=max(y0,y1);
			k=min(y0,y1);
			for(n=k;n<j;n=n+1){
				draw_point(x0,n);
			}
		}
	}
	else {
			j=max(x0,x1);
			k=min(x0,x1);
			i=(y0-y1)/(x0-x1);
			for(n=k;n<j;n=n+0.1){
				y=(n-x0)*i+y0;
				draw_point(n,y);
			}

	}
}

void LCD12864_6X8(int x,int y,char* ch){
	unsigned char c=0,i=0,j=0,n=0,m=0;
  while (ch[j]!='\0')
  {
    c =ch[j]-32;
		i=128/6;
    if(x>=i*6)return;
		n=y/8;
		m=y%8;
    for(i=0;i<6;i++){
			oled[n*128+x+i]=((F6x8[c][i]<<m)&(0x00ff<<m))|(oled[n*128+x+i]&(0x00ff>>(8-m)));
			oled[n*128+128+x+i]=((F6x8[c][i]>>(8-m))&(0x00ff>>(8-m)))|(oled[n*128+128+x+i]&(0x00ff<<m));
		}
    x+=6;
    j++;
  }
}
