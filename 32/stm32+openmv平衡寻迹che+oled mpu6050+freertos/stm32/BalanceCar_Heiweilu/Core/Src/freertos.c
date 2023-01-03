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
//-----全局变量---
int Vertical_out,Velocity_out,Turn_out; // 直立环&速度环&转向环的输出变量
int Encoder_Left,Encoder_Right,Balance_PWM;
int Moto1,Moto2;//控制2个电机
int PWM_out,Turn_Pwm; //闭环输出变量
int speedControlPeriod = 0;//控制速度环平缓输出的周期

u8 AIN1,AIN2;//控制电机正反转
u8 BIN1,BIN2;
u8 Moto_Flag=0;//电机控制标志
u8 Start_Flag=0;//系统初始化完成标志	
u8 Tracking_Flag=1;//寻迹标志


float Pitch,Roll,Yaw;	        // Pitch：俯仰角，Roll：横滚角，Yaw：偏航角
short gyrox,gyroy,gyroz;        // 角速度
short aacx,aacy,aacz;           // 加速度
float Med_Angle=-2;//机械中值为2，能使得小车真正平衡住的角度，可以二次开发作作为一定速度(给定一定倾斜角)





unsigned char Usart_TX_Buf[40];
uint16_t date1;//stm32接收到openmv的PID算法得到speed*1000后的数据
uint16_t Tracking_speed;//寻迹速度
uint16_t start_speed = 0;//初始速度
uint8_t Usart1_RX_Buf;  //串口接收数据缓存buf


//----子函数声明-----
void UART_Proc();
int Vertical(float Angle,float Gyro_Y);			 				//直立环
int Velocity(int Encoder_left,int Encoder_right);				//速度环
int Turn(int datex,short yaw);	//转向环
u8 Stop(signed int angle);    		//倒下保护
void Limit(int *motoA,int *motoB);  //电机速度限幅
void Set_Pwm(int Moto1,int Moto2);	//控制PWM最终输出
int ComplementaryFiltering();
int ComplementaryFiltering2();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

//-----printf重定向-----
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
//静态方法去创建任务
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




/*****************   100HZ任务   **************************/

