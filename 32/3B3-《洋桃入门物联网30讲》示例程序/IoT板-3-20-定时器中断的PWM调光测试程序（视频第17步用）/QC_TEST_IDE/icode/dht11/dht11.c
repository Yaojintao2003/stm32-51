/*
 * dht11.c
 *
 *  Created on: Oct 21, 2021
 *      Author: Administrator
 */

/*
//杜洋工作室出品
//洋桃系列开发板应用程序
//关注微信公众号：洋桃电子
//洋桃开发板资料下载 www.DoYoung.net/YT
//即可免费看所有教学视频，下载技术资料，技术疑难提问
//更多内容尽在 杜洋工作室主页 www.doyoung.net
*/

/*
《修改日志》
1-201708202309 创建。


*/



#include "dht11.h"
#include "main.h"

void DHT11_IO_OUT (void){ //端口变为输出
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DHT11_DA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void DHT11_IO_IN (void){ //端口变为输入
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DHT11_DA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void DHT11_RST (void){ //DHT11端口复位，发出起始信号（IO发送）
	DHT11_IO_OUT();
	HAL_GPIO_WritePin(GPIOB,DHT11_DA_Pin, GPIO_PIN_RESET);
	HAL_Delay(20); //拉低至少18ms
	HAL_GPIO_WritePin(GPIOB,DHT11_DA_Pin, GPIO_PIN_SET);
	delay_us(30); //主机拉高20~40us
}

uint8_t Dht11_Check(void){ //等待DHT11回应，返回1:未检测到DHT11，返回0:成功（IO接收）
	uint8_t retry=0;
    DHT11_IO_IN();//IO到输入状态
    while (HAL_GPIO_ReadPin(GPIOB,DHT11_DA_Pin)&&retry<100){//DHT11会拉低40~80us
        retry++;
        delay_us(1);
    }
    if(retry>=100)return 1; else retry=0;
    while (!HAL_GPIO_ReadPin(GPIOB,DHT11_DA_Pin)&&retry<100){//DHT11拉低后会再次拉高40~80us
        retry++;
        delay_us(1);
    }
    if(retry>=100)return 1;
    return 0;
}

uint8_t Dht11_ReadBit(void){ //从DHT11读取一个位 返回值：1/0
	uint8_t retry=0;
    while(HAL_GPIO_ReadPin(GPIOB,DHT11_DA_Pin)&&retry<100){//等待变为低电平
        retry++;
        delay_us(1);
    }
    retry=0;
    while(!HAL_GPIO_ReadPin(GPIOB,DHT11_DA_Pin)&&retry<100){//等待变高电平
        retry++;
        delay_us(1);
    }
    delay_us(40);//等待40us	//用于判断高低电平，即数据1或0
    if(HAL_GPIO_ReadPin(GPIOB,DHT11_DA_Pin))return 1; else return 0;
}

uint8_t Dht11_ReadByte(void){  //从DHT11读取一个字节  返回值：读到的数据
	uint8_t i,dat;
    dat=0;
    for (i=0;i<8;i++){
        dat<<=1;
        dat|=Dht11_ReadBit();
    }
    return dat;
}

uint8_t DHT11_Init (void){	//DHT11初始化
	DHT11_RST();//DHT11端口复位，发出起始信号
	return Dht11_Check(); //等待DHT11回应
}

uint8_t DHT11_ReadData(uint8_t *h){ //读取一次数据//湿度值(十进制，范围:20%~90%) ，温度值(十进制，范围:0~50°)，返回值：0,正常;1,失败
	uint8_t buf[5];
	uint8_t i;
    DHT11_RST();//DHT11端口复位，发出起始信号
    if(Dht11_Check()==0){ //等待DHT11回应
        for(i=0;i<5;i++){//读取5位数据
            buf[i]=Dht11_ReadByte(); //读出数据
        }
        if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]){	//数据校验
            *h=buf[0]; //将湿度值放入指针1
			h++;
            *h=buf[2]; //将温度值放入指针2
        }
    }else return 1;
    return 0;
}

/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT
*********************************************************************************************/
