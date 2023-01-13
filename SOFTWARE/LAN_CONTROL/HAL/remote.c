#include "stm32f10x.h" 
#include "remote.h"
#include "ADC_DMA_Config.h"
#include "sys.h"
#include "nrf24l01.h"
#include <math.h>
#include "eeprom.h"
#include "delay.h"
#include "usart.h"
__align(4) _st_Remote Remote={1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};


void key(void);
	
uint16_t styb;
struct //У׼����
{
	int16_t flag;		//У׼��־λ
	int16_t roll;
	int16_t pitch;	
	int16_t thr;	
	int16_t yaw;	
}offset ;
/******************************************************************************
����ԭ�ͣ�
��    �ܣ�	������ʼ��
*******************************************************************************/ 
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	AFIO->MAPR = 0X02000000;   //ʹ������SWD���س���
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);	

}
/******************************************************************************
����ԭ�ͣ�
��    �ܣ�	У׼���ݳ�ʼ��
*******************************************************************************/ 
void RC_INIT(void)
{

	offset.flag = *(int16_t*)FLASH_Start_Addr;   //�����洢������
	
	if(offset.flag != 0x0066) //����Ƿ���У׼���� �ı�
	{
		delay_ms(1000);
		offset.roll = 500- (uint16_t)(0.25f*ADC_ConvertedValue[0]);//����;
		offset.pitch = (uint16_t)(0.25f*ADC_ConvertedValue[1]) -500;//����
		
		offset.thr = (uint16_t)(0.25f*ADC_ConvertedValue[3]);//����;
		offset.yaw = 500- (uint16_t)(0.25f*ADC_ConvertedValue[2]);//ƫ��
		offset.flag = 0x0066;
    
		printf("offset thr: %d   offset pitch: %d   offset yaw: %d   offset roll: %d \r\n",offset.thr,offset.pitch,offset.yaw,offset.roll);  //����1���2��ҡ������		
		
		if(offset.thr <= 1)  //�ж����Ŵ���ͲŴ洢ҡ��ֵ  ��Ȼ�洢�˾ͽ������˷ɻ�
	  {
			FLASH_write((int16_t *)&offset,5);
		}
	}
	else
	{
			offset.roll = *((int16_t*)FLASH_Start_Addr+1);
			offset.pitch = *((int16_t*)FLASH_Start_Addr+2);
			offset.thr = *((int16_t*)FLASH_Start_Addr+3);
			offset.yaw = *((int16_t*)FLASH_Start_Addr+4);
		
		  
	}
	key_init();
}
/******************************************************************************
����ԭ�ͣ�
��    �ܣ�	10ms����һ�����ݣ�ÿ��ͨ�����Ǹ�λ��ǰ��λ�ں�
*******************************************************************************/ 
uint8_t tx_data[32] = {0xAA,0xAF,0x03,32-5};//����ͨ��Э��
void NRF_SEND(void)
{
	for(uint8_t i=4;i<26;i+=2)
	{
		tx_data[i] = *((uint8_t*)&Remote+i-3);	 //��λ��ǰ
		tx_data[31] += tx_data[i];
		tx_data[i+1] = *((uint8_t*)&Remote+i-4);	
		tx_data[31] += tx_data[i];
	}
	tx_data[31] = 0;
		for(uint8_t i=0;i<31;i++)  //У��λ
		{
			tx_data[31] +=  tx_data[i];
		}
	NRF24L01_TxPacket((uint8_t*)&tx_data); //����NRF��������
}
/******************************************************************************
����ԭ�ͣ�
��    �ܣ�	ң�����ݽ���   �����ų���
*******************************************************************************/ 
void RC_Analy(void) //10ms����һ��
{
	static uint16_t last_thr,last_roll,last_pitch,last_yaw;

	Remote.thr = (uint16_t)(0.25f*ADC_ConvertedValue[3])+1000 + offset.thr;//����
	last_thr = Remote.thr = Remote.thr*0.25f + 0.75f*last_thr;
	Remote.pitch 	= 	1000 + (uint16_t)(0.25f*ADC_ConvertedValue[1]) + offset.pitch;//
	last_pitch = Remote.pitch = Remote.pitch*0.25f + 0.75f*last_pitch;
	Remote.yaw 		= 	1000 + (uint16_t)(0.25f*ADC_ConvertedValue[2]) + offset.yaw;//����
			if(Remote.yaw>1500)		//�Ѵ���ת��ΪС��
		{
			if(Remote.yaw>1999)
			{
			  Remote.yaw=1005;
			}
			else 
			{
				Remote.yaw=Remote.yaw-1500;
				Remote.yaw=1500-Remote.yaw;
			}
		}
		else if(Remote.yaw<1500)	//��С��ת��Ϊ����
		{
			if(Remote.yaw<1010)
			{
				Remote.yaw=2000;
			}
			else
			{
			   Remote.yaw=1500-Remote.yaw;
			   Remote.yaw=1500+Remote.yaw;
			}	 
		}
	last_yaw = Remote.yaw = Remote.yaw*0.25f + 0.75f*last_yaw;
	Remote.roll 	=	1000 +(uint16_t)(0.25f*ADC_ConvertedValue[0]) + offset.roll;//����
		if(Remote.roll>1500)		//�Ѵ���ת��ΪС��
		{
			if(Remote.roll>1999)
			{
			  Remote.roll=1005;
			}
			else 
			{
				Remote.roll=Remote.roll-1500;
				Remote.roll=1500-Remote.roll;
			}
		}
		else if(Remote.roll<1500)	//��С��ת��Ϊ����
		{
			if(Remote.roll<1010)
			{
				Remote.roll=2000;
			}
			else
			{
			   Remote.roll=1500-Remote.roll;
			   Remote.roll=1500+Remote.roll;
			}	 
		}
	last_roll = Remote.roll = Remote.roll*0.25f + 0.75f*last_roll;
	styb++;
	if(styb >= 200)
	{	
		printf("ǰ��: %d   ����: %d   ����: %d   ����: %d \r\n",Remote.pitch,Remote.roll,Remote.yaw,Remote.thr);  //����1���2��ҡ������
		styb=0;
	}
	key();//ɨ�谴��
	NRF_SEND();
}