void Main_Task_Start(void const * argument)
{
  /* USER CODE BEGIN Main_Task_Start */
  /* Infinite loop */

	 HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//PWM
  	 HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);	
  
	 HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1);//编码器
	 HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_2);
	 HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_1);
	 HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_2);	 
    HAL_UART_Receive_IT(&huart1, &Usart1_RX_Buf, 1);//使能串口1接收中断
	 

	//------MPU6050测试-----

  //----iic读取MUP器件ID----
	/* 第1个参数为I2C操作句柄

第2个参数为从机设备地址

第3个参数为从机寄存器地址

第4个参数为从机寄存器地址长度

第5个参数为发送的数据的起始地址

第6个参数为传输数据的大小

第7个参数为操作超时时间 */
  uint8_t recv = 0x00;
  HAL_I2C_Mem_Read(&hi2c2, (0x68 << 1), 0x75, I2C_MEMADD_SIZE_8BIT, &recv, 1, 0xfff);//句柄，MUP地址,器件ID寄存器
  
  if (recv == 0x68)
  { 
  }
  else
  {
  }  	 
  
  //-----DMP初始化----
  while(mpu_dmp_init())//成功返回0，否则返回1
  {
		uint8_t res;
		res = mpu_dmp_init();
		HAL_Delay(300);		  
  }
 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0); //打开PC13 LED
 Start_Flag = 1; //标志系统初始化成功




	
  for(;;)	
  { 
	  
	  
		Vertical_out= Vertical(Pitch+0.1,gyroy+22);//直立环
	  
	   Velocity_out = Velocity(Encoder_Left,Encoder_Right);// 速度环
	  
		Turn_Pwm = Turn(date1,Yaw+1);
	  
		//最终输出
	   Moto1 = Vertical_out-Velocity_out+Turn_Pwm; // 左电机，此处与单平衡车输出不同(线性相加)
		Moto2 = Vertical_out-Velocity_out-Turn_Pwm; // 右电机 
	  
		Limit(&Moto1,&Moto2);     // PWM限幅 
		Set_Pwm(Moto1,Moto2);        // 加载到电机上	 		  

   	  	  
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




/*****************   200HZ任务   **************************/

void Encoder_Task_Start(void const * argument)
{
  /* USER CODE BEGIN Encoder_Task_Start */
  /* Infinite loop */
  for(;;)
  {
	  // 1.采集编码器数据&MPU6050角度信息
   // 电机是相对安装，刚好相差180度，为了编码器输出极性一致，就需要对其中一个取反  		  
	  Encoder_Left  =   Read_Encoder(3);//读取编码器测量的电机转速
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



/*****************   200HZ任务   **************************/

void MPU6050_Task_Start(void const * argument)
{
  /* USER CODE BEGIN MPU6050_Task_Start */
  /* Infinite loop */
  for(;;)
  {
		
		mpu_dmp_get_data(&Pitch,&Roll,&Yaw);	    // 读取角度
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);  // 读取角速度
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz); // 读取加速度  	 	  
    
	  
	  osDelay(5);
  }
  /* USER CODE END MPU6050_Task_Start */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */








/*************************************************************************** 
直立环PD控制器：Kp*Ek+Kd*Ek_D
入口：Med:机械中值(期望角度)，Angle:真实角度，Med_Angle：机械0点，gyro_Y:真实角速度
出口：PMW数值
******************************************************************************/
int Vertical(float Angle,float Gyro_Y) 
{
	float Vertical_Kp= -360,Vertical_Kd=-1.2;	//直立环Kp,Kd
	PWM_out = Vertical_Kp*(Angle-Med_Angle)+Vertical_Kd*(Gyro_Y-0);	//-5
	return PWM_out;	
} 




/********************************************************************* 
速度环PI控制器：Kp*Ek+Ki*Ek_S(Ek_S：偏差的积分)
入口：左右编码器测到的数值
出口：速度环输出pwm                 
**********************************************************************/
int Velocity(int Encoder_left,int Encoder_right)	
{
	// 定义成静态变量，保存在静态存储器，使得变量不丢掉
	static float  Velocity,Encoder_Err,Encoder,Encoder_last; 
	static float Encoder_Integral,Movement=10;	//Movement控制速度	
	float kp=37,ki=0.185;	 //速度环P I 37,0.185		
	
	// 1.计算速度偏差 	
	Encoder_Err = ((Encoder_Left+Encoder_Right)-0)*0.5;	
 
	// 2.对速度偏差进行--低通滤波--
  // low_out = (1-a)*Ek+a*low_out_last	
	Encoder = Encoder_Err*0.3 + Encoder_last*0.7;// 使得波形更加平滑,上次的速度占70%,本次的速度占30% 
	Encoder_last = Encoder; 							// 防止速度过大影响直立环的正常工作

	// 3.对速度偏差积分出位移,遥控的速度通过积分融入速度控制器，减缓速度突变对直立控制的影响
  Encoder_Integral += Encoder-Movement; //积分出位移 积分时间：10ms	


	// 4.积分限幅，寻迹效果需要降低积分，如果是单直立控制，10000左右
	if(Encoder_Integral>300)  	Encoder_Integral=300;   
	if(Encoder_Integral<-200)	   Encoder_Integral=-200;           	

	if(Moto_Flag == 1||Start_Flag ==0) 			Encoder_Integral=0;     		//===电机关闭后或者复位清除积分
    //5.速度环控制输出	
   Velocity=Encoder*kp+Encoder_Integral*ki;
	
//	speedControlPeriod++; 	
//	if(speedControlPeriod>10)//控制速度环平滑输出，采集10次才输出一次
//	{
//		speedControlPeriod = 0;
		return Velocity;

//	}
	
}


/*********************************************************************
转向环：控制循迹
入口：date1,yaw = z轴陀螺仪数值
**********************************************************************/
int Turn(int datex,short yaw)
{
   static float Bias,Last_Bias,date,Turn_Kp=15,Turn_Kd=0.18; 
   float Turn_PWM;	
			
	date=datex;
	if(date>=100) //左偏,左轮减速右轮加速，Turn_Pwm输出为+   Openmv端+100来处理负数
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
函数功能：控制电机
******************************************************************************/
void Contrl(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, AIN1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, AIN2);
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, BIN1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, BIN2);			
}




/**************************************************************************
函数功能：电机异常关闭函数
入口参数：角度
返回  值：1：关闭，0：不关闭
**************************************************************************/	 		
u8 Stop(signed int angle)
{
	    u8 temp=0;
			if(angle<-40||angle>40)
			{	                                //===倾角大于40度关闭电机
				temp=1;                   		   //===Flag_Stop置1关闭电机
		//		Moto1 = 0;
			//	Moto2 = 0;
      }
	
		return temp;
}



/**************************************************************************
函数功能：电机转动控制函数
入口参数：闭环控制最终输出值
**************************************************************************/	
void Set_Pwm(int Moto1,int Moto2)
{
	/*
	 Moto_Flag=Stop(Pitch+0.1);	//获取是否倒下的标志
	int   Dead_Zone=200;     //电机死区
	
	if(Start_Flag == 1)		//一级判断系统是否正常初始化
	{
		if(Moto_Flag==0)	//二级判断
		{
			if(Moto1>0)  AIN1 = 1,AIN2 = 0;		
			else			 AIN1 = 0,AIN2 = 1;							

			
			if(Moto2>0)  BIN1 = 1,BIN2 = 0;		
			else		    BIN1 = 0 ,BIN2 = 1;											
		   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,abs(Moto1)+Dead_Zone);
		   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,abs(Moto1)+Dead_Zone); //0-7200			
		
	  }			
		else//倒下就关闭电机
		{
		   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);	//4500-6000
		   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0);	//4500-6000
		}
		 Contrl();
	}
	*/
}


/**************************************************************************
函数功能：限制电机速度
入口参数：闭环控制最终输出值
**************************************************************************/	
void Limit(int *motoA,int *motoB)
{
	if(*motoA>7000)*motoA=7000;//最大7200
	if(*motoA<-7000)*motoA=-7000;

	if(*motoB>7000)*motoB=7000;
	if(*motoB<-7000)*motoB=-7000;
}




/**************************************************************************
函数功能：USART1接收中断回调 or 和openmv串口通信
引脚：		 TX_PA9,RX_PA10
**************************************************************************/
//串口中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{

		uint8_t i;
		static uint8_t RxCounter1=0;//数据缓冲区的索引
		static uint8_t RxBuffer1[5]={0};//存放数据的接收缓存区
		static uint8_t RxState = 0;	//接收标志位
			
		if(RxState==0&&Usart1_RX_Buf==0x2C)  //0x2c帧头
				{
					RxState=1;
					RxBuffer1[RxCounter1++]=Usart1_RX_Buf;//RxBuffer1[0]==0x2c RxCounter1==1
				}
		
				else if(RxState==1&&Usart1_RX_Buf==0x12)  //0x12帧头
				{
					RxState=2;
					RxBuffer1[RxCounter1++]=Usart1_RX_Buf;
				}
		
				else if(RxState==2)//开始接收有效数据
				{
					RxBuffer1[RxCounter1++]=Usart1_RX_Buf;//全部接收完，RxCounter1==7

					if(RxCounter1>=5||Usart1_RX_Buf == 0x5B)       //RxBuffer1接受满了,接收数据结束
					{
						  RxState=3;
						//RxBuffer1[2]是openmv发送的第一个数据的低八位,RxBuffer1[3]是openmv发送的第一个数据的高八位
						//date1=(RxBuffer1[RxCounter1-4]<<8)+(RxBuffer1[RxCounter1-5]);//分为低八位和高八位，先发低后发高
						//date2=(RxBuffer1[RxCounter1-2]<<8)+(RxBuffer1[RxCounter1-3]);
						date1=(RxBuffer1[RxCounter1-2]<<8)+(RxBuffer1[RxCounter1-3]);
//						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);						
						
					}
				}
		
				else if(RxState==3)		//检测是否接受到结束标志
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
						else   //接收错误
						{
									RxState = 0;
									RxCounter1=0;
									for(i=0;i<5;i++)
									{
											RxBuffer1[i]=0x00;      //将存放数据数组清零
											Tracking_Flag = 0;
									}
						}
				} 
	
				else   //接收异常
				{
						RxState = 0;
						RxCounter1=0;
						for(i=0;i<5;i++)
						{
								RxBuffer1[i]=0x00;      //将存放数据数组清零
								Tracking_Flag=0;
						}
				}

		


	HAL_UART_Receive_IT(&huart1,&Usart1_RX_Buf,1);

}



/* USER CODE END Application */

