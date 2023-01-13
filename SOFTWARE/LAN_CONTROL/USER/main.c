#include "stm32f10x.h"
#include "sys.h"
#include "stdio.h"
#include "delay.h"
#include "oled.h"
#include "usart.h"

//����ϵͳʱ��,ʹ�ܸ�����ʱ��
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
	
	/* ʹ�ܴ���1�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	//USART1  ����1ȫ���ж� 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�1
	/*IRQͨ��ʹ��*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/*����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1*/
	NVIC_Init(&NVIC_InitStructure);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

void BEEP_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //ʹ��GPIO��ʱ��  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;          //������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	BEEP_L;
}

int main(void)
{
	RCC_Configuration();	
	cycleCounterInit();
	OLED_Init();			//��ʼ��OLED 	
  OLED_ShowString(50,0,"LAN",16);  
	OLED_ShowString(0,20, "0.96' OLED TEST",16);  
 	OLED_ShowString(0,35,"6+7 2021/12/6",12);  
 	OLED_ShowString(0,52,"ASCII:",12);  
 	OLED_ShowString(64,52,"CODE:",12); 
	OLED_Refresh_Gram();		//������ʾ��OLED 
	
	BEEP_INIT(); //����������
	{
		int32_t beep_cnt = 400000;
		while(beep_cnt-->0)
		{
			
		}
		BEEP_H;
	}
	
	SysTick_Config(SystemCoreClock / 1000);	//ϵͳ�δ�ʱ�� 1ms����һ���ж�
	
	
	_g_Init_sys(); //15ms�жϴ����������SYS.C
	
  
	NVIC_Configuration();

	while(1)
	{
		
BEEP_L;
delay_ms(200);
BEEP_H;
delay_ms(200);		
	}
}










