#include "stm32f10x.h"
#include "sys.h"
#include "stdio.h"
#include "delay.h"
#include "oled.h"
#include "usart.h"

//配置系统时钟,使能各外设时钟
void RCC_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA 
                           |RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
						   |RCC_APB2Periph_ADC1  | RCC_APB2Periph_AFIO 
                           |RCC_APB2Periph_SPI1  
						   	, ENABLE );
	
   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 | RCC_APB1Periph_I2C1| RCC_APB1Periph_TIM3, ENABLE );
	
}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* 使能串口1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	//USART1  串口1全局中断 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //子优先级1
	/*IRQ通道使能*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/*根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1*/
	NVIC_Init(&NVIC_InitStructure);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

void BEEP_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //使能GPIO的时钟  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;          //蜂鸣器
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	BEEP_L;
}

int main(void)
{
	RCC_Configuration();	
	cycleCounterInit();
	OLED_Init();			//初始化OLED 	
  OLED_ShowString(50,0,"LAN",16);  
	OLED_ShowString(0,20, "0.96' OLED TEST",16);  
 	OLED_ShowString(0,35,"6+7 2021/12/6",12);  
 	OLED_ShowString(0,52,"ASCII:",12);  
 	OLED_ShowString(64,52,"CODE:",12); 
	OLED_Refresh_Gram();		//更新显示到OLED 
	
	BEEP_INIT(); //开机蜂鸣声
	{
		int32_t beep_cnt = 400000;
		while(beep_cnt-->0)
		{
			
		}
		BEEP_H;
	}
	
	SysTick_Config(SystemCoreClock / 1000);	//系统滴答时钟 1ms发生一次中断
	
	
	_g_Init_sys(); //15ms中断处理，详情请见SYS.C
	
  
	NVIC_Configuration();

	while(1)
	{
		
BEEP_L;
delay_ms(200);
BEEP_H;
delay_ms(200);		
	}
}










