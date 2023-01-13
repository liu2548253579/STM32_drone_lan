#include "ALL_DEFINE.h"


volatile uint32_t SysTick_count; //ÏµÍ³Ê±¼ä¼ÆÊý
_st_Mpu MPU6050;   //MPU6050Ô­Ê¼Êý¾Ý
_st_Mag AK8975;
_st_AngE Angle;    //µ±Ç°½Ç¶È×ËÌ¬Öµ
_st_Remote Remote; //Ò£¿ØÍ¨µÀÖµ


_st_ALL_flag ALL_flag; //ÏµÍ³±êÖ¾Î»£¬°üº¬½âËø±êÖ¾Î»µÈ


PidObject pidRateX; //ÄÚ»·PIDÊý¾Ý
PidObject pidRateY;
PidObject pidRateZ;

PidObject pidPitch; //Íâ»·PIDÊý¾Ý
PidObject pidRoll;
PidObject pidYaw;

PidObject pidHeightRate;
PidObject pidHeightHigh;

void pid_param_Init(void); //PID¿ØÖÆ²ÎÊý³õÊ¼»¯£¬¸ÄÐ´PID²¢²»»á±£´æÊý¾Ý£¬Çëµ÷ÊÔÍê³ÉºóÖ±½ÓÔÚ³ÌÐòÀï¸ü¸Ä ÔÙÉÕÂ¼µ½·É¿Ø


void ALL_Init(void)
{
	LED_Init();	          //RGBµÆ³õÊ¼»¯

  BATT_Init();         //µç³ØADC²É¼¯µçÑ¹

	IIC_Init();             //I2C³õÊ¼»¯

	spl06_init();	  	

  Init_Altitude();		
	
	IIC_Init();   
	
	pid_param_Init();       //PID²ÎÊý³õÊ¼»¯

	MpuInit();              //MPU6050³õÊ¼»¯
	
  USART_CONFIG(500000);     //ÉÏÎ»»ú´®¿Ú³õÊ¼»¯

	NRF24L01_init();				//2.4GÒ£¿ØÍ¨ÐÅ³õÊ¼»¯
	
	TIM3_PWM_Config();			//4Â·PWM³õÊ¼»¯
	
	TIM2_Config();					//ÏµÍ³¹¤×÷ÖÜÆÚ³õÊ¼»
}



void pid_param_Init(void)
{
	
///////////////////////////////////////////////	
	pidRateX.kp = 1.7f;
	pidRateY.kp = 1.7f;
	pidRateZ.kp = 3.0f;
	
	pidRateX.ki = 0.0f;
	pidRateY.ki = 0.0f;
	pidRateZ.ki = 0.0f;	
	
	pidRateX.kd = 0.08f;
	pidRateY.kd = 0.08f;
	pidRateZ.kd = 0.5f;	
	
	pidPitch.kp = 7.0f;
	pidRoll.kp = 7.0f;
	pidYaw.kp = 7.0f;	
////////////////////////////////////////////////////
	
//	pidRateX.kp = 2.0f;
//	pidRateY.kp = 2.0f;
//	pidRateZ.kp = 4.0f;
//	
//	pidRateX.ki = 0.0f;
//	pidRateY.ki = 0.0f;
//	pidRateZ.ki = 0.0f;	
//	
//	pidRateX.kd = 0.28f;
//	pidRateY.kd = 0.28f;
//	pidRateZ.kd = 0.4f;	
//	
//	pidPitch.kp = 7.0f;
//	pidRoll.kp = 7.0f;
//	pidYaw.kp = 7.0f;	

}










