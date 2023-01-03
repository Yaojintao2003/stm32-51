/*
*******************************************************************************
*                     ���ְ��ֽ���ѧ51��Ƭ��(C���԰�)��
*                    ���� KST-51 ��Ƭ�������� ʾ��Դ����
*
*         (c) ��Ȩ���� 2014 ��ɳ̲������/�廪��ѧ������ ��������Ȩ��
*                 ��ȡ������������ʣ�http://www.kingst.org
*
* �ļ�����main.c
* ��  ������17�� ��ҵ��2 ��ģ������ͨ��0��1�ĵ�ѹֵ��ʾ���������
* �汾�ţ�v1.0.0
* ��  ע��ͨ��0��ѹֵ��ʾ����࣬ͨ��1��ѹֵ��ʾ���Ҳ࣬�м����λ����ʾ��
*******************************************************************************
*/

#include <reg52.h>

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

bit flag300ms = 1;       //300ms��ʱ��־
unsigned char T0RH = 0;  //T0����ֵ�ĸ��ֽ�
unsigned char T0RL = 0;  //T0����ֵ�ĵ��ֽ�
unsigned char code LedChar[] = {  //�������ʾ�ַ�ת����
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};
unsigned char LedBuff[6] = {  //�������ʾ����������ֵ0xFFȷ������ʱ������
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

void ConfigTimer0(unsigned int ms);
unsigned char GetADCValue(unsigned char chn);
extern void I2CStart();
extern void I2CStop();
extern unsigned char I2CReadACK();
extern unsigned char I2CReadNAK();
extern bit I2CWrite(unsigned char dat);
extern void InitLcd1602();
extern void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);

void main()
{
    unsigned char val;
    
    EA = 1;           //�����ж�
    ENLED = 0;        //ʹ��U3��ѡ����������
    ADDR3 = 1;        //��Ϊ��Ҫ��̬�ı�ADDR0-2��ֵ������0-2����Ҫ��ʼ����
    ConfigTimer0(2);  //����T0��ʱ2ms
    
    while (1)
    {
        if (flag300ms)
        {
            flag300ms = 0;
            //��ʾͨ��0�ĵ�ѹ
            val = GetADCValue(0);   //��ȡADCͨ��0��ת��ֵ
            val = (val*25) / 255;   //��ѹֵ=ת�����*2.5V/255��ʽ�е�25������һλʮ����С��
            LedBuff[5] = LedChar[val/10] & 0x7F;  //ͨ��0������λ��ʾ������ܵ�5λ����������5λ��С����
            LedBuff[4] = LedChar[val%10];         //ͨ��0��С��λ��ʾ������ܵ�4λ
             //��ʾͨ��1�ĵ�ѹ
            val = GetADCValue(1);   //��ȡADCͨ��1��ת��ֵ
            val = (val*25) / 255;   //��ѹֵ=ת�����*2.5V/255��ʽ�е�25������һλʮ����С��
            LedBuff[1] = LedChar[val/10] & 0x7F;  //ͨ��1������λ��ʾ������ܵ�1λ����������1λ��С����
            LedBuff[0] = LedChar[val%10];         //ͨ��1��С��λ��ʾ������ܵ�0λ
        }
    }
}
/* ��ȡ��ǰ��ADCת��ֵ��chn-ADCͨ����0~3 */
unsigned char GetADCValue(unsigned char chn)
{
    unsigned char val;
    
    I2CStart();
    if (!I2CWrite(0x48<<1))  //ѰַPCF8591����δӦ����ֹͣ����������0
    {
        I2CStop();
        return 0;
    }
    I2CWrite(0x40|chn);        //д������ֽڣ�ѡ��ת��ͨ��
    I2CStart();
    I2CWrite((0x48<<1)|0x01);  //ѰַPCF8591��ָ������Ϊ������    
    I2CReadACK();              //�ȿն�һ���ֽڣ��ṩ����ת��ʱ��
    val = I2CReadNAK();        //��ȡ�ո�ת�����ֵ
    I2CStop();
    
    return val;
}
/* ���ò�����T0��ms-T0��ʱʱ�� */
void ConfigTimer0(unsigned int ms)
{
    unsigned long tmp;  //��ʱ����
    
    tmp = 11059200 / 12;      //��ʱ������Ƶ��
    tmp = (tmp * ms) / 1000;  //��������ļ���ֵ
    tmp = 65536 - tmp;        //���㶨ʱ������ֵ
    tmp = tmp + 12;           //�����ж���Ӧ��ʱ��ɵ����
    T0RH = (unsigned char)(tmp>>8);  //��ʱ������ֵ���Ϊ�ߵ��ֽ�
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;   //����T0�Ŀ���λ
    TMOD |= 0x01;   //����T0Ϊģʽ1
    TH0 = T0RH;     //����T0����ֵ
    TL0 = T0RL;
    ET0 = 1;        //ʹ��T0�ж�
    TR0 = 1;        //����T0
}
/* T0�жϷ�������ִ������ܶ�̬��ʾ��300ms��ʱ */
void InterruptTimer0() interrupt 1
{
    static unsigned char i = 0;  //��̬ɨ�������
    static unsigned char tmr300ms = 0;  //300ms�����ʱ������
    
    TH0 = T0RH;  //���¼�������ֵ
    TL0 = T0RL;
    tmr300ms++;
    if (tmr300ms >= 150)  //��ʱ300ms
    {
        tmr300ms = 0;
        flag300ms = 1;
    }
    //���´����������ܶ�̬ɨ��ˢ��
    P0 = 0xFF;   //��ʾ����
    switch (i)
    {
        case 0: ADDR2=0; ADDR1=0; ADDR0=0; i++; P0=LedBuff[0]; break;
        case 1: ADDR2=0; ADDR1=0; ADDR0=1; i++; P0=LedBuff[1]; break;
        case 2: ADDR2=0; ADDR1=1; ADDR0=0; i++; P0=LedBuff[2]; break;
        case 3: ADDR2=0; ADDR1=1; ADDR0=1; i++; P0=LedBuff[3]; break;
        case 4: ADDR2=1; ADDR1=0; ADDR0=0; i++; P0=LedBuff[4]; break;
        case 5: ADDR2=1; ADDR1=0; ADDR0=1; i=0; P0=LedBuff[5]; break;
        default: break;
    }
}
