#include "i2c.h"
#include "oled.h"

int num = 0;//计数
extern uint8_t oled[1024];
uint8_t I2C_DMA_Flag = 0;

#if I2C_SOFT !=1
I2C_HandleTypeDef hi2c2;
DMA_HandleTypeDef hdma_i2c2_tx;
#endif

#if I2C_SOFT == 1

void I2C_init(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	__HAL_RCC_GPIOF_CLK_ENABLE();
	/**I2C2 GPIO Configuration
	PF0     ------> I2C2_SDA
	PF1     ------> I2C2_SCL
	*/
	GPIO_InitStruct.Pin = I2C_SDA_GPIO|I2C_SCK_GPIO;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
	SDA_H;
	SCK_H;
}
void SDA_IN(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = I2C_SDA_GPIO;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
}

void SDA_OUT(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = I2C_SDA_GPIO;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
}

void delay(){
	int i=0;
	for(;i<10;i++);
}

void I2C_START(){
	SDA_OUT();
	SDA_H;
	SCK_H;
	delay();
	SDA_L;
	delay();
	SCK_L;
	delay();
}

void I2C_STOP(){
	SDA_OUT();
	SCK_L;
	SDA_L;
	delay();
	SCK_H;
	delay();
	SDA_H;
	delay();
}

void I2C_ACK(void){
	SDA_OUT();
	SCK_L;
	delay();
	SDA_L;
	delay();
	SCK_H;
	delay();
	SCK_L;
	delay();
}

void I2C_NACK(void){
	SDA_OUT();
	SCK_L;
	delay();
	SDA_H;
	delay();
	SCK_H;
	delay();
	SCK_L;
}

int I2C_waitACK(void){
	int i=0;
	SDA_IN();
	delay();
	SCK_H;
	while(SDA_READ){
		i++;
		if(i>250){I2C_STOP();return 0;};
	}
	SCK_L;
	delay();
	return 1;
}

void I2C_senddata(uint8_t data){
	int i=0;
	SDA_OUT();
	SCK_L;
	delay();
	while(i<8){
		if(data&0x80)SDA_H;
		else SDA_L;
		delay();
		SCK_H;
		delay();
		SCK_L;
		delay();
		data<<=1;
		i++;
	}
	I2C_waitACK();
}

uint8_t I2C_getdata(){
	int i=0;
	uint8_t data;
	SCK_L;
	SDA_IN();
	delay();
	while(i<8){
		SCK_H;
		delay();
		data<<=1;
		if(SDA_READ)data++;
		delay();
		SCK_L;
		delay();
		i++;
	}
	return data;
}

void I2C_Send_Data(uint8_t id,uint8_t add,uint8_t data){
	I2C_START();
	I2C_senddata(id);
	I2C_senddata(add);
	I2C_senddata(data);
	I2C_STOP();
}

void I2C_Send_nData(uint8_t id,uint8_t add,uint8_t *data,uint16_t n){
	I2C_START();
	I2C_senddata(id);
	I2C_senddata(add);
	while(n>0){
		I2C_senddata(*data);
		data++;
		n--;
	}
	I2C_STOP();
	num++;//计数
}

uint8_t I2C_Get_Data(uint8_t id,uint8_t add){
	uint8_t t;
	I2C_START();
	I2C_senddata(id);
	I2C_senddata(add);
	I2C_START();
	I2C_senddata(id+1);
	t=I2C_getdata();
	I2C_STOP();
	return t;
}
	
void I2C_Get_nData(uint8_t id,uint8_t add,uint8_t *data,uint16_t n){
	I2C_START();
	I2C_senddata(id);
	I2C_senddata(add);
	I2C_START();
	I2C_senddata(id+1);
	while(n>0){
		*data=I2C_getdata();
		data++;
		n--;
		if(n==0)I2C_NACK();
		else I2C_ACK();
	}
	I2C_STOP();
}


#elif I2C_SOFT == 0

void MX_DMA_Init(void)
{
  __HAL_RCC_DMA1_CLK_ENABLE();

  HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);

}

