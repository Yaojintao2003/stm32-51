#ifndef _OLED_H
#define _OLED_H
#include "main.h"
#define OLED_FONT 0

#define XLevelL  	0x00
#define XLevelH  	0x10
#define XLevel     ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column 	128
#define Max_Row  	64
#define Brightness 	0xCF
#define X_WIDTH 	128
#define Y_WIDTH 	64
#define high 1
#define low 0

#define max(x,y) x>=y?x:y
#define min(x,y) x<=y?x:y

extern uint8_t oled[1024];


void LCD_P6x8Char(unsigned char x,unsigned char  y,unsigned char ch);
void LCD_P6x8Str(unsigned char x,unsigned char  y,unsigned char ch[]);
void write_6_8_number(unsigned char x,unsigned char  y,float number);
void LCD_P8x16Char(unsigned char x,unsigned char  y,unsigned char ch);
void LCD_P8x16Str(unsigned char x,unsigned char  y,unsigned char ch[]);
void write_8_16_number(unsigned char x,unsigned char  y,float number);
void LCD_clear_L(unsigned char x,unsigned char y);
void OLED_CLS(void);
void OLED_Init(void);
void Draw_Logo(unsigned char *LOGO128x64);
void LCD_CLS(void);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);

//12864
void LCD12864_6X8(int x,int y,char* ch);
void draw_point(int x,int y);
void draw_line(double x0,double y0,double x1,double y1);
void show_oled(void);




#endif
