#include "ALL_DEFINE.h"
#include "ALL_DATA.h"
#include "scheduler.h"


void TIM2_IRQHandler(void)   //TIM3中断 3ms
{
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		static uint8_t cnt_3ms = 0;
		static uint8_t cnt_6ms = 0;	 
		static uint8_t cnt_60ms = 0;	 
		SysTick_count++;
		cnt_3ms++;
		cnt_6ms++;
		cnt_60ms++;
		
		if(cnt_3ms == 1)  //3ms更新一次
		{
			cnt_3ms = 0;	
		  IIC_Init();	
			RC_Analy();	    //主控	
			MpuGetData();
			FlightPidControl(0.003f);
			MotorControl();
			GetAngle(&MPU6050,&Angle,0.00313f);						
		}		
		if(cnt_6ms == 2) //6ms更新一次
		{
			cnt_6ms = 0;			
			BATT_GetVoltage();
		}		

		if(cnt_60ms == 20)  //60ms更新一次 (LED灯光控制)
		{
			cnt_60ms = 0;		
			Waiting_Unolck_LED ();
		  LOW_VOL_Alarm (2.8); //小于2.8V开启警报	
			
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
			
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update );  //清除TIMx的中断待处理位:TIM 中断源 
	}
}


