#include "sys.h"
#include "ADC_DMA_Config.h"
#include "SPI.h"
#include "nrf24l01.h"
#include "TIM.h"
#include "eeprom.h"
#include "remote.h"
#include <string.h>
#include "usart.h"
#include "delay.h"
#include "oled.h"

uint32_t SysTick_count = 0;


void _g_Init_sys(void)
{

	
	
	NRF24L01_init();        //初始化NRF24L01
  USART1_Config();        //初始化串口1	
	delay_ms(100);	
	printf("\r\nstm32f103C8t6\r\n");//串口1打印单片机型号
	
	ADC1_GPIO_Config();      //初始化ADC IO	
	ADC1_Mode_Config();      //初始化ADC模式	

	RC_INIT();               //校准摇杆数据初始化
}




//通信协议：
//起始帧0XAA,0XAF + 地址帧 0X03 + 数据长度 + 12个遥控数据 + 校验字节


void SysTick_Handler(void)
{   
	static uint8_t Count_10ms;
	
	SysTick_count++;	
	
	if(Count_10ms++>=1)
	{
		Count_10ms = 0;
		RC_Analy();
	}
}




