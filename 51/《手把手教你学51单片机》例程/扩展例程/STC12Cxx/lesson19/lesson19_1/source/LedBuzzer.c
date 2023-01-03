/*
*******************************************************************************
*                     ���ְ��ֽ���ѧ51��Ƭ��(C���԰�)��
*                    ���� KST-51 ��Ƭ�������� ʾ��Դ����
*
*         (c) ��Ȩ���� 2014 ��ɳ̲������/�廪��ѧ������ ��������Ȩ��
*                 ��ȡ������������ʣ�http://www.kingst.org
*
* �ļ�����LedBuzzer.c
* ��  ��������LED������ܡ�����LED����Դ������������ģ��
* �汾�ţ�v1.0.0
* ��  ע������Ķ�ʱ�ж���STC12ϵ�е�PCAģ��ʵ��
*******************************************************************************
*/

#define  _LED_BUZZER_C
#include "config.h"
#include "LedBuzzer.h"


//����STC12ϵ��PCAģ��ļĴ���
sfr CMOD   = 0xD9;  //PCA ����ģʽ�Ĵ���
sfr CCON   = 0xD8;  //PCA ���ƼĴ���
sfr CL     = 0xE9;  //PCA ��������λ
sfr CH     = 0xF9;  //PCA ��������λ
sfr CCAPM0 = 0xDA; //PCA ģ��0 PWM �Ĵ���
sfr CCAPM1 = 0xDB; //PCA ģ��1 PWM �Ĵ���
sfr CCAP0L = 0xEA; //PCA ģ�� 0 �Ĳ�׽/�ȽϼĴ����� 8 λ
sfr CCAP0H = 0xFA; //PCA ģ�� 0 �Ĳ�׽/�ȽϼĴ����� 8 λ
sfr CCAP1L = 0xEB; //PCA ģ�� 1 �Ĳ�׽/�ȽϼĴ����� 8 λ
sfr CCAP1H = 0xFB; //PCA ģ�� 1 �Ĳ�׽/�ȽϼĴ����� 8 λ
sfr PCA_PWM0 = 0xF2; //PCA ģ��0 PWM �Ĵ���
sfr PCA_PWM1 = 0xF3; //PCA ģ��1 PWM �Ĵ���
sbit PPCA = IP^7;    //PCA ģ���ж����ȼ�
sbit CF   = CCON^7;  //PCA�����������־,��Ӳ���������λ,�����������0��
sbit CR   = CCON^6;  //1:���� PCA ����������, �����������0��
sbit CCF1 = CCON^1;  //PCA ģ��1 �жϱ�־, ��Ӳ����λ, �����������0��
sbit CCF0 = CCON^0;  //PCA ģ��0 �жϱ�־, ��Ӳ����λ, �����������0��


uint8 code LedChar[] = {  //�������ʾ�ַ�ת����
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};

uint16 reload = 0;  //��ʱ���������ֵ
uint16 tmrcmp = 0;  //PCA0�����ʱ�Ƚ�ֵ
bit staBuzzer = 0;  //������״̬����λ��1-���С�0-�ر�
struct sLedBuff ledBuff; //LED��ʾ��������Ĭ�ϳ�ֵȫ0�����ôﵽ�ϵ�ȫ����Ч��

/* LED��ʼ����������ʼ��IO�����ö�ʱ�� */
void InitLed()
{
    //��ʼ��IO��
    P0 = 0xFF;
    ENLED = 0;
    //����PCA��Ϊ��̬ɨ�趨ʱ
    reload = SYS_MCLK/1500;  //������������ֵ��ÿ�����1500���ж�
    tmrcmp = reload;  //��ʼ�Ƚ�ֵ��Ϊ����ֵ
    CMOD   = 0x88;    //����PCAʹ����ʱ�ӣ���ֹ�����ж�
    CCAPM0 = 0x49;    //����PCA0Ϊ16λ�����ʱ��ģʽ
    CCAP0L = tmrcmp;  //�����ʱ�Ƚ�ֵ�͵��ȽϼĴ���
    CCAP0H = tmrcmp>>8;
    PPCA   = 1;       //�ж����ȼ�����Ϊ��
	CR     = 1;       //����PCA������
}
/* ��ˮ��ʵ�ֺ������������ʵ������Ч�� */
void FlowingLight()
{
    static uint8 i = 0;
    const uint8 code tab[] = {  //������
        0x7F, 0x3F, 0x1F, 0x0F, 0x87, 0xC3, 0xE1, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF
    };
    
    ledBuff.alone = tab[i];   //���ж�Ӧֵ�͵�����LED����ʾ������
    if (i < (sizeof(tab)-1))  //��������ѭ������������������
        i++;
    else
        i = 0;
}
/* ���������ʾһλ���֣�index-�����λ����(���ҵ����Ӧ0��5)��
**     num-����ʾ�����֣�point-�����Ƿ���ʾ��λ�ϵ�С���� */
void ShowLedNumber(uint8 index, uint8 num, uint8 point)
{
    ledBuff.number[index] = LedChar[num];  //��������ת��Ϊ������ַ�0��F
    if (point != 0)
    {
        ledBuff.number[index] &= 0x7F;  //point��Ϊ0ʱ������ǰλ��С����
    }
}
/* ��������ʾһ֡ͼƬ��ptr-����ʾͼƬָ�� */
void ShowLedArray(uint8 *ptr)
{
    uint8 i;
    
    for (i=0; i<sizeof(ledBuff.array); i++)
    {
        ledBuff.array[i] = *ptr++;
    }
}
/* PCA�жϷ�������LED��̬ɨ�衢���������� */
void InterruptPCA() interrupt 7
{
    static uint8 i = 0;  //LEDλѡ����
    
    CCF0 = 0;  //����PCA0�жϱ�־
    tmrcmp += reload;  //����ֵ�ۼӵ��Ƚ�ֵ��
    CCAP0L = tmrcmp;   //�Ƚ�ֵ�͵��ȽϼĴ���
    CCAP0H = tmrcmp>>8;
    //ȫ��LED��̬ɨ����ʾ
    if (ENLED == 0)  //LEDʹ��ʱ�Ž��ж�̬ɨ��
    {
        P0 = 0xFF;                       //�ر����ж�ѡλ����ʾ����
        P1 = (P1 & 0xF0) | i;            //λѡ����ֵ��ֵ��P1�ڵ�4λ
        P0 = *((uint8 data*)&ledBuff+i); //������������λ�õ������͵�P0��
        if (i < (sizeof(ledBuff)-1))     //��������ѭ������������������
            i++;
        else
            i = 0;
    }
    //�ɷ�����״̬λ���Ʒ�����
    if (staBuzzer == 1)
        BUZZER = ~BUZZER;  //����������
    else
        BUZZER = 1;        //����������
}
