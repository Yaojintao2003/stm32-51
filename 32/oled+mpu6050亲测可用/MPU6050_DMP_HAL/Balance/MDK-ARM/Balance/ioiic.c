#include "ioiic.h"
#include "gpio.h"

#define SDA_Pin MIIC_SDA_Pin //tx
#define SCL_Pin MIIC_SCL_Pin	//Rx
#define GPIOx GPIOB

//us延时函数
void HAL_Delay_us(uint32_t Delay)
{
	//将systic设置为1us中段
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000000);
	HAL_Delay(Delay-1);
	//将systic恢复设置为1ms中段
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
}

void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void IIC_SCL(GPIO_PinState PinState)
{
	HAL_GPIO_WritePin(GPIOx,SCL_Pin,PinState);
}

void IIC_SDA(GPIO_PinState PinState)
{
	HAL_GPIO_WritePin(GPIOx,SDA_Pin,PinState);
}

uint8_t IIC_Read_SDA(void)
{
	 return HAL_GPIO_ReadPin(GPIOx,SDA_Pin);
}

//IIC的开始信号
void IIC_Start(void)
{
	SDA_OUT();
	IIC_SDA(GPIO_PIN_SET);
	IIC_SCL(GPIO_PIN_SET);
	HAL_Delay_us(4);
	IIC_SDA(GPIO_PIN_RESET);
	HAL_Delay_us(4);
	IIC_SCL(GPIO_PIN_RESET);
}

//IIC的停止信号
void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL(GPIO_PIN_RESET);
	IIC_SDA(GPIO_PIN_RESET);
	HAL_Delay_us(4);
	IIC_SCL(GPIO_PIN_SET);
	IIC_SDA(GPIO_PIN_SET);
	HAL_Delay_us(4);
}

//IIC的应答信号
void IIC_Ack(void)
{
	IIC_SCL(GPIO_PIN_RESET);
	SDA_OUT();
	IIC_SDA(GPIO_PIN_RESET);
	HAL_Delay_us(2);
	IIC_SCL(GPIO_PIN_SET);
	HAL_Delay_us(2);
	IIC_SCL(GPIO_PIN_RESET);
}

//IIC无应答信号
void IIC_NAck(void)
{
	IIC_SCL(GPIO_PIN_RESET);
	SDA_OUT();
	IIC_SDA(GPIO_PIN_SET);
	HAL_Delay_us(2);
	IIC_SCL(GPIO_PIN_SET);
	HAL_Delay_us(2);
	IIC_SCL(GPIO_PIN_RESET);
}

//等待应答，成功返回0，失败返回1
uint8_t IIC_Wait_Ack(void)
{
	uint8_t times  = 0;
	SDA_IN();
	IIC_SDA(GPIO_PIN_SET);
	HAL_Delay_us(1);
	IIC_SCL(GPIO_PIN_SET);
	HAL_Delay_us(1);
	while(IIC_Read_SDA())
	{
		times++;
		if(times > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	return 0;
}

//IIC发送一个字节，返回1有应答，返回0无应答
void IIC_Send_Byte(uint8_t TX)
{
	uint8_t i;
	SDA_OUT();
	IIC_SCL(GPIO_PIN_RESET); //拉低时钟线开始传输数据
	for(i=0;i<8;i++)
	{
		if((TX&0x80)>>7){
			IIC_SDA(GPIO_PIN_SET);
		}
		else{
			IIC_SDA(GPIO_PIN_RESET);
		}
		
		TX <<= 1;
		HAL_Delay_us(2);
		IIC_SCL(GPIO_PIN_SET);
		HAL_Delay_us(2);
		IIC_SCL(GPIO_PIN_RESET);
		HAL_Delay_us(2);
	}
}

//IIC读一个字节
//形参为1，发送一个应答
//形参为0，不发送应答
uint8_t IIC_Read_Byte(unsigned char Ack)
{
	unsigned char i;
	unsigned char receive = 0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		IIC_SCL(GPIO_PIN_RESET);
		HAL_Delay_us(2);
		IIC_SCL(GPIO_PIN_SET);
		receive<<=1;
		if(IIC_Read_SDA())
		{
			receive++;
		}
		HAL_Delay_us(1);
	}
	if(Ack)
	{
			IIC_Ack();
	}
	else
	{
			IIC_NAck();
	}
	return receive;
}

