/*
 * rtc.c
 *
 *  Created on: 2021��10��20��
 *      Author: Administrator
 */

#include "rtc.h"

//����2��ȫ�ֱ���������RTCʱ��Ķ�ȡ�����
uint16_t ryear; //4λ��
uint8_t rmon,rday,rhour,rmin,rsec,rweek;//2λ����ʱ������

void RTC_Init(void) //�û��Խ��Ĵ����ϵ�BPK�жϵ�RTC��ʼ��
{
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  if(HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR1)!=0X5050){ //�ж��Ƿ��״��ϵ�
	   HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR1,0X5050); //�����ֵ �´β�ִ�С��״��ϵ硱�Ĳ���
	   RTC_Set(2022,1,1,0,0,0);//д��RTCʱ��Ĳ���RTC_Set(4λ��,2λ��,2λ��,2λʱ,2λ��,2λ��)
  }
}

//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
uint8_t Is_Leap_Year(uint16_t year){
	if(year%4==0){ //�����ܱ�4����
		if(year%100==0){
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400����
			else return 0;
		}else return 1;
	}else return 0;
}
//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����

//�·����ݱ�
uint8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�
const uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};//ƽ����·����ڱ�

//д��ʱ��
uint8_t RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec){ //д�뵱ǰʱ�䣨1970~2099����Ч����
	uint16_t t;
	uint32_t seccount=0;
	if(syear<2000||syear>2099)return 1;//syear��Χ1970-2099���˴����÷�ΧΪ2000-2099
	for(t=1970;t<syear;t++){ //��������ݵ��������
		if(Is_Leap_Year(t))seccount+=31622400;//�����������
		else seccount+=31536000;                    //ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++){         //��ǰ���·ݵ����������
		seccount+=(uint32_t)mon_table[t]*86400;//�·����������
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//����2�·�����һ���������
	}
	seccount+=(uint32_t)(sday-1)*86400;//��ǰ�����ڵ����������
	seccount+=(uint32_t)hour*3600;//Сʱ������
	seccount+=(uint32_t)min*60;      //����������
	seccount+=sec;//�������Ӽ���ȥ

	//���Ĵ�����������ΪHAL��Ĳ����ƣ��޷�ֱ�ӵ���RTC_ReadTimeCounter�������˴����üĴ���ֱ�Ӳ�����
	RTC->CRL|=1<<4;   //��������
	RTC->CNTL=seccount&0xffff;
	RTC->CNTH=seccount>>16;
	RTC->CRL&=~(1<<4);//���ø���
	while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������
	//���Ĵ�������������

	return 0; //����ֵ:0,�ɹ�;����:�������.
}

//����ʱ��
uint8_t RTC_Get(void){//������ǰʱ��ֵ //����ֵ:0,�ɹ�;����:�������.
	static uint16_t daycnt=0;
	uint32_t timecount=0;
	uint32_t temp=0;
	uint16_t temp1=0;

	//���Ĵ�����������ΪHAL��Ĳ����ƣ��޷�ֱ�ӵ���RTC_WriteTimeCounter�������˴����üĴ���ֱ�Ӳ�����
 	timecount=RTC->CNTH;//�õ��������е�ֵ(������)
	timecount<<=16;
	timecount+=RTC->CNTL;
	//���Ĵ�������������

	temp=timecount/86400;   //�õ�����(��������Ӧ��)
	if(daycnt!=temp){//����һ����
		daycnt=temp;
		temp1=1970;  //��1970�꿪ʼ
		while(temp>=365){
		     if(Is_Leap_Year(temp1)){//������
			     if(temp>=366)temp-=366;//�����������
			     else {temp1++;break;}
		     }
		     else temp-=365;       //ƽ��
		     temp1++;
		}
		ryear=temp1;//�õ����
		temp1=0;
		while(temp>=28){//������һ����
			if(Is_Leap_Year(ryear)&&temp1==1){//�����ǲ�������/2�·�
				if(temp>=29)temp-=29;//�����������
				else break;
			}else{
	            if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
	            else break;
			}
			temp1++;
		}
		rmon=temp1+1;//�õ��·�
		rday=temp+1;  //�õ�����
	}
	temp=timecount%86400;     //�õ�������
	rhour=temp/3600;     //Сʱ
	rmin=(temp%3600)/60; //����
	rsec=(temp%3600)%60; //����
	rweek=RTC_Get_Week(ryear,rmon,rday);//��ȡ����
	return 0;
}

uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day){ //�������ռ�������(ֻ����1901-2099��)//����RTC_Get����
	uint16_t temp2;
	uint8_t yearH,yearL;
	yearH=year/100;
	yearL=year%100;
	// ���Ϊ21����,�������100
	if (yearH>19)yearL+=100;
	// ����������ֻ��1900��֮���
	temp2=yearL+yearL/4;
	temp2=temp2%7;
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7); //��������ֵ
}


