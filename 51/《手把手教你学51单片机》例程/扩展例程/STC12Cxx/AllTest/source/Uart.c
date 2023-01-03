/*
*******************************************************************************
*                     ���ְ��ֽ���ѧ51��Ƭ��(C���԰�)��
*                    ���� KST-51 ��Ƭ�������� ʾ��Դ����
*
*         (c) ��Ȩ���� 2014 ��ɳ̲������/�廪��ѧ������ ��������Ȩ��
*                 ��ȡ������������ʣ�http://www.kingst.org
*
* �ļ�����Uart.c
* ��  ��������֡ģʽ��UART��������ģ��
* �汾�ţ�v1.0.0
* ��  ע�����STC12ϵ�У�����ͨ�Ų������ɶ��������ʷ������ṩ
*******************************************************************************
*/

#define  _UART_C
#include "config.h"
#include "Uart.h"
#include "main.h"

//����STC12ϵ�ж��������ʷ�������ؼĴ���
sfr BRT  = 0x9C; //���������ʷ������Ĵ���
sfr AUXR = 0x8E; //�����Ĵ���


bit flagFrame = 0;  //֡������ɱ�־�������յ�һ֡������
bit flagTxd = 0;    //���ֽڷ�����ɱ�־���������TXD�жϱ�־λ
uint8 cntRxd = 0;   //�����ֽڼ�����
uint8 pdata bufRxd[50];  //�����ֽڻ�����

extern void UartAction(uint8 *buf, uint8 len);

/* �������ú�����baud-ͨ�Ų����� */
void ConfigUART(uint16 baud)
{
    SCON  = 0x50;  //���ô���Ϊģʽ1
    BRT   = 256-(uint8)(OSC_FREQ/32/baud);  //���㲨���ʷ���������ֵ
    AUXR |= 0x15;  //���ö��������ʷ���������
    ES    = 1;     //ʹ�ܴ����ж�
}
/* ��������д�룬�����ڷ��ͺ�����buf-���������ݵ�ָ�룬len-ָ���ķ��ͳ��� */
void UartWrite(uint8 *buf, uint8 len)
{
    while (len--)  //ѭ�����������ֽ�
    {
        flagTxd = 0;      //���㷢�ͱ�־
        SBUF = *buf++;    //����һ���ֽ�����
        while (!flagTxd); //�ȴ����ֽڷ������
    }
}
/* �������ݶ�ȡ������buf-����ָ�룬len-ָ���Ķ�ȡ���ȣ�����ֵ-ʵ�ʶ����ĳ��� */
uint8 UartRead(uint8 *buf, uint8 len)
{
    uint8 i;
    
    if (len > cntRxd)  //ָ����ȡ���ȴ���ʵ�ʽ��յ������ݳ���ʱ��
    {                  //��ȡ��������Ϊʵ�ʽ��յ������ݳ���
        len = cntRxd;
    }
    for (i=0; i<len; i++)  //�������յ������ݵ�����ָ����
    {
        *buf++ = bufRxd[i];
    }
    cntRxd = 0;  //���ռ���������
    
    return len;  //����ʵ�ʶ�ȡ����
}
/* ���ڽ��ռ�أ��ɿ���ʱ���ж�֡���������ڶ�ʱ�ж��е��ã�ms-��ʱ��� */
void UartRxMonitor(uint8 ms)
{
    static uint8 cntbkp = 0;
    static uint8 idletmr = 0;

    if (cntRxd > 0)  //���ռ�����������ʱ��������߿���ʱ��
    {
        if (cntbkp != cntRxd)  //���ռ������ı䣬���ս��յ�����ʱ��������м�ʱ
        {
            cntbkp = cntRxd;
            idletmr = 0;
        }
        else                   //���ռ�����δ�ı䣬�����߿���ʱ���ۻ�����ʱ��
        {
            if (idletmr < 30)  //���м�ʱС��30msʱ�������ۼ�
            {
                idletmr += ms;
                if (idletmr >= 30)  //����ʱ��ﵽ30msʱ�����ж�Ϊһ֡�������
                {
                    flagFrame = 1;  //����֡������ɱ�־
                }
            }
        }
    }
    else
    {
        cntbkp = 0;
    }
}
/* ���������������������֡�Ľ��գ����ȹ��ܺ�����������ѭ���е��� */
void UartDriver()
{
    uint8 len;
    uint8 pdata buf[40];

    if (flagFrame) //�������ʱ����ȡ���������
    {
        flagFrame = 0;
        len = UartRead(buf, sizeof(buf));  //�����յ��������ȡ����������
        UartAction(buf, len);  //��������֡�����ö���ִ�к���
    }
}
/* �����жϷ����� */
void InterruptUART() interrupt 4
{
    if (RI)  //���յ����ֽ�
    {
        RI = 0;  //��������жϱ�־λ
        if (cntRxd < sizeof(bufRxd)) //���ջ�������δ����ʱ��
        {                            //��������ֽڣ�������������
            bufRxd[cntRxd++] = SBUF;
        }
    }
    if (TI)  //�ֽڷ������
    {
        TI = 0;   //���㷢���жϱ�־λ
        flagTxd = 1;  //�����ֽڷ�����ɱ�־
    }
}
