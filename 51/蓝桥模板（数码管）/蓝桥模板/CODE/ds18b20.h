#ifndef __ds18b20_H_
#define __ds18b20_H_

#include "num.h"


sbit DQ = P1^4;  //单总线接口
float wendu_get();
#endif