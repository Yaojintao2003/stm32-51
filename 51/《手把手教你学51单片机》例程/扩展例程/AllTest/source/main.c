/*
*******************************************************************************
*                     ���ְ��ֽ���ѧ51��Ƭ��(C���԰�)��
*                    ���� KST-51 ��Ƭ�������� ʾ��Դ����
*
*         (c) ��Ȩ���� 2014 ��ɳ̲������/�廪��ѧ������ ��������Ȩ��
*                 ��ȡ������������ʣ�http://www.kingst.org
*
* �ļ�����main.c
* ��  ����ȫ����Գ��� �������ļ�
* �汾�ţ�v1.0.0
* ��  ע�����Կ������ϵĸ�������
*******************************************************************************
*/

#define  _MAIN_C
#include "config.h"
#include "Lcd1602.h"
#include "LedBuzzer.h"
#include "keyboard.h"
#include "DS1302.h"
#include "DS18B20.h"
#include "Infrared.h"
#include "Uart.h"
#include "I2C.h"
#include "eeprom.h"
#include "PCF8591.h"
#include "main.h"


bit flag100ms = 0; //100ms��ʱ��־
uint8 T0RH = 0;    //T0����ֵ�ĸ��ֽ�
uint8 T0RL = 0;    //T0����ֵ�ĵ��ֽ�

void main()
{
    uint8 tmr1s = 10;
    uint8 delay = 20;
    
    EA = 1;           //�����ж�
    ConfigTimer0(1);  //����T0��ʱ1ms
    ConfigUART(9600); //���ô���ͨ��
    InitLed();        //��ʼ��LEDģ��
    InitInfrared();   //��ʼ���������ģ��
    InitLcd1602();    //��ʼ��Һ��ģ��
    Start18B20();     //�����״��¶�ת��
    SetDACOut(0);     //����DAC���0V
    BuzzBeep();       //��������ʾ
    
    while (1)  //������ѭ��
    {
        KeyDriver();      //ִ�а�������
        UartDriver();     //ִ�д���ͨ������
        InfraredDriver(); //ִ�к����������
        if (flag100ms)    //ÿ��100msִ�����·�֧
        {
            flag100ms = 0;
            if (delay > 0)
            {
                delay--;
                if (delay == 0)
                {   //���δ�������ϵ���ʱ��ִ��һ��
                    InitDS1302();  //��ʼ��ʵʱʱ��ģ��
                    CheckE2rom();  //ִ��EEPROM�ļ��
                }
                continue;
            }            
            FlowingLight();  //��ˮ��Ч��ʵ��
            TestLedArray();  //LED�������ʵ��
            ShowRealTime();  //ʱ����ʾˢ��
            tmr1s++;         //ÿ��1sִ�����·�֧
            if (tmr1s >= 10)
            {
                tmr1s = 0;
                TestLedNumber(); //����ܲ�����ʾ
                RefreshTemp();   //�¶�ˢ����ʾ
                RefreshADC();    //ˢ��ADC����ֵ
            }
        }
    }
}
/* EEPROM��⺯����������Ƿ������� */
void CheckE2rom()
{
    uint8 buf[2];
    
    buf[0] = 0x33;  //���ü������
    buf[1] = 0xAA;
    E2Write(buf, 0x00, 2);  //�ɵ�ַ0x00д��
    buf[0] = 0x00;  //����������
    buf[1] = 0x00;
    E2Read(buf, 0x00, 2);  //�ɵ�ַ0x00������У��
    if ((buf[0]!=0x33) || (buf[1]!=0xAA))
    {
        LcdShowStr(3, 1, "N");  //���ݴ���ʱ��ʾ��N��
        return;
    }
    
    buf[0] = 0x55;  //���ü������
    buf[1] = 0xCC;
    E2Write(buf, 0x7F, 2);  //�ɵ�ַ0x7Fд��
    buf[0] = 0x00;  //����������
    buf[1] = 0x00;
    E2Read(buf, 0x7F, 2);  //�ɵ�ַ0x7F������У��
    if ((buf[0]!=0x55) || (buf[1]!=0xCC))
    {
        LcdShowStr(3, 1, "N");  //���ݴ���ʱ��ʾ��N��
    }
    else
    {
        LcdShowStr(3, 1, "Y");  //������ȷʱ��ʾ��Y��
    }
}
/* ��һ���ֽڰ�ʮ�����Ƹ�ʽ��ʾ����Ļ�ϣ�(x,y)-��Ļ��ʼ���꣬byt-����ʾ�ֽ� */
void ShowHexByte(uint8 x, uint8 y, uint8 byt)
{
    uint8 tmp;
    uint8 str[4];
    
    //����λתΪʮ�������ַ�
    tmp = byt >> 4;
    if (tmp <= 9)
        str[0] = tmp + '0';
    else
        str[0] = tmp - 10 + 'A';
    //����λתΪʮ�������ַ�
    tmp = byt & 0x0F;
    if (tmp <= 9)
        str[1] = tmp + '0';
    else
        str[1] = tmp - 10 + 'A';
    //��ӽ���������ʾ��Һ��
    str[2] = '\0';
    LcdShowStr(x, y, str);
}
/* �����������������ݼ���ִ����Ӧ�Ĳ�����keycode-�������� */
void KeyAction(uint8 keycode)
{
	uint8 str[4];
    
    BuzzBeep();  //���������֡�
    ShowHexByte(14, 0, keycode);  //��ʾ����
	//������ֵ��ʮ��������ʽת��Ϊ�ַ���
	str[0] = keycode >> 4;
	if (str[0] <= 9)
		str[0] += '0';
	else 
		str[0] += ('A'-10);
	str[1] = keycode & 0xF;
	if (str[1] <= 9)
		str[1] += '0';
	else
		str[1] += ('A'-10);
	str[2] = '\r';
	str[3] = '\n';
	//�����ɴ��ڷ���
	UartWrite("key: 0x", sizeof("key: 0x")-1);
	UartWrite(str, 4);
}
/* ���ڶ������������ݽ��յ�������ִ֡����Ӧ�Ķ���
   buf-���յ�������ָ֡�룬len-����֡���� */
