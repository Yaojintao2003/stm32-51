#include "i2c.h"
#include "oledfont.h"
#include "oled.h"
#include "math.h"

uint8_t oled[1024];

/*********************OLEDд����************************************/ 
void OLED_WrDat(unsigned char IIC_Data)
{
	I2C_Send_Data(0x78,0x40,IIC_Data);
}
/*********************OLEDд����************************************/
void OLED_WrCmd(unsigned char IIC_Command)
{
	I2C_Send_Data(0x78,0x00,IIC_Command);
}
/*********************OLED ��������************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
//  OLED_WrCmd(0xb0+y);
//  OLED_WrCmd(((x&0xf0)>>4)|0x10);
//  OLED_WrCmd((x&0x0f)|0x01);
} 
/*********************OLEDȫ��************************************/
void OLED_Fill(unsigned char bmp_dat) 
{
  unsigned char y,x;
  for(y=0;y<8;y++)
  {
    for(x=0;x<X_WIDTH;x++)
      oled[y*128+x] = bmp_dat;
  }
}
/*********************OLED��λ************************************/
void OLED_CLS(void)
{
  unsigned char y,x;
  for(y=0;y<8;y++)
  {
    for(x=0;x<X_WIDTH;x++)
       oled[y*128+x] = 0;
  }
}
/*********************OLED��ʼ��************************************/
void OLED_Init_I2C(void)
{
  HAL_Delay(100);//��ʼ��֮ǰ����ʱ����Ҫ��
  OLED_WrCmd(0xae);//--turn off oled panel
  OLED_WrCmd(0x00);//---set low column address
  OLED_WrCmd(0x10);//---set high column address
  OLED_WrCmd(0x40);//--set start line address  Set oledping RAM Display Start Line (0x00~0x3F)
  OLED_WrCmd(0x81);//--set contrast control register
  OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
  OLED_WrCmd(0xa1);//--Set SEG/Column oledping     0xa0���ҷ��� 0xa1����
  OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
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
	
  OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02) //Ѱַģʽѡ��
  OLED_WrCmd(0x00);//
	
  OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
  OLED_WrCmd(0x14);//--set(0x10) disable
  OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
  OLED_WrCmd(0xaf);//--turn on oled panel
} 


/*---------------LCD��λ-----------------*/
void LCD_CLS(void)
{
  unsigned char y,x;
  for(y=0;y<8;y++)
  {
    for(x=0;x<X_WIDTH;x++)
      oled[y*128+x] = 0;
  }
}

/*------��ʾ6X8һ���׼��ASCII�ַ�������ʾ����Ϊ��x��y��------*/
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
//��ʾһ��6X8���ַ�
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

/*--------------��ʾ6X8�ĸ�����--------------*/
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
  decimal=number-data;					//С������
  
  if(data>=1000000000)           //�Ƿ��ܱ�10^9����
  {
    temp[i]=48+data/1000000000;
    data=data%1000000000;
    i++;
  }
  if(data>=100000000)           //�Ƿ��ܱ�10^8����
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
  if(data>=10000000)           //�Ƿ��ܱ�10^7����
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
  if(data>=1000000)           //�Ƿ��ܱ�10^6����
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
  if(data>=100000)           //�Ƿ��ܱ�100000����
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
  if(data>=10000)           //�Ƿ��ܱ�10000����
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
  if(data>=1000)           //�Ƿ��ܱ�1000����
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
  if(data>=100)              //�Ƿ��ܱ�100����
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
  if(data>=10)                  //�Ƿ��ܱ�10����
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
  if(decimal>=0.0001f)           //�ж��Ƿ���С������
  {
    i++;
    temp[i]='.';                //��ʾС����
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
/*------��ʾ8X16һ���׼��ASCII�ַ�������ʾ����Ϊ��x��y��------*/
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
//��ʾһ��8X16���ַ�
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
/*---------------------��ʾ8X16�ĸ�����--------------------*/
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
  decimal=number-data;     //С������
  if(data>=1000)           //�Ƿ�ɱ�1000����
  {
    temp[i]=48+data/1000;
    data=data%1000;
    i++;
  }
  if(data>=100)              //�ɷ�100����
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
  if(data>=10)                  //�ɷ�10����
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
  if(decimal>=0.0001f)           //�ж��ǹ�ΪС��
  {
    i++;
    temp[i]='.';                //��ʾС����
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
������: void OLED_Init(void)
˵��:	OLED��ʾ����ʼ��
���:	��
����:	��
��ע:	�ϵ��ʼ��������һ��
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
