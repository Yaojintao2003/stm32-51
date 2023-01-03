#ifndef __IOIIC_H
#define __IOIIC_H

#include "main.h"

void IIC_Start(void);
void IIC_Stop(void);
void IIC_Ack(void);
void IIC_NAck(void);
uint8_t IIC_Wait_Ack(void);

void HAL_Delay_us(uint32_t Delay);
void IIC_Send_Byte(uint8_t TX);
uint8_t IIC_Read_Byte(unsigned char Ack);

#endif