void MX_I2C2_Init(void)
{
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 600000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C2)
  {

    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PF0     ------> I2C2_SDA
    PF1     ------> I2C2_SCL
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* I2C2 clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();

    /* I2C2 DMA Init */
    /* I2C2_TX Init */
    hdma_i2c2_tx.Instance = DMA1_Stream7;
    hdma_i2c2_tx.Init.Channel = DMA_CHANNEL_7;
    hdma_i2c2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_i2c2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c2_tx.Init.Mode = DMA_NORMAL;
    hdma_i2c2_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_i2c2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_i2c2_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(i2cHandle,hdmatx,hdma_i2c2_tx);

    /* I2C2 interrupt Init */
    HAL_NVIC_SetPriority(I2C2_EV_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);

  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C2)
  {
    __HAL_RCC_I2C2_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0);

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_1);

    HAL_DMA_DeInit(i2cHandle->hdmatx);

    HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);

  }
}

void I2C_init(){
#if I2C_DMA == 1
	MX_DMA_Init();
#endif
	MX_I2C2_Init();
}


void I2C_Send_Data(uint8_t id,uint8_t add,uint8_t data){
		if(I2C_DMA_Flag == 1)return;
		HAL_I2C_Mem_Write(&hi2c2,id,add,I2C_MEMADD_SIZE_8BIT,&data,1,0xffff);
}

void I2C_Send_nData(uint8_t id,uint8_t add,uint8_t *data,uint16_t n){
		if(I2C_DMA_Flag == 1)return;
#if I2C_DMA == 0
		HAL_I2C_Mem_Write(&hi2c2,id,add,I2C_MEMADD_SIZE_8BIT,data,n,0xffff);
		num++;//计数
#elif I2C_DMA == 1
		I2C_DMA_Flag = 1;
		HAL_I2C_Mem_Write_DMA(&hi2c2,id,add,I2C_MEMADD_SIZE_8BIT,data,n);
#endif
}

uint8_t I2C_Get_Data(uint8_t id,uint8_t add){
	uint8_t res=0;
	
	return res;
}

void I2C_Get_nData(uint8_t id,uint8_t add,uint8_t *data,uint16_t n){
}


void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c){
	I2C_DMA_Flag = 0;
	num++;//计数
}


#elif I2C_SOFT == 2

uint8_t I2C_Flag = 0;

void I2C_soft_init(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	__HAL_RCC_GPIOF_CLK_ENABLE();
	/**I2C2 GPIO Configuration
	PF0     ------> I2C2_SDA
	PF1     ------> I2C2_SCL
	*/
	HAL_GPIO_DeInit(I2C_PORT, I2C_SCK_GPIO);
  HAL_GPIO_DeInit(I2C_PORT, I2C_SDA_GPIO);
	GPIO_InitStruct.Pin = I2C_SDA_GPIO|I2C_SCK_GPIO;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
	SDA_H;
	SCK_H;
	I2C_Flag = 0;
}
void SDA_IN(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = I2C_SDA_GPIO;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
}

void SDA_OUT(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = I2C_SDA_GPIO;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
}


void delay(){
	int i=0;
	for(;i<10;i++);
}

void I2C_START(){
	SDA_OUT();
	SDA_H;
	SCK_H;
	delay();
	SDA_L;
	delay();
	SCK_L;
	delay();
}

void I2C_STOP(){
	SDA_OUT();
	SCK_L;
	SDA_L;
	delay();
	SCK_H;
	delay();
	SDA_H;
	delay();
}

void I2C_ACK(void){
	SDA_OUT();
	SCK_L;
	delay();
	SDA_L;
	delay();
	SCK_H;
	delay();
	SCK_L;
	delay();
}

void I2C_NACK(void){
	SDA_OUT();
	SCK_L;
	delay();
	SDA_H;
	delay();
	SCK_H;
	delay();
	SCK_L;
}

int I2C_waitACK(void){
	int i=0;
	SDA_IN();
	delay();
	SCK_H;
	while(SDA_READ){
		i++;
		if(i>250){I2C_STOP();return 0;};
	}
	SCK_L;
	delay();
	return 1;
}

void I2C_senddata(uint8_t data){
	int i=0;
	SDA_OUT();
	SCK_L;
	delay();
	while(i<8){
		if(data&0x80)SDA_H;
		else SDA_L;
		delay();
		SCK_H;
		delay();
		SCK_L;
		delay();
		data<<=1;
		i++;
	}
	I2C_waitACK();
}

