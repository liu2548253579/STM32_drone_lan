#include "stm32f10x.h"
#include "sys.h"
#include "TIM.h"

void TIM3_PWM_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;   //定义GPIO结构体变量
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;   //定义定时器结构体变量
	TIM_OCInitTypeDef TIM_OCInitStruct;   //定义输出比较结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);   //GPIOA、B、复用时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);   //使能TIM3的时钟
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;   //配置GPIO第6、7引脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   	  //配置GPIO为复用推挽输出
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;  	  //配置GPIO速率
	GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO初始化函数
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;   //配置GPIO第0、1引脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   	  //配置GPIO为复用推挽输出
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   	  //配置GPIO速率
	GPIO_Init(GPIOB,&GPIO_InitStruct);   //GPIO初始化函数
	
	TIM_TimeBaseInitStruct.TIM_Period=1000-1;   	  //设置自动重装载的周期值
	TIM_TimeBaseInitStruct.TIM_Prescaler=8;   	  //设置预分频值
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //设置时钟分割
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);   		   //定时器初始化函数
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;   //PWM模式1
	TIM_OCInitStruct.TIM_Pulse=0;   						   //初始化占空比为0
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;   	   //输出比较极性低
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   //比较输出使能
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);   					   //比较输出初始化函数
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);   //输出比较1预装载寄存器使能
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;   //PWM模式1
	TIM_OCInitStruct.TIM_Pulse=0;   					   		//初始化占空比为0
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;   		//输出比较极性低
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   	//比较输出使能
	TIM_OC2Init(TIM3,&TIM_OCInitStruct);   						//比较输出初始化函数
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);   //输出比较2预装载寄存器使能
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;   //PWM模式1
	TIM_OCInitStruct.TIM_Pulse=0;   							//初始化占空比为0
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;   		//输出比较极性低
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   	//比较输出使能
	TIM_OC3Init(TIM3,&TIM_OCInitStruct);   						//比较输出初始化函数
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);   //输出比较3预装载寄存器使能
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;   //PWM模式1
	TIM_OCInitStruct.TIM_Pulse=0;   							//初始化占空比为0
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;   		//输出比较极性低
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   	//比较输出使能
	TIM_OC4Init(TIM3,&TIM_OCInitStruct);   						//比较输出初始化函数
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);   //输出比较4预装载寄存器使能
	
	TIM_Cmd(TIM3,ENABLE);   //TIM3使能
}
//{

//  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//  TIM_OCInitTypeDef TIM_OCInitStructure;
//  GPIO_InitTypeDef GPIO_InitStructure;
//  /* 使能GPIOA时钟时钟 */
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
//  /* 使能定时器2时钟 */
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//  /* Time base configuration */
//  TIM_TimeBaseStructure.TIM_Period = 999; //定时器计数周期 0-999  1000	
//  TIM_TimeBaseStructure.TIM_Prescaler = 8; //设置预分频：8+1分频   8K PWM频率
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分频系数：不分频
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式

//  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

//  /* PWM1 Mode configuration: Channel */
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //配置为PWM模式1
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = 0;
//    //设置跳变值，当计数器计数到这个值时，电平发生跳变(即占空比) 初始值0
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//    //当定时器计数值小于定时设定值时为高电平
//  /* 使能通道1 */
//  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
//  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
//  /* 使能通道2 */
//  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
//  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
//  /* 使能通道3 */
//  TIM_OC3Init(TIM2, &TIM_OCInitStructure);
//  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
//  /* 使能通道4 */
//  TIM_OC4Init(TIM2, &TIM_OCInitStructure);
//  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

//  TIM_ARRPreloadConfig(TIM2, ENABLE); // 使能TIM2重载寄存器ARR
//  TIM_Cmd(TIM2, ENABLE); //使能定时器2
//}



//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM2_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure; 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	TIM_TimeBaseStructure.TIM_Period = 299; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到300为3ms
	TIM_TimeBaseStructure.TIM_Prescaler =719; //设置用来作为TIMx时钟频率除数的预分频值  100Khz的计数频率  719
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);


	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
							 
}













