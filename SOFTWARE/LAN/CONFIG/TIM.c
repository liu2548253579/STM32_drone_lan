#include "stm32f10x.h"
#include "sys.h"
#include "TIM.h"

void TIM3_PWM_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;   //����GPIO�ṹ�����
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;   //���嶨ʱ���ṹ�����
	TIM_OCInitTypeDef TIM_OCInitStruct;   //��������ȽϽṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);   //GPIOA��B������ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);   //ʹ��TIM3��ʱ��
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;   //����GPIO��6��7����
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   	  //����GPIOΪ�����������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;  	  //����GPIO����
	GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO��ʼ������
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;   //����GPIO��0��1����
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   	  //����GPIOΪ�����������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   	  //����GPIO����
	GPIO_Init(GPIOB,&GPIO_InitStruct);   //GPIO��ʼ������
	
	TIM_TimeBaseInitStruct.TIM_Period=1000-1;   	  //�����Զ���װ�ص�����ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler=8;   	  //����Ԥ��Ƶֵ
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //����ʱ�ӷָ�
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);   		   //��ʱ����ʼ������
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;   //PWMģʽ1
	TIM_OCInitStruct.TIM_Pulse=0;   						   //��ʼ��ռ�ձ�Ϊ0
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;   	   //����Ƚϼ��Ե�
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   //�Ƚ����ʹ��
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);   					   //�Ƚ������ʼ������
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);   //����Ƚ�1Ԥװ�ؼĴ���ʹ��
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;   //PWMģʽ1
	TIM_OCInitStruct.TIM_Pulse=0;   					   		//��ʼ��ռ�ձ�Ϊ0
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;   		//����Ƚϼ��Ե�
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   	//�Ƚ����ʹ��
	TIM_OC2Init(TIM3,&TIM_OCInitStruct);   						//�Ƚ������ʼ������
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);   //����Ƚ�2Ԥװ�ؼĴ���ʹ��
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;   //PWMģʽ1
	TIM_OCInitStruct.TIM_Pulse=0;   							//��ʼ��ռ�ձ�Ϊ0
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;   		//����Ƚϼ��Ե�
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   	//�Ƚ����ʹ��
	TIM_OC3Init(TIM3,&TIM_OCInitStruct);   						//�Ƚ������ʼ������
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);   //����Ƚ�3Ԥװ�ؼĴ���ʹ��
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;   //PWMģʽ1
	TIM_OCInitStruct.TIM_Pulse=0;   							//��ʼ��ռ�ձ�Ϊ0
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;   		//����Ƚϼ��Ե�
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   	//�Ƚ����ʹ��
	TIM_OC4Init(TIM3,&TIM_OCInitStruct);   						//�Ƚ������ʼ������
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);   //����Ƚ�4Ԥװ�ؼĴ���ʹ��
	
	TIM_Cmd(TIM3,ENABLE);   //TIM3ʹ��
}
//{

//  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//  TIM_OCInitTypeDef TIM_OCInitStructure;
//  GPIO_InitTypeDef GPIO_InitStructure;
//  /* ʹ��GPIOAʱ��ʱ�� */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);	
//  /* ʹ�ܶ�ʱ��2ʱ�� */
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//  /* Time base configuration */
//  TIM_TimeBaseStructure.TIM_Period = 999; //��ʱ���������� 0-999  1000	
//  TIM_TimeBaseStructure.TIM_Prescaler = 8; //����Ԥ��Ƶ��8+1��Ƶ   8K PWMƵ��
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷ�Ƶϵ��������Ƶ
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ

//  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

//  /* PWM1 Mode configuration: Channel */
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //����ΪPWMģʽ1
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = 0;
//    //��������ֵ�������������������ֵʱ����ƽ��������(��ռ�ձ�) ��ʼֵ0
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//    //����ʱ������ֵС�ڶ�ʱ�趨ֵʱΪ�ߵ�ƽ
//  /* ʹ��ͨ��1 */
//  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
//  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
//  /* ʹ��ͨ��2 */
//  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
//  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
//  /* ʹ��ͨ��3 */
//  TIM_OC3Init(TIM2, &TIM_OCInitStructure);
//  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
//  /* ʹ��ͨ��4 */
//  TIM_OC4Init(TIM2, &TIM_OCInitStructure);
//  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

//  TIM_ARRPreloadConfig(TIM2, ENABLE); // ʹ��TIM2���ؼĴ���ARR
//  TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��2
//}



//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM2_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure; 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	TIM_TimeBaseStructure.TIM_Period = 299; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������300Ϊ3ms
	TIM_TimeBaseStructure.TIM_Prescaler =719; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  100Khz�ļ���Ƶ��  719
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM2, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);


	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
							 
}