uint8_t I2C_getdata(){
	int i=0;
	uint8_t data;
	SCK_L;
	SDA_IN();
	delay();
	while(i<8){
		SCK_H;
		delay();
		data<<=1;
		if(SDA_READ)data++;
		delay();
		SCK_L;
		delay();
		i++;
	}
	return data;
}

void I2C_soft_Send_Data(uint8_t id,uint8_t add,uint8_t data){
	I2C_START();
	I2C_senddata(id);
	I2C_senddata(add);
	I2C_senddata(data);
	I2C_STOP();
}

void I2C_soft_Send_nData(uint8_t id,uint8_t add,uint8_t *data,uint16_t n){
	I2C_START();
	I2C_senddata(id);
	I2C_senddata(add);
	while(n>0){
		I2C_senddata(*data);
		data++;
		n--;
	}
	I2C_STOP();
	num++;//计数
}

uint8_t I2C_soft_Get_Data(uint8_t id,uint8_t add){
	uint8_t t;
	I2C_START();
	I2C_senddata(id);
	I2C_senddata(add);
	I2C_START();
	I2C_senddata(id+1);
	t=I2C_getdata();
	I2C_STOP();
	return t;
}
	
void I2C_soft_Get_nData(uint8_t id,uint8_t add,uint8_t *data,uint16_t n){
	I2C_START();
	I2C_senddata(id);
	I2C_senddata(add);
	I2C_START();
	I2C_senddata(id+1);
	while(n>0){
		*data=I2C_getdata();
		data++;
		n--;
		if(n==0)I2C_NACK();
		else I2C_ACK();
	}
	I2C_STOP();
}

void MX_DMA_Init(void)
{
  __HAL_RCC_DMA1_CLK_ENABLE();

  HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);

}

void MX_I2C2_Init(void)
{
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 1000000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C2)
  {

    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PF0     ------> I2C2_SDA
    PF1     ------> I2C2_SCL
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* I2C2 clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();

    /* I2C2 DMA Init */
    /* I2C2_TX Init */
    hdma_i2c2_tx.Instance = DMA1_Stream7;
    hdma_i2c2_tx.Init.Channel = DMA_CHANNEL_7;
    hdma_i2c2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_i2c2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c2_tx.Init.Mode = DMA_NORMAL;
    hdma_i2c2_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_i2c2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_i2c2_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(i2cHandle,hdmatx,hdma_i2c2_tx);

    /* I2C2 interrupt Init */
    HAL_NVIC_SetPriority(I2C2_EV_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);

  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C2)
  {
    __HAL_RCC_I2C2_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0);

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_1);

    HAL_DMA_DeInit(i2cHandle->hdmatx);

    HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);

  }
}
void I2C_hard_init(){
#if I2C_DMA == 1
	MX_DMA_Init();
#endif
	MX_I2C2_Init();
	I2C_Flag=1;
}

void I2C_hard_Send_Data(uint8_t id,uint8_t add,uint8_t data){
		if(I2C_DMA_Flag == 1)return;
		HAL_I2C_Mem_Write(&hi2c2,id,add,I2C_MEMADD_SIZE_8BIT,&data,1,0xffff);
		
}

void I2C_hard_Send_nData(uint8_t id,uint8_t add,uint8_t *data,uint16_t n){
		if(I2C_DMA_Flag == 1)return;
#if I2C_DMA == 0
		num++;
		HAL_I2C_Mem_Write(&hi2c2,id,add,I2C_MEMADD_SIZE_8BIT,data,n,0xffffffff);
#elif I2C_DMA == 1
		I2C_DMA_Flag = 1;
		HAL_I2C_Mem_Write_DMA(&hi2c2,id,add,I2C_MEMADD_SIZE_8BIT,data,n);
#endif
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c){
	I2C_DMA_Flag = 0;
	num++;//计数
}


void I2C_Send_Data(uint8_t id,uint8_t add,uint8_t data){
		if(I2C_Flag==0)I2C_soft_Send_Data(id,add,data);
		else I2C_hard_Send_Data(id,add,data);
}
void I2C_Send_nData(uint8_t id,uint8_t add,uint8_t *data,uint16_t n){
		if(I2C_Flag==0)I2C_soft_Send_nData(id,add,data,n);
		else I2C_hard_Send_nData(id,add,data,n);
}

#endif


