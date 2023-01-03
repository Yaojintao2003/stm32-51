#ifndef __eeprom_H_
#define __eeprom_H_

#include "iic.h"
void write_eeprom(uchar *p,uchar add,int leng);
void read_eeprom(uchar *p,uchar add,int leng);
#endif 