void UartAction(uint8 *buf, uint8 len)
{
    BuzzBeep();  //���������֡�
    UartWrite(buf, len);  //�����յ�������ԭ���ͻ�
}
/* ��ȡDS1302ʱ�䣬��ʾ��Һ������ */
void ShowRealTime()
{
    struct sTime buf;
    static struct sTime pdata bkp = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};

    GetRealTime(&buf);
    if (bkp.hour != buf.hour)  //Сʱ�仯ʱˢ����ʾ
    {
        ShowHexByte(0, 0, buf.hour);
        bkp.hour = buf.hour;
    }
    if (bkp.min != buf.min)  //���ӱ仯ʱˢ����ʾ
    {
        ShowHexByte(3, 0, buf.min);
        bkp.min = buf.min;
    }
    if (bkp.sec != buf.sec)  //��仯ʱˢ����ʾ
    {
        ShowHexByte(6, 0, buf.sec);
        bkp.sec = buf.sec;
    }
}
/* �¶�ˢ�º�������ȡ��ǰ�¶Ȳ�������Ҫˢ��Һ����ʾ */
void RefreshTemp()
{
    int16 temp;
    uint8 pdata str[4];
    static int16 backup = 32767;  //��֤�״�ִ��ʱ�ض�ˢ��
    
    Get18B20Temp(&temp); //��ȡ��ǰ�¶�ֵ
    Start18B20();        //������һ��ת��
    temp >>= 4;          //����4bitС��λ
    if (backup != temp)  //�¶ȱ仯ʱˢ��Һ����ʾ
    {
        str[0] = (temp/10) + '0';  //ʮλתΪASCII��
        str[1] = (temp%10) + '0';  //��λתΪASCII��
        str[2] = '\0';             //�ַ���������
        LcdShowStr(7, 1, str);    //��ʾ��Һ����
        backup = temp;             //ˢ���ϴ��¶�ֵ
    }
}
/* ��ȡADCת���������ˢ����ʾ */
void RefreshADC()
{
    uint8 val;
    uint8 str[4];
    
    val = GetADCValue(0);  //��ȡ��ǰת��ֵ
    str[0] = (val/100) + '0';  //ת��Ϊ3λʮ��������
    str[1] = (val%100)/10 + '0';
    str[2] = (val%10) + '0';
    str[3] = '\0';
    LcdShowStr(13, 1, str);  //��ʾ��Һ����
}
/* ���ò�����T0��ms-T0��ʱʱ�� */
void ConfigTimer0(uint16 ms)
{
    uint32 tmp;
    
    tmp = (SYS_MCLK*ms)/1000; //��������ļ���ֵ
    tmp = 65536 - tmp;        //���㶨ʱ������ֵ
    tmp = tmp + 33;           //�����ж���Ӧ��ʱ��ɵ����   
    T0RH = (uint8)(tmp>>8);   //��ʱ������ֵ���Ϊ�ߵ��ֽ�
    T0RL = (uint8)tmp;
    TMOD &= 0xF0;   //����T0�Ŀ���λ
    TMOD |= 0x01;   //����T0Ϊģʽ1
    TH0 = T0RH;     //����T0����ֵ
    TL0 = T0RL;
    ET0 = 1;        //ʹ��T0�ж�
    TR0 = 1;        //����T0
}
/* T0�жϷ�������ʵ��ϵͳ��ʱ�Ͱ���ɨ�� */
void InterruptTimer0() interrupt 1
{
    static uint8 tmr100ms = 0;
    
    TH0 = T0RH;  //���¼�������ֵ
    TL0 = T0RL;
    BuzzCtrl();  //��������������
    LedScan();   //ִ��LEDɨ��
    KeyScan();   //ִ�а���ɨ��
    UartRxMonitor(1);  //ִ�д��ڽ��ռ��
    tmr100ms++;  //��ʱ100ms
    if (tmr100ms >= 100)
    {
        tmr100ms = 0;
        flag100ms = 1;
    }
}
