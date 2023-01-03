/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "math.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId Main_TaskHandle;
osThreadId Encoder_TaskHandle;
osThreadId MPU6050_TaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
//-----ȫ�ֱ���---
int Vertical_out,Velocity_out,Turn_out; // ֱ����&�ٶȻ�&ת�򻷵��������
int Encoder_Left,Encoder_Right,Balance_PWM;
int Moto1,Moto2;//����2�����
int PWM_out,Turn_Pwm; //�ջ��������
int speedControlPeriod = 0;//�����ٶȻ�ƽ�����������

u8 AIN1,AIN2;//���Ƶ������ת
u8 BIN1,BIN2;
u8 Moto_Flag=0;//������Ʊ�־
u8 Start_Flag=0;//ϵͳ��ʼ����ɱ�־	
u8 Tracking_Flag=1;//Ѱ����־


float Pitch,Roll,Yaw;	        // Pitch�������ǣ�Roll������ǣ�Yaw��ƫ����
short gyrox,gyroy,gyroz;        // ���ٶ�
short aacx,aacy,aacz;           // ���ٶ�
float Med_Angle=-2;//��е��ֵΪ2����ʹ��С������ƽ��ס�ĽǶȣ����Զ��ο�������Ϊһ���ٶ�(����һ����б��)





unsigned char Usart_TX_Buf[40];
uint16_t date1;//stm32���յ�openmv��PID�㷨�õ�speed*1000�������
uint16_t Tracking_speed;//Ѱ���ٶ�
uint16_t start_speed = 0;//��ʼ�ٶ�
uint8_t Usart1_RX_Buf;  //���ڽ������ݻ���buf


//----�Ӻ�������-----
void UART_Proc();
int Vertical(float Angle,float Gyro_Y);			 				//ֱ����
int Velocity(int Encoder_left,int Encoder_right);				//�ٶȻ�
int Turn(int datex,short yaw);	//ת��
u8 Stop(signed int angle);    		//���±���
void Limit(int *motoA,int *motoB);  //����ٶ��޷�
void Set_Pwm(int Moto1,int Moto2);	//����PWM�������
int ComplementaryFiltering();
int ComplementaryFiltering2();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

//-----printf�ض���-----
//int fputc(int ch,FILE *f)
//{
//	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xFFFF);
//	return ch;

//}




/* USER CODE END FunctionPrototypes */

