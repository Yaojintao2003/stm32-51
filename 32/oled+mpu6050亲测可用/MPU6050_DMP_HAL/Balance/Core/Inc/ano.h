#ifndef __ANO_H
#define __ANO_H
#include "main.h"
#define u8 uint8_t
void usart2_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw);
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);
void usart2_niming_report(u8 fun,u8*data,u8 len);
void mpu6050_send_status_data(short roll,short pitch,short yaw);
void usart2_send_char(u8 c);

#endif

