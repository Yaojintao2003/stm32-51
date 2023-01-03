#include <reg52.h>
sbit beep = P1^5;
unsigned char timer0H, timer0L, time;


code unsigned char FREQH[] = {
    0xF2, 0xF3, 0xF5, 0xF5, 0xF6, 0xF7, 0xF8,   
    0xF9, 0xF9, 0xFA, 0xFA, 0xFB, 0xFB, 0xFC, 0xFC,
    0xFC, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE,            
    0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF};  

code unsigned char FREQL[] = {
    0x42, 0xC1, 0x17, 0xB6, 0xD0, 0xD1, 0xB6,    
    0x21, 0xE1, 0x8C, 0xD8, 0x68, 0xE9, 0x5B, 0x8F, 
    0xEE, 0x44, 0x6B, 0xB4, 0xF4, 0x2D,            
    0x47, 0x77, 0xA2, 0xB6, 0xDA, 0xFA, 0x16};  
      
code unsigned char song[] = {
6,1,2,6,1,1,3,2,1,2,2,2,1,2,2,7,1,2,7,1,1,7,1,1,2,2,2,1,2,1,7,1,1,
6,1,2,6,1,1,1,3,1,7,2,1,1,3,1,6,1,2,6,1,1,1,3,1,7,2,1,1,3,1,7,2,1,1,3,1
	
	
};
//一个音符有三个数字。前为第几个音、中为第几个八度、后为时长（以半拍为单位）。
//6, 2, 3 分别代表：６, 中音, ３个半拍;
//5, 2, 1 分别代表：５, 中音, １个半拍;


void delay(unsigned char t)     
{
    unsigned char t1;
    unsigned long t2;
    for(t1 = 0; t1 < t; t1++)   
      for(t2 = 0; t2 < 4000; t2++);
    TR1 = 0;                       
}

void sing()                      
{
    TH1= timer0H;              
    TL1 = timer0L;
    TR1 = 1;                    
    delay(time);               
}

void main(void)
{
    unsigned char k, i;
    TMOD = 1;                   
    ET1 = 1;                   
    EA = 1;                   
    while(1) 
	{
      i = 0;
      time = 1; 
      while(time) 
	  {
        k = song[i] + 7 * song[i + 1] - 1;
        timer0H = FREQH[k];   
        timer0L = FREQL[k];     
        time = song[i + 2];   
        i += 3;
			if(i>69)
			{
				i=0;
			}
        sing();                 
	  }  
    }  
}
void t0int() interrupt 3     
{
    TR1 = 0;                   
    beep = !beep;          
    TH1= timer0H;            
    TL1 = timer0L;
    TR1 = 1;                   
}


//双线程
//必须有1个中断
//定时器中断控制音调（发声）
//延时函数控制一个音调的发声时长

