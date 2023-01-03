#include "ioiic.h"
#include "gpio.h"

#define SDA_Pin MIIC_SDA_Pin //tx
#define SCL_Pin MIIC_SCL_Pin	//Rx
#define GPIOx GPIOB

//us��ʱ����
void HAL_Delay_us(uint32_t Delay)
{
	//��systic����Ϊ1us�ж�
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000000);
	HAL_Delay(Delay-1);
	//��systic�ָ�����Ϊ1ms�ж�
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

//IIC�Ŀ�ʼ�ź�
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

//IIC��ֹͣ�ź�
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

//IIC��Ӧ���ź�
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

//IIC��Ӧ���ź�
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

//�ȴ�Ӧ�𣬳ɹ�����0��ʧ�ܷ���1
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

//IIC����һ���ֽڣ�����1��Ӧ�𣬷���0��Ӧ��
void IIC_Send_Byte(uint8_t TX)
{
	uint8_t i;
	SDA_OUT();
	IIC_SCL(GPIO_PIN_RESET); //����ʱ���߿�ʼ��������
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

//IIC��һ���ֽ�
//�β�Ϊ1������һ��Ӧ��
//�β�Ϊ0��������Ӧ��
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

