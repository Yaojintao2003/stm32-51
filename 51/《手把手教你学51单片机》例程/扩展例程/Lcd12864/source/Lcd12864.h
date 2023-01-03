/*
*******************************************************************************
*                     ���ְ��ֽ���ѧ51��Ƭ��(C���԰�)��
*                    ���� KST-51 ��Ƭ�������� ʾ��Դ����
*
*         (c) ��Ȩ���� 2014 ��ɳ̲������/�廪��ѧ������ ��������Ȩ��
*                 ��ȡ������������ʣ�http://www.kingst.org
*
* �ļ�����Lcd12864.c
* ��  ����12864����Һ������ģ���ͷ�ļ�
* �汾�ţ�v1.0.0
* ��  ע��
*******************************************************************************
*/

#ifndef _LCD_12864_H
#define _LCD_12864_H


//12864Һ�����������1602Һ����ͬ
#define LCD12864_DB   LCD1602_DB
#define LCD12864_RS   LCD1602_RS
#define LCD12864_RW   LCD1602_RW
#define LCD12864_E    LCD1602_E


#ifndef _LCD_12864_C

#endif


void InitLcd12864();
void LcdShowString(uint8 x, uint8 y, uint8 *str);
void LcdShowImage(uint8 x, uint8 y, uint8 w, uint8 h, uint8 *img);
void LcdClearArea(uint8 x, uint8 y, uint8 w, uint8 h);


#endif
