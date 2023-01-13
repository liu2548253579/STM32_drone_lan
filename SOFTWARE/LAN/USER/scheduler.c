#include "ALL_DEFINE.h"
#include "ALL_DATA.h"
#include "scheduler.h"


void TIM2_IRQHandler(void)   //TIM3�ж� 3ms
{
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		static uint8_t cnt_3ms = 0;
		static uint8_t cnt_6ms = 0;	 
		static uint8_t cnt_60ms = 0;	 
		SysTick_count++;
		cnt_3ms++;
		cnt_6ms++;
		cnt_60ms++;
		
		if(cnt_3ms == 1)  //3ms����һ��
		{
			cnt_3ms = 0;	
		  IIC_Init();	
			RC_Analy();	    //����	
			MpuGetData();
			FlightPidControl(0.003f);
			MotorControl();
			GetAngle(&MPU6050,&Angle,0.00313f);						
		}		
		if(cnt_6ms == 2) //6ms����һ��
		{
			cnt_6ms = 0;			
			BATT_GetVoltage();
		}		

		if(cnt_60ms == 20)  //60ms����һ�� (LED�ƹ����)
		{
			cnt_60ms = 0;		
			Waiting_Unolck_LED ();
		  LOW_VOL_Alarm (2.8); //С��2.8V��������	
			
//			 IIC_Init();	
//			printf("gao:%.2fm\r\n",Get_Absolute_Altitude());	
//			if(Get_Absolute_Altitude()>=1.6)
//			{
//RGB_LED_Blue();
//			}
//    else
//      {
//RGB_LED_Violet();
//      }	
			
		}			
			
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	}
}


