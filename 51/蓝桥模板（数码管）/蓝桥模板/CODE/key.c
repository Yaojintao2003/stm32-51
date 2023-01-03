#include "key.h"


uchar keyscan()
{
uchar temp;
uint key_value;
W1=0;W2=1;W3=1;W4=1;	
key_value=P3&0x0f;
W1=1;W2=0;W3=1;W4=1;	
key_value=(key_value<<4)|(P3&0x0f);
W1=1;W2=1;W3=0;W4=1;	
key_value=(key_value<<4)|(P3&0x0f);
W1=1;W2=1;W3=1;W4=0;	
key_value=(key_value<<4)|(P3&0x0f);
switch(~key_value)
{
	case 0x8000: temp=s4;break;
	case 0x4000: temp=s5;break;
	case 0x2000: temp=s6;break;
	case 0x1000: temp=s7;break;
	case 0x0800: temp=s8;break;
	case 0x0400: temp=s9;break;
	case 0x0200: temp=s10;break;
	case 0x0100: temp=s11;break;
	case 0x0080: temp=s12;break;
	case 0x0040: temp=s13;break;
	case 0x0020: temp=s14;break;
	case 0x0010: temp=s15;break;	
	case 0x0008: temp=s16;break;
	case 0x0004: temp=s17;break;
	case 0x0002: temp=s18;break;
	case 0x0001: temp=s19;break;
  default:   	temp=0; break;
}
return temp;	
}