#ifndef __usart1_H_
#define __usart1_H_

#include "num.h"
#define NUM  10

extern uchar pc;
extern bit u_f;
extern uchar usart1_ms;
extern uchar usart1_r[NUM];
void usart1_init();
void usart1_send(uchar *p,int leng);
#endif 