void Main_Task_Start(void const * argument);
void Encoder_Task_Start(void const * argument);
void MPU6050_Task_Start(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
//��̬����ȥ��������
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Main_Task */
  osThreadDef(Main_Task, Main_Task_Start, osPriorityAboveNormal, 0, 128);
  Main_TaskHandle = osThreadCreate(osThread(Main_Task), NULL);

  /* definition and creation of Encoder_Task */
  osThreadDef(Encoder_Task, Encoder_Task_Start, osPriorityNormal, 0, 128);
  Encoder_TaskHandle = osThreadCreate(osThread(Encoder_Task), NULL);

  /* definition and creation of MPU6050_Task */
  osThreadDef(MPU6050_Task, MPU6050_Task_Start, osPriorityNormal, 0, 128);
  MPU6050_TaskHandle = osThreadCreate(osThread(MPU6050_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Main_Task_Start */
/**
  * @brief  Function implementing the Main_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Main_Task_Start */




/*****************   100HZ����   **************************/

void Main_Task_Start(void const * argument)
{
  /* USER CODE BEGIN Main_Task_Start */
  /* Infinite loop */

	 HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//PWM
  	 HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);	
  
	 HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1);//������
	 HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_2);
	 HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_1);
	 HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_2);	 
    HAL_UART_Receive_IT(&huart1, &Usart1_RX_Buf, 1);//ʹ�ܴ���1�����ж�
	 

	//------MPU6050����-----

  //----iic��ȡMUP����ID----
	/* ��1������ΪI2C�������

��2������Ϊ�ӻ��豸��ַ

��3������Ϊ�ӻ��Ĵ�����ַ

��4������Ϊ�ӻ��Ĵ�����ַ����

��5������Ϊ���͵����ݵ���ʼ��ַ

��6������Ϊ�������ݵĴ�С

��7������Ϊ������ʱʱ�� */
  uint8_t recv = 0x00;
  HAL_I2C_Mem_Read(&hi2c2, (0x68 << 1), 0x75, I2C_MEMADD_SIZE_8BIT, &recv, 1, 0xfff);//�����MUP��ַ,����ID�Ĵ���
  
  if (recv == 0x68)
  { 
  }
  else
  {
  }  	 
  
  //-----DMP��ʼ��----
  while(mpu_dmp_init())//�ɹ�����0�����򷵻�1
  {
		uint8_t res;
		res = mpu_dmp_init();
		HAL_Delay(300);		  
  }
 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0); //��PC13 LED
 Start_Flag = 1; //��־ϵͳ��ʼ���ɹ�




	
  for(;;)	
  { 
	  
	  
		Vertical_out= Vertical(Pitch+0.1,gyroy+22);//ֱ����
	  
	   Velocity_out = Velocity(Encoder_Left,Encoder_Right);// �ٶȻ�
	  
		Turn_Pwm = Turn(date1,Yaw+1);
	  
		//�������
	   Moto1 = Vertical_out-Velocity_out+Turn_Pwm; // �������˴��뵥ƽ�⳵�����ͬ(�������)
		Moto2 = Vertical_out-Velocity_out-Turn_Pwm; // �ҵ�� 
	  
		Limit(&Moto1,&Moto2);     // PWM�޷� 
		Set_Pwm(Moto1,Moto2);        // ���ص������	 		  

   	  	  
		osDelay(10); 		  		  		 	 	  		   			
  }
  /* USER CODE END Main_Task_Start */
}

/* USER CODE BEGIN Header_Encoder_Task_Start */
/**
* @brief Function implementing the Encoder_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Encoder_Task_Start */




/*****************   200HZ����   **************************/

void Encoder_Task_Start(void const * argument)
{
  /* USER CODE BEGIN Encoder_Task_Start */
  /* Infinite loop */
  for(;;)
  {
	  // 1.�ɼ�����������&MPU6050�Ƕ���Ϣ
   // �������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ��  		  
	  Encoder_Left  =   Read_Encoder(3);//��ȡ�����������ĵ��ת��
	  Encoder_Right =   Read_Encoder(4);		  
		
	  
	  osDelay(5);
  }
  /* USER CODE END Encoder_Task_Start */
}

/* USER CODE BEGIN Header_MPU6050_Task_Start */
/**
* @brief Function implementing the MPU6050_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MPU6050_Task_Start */



/*****************   200HZ����   **************************/

void MPU6050_Task_Start(void const * argument)
{
  /* USER CODE BEGIN MPU6050_Task_Start */
  /* Infinite loop */
  for(;;)
  {
		
		mpu_dmp_get_data(&Pitch,&Roll,&Yaw);	    // ��ȡ�Ƕ�
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);  // ��ȡ���ٶ�
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz); // ��ȡ���ٶ�  	 	  
    
	  
	  osDelay(5);
  }
  /* USER CODE END MPU6050_Task_Start */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */








/*************************************************************************** 
ֱ����PD��������Kp*Ek+Kd*Ek_D
��ڣ�Med:��е��ֵ(�����Ƕ�)��Angle:��ʵ�Ƕȣ�Med_Angle����е0�㣬gyro_Y:��ʵ���ٶ�
���ڣ�PMW��ֵ
******************************************************************************/
int Vertical(float Angle,float Gyro_Y) 
{
	float Vertical_Kp= -360,Vertical_Kd=-1.2;	//ֱ����Kp,Kd
	PWM_out = Vertical_Kp*(Angle-Med_Angle)+Vertical_Kd*(Gyro_Y-0);	//-5
	return PWM_out;	
} 




