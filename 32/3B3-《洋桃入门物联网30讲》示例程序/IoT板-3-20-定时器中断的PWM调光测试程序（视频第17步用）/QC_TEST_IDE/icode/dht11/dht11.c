/*
 * dht11.c
 *
 *  Created on: Oct 21, 2021
 *      Author: Administrator
 */

/*
//�������ҳ�Ʒ
//����ϵ�п�����Ӧ�ó���
//��ע΢�Ź��ںţ����ҵ���
//���ҿ������������� www.DoYoung.net/YT
//������ѿ����н�ѧ��Ƶ�����ؼ������ϣ�������������
//�������ݾ��� ����������ҳ www.doyoung.net
*/

/*
���޸���־��
1-201708202309 ������


*/



#include "dht11.h"
#include "main.h"

void DHT11_IO_OUT (void){ //�˿ڱ�Ϊ���
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DHT11_DA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void DHT11_IO_IN (void){ //�˿ڱ�Ϊ����
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DHT11_DA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void DHT11_RST (void){ //DHT11�˿ڸ�λ��������ʼ�źţ�IO���ͣ�
	DHT11_IO_OUT();
	HAL_GPIO_WritePin(GPIOB,DHT11_DA_Pin, GPIO_PIN_RESET);
	HAL_Delay(20); //��������18ms
	HAL_GPIO_WritePin(GPIOB,DHT11_DA_Pin, GPIO_PIN_SET);
	delay_us(30); //��������20~40us
}

uint8_t Dht11_Check(void){ //�ȴ�DHT11��Ӧ������1:δ��⵽DHT11������0:�ɹ���IO���գ�
	uint8_t retry=0;
    DHT11_IO_IN();//IO������״̬
    while (HAL_GPIO_ReadPin(GPIOB,DHT11_DA_Pin)&&retry<100){//DHT11������40~80us
        retry++;
        delay_us(1);
    }
    if(retry>=100)return 1; else retry=0;
    while (!HAL_GPIO_ReadPin(GPIOB,DHT11_DA_Pin)&&retry<100){//DHT11���ͺ���ٴ�����40~80us
        retry++;
        delay_us(1);
    }
    if(retry>=100)return 1;
    return 0;
}

uint8_t Dht11_ReadBit(void){ //��DHT11��ȡһ��λ ����ֵ��1/0
	uint8_t retry=0;
    while(HAL_GPIO_ReadPin(GPIOB,DHT11_DA_Pin)&&retry<100){//�ȴ���Ϊ�͵�ƽ
        retry++;
        delay_us(1);
    }
    retry=0;
    while(!HAL_GPIO_ReadPin(GPIOB,DHT11_DA_Pin)&&retry<100){//�ȴ���ߵ�ƽ
        retry++;
        delay_us(1);
    }
    delay_us(40);//�ȴ�40us	//�����жϸߵ͵�ƽ��������1��0
    if(HAL_GPIO_ReadPin(GPIOB,DHT11_DA_Pin))return 1; else return 0;
}

uint8_t Dht11_ReadByte(void){  //��DHT11��ȡһ���ֽ�  ����ֵ������������
	uint8_t i,dat;
    dat=0;
    for (i=0;i<8;i++){
        dat<<=1;
        dat|=Dht11_ReadBit();
    }
    return dat;
}

uint8_t DHT11_Init (void){	//DHT11��ʼ��
	DHT11_RST();//DHT11�˿ڸ�λ��������ʼ�ź�
	return Dht11_Check(); //�ȴ�DHT11��Ӧ
}

uint8_t DHT11_ReadData(uint8_t *h){ //��ȡһ������//ʪ��ֵ(ʮ���ƣ���Χ:20%~90%) ���¶�ֵ(ʮ���ƣ���Χ:0~50��)������ֵ��0,����;1,ʧ��
	uint8_t buf[5];
	uint8_t i;
    DHT11_RST();//DHT11�˿ڸ�λ��������ʼ�ź�
    if(Dht11_Check()==0){ //�ȴ�DHT11��Ӧ
        for(i=0;i<5;i++){//��ȡ5λ����
            buf[i]=Dht11_ReadByte(); //��������
        }
        if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]){	//����У��
            *h=buf[0]; //��ʪ��ֵ����ָ��1
			h++;
            *h=buf[2]; //���¶�ֵ����ָ��2
        }
    }else return 1;
    return 0;
}

/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT
*********************************************************************************************/