/******************************************************************************
����ԭ�ͣ�
��    �ܣ�	����ɨ��
*******************************************************************************/ 
void key(void)
{
	#define KEY_LEFT GPIO_Pin_3//GPIOC
	#define KEY_ARIGHT GPIO_Pin_4//GPIOC
	#define KEY_ALEFT GPIO_Pin_5 //GPIOA
	#define KEY_RIGHT GPIO_Pin_8 //GPIOB
  volatile static uint8_t status = 0;	
 	static uint32_t temp;
	switch(status)
	{
		case 0:
			if(SysTick_count - temp >20) //200ms ������
			{
				if(
					((GPIOB->IDR & (GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8)) == (GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8))					
					)
					status = 1;
				BEEP_H;
			  //GPIO_SetBits(GPIOB,GPIO_Pin_10);
				GPIO_ResetBits(GPIOB,GPIO_Pin_10);
			}
			break;
		case 1:
			  if(
					((GPIOB->IDR & (GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8)) != (GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8))					
					)
				status = 2;
			break;
		case 2:	

		      if(!(GPIOB->IDR & KEY_ARIGHT))
			{
//					offset.roll=offset.roll-5;
			}
			else if(!(GPIOB->IDR & KEY_ALEFT))
			{
//					Remote.AUX7 ^= (2000^1000); //1000��2000֮��仯	  ��������һ���շ�
			}
			else if(!(GPIOB->IDR & KEY_LEFT))
			{

					offset.pitch=offset.pitch-5;	
			}

			else if(!(GPIOB->IDR & KEY_RIGHT))
			{
					offset.pitch=offset.pitch+5;
			}	
			status = 0;			
			BEEP_L;  
			//GPIO_ResetBits(GPIOB,GPIO_Pin_10);
			GPIO_SetBits(GPIOB,GPIO_Pin_10);
			temp = SysTick_count;
			break;
		}			
}