/********************************************************************* 
�ٶȻ�PI��������Kp*Ek+Ki*Ek_S(Ek_S��ƫ��Ļ���)
��ڣ����ұ������⵽����ֵ
���ڣ��ٶȻ����pwm                 
**********************************************************************/
int Velocity(int Encoder_left,int Encoder_right)	
{
	// ����ɾ�̬�����������ھ�̬�洢����ʹ�ñ���������
	static float  Velocity,Encoder_Err,Encoder,Encoder_last; 
	static float Encoder_Integral,Movement=10;	//Movement�����ٶ�	
	float kp=37,ki=0.185;	 //�ٶȻ�P I 37,0.185		
	
	// 1.�����ٶ�ƫ�� 	
	Encoder_Err = ((Encoder_Left+Encoder_Right)-0)*0.5;	
 
	// 2.���ٶ�ƫ�����--��ͨ�˲�--
  // low_out = (1-a)*Ek+a*low_out_last	
	Encoder = Encoder_Err*0.3 + Encoder_last*0.7;// ʹ�ò��θ���ƽ��,�ϴε��ٶ�ռ70%,���ε��ٶ�ռ30% 
	Encoder_last = Encoder; 							// ��ֹ�ٶȹ���Ӱ��ֱ��������������

	// 3.���ٶ�ƫ����ֳ�λ��,ң�ص��ٶ�ͨ�����������ٶȿ������������ٶ�ͻ���ֱ�����Ƶ�Ӱ��
  Encoder_Integral += Encoder-Movement; //���ֳ�λ�� ����ʱ�䣺10ms	


	// 4.�����޷���Ѱ��Ч����Ҫ���ͻ��֣�����ǵ�ֱ�����ƣ�10000����
	if(Encoder_Integral>300)  	Encoder_Integral=300;   
	if(Encoder_Integral<-200)	   Encoder_Integral=-200;           	

	if(Moto_Flag == 1||Start_Flag ==0) 			Encoder_Integral=0;     		//===����رպ���߸�λ�������
    //5.�ٶȻ��������	
   Velocity=Encoder*kp+Encoder_Integral*ki;
	
//	speedControlPeriod++; 	
//	if(speedControlPeriod>10)//�����ٶȻ�ƽ��������ɼ�10�β����һ��
//	{
//		speedControlPeriod = 0;
		return Velocity;

//	}
	
}


/*********************************************************************
ת�򻷣�����ѭ��
��ڣ�date1,yaw = z����������ֵ
**********************************************************************/
int Turn(int datex,short yaw)
{
   static float Bias,Last_Bias,date,Turn_Kp=15,Turn_Kd=0.18; 
   float Turn_PWM;	
			
	date=datex;
	if(date>=100) //��ƫ,���ּ������ּ��٣�Turn_Pwm���Ϊ+   Openmv��+100��������
	{
	  date = (datex-200);
				
	}	
	//=============turing PD controller==================//			
				Bias = (date-0);
				Turn_PWM= -Bias*Turn_Kp-yaw*Turn_Kd;	
				Last_Bias = Bias;
			return Turn_PWM; 
}


	
	
/*************************************************************************** 
�������ܣ����Ƶ��
******************************************************************************/
void Contrl(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, AIN1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, AIN2);
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, BIN1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, BIN2);			
}




/**************************************************************************
�������ܣ�����쳣�رպ���
��ڲ������Ƕ�
����  ֵ��1���رգ�0�����ر�
**************************************************************************/	 		
u8 Stop(signed int angle)
{
	    u8 temp=0;
			if(angle<-40||angle>40)
			{	                                //===��Ǵ���40�ȹرյ��
				temp=1;                   		   //===Flag_Stop��1�رյ��
		//		Moto1 = 0;
			//	Moto2 = 0;
      }
	
		return temp;
}



