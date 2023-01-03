/*
*******************************************************************************
*                     ���ְ��ֽ���ѧ51��Ƭ��(C���԰�)��
*                    ���� KST-51 ��Ƭ�������� ʾ��Դ����
*
*         (c) ��Ȩ���� 2016 ��ɳ̲������/�廪��ѧ������ ��������Ȩ��
*                 ��ȡ������������ʣ�http://www.qdkingst.com
*
* �ļ�����main.c
* ��  ������������࣬��ʱ���ж�0��ʱ���������ʾ����
* �汾�ţ�v1.0.0
* ��  ע��
*******************************************************************************
*/

#include "config.h"
#include "Ultrasonic.h"

uint16 flag = 0;      //���嶨ʱ��־
uint8 LedBuff[7] = {  //����ܺ�8������LED����ʾ����������ֵ0xFFȷ������ʱ������
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
const int8 code LedChar[] = { //�������ʾ�ַ�ת����
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};
const uint8 code ledtab[8] = { //led��ʾת����
    0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00
};

void ShowDistance();

/* ������ */
void main()
{
    EA = 1;            //ʹ�����ж�
    ENLED = 0;         //ʹ��U3��ѡ����������
    ADDR3 = 1;         //��Ϊ��Ҫ��̬�ı�ADDR0-2��ֵ�����Բ���Ҫ�ٳ�ʼ����
    TMOD &= 0x0f;
    TMOD |= 0x10;      //����T1Ϊģʽ1
    TH1 = 0xFC;        //ΪT1����ֵ0xFC67����ʱ1ms
    TL1 = 0x67;
    ET1 = 1;           //ʹ��T1�ж�
    TR1 = 1;           //����T1
    InitUltrasonic();  //��ʼ��������ģ��

    while (1)
    {
        if(flag == 1)       //ÿ���200ms����һ�β��
        {
            flag = 0;
            ShowDistance();
        }
    }
}

/* ִ�в�ಢ��ʾ��� */
void ShowDistance()
{
    int8 i;
    uint8 buff[5];
    uint16 distance ;   //�ݴ����ֵ

    distance = GetDistance();
    buff[0] = distance%10;
    buff[1] = distance/10%10;
    buff[2] = distance/100%10;
    buff[3] = distance/1000%10;
    buff[4] = distance/10000%10;
    for(i = 4; i >= 1;i--)  //�������ָ�λ��0ת��Ϊ�հ׷�
    {
        if(buff[i]==0)
            LedBuff[i] = 0xff;
        else
            break;
    }
    for(; i >= 0; i--)      //��Ч����ת��Ϊ��ʾ�ַ�
    {
        LedBuff[i] = LedChar[ buff[i] ];
    }
    LedBuff[1] &= 0x7f;     //�ڴӵ����2λ���С���㣬��ʾ��ʾ��ֵ�ĵ�λΪ����
    LedBuff[6] = ledtab[(distance-1)/500 ]; //��4m�ľ������ƽ����Ϊ8�����䣬��LED��ֱ�۷�Ӧ����
}

/* ��ʱ��1�жϷ����� */
void InterruptTimer1() interrupt 3
{
    static uint8 i = 0;     //LEDλѡ����
    static uint16 cnt = 0;  //�����������

    TH1 = 0xFC;                   //���¼��س�ֵ
    TL1 = 0x67;
    P0 = 0xFF;                    //�ر����ж�ѡλ����ʾ����
    P1 = (P1 & 0xF8) | i;         //λѡ����ֵ��ֵ��P1�ڵ�3λ
    P0 = LedBuff[i];              //������������λ�õ������͵�P0��
    if (i < (sizeof(LedBuff)-1))  //��������ѭ������������������
    {
        i++;
    }
    else
    {
        i = 0 ;
    }
    cnt++;
	if(cnt >= 200)   //��ʱ200ms
    {
        cnt = 0;     //�������ֵ
        flag = 1;    //���ö�ʱ��־Ϊ1
    }
}
