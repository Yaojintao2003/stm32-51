#ifndef _I2C_H
#define _I2C_H
#include "main.h"

#define I2C_SOFT 2   //0:硬件模式，1:软件模式，2:软硬结合模式


#define I2C_PORT GPIOF
#define I2C_SCK_GPIO GPIO_PIN_1
#define I2C_SDA_GPIO GPIO_PIN_0

#if I2C_SOFT == 1

void SDA_OUT(void);
void SDA_IN(void);

#define SCK_H HAL_GPIO_WritePin(I2C_PORT,I2C_SCK_GPIO,GPIO_PIN_SET)
#define SCK_L HAL_GPIO_WritePin(I2C_PORT,I2C_SCK_GPIO,GPIO_PIN_RESET)
#define SDA_H HAL_GPIO_WritePin(I2C_PORT,I2C_SDA_GPIO,GPIO_PIN_SET)
#define SDA_L HAL_GPIO_WritePin(I2C_PORT,I2C_SDA_GPIO,GPIO_PIN_RESET)

#define SDA_READ HAL_GPIO_ReadPin(I2C_PORT,I2C_SDA_GPIO)

void I2C_init(void);
void I2C_START(void);
void I2C_STOP(void);
void I2C_ACK(void);
void I2C_NACK(void);
int I2C_waitACK(void);
void I2C_senddata(uint8_t data);
uint8_t I2C_getdata(void);


#elif I2C_SOFT == 0

#define I2C_DMA 1

#ifndef I2C_DMA
#define I2C_DMA 0
#endif

void I2C_init(void);


#elif I2C_SOFT == 2
extern uint8_t I2C_Flag;

void SDA_OUT(void);
void SDA_IN(void);

#define SCK_H HAL_GPIO_WritePin(I2C_PORT,I2C_SCK_GPIO,GPIO_PIN_SET)
#define SCK_L HAL_GPIO_WritePin(I2C_PORT,I2C_SCK_GPIO,GPIO_PIN_RESET)
#define SDA_H HAL_GPIO_WritePin(I2C_PORT,I2C_SDA_GPIO,GPIO_PIN_SET)
#define SDA_L HAL_GPIO_WritePin(I2C_PORT,I2C_SDA_GPIO,GPIO_PIN_RESET)

#define SDA_READ HAL_GPIO_ReadPin(I2C_PORT,I2C_SDA_GPIO)

void I2C_soft_init(void);
void I2C_START(void);
void I2C_STOP(void);
void I2C_ACK(void);
void I2C_NACK(void);
int I2C_waitACK(void);
void I2C_senddata(uint8_t data);
uint8_t I2C_getdata(void);

void I2C_soft_Send_Data(uint8_t id,uint8_t add,uint8_t data);
void I2C_soft_Send_nData(uint8_t id,uint8_t add,uint8_t *data,uint16_t n);
uint8_t I2C_soft_Get_Data(uint8_t id,uint8_t add);
void I2C_soft_Get_nData(uint8_t id,uint8_t add,uint8_t *data,uint16_t n);

#define I2C_DMA 1

#ifndef I2C_DMA
#define I2C_DMA 0
#endif

void I2C_hard_init(void);
void I2C_hard_Send_Data(uint8_t id,uint8_t add,uint8_t data);
void I2C_hard_Send_nData(uint8_t id,uint8_t add,uint8_t *data,uint16_t n);
uint8_t I2C_hard_Get_Data(uint8_t id,uint8_t add);
void I2C_hard_Get_nData(uint8_t id,uint8_t add,uint8_t *data,uint16_t n);

#endif

void I2C_Send_Data(uint8_t id,uint8_t add,uint8_t data);
void I2C_Send_nData(uint8_t id,uint8_t add,uint8_t *data,uint16_t n);
uint8_t I2C_Get_Data(uint8_t id,uint8_t add);
void I2C_Get_nData(uint8_t id,uint8_t add,uint8_t *data,uint16_t n);

#endif
