/*
*******************************************************************************
*                     ���ְ��ֽ���ѧ51��Ƭ��(C���԰�)��
*                    ���� KST-51 ��Ƭ�������� ʾ��Դ����
*
*         (c) ��Ȩ���� 2014 ��ɳ̲������/�廪��ѧ������ ��������Ȩ��
*                 ��ȡ������������ʣ�http://www.kingst.org
*
* �ļ�����main.c
* ��  ����ȫ����Գ��� �������ļ���ͷ�ļ�
* �汾�ţ�v1.0.0
* ��  ע��
*******************************************************************************
*/

#ifndef _MAIN_H
#define _MAIN_H


#ifndef _MAIN_C

#endif

void KeyAction(uint8 keycode);
void ShowRealTime();
void RefreshTemp();
void CheckE2rom();
void RefreshADC();
void ConfigTimer0(uint16 ms);

#endif
