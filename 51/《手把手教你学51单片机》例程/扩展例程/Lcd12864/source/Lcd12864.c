/*
*******************************************************************************
*                     ���ְ��ֽ���ѧ51��Ƭ��(C���԰�)��
*                    ���� KST-51 ��Ƭ�������� ʾ��Դ����
*
*         (c) ��Ȩ���� 2014 ��ɳ̲������/�廪��ѧ������ ��������Ȩ��
*                 ��ȡ������������ʣ�http://www.kingst.org
*
* �ļ�����Lcd12864.c
* ��  ����12864����Һ������ģ��
* �汾�ţ�v1.0.0
* ��  ע��������KST-51���������׵�12864Һ��ģ��
*******************************************************************************
*/

#define _LCD_12864_C
#include "config.h"
#include "Lcd12864.h"


/* �ȴ�Һ��׼���� */
void LcdWaitReady()
{
    uint8 sta;
    
    LCD12864_DB = 0xFF;
    LCD12864_RS = 0;
    LCD12864_RW = 1;
    do {
        LCD12864_E = 1;
        sta = LCD12864_DB;
        LCD12864_E = 0;
    } while (sta & 0x80); //bit7����1��ʾҺ����æ���ظ����ֱ�������0Ϊֹ
}
/* ��Һ��д��һ�ֽ����cmd-��д������ֵ */
void LcdWriteCmd(uint8 cmd)
{
    LcdWaitReady();
    LCD12864_RS = 0;
    LCD12864_RW = 0;
    LCD12864_DB = cmd;
    LCD12864_E  = 1;
    LCD12864_E  = 0;
}
/* ��Һ��д��һ�ֽ����ݣ�dat-��д������ֵ */
void LcdWriteDat(uint8 dat)
{
    LcdWaitReady();
    LCD12864_RS = 1;
    LCD12864_RW = 0;
    LCD12864_DB = dat;
    LCD12864_E  = 1;
    LCD12864_E  = 0;
}
/*
* ��������LcdShowString
* ��  ��������ʾ������ʾһ���ַ���
* ��  �룺str - ����ʾ�ַ���ָ��
*         x - ��Ļ��ʾ������(������Ϊ��λ)
*         y - ��Ļ��ʾ������(������Ϊ��λ)
* ��  ������
* ��  ע��������ַ����������C���Թ淶������'\0'-NULLΪ������ʶ��
*         x��y���������16������������DDRAM��ַ��ȫ���ַ�(16*16pixel)Ϊ��λ��
*/
void LcdShowString(uint8 x, uint8 y, uint8 *str)
{
    uint8 addr;
    
    //���������ʾ�������DDRAM�ĵ�ַ
    x >>= 4;
    y >>= 4;
    if (y >= 2)
    {
        y -= 2;
        x += 8;
    }
    addr = y*16 + x;
    //����ʼDDRAM��ַ����д���ַ���
    LcdWriteCmd(0x30); //����DDRAM����
    LcdWriteCmd(0x80|addr);
    while (*str != '\0')
    {
        LcdWriteDat(*str);
        str++;
    }
}
/*
* ��������LcdShowImage
* ��  ��������ʾ������ʾһ��ͼ��
* ��  �룺img - ����ʾͼ��ָ��
*         x - ��Ļ��ʾ������(������Ϊ��λ)
*         y - ��Ļ��ʾ������(������Ϊ��λ)
*         w - ͼ����(������Ϊ��λ)
*         h - ͼ��߶�(������Ϊ��λ)
* ��  ������
* ��  ע��x��w������16������������CGRAM��СѰַ��λΪ2�ֽڣ�y��h��Ϊ0-63������ֵ��
*/
void LcdShowImage(uint8 x, uint8 y, uint8 w, uint8 h, uint8 *img)
{
    int16 i;
    uint8 xi,yi;
    uint8 xt,yt;

    x >>= 4;
    w >>= 3;
    i = 0;
    LcdWriteCmd(0x36); //����CGRAM����
    for (yi=0; yi<h; yi++)
    {
        yt = y+yi;
        xt = x;
        if (yt >= 32)
        {
            yt -= 32;
            xt += 8;
        }
        LcdWriteCmd(0x80|yt);
        LcdWriteCmd(0x80|xt);
        for (xi=0; xi<w; xi++)
        {
            LcdWriteDat(img[i++]);
        }
    }
}

/*
* ��������LcdClearArea
* ��  ���������Ļ�ϵ�һ��ͼ����ʾ����
* ��  �룺x - ������ʼ������(������Ϊ��λ)
*         y - ������ʼ������(������Ϊ��λ)
*         w - ������(������Ϊ��λ)
*         h - ����߶�(������Ϊ��λ)
* ��  ������
* ��  ע��x��w������16������������CGRAM��СѰַ��λΪ2�ֽڣ�y��h��Ϊ0-63������ֵ��
*/
void LcdClearArea(uint8 x, uint8 y, uint8 w, uint8 h)
{
    uint8 xi,yi;
    uint8 xt,yt;

    x >>= 4;
    w >>= 3;
    LcdWriteCmd(0x36); //����CGRAM����
    for (yi=0; yi<h; yi++)
    {
        yt = y+yi;
        xt = x;
        if (yt >= 32)
        {
            yt -= 32;
            xt += 8;
        }
        LcdWriteCmd(0x80|yt);
        LcdWriteCmd(0x80|xt);
        for (xi=0; xi<w; xi++)
        {
            LcdWriteDat(0x00); //д��0x00�������ʾ
        }
    }
}

/* 12864Һ����ʼ������ */
void InitLcd12864()
{
    uint8 x, y;

    //�ַ�ģʽ��ʼ��
    LcdWriteCmd(0x30);  //����ָ�
    LcdWriteCmd(0x01);  //�����ַ���ʾ
    LcdWriteCmd(0x02);  //��ַ��λ
    LcdWriteCmd(0x0C);  //����ʾ
    
    //ͼ��ģʽ��ʼ��
    LcdWriteCmd(0x34);    //������չָ��ر�ͼ����ʾ
    for (y=0; y<32; y++)  //����ͼ����ʾ������
    {
        LcdWriteCmd(0x80|y);
        LcdWriteCmd(0x80|0);
        for (x=0; x<32; x++)
        {
            LcdWriteDat(0x00);
        }
    }
    LcdWriteCmd(0x36);  //����ͼ��ģʽ��ʾ
}
