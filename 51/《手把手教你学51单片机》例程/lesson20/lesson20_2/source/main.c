/*
*******************************************************************************
*                     ���ְ��ֽ���ѧ51��Ƭ��(C���԰�)��
*                    ���� KST-51 ��Ƭ�������� ʾ��Դ����
*
*         (c) ��Ȩ���� 2016 ��ɳ̲������/�廪��ѧ������ ��������Ȩ��
*                 ��ȡ������������ʣ�http://www.qdkingst.com
*
* �ļ�����main.c
* ��  ������������࣬��1602Һ������ʾ
* �汾�ţ�v1.0.0
* ��  ע��
*******************************************************************************
*/

#include "config.h"
#include "Lcd1602.h"
#include "Ultrasonic.h"

uint16 flag;   //���嶨ʱ��־

void ShowDistance();

/* ������ */
void main()
{
    EA = 1;            //�������ж�
    TMOD &= 0Xf0;
    TMOD |= 0X01;      //����T0Ϊ��ʽ1
    TH0 = 0xFC;        //ΪT0����ֵ0xFC67����ʱ1ms
    TL0 = 0x67;
    ET0 = 1;           //ʹ��T0�ж�
    TR0 = 1;           //����T0
    InitUltrasonic();  //������ģ���ʼ��
    InitLcd1602();     //1602��ʼ��

    while(1)
    {
        if(flag == 1)
        {
            flag = 0;
            ShowDistance();
        }
    }
}

/* ִ�в�ಢ��ʾ��� */
void ShowDistance()
{
    uint8 i;
    uint8 buff[6];
    uint16 Distance ;   //�ݴ����ֵ

    Distance = GetDistance();
    buff[5] = Distance%10 + '0';   //������ת��Ϊ�ַ���ʽ
    buff[4] ='.';
    buff[3] = (Distance/10)%10 + '0';
    buff[2] = (Distance/100)%10 + '0';
    buff[1] = (Distance/1000)%10 + '0';
    buff[0] = (Distance/10000)%10 + '0';
    for(i = 0; i <= 3; i++)  //�������ָ�λ��0ת��Ϊ�հ׷�
    {
        if(buff[i] == '0')   //�ж��ǲ����ַ�0
            buff[i] = ' ';   //���ַ�0 ת��Ϊ�ո� ������ʾ
        else
            break;
    }
    LcdShowStr(0, 1, &buff); //��ʾ����ֵ
    LcdShowStr(6, 1, "cm");  //��ʾ���뵥λ
}

/* ��ʱ��0�жϷ����� */
void InterruptTimer0() interrupt 1 using 3
{
    static uint16 cnt = 0;   //�����������

    TH0 = 0xFC;      //���¼��س�ֵ
    TL0 = 0x67;
    cnt++;
    if(cnt >= 330)   //��ʱ330ms
    {
        cnt = 0;     //�������ֵ
        flag = 1;    //���ö�ʱ��־Ϊ1
    }
}