/**************************************************************************
�������ܣ����ת�����ƺ���
��ڲ������ջ������������ֵ
**************************************************************************/	
void Set_Pwm(int Moto1,int Moto2)
{
	/*
	 Moto_Flag=Stop(Pitch+0.1);	//��ȡ�Ƿ��µı�־
	int   Dead_Zone=200;     //�������
	
	if(Start_Flag == 1)		//һ���ж�ϵͳ�Ƿ�������ʼ��
	{
		if(Moto_Flag==0)	//�����ж�
		{
			if(Moto1>0)  AIN1 = 1,AIN2 = 0;		
			else			 AIN1 = 0,AIN2 = 1;							

			
			if(Moto2>0)  BIN1 = 1,BIN2 = 0;		
			else		    BIN1 = 0 ,BIN2 = 1;											
		   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,abs(Moto1)+Dead_Zone);
		   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,abs(Moto1)+Dead_Zone); //0-7200			
		
	  }			
		else//���¾͹رյ��
		{
		   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);	//4500-6000
		   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0);	//4500-6000
		}
		 Contrl();
	}
	*/
}


/**************************************************************************
�������ܣ����Ƶ���ٶ�
��ڲ������ջ������������ֵ
**************************************************************************/	
void Limit(int *motoA,int *motoB)
{
	if(*motoA>7000)*motoA=7000;//���7200
	if(*motoA<-7000)*motoA=-7000;

	if(*motoB>7000)*motoB=7000;
	if(*motoB<-7000)*motoB=-7000;
}




/**************************************************************************
�������ܣ�USART1�����жϻص� or ��openmv����ͨ��
���ţ�		 TX_PA9,RX_PA10
**************************************************************************/
//�����жϻص�����
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{

		uint8_t i;
		static uint8_t RxCounter1=0;//���ݻ�����������
		static uint8_t RxBuffer1[5]={0};//������ݵĽ��ջ�����
		static uint8_t RxState = 0;	//���ձ�־λ
			
		if(RxState==0&&Usart1_RX_Buf==0x2C)  //0x2c֡ͷ
				{
					RxState=1;
					RxBuffer1[RxCounter1++]=Usart1_RX_Buf;//RxBuffer1[0]==0x2c RxCounter1==1
				}
		
				else if(RxState==1&&Usart1_RX_Buf==0x12)  //0x12֡ͷ
				{
					RxState=2;
					RxBuffer1[RxCounter1++]=Usart1_RX_Buf;
				}
		
				else if(RxState==2)//��ʼ������Ч����
				{
					RxBuffer1[RxCounter1++]=Usart1_RX_Buf;//ȫ�������꣬RxCounter1==7

					if(RxCounter1>=5||Usart1_RX_Buf == 0x5B)       //RxBuffer1��������,�������ݽ���
					{
						  RxState=3;
						//RxBuffer1[2]��openmv���͵ĵ�һ�����ݵĵͰ�λ,RxBuffer1[3]��openmv���͵ĵ�һ�����ݵĸ߰�λ
						//date1=(RxBuffer1[RxCounter1-4]<<8)+(RxBuffer1[RxCounter1-5]);//��Ϊ�Ͱ�λ�͸߰�λ���ȷ��ͺ󷢸�
						//date2=(RxBuffer1[RxCounter1-2]<<8)+(RxBuffer1[RxCounter1-3]);
						date1=(RxBuffer1[RxCounter1-2]<<8)+(RxBuffer1[RxCounter1-3]);
//						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);						
						
					}
				}
		
				else if(RxState==3)		//����Ƿ���ܵ�������־
				{
						if(RxBuffer1[RxCounter1-1] == 0x5B)
						{
									
//								
//									RxFlag1 = 0;
									RxCounter1 = 0;
									RxState = 0;
//						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);	
//						HAL_UART_Receive_IT(&huart1,&Usart1_RX_Buf,1);	
						}
						else   //���մ���
						{
									RxState = 0;
									RxCounter1=0;
									for(i=0;i<5;i++)
									{
											RxBuffer1[i]=0x00;      //�����������������
											Tracking_Flag = 0;
									}
						}
				} 
	
				else   //�����쳣
				{
						RxState = 0;
						RxCounter1=0;
						for(i=0;i<5;i++)
						{
								RxBuffer1[i]=0x00;      //�����������������
								Tracking_Flag=0;
						}
				}

		


	HAL_UART_Receive_IT(&huart1,&Usart1_RX_Buf,1);

}



/* USER CODE END Application */

