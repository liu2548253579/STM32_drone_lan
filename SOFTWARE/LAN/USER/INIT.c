#include "ALL_DEFINE.h"


volatile uint32_t SysTick_count; //ϵͳʱ�����
_st_Mpu MPU6050;   //MPU6050ԭʼ����
_st_Mag AK8975;
_st_AngE Angle;    //��ǰ�Ƕ���ֵ̬
_st_Remote Remote; //ң��ͨ��ֵ


_st_ALL_flag ALL_flag; //ϵͳ��־λ������������־λ��


PidObject pidRateX; //�ڻ�PID����
PidObject pidRateY;
PidObject pidRateZ;

PidObject pidPitch; //�⻷PID����
PidObject pidRoll;
PidObject pidYaw;

PidObject pidHeightRate;
PidObject pidHeightHigh;

void pid_param_Init(void); //PID���Ʋ�����ʼ������дPID�����ᱣ�����ݣ��������ɺ�ֱ���ڳ�������� ����¼���ɿ�


void ALL_Init(void)
{
	LED_Init();	          //RGB�Ƴ�ʼ��

  BATT_Init();         //���ADC�ɼ���ѹ

	IIC_Init();             //I2C��ʼ��

	spl06_init();	  	

  Init_Altitude();		
	
	IIC_Init();   
	
	pid_param_Init();       //PID������ʼ��

	MpuInit();              //MPU6050��ʼ��
	
  USART_CONFIG(500000);     //��λ�����ڳ�ʼ��

	NRF24L01_init();				//2.4Gң��ͨ�ų�ʼ��
	
	TIM3_PWM_Config();			//4·PWM��ʼ��
	
	TIM2_Config();					//ϵͳ�������ڳ�ʼ�
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










