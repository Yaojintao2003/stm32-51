#ifndef __csb_H_
#define __csb_H_

#include "num.h"
sbit csb_tx        =   P1^0;
sbit csb_rx        =   P1^1;


void csb_init();
uchar csb_get();
#endif