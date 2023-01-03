/*
*******************************************************************************
*                     ���ְ��ֽ���ѧ51��Ƭ��(C���԰�)��
*                    ���� KST-51 ��Ƭ�������� ʾ��Դ����
*
*         (c) ��Ȩ���� 2014 ��ɳ̲������/�廪��ѧ������ ��������Ȩ��
*                 ��ȡ������������ʣ�http://www.kingst.org
*
* �ļ�����PCF8591.c
* ��  ����AD/DAоƬPCF8591����ģ��
* �汾�ţ�v1.0.0
* ��  ע��
*******************************************************************************
*/

#define  _PCF8591_C
#include "config.h"
#include "I2C.h"
#include "PCF8591.h"

/* ��ȡ��ǰ��ADCת��ֵ��chn-ADCͨ����0~3 */
uint8 GetADCValue(uint8 chn)
{
    uint8 val;
    
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

/* ����DAC���ֵ��val-�趨ֵ */
void SetDACOut(uint8 val)
{
    I2CStart();
    if (!I2CWrite(0x48<<1)) //ѰַPCF8591����δӦ����ֹͣ����������
    {
        I2CStop();
        return;
    }
    I2CWrite(0x40);         //д������ֽ�
    I2CWrite(val);          //д��DAֵ  
    I2CStop();
}
