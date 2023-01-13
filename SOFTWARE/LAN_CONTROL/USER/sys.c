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

	
	
	NRF24L01_init();        //��ʼ��NRF24L01
  USART1_Config();        //��ʼ������1	
	delay_ms(100);	
	printf("\r\nstm32f103C8t6\r\n");//����1��ӡ��Ƭ���ͺ�
	
	ADC1_GPIO_Config();      //��ʼ��ADC IO	
	ADC1_Mode_Config();      //��ʼ��ADCģʽ	

	RC_INIT();               //У׼ҡ�����ݳ�ʼ��
}




//ͨ��Э�飺
//��ʼ֡0XAA,0XAF + ��ַ֡ 0X03 + ���ݳ��� + 12��ң������ + У���ֽ�


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




