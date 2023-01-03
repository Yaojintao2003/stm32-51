#include "shuma.h"
xdata uchar duanma[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};




void shuma_trans(uchar *p,int leng)
{
uchar i,j;
for(i=0,j=0;i<8,j<leng;i++,j++)
{
 switch(p[j])
 {
	 case '0':duanma[i]=0xc0;break;
	 case '1':duanma[i]=0xf9;break;
	 case '2':duanma[i]=0xa4;break;
	 case '3':duanma[i]=0xb0;break;
	 case '4':duanma[i]=0x99;break;
	 case '5':duanma[i]=0x92;break;
	 case '6':duanma[i]=0x82;break;
	 case '7':duanma[i]=0xf8;break;
	 case '8':duanma[i]=0x80;break;
	 case '9':duanma[i]=0x90;break;
	 case ' ':duanma[i]=0xff;break;
	 case '-':duanma[i]=0xbf;break;
	 case 'A':duanma[i]=0x88;break;
	 case 'B':duanma[i]=0x83;break;
	 case 'C':duanma[i]=0xc6;break;
	 case 'D':duanma[i]=0xa1;break;
	 case 'E':duanma[i]=0x86;break;
	 case 'F':duanma[i]=0x8e;break;
	 case 'H':duanma[i]=0x89;break;
	 case 'L':duanma[i]=0xc7;break;
	 case 'N':duanma[i]=0xc8;break;
	 case 'P':duanma[i]=0x8c;break;
	 case 'U':duanma[i]=0xc1;break;
   default: duanma[i]=0xff;break;		 
 }
 if(p[j+1]=='.')
 {
 duanma[i]&=0x7f;
	 j++;
 }
}	
}


void shuma_dis()
{
static uchar wei;
P0=0xff;
P2=(P2&0x1f)|0xe0;	
P2=(P2&0x1f);
P0=duanma[wei];
P2=(P2&0x1f)|0xe0;	
P2=(P2&0x1f);
P0=0x01<<wei;
P2=(P2&0x1f)|0xc0;	
P2=(P2&0x1f);	
wei++;if(wei>7){wei=0;}
}

