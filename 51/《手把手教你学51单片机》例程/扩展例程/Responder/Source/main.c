/*
*******************************************************************************
*                     ���ְ��ֽ���ѧ51��Ƭ��(C���԰�)��
*                    ���� KST-51 ��Ƭ�������� ʾ��Դ����
*
*         (c) ��Ȩ���� 2016 ��ɳ̲������/�廪��ѧ������ ��������Ȩ��
*                 ��ȡ������������ʣ�http://www.qdkingst.com
*
* �ļ�����main.c
* ��  ������·������
* �汾�ţ�v1.1.0
* ��  ע��1~9Ϊ����ť��EseΪ�����ť��EnterΪ��������ť
          �Ȱ� ENter���ٰ�����ť�����Ese ������ݣ����¿�ʼ
          �Ȱ� ����ť ��ΪΥ�棬�������������˳��밴Ese
*******************************************************************************
*/
#include "config.h"
#include "keyboard.h"
#include "Lcd1602.h"

sbit BUZZ = P1^6;   //��������������
uint8 T0RH = 0;
uint8 T0RL = 0;
uint8 T1RH = 0;
uint8 T1RL = 0;

void ConfigTimer0(unsigned int ms);
void OpenBuzz(unsigned int frequ);
void StopBuzz();

void main()
{
    EA = 1;                    //�����ж�
    ConfigTimer0(1);           //���ö�ʱ��0 1ms��һ���ж�
    InitLcd1602();
    LcdShowStr(0,0,"State :"); //1602��ʾ��������
    LcdShowStr(0,1,"Number:"); //1602��ʾ�������

    while(1)
    {
        KeyDriver();           //������������
    }
}

void KeyAction(uint8 keycode)
{
    uint8 buf[1];   //���ڴ�Ű�������
    static uint8 permit = 0;   //���������־
    static uint8 done = 0;     //������ɱ�־

    if((keycode>='1') && (keycode<='9'))  //���ּ�1-9ִ������
    {
        if(done == 0)
        {
            if(permit == 0)
            {
                buf[0] = keycode;
                LcdShowStr(8,0,"Warning");   //��ʾ����
                LcdShowStr(8,1,buf);         //��ʾ��������
                done = 1;
                OpenBuzz(4000);
            }
            else
            {
                buf[0] = keycode;
                LcdShowStr(8,1,buf);         //��ʾ��������
                done = 1;
            }
        }
    }
    else if (keycode == 0x0D)  //�س�������ʼ����
    {
        if(done == 0)
        {
            LcdAreaClear(8,0,8);
            LcdShowStr(8,0,"Permit");
            permit = 1;
        }
    }
    else if (keycode == 0x1B)  //Esc�������
    {
        LcdFullClear();
        LcdShowStr(0,0,"State :");
        LcdShowStr(0,1,"Number:");
        permit = 0;
        done = 0;
        StopBuzz();
    }
}

/* ���ò�����T0��ms-T0��ʱʱ�� */
void ConfigTimer0(unsigned int ms)
{
    uint32 tmp;  //��ʱ����

    tmp = 11059200 / 12;      //��ʱ������Ƶ��
    tmp = (tmp * ms) / 1000;  //��������ļ���ֵ
    tmp = 65536 - tmp;        //���㶨ʱ������ֵ
    tmp = tmp + 13;           //�����ж���Ӧ��ʱ��ɵ����
    T0RH = (uint8)(tmp>>8);  //��ʱ������ֵ���Ϊ�ߵ��ֽ�
    T0RL = (uint8)tmp;
    TMOD &= 0x00;   //����T0�Ŀ���λ
    TMOD |= 0x11;   //����T0Ϊģʽ1
    TH0 = T0RH;     //����T0����ֵ
    TL0 = T0RL;
    ET0 = 1;        //ʹ��T0�ж�
    TR0 = 1;        //����T0
}
/* ����������������frequ-����Ƶ�� */
void OpenBuzz(unsigned int frequ)
{
    uint16 reload;    //��������Ķ�ʱ������ֵ

    reload = 65536 - (11059200/12)/(frequ*2);  //�ɸ���Ƶ�ʼ��㶨ʱ������ֵ
    T1RH = (uint8)(reload >> 8);  //16λ����ֵ�ֽ�Ϊ�ߵ������ֽ�
    T1RL = (uint8)reload;
    TH1  = 0xFF;  //�趨һ���ӽ�����ĳ�ֵ����ʹ��ʱ������Ͷ�빤��
    TL1  = 0xFE;
    ET1  = 1;     //ʹ��T1�ж�
    TR1  = 1;     //����T1
}
/* ������ֹͣ���� */
void StopBuzz()
{
    ET1 = 0;   //����T1�ж�
    TR1 = 0;   //ֹͣT1
}

/* T0�жϷ���������ɰ���ɨ�� */
void InterruptTimer0() interrupt 1
{
    TH0 = T0RH;  //���¼�������ֵ
    TL0 = T0RL;
    KeyScan();   //����ɨ��
}

/* T1�жϷ����������ڿ��Ʒ��������� */
void InterruptTimer1() interrupt 3
{
    TH1 = T1RH;   //���¼�������ֵ
    TL1 = T1RL;
    BUZZ = ~BUZZ; //��ת���������Ƶ�ƽ
}
