#include "usart.h"
#include "misc.h"
#include "stdio.h"


/*
 * 函数名：USART1_Config
 * 描述  ：USART1 GPIO 配置,工作模式配置。
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */

 
void USART3_Config(int btl)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //??USART3??
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);	

	
	/* 使能串口1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	//USART3  串口3全局中断 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //子优先级1
	/*IRQ通道使能*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/*根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1*/
	NVIC_Init(&NVIC_InitStructure);
	
	

	/*串口GPIO端口配置*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate =btl;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
	
	USART_Cmd(USART3, ENABLE); //使能串口 
}


void USART1_Config(int btl)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);   //使能GPIOA、USART1的时钟
	
	/* 使能串口1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	//USART3  串口3全局中断 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //子优先级1
	/*IRQ通道使能*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/*根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1*/
	NVIC_Init(&NVIC_InitStructure);

	/*串口GPIO端口配置*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate =btl;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	
	USART_Cmd(USART1, ENABLE); //使能串口 
}

void USART_CONFIG(int btl)
{
if(SEND_PORT==USART1)  USART1_Config(btl);

if(SEND_PORT==USART3)  USART3_Config(btl);
}

void USART_SendByte(const int8_t *Data,uint8_t len)
{ 
	uint8_t i;
	
	for(i=0;i<len;i++)
	{
		while (!(SEND_PORT->SR & USART_FLAG_TXE));	
		USART_SendData(SEND_PORT,*(Data+i));	 
	}
}


int8_t CheckSend[7]={0xAA,0XAA,0xEF,2,0,0,0};

void USART1_setBaudRate(uint32_t baudRate)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate =  baudRate;
	USART_Init(USART1, &USART_InitStructure);
}


/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到PRINTF_PORT
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
	/* 将Printf内容发往串口 */
	USART_SendData(SEND_PORT, (unsigned char) ch);
	while( USART_GetFlagStatus(SEND_PORT,USART_FLAG_TC)!= SET);	
	return (ch);
}

