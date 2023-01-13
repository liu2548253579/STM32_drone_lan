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
struct //校准数据
{
	int16_t flag;		//校准标志位
	int16_t roll;
	int16_t pitch;	
	int16_t thr;	
	int16_t yaw;	
}offset ;
/******************************************************************************
函数原型：
功    能：	按键初始化
*******************************************************************************/ 
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	AFIO->MAPR = 0X02000000;   //使用四线SWD下载程序
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);	

}
/******************************************************************************
函数原型：
功    能：	校准数据初始化
*******************************************************************************/ 
void RC_INIT(void)
{

	offset.flag = *(int16_t*)FLASH_Start_Addr;   //读出存储的数据
	
	if(offset.flag != 0x0066) //检查是否有校准数据 改变
	{
		delay_ms(1000);
		offset.roll = 500- (uint16_t)(0.25f*ADC_ConvertedValue[0]);//方向;
		offset.pitch = (uint16_t)(0.25f*ADC_ConvertedValue[1]) -500;//方向
		
		offset.thr = (uint16_t)(0.25f*ADC_ConvertedValue[3]);//油门;
		offset.yaw = 500- (uint16_t)(0.25f*ADC_ConvertedValue[2]);//偏航
		offset.flag = 0x0066;
    
		printf("offset thr: %d   offset pitch: %d   offset yaw: %d   offset roll: %d \r\n",offset.thr,offset.pitch,offset.yaw,offset.roll);  //串口1输出2个摇杆数据		
		
		if(offset.thr <= 1)  //判断油门打最低才存储摇杆值  不然存储了就解锁不了飞机
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
函数原型：
功    能：	10ms发送一次数据，每个通道都是高位在前低位在后
*******************************************************************************/ 
uint8_t tx_data[32] = {0xAA,0xAF,0x03,32-5};//匿名通信协议
void NRF_SEND(void)
{
	for(uint8_t i=4;i<26;i+=2)
	{
		tx_data[i] = *((uint8_t*)&Remote+i-3);	 //高位在前
		tx_data[31] += tx_data[i];
		tx_data[i+1] = *((uint8_t*)&Remote+i-4);	
		tx_data[31] += tx_data[i];
	}
	tx_data[31] = 0;
		for(uint8_t i=0;i<31;i++)  //校验位
		{
			tx_data[31] +=  tx_data[i];
		}
	NRF24L01_TxPacket((uint8_t*)&tx_data); //调用NRF发射数据
}
/******************************************************************************
函数原型：
功    能：	遥控数据解析   左油门程序
*******************************************************************************/ 
void RC_Analy(void) //10ms调用一次
{
	static uint16_t last_thr,last_roll,last_pitch,last_yaw;

	Remote.thr = (uint16_t)(0.25f*ADC_ConvertedValue[3])+1000 + offset.thr;//油门
	last_thr = Remote.thr = Remote.thr*0.25f + 0.75f*last_thr;
	Remote.pitch 	= 	1000 + (uint16_t)(0.25f*ADC_ConvertedValue[1]) + offset.pitch;//
	last_pitch = Remote.pitch = Remote.pitch*0.25f + 0.75f*last_pitch;
	Remote.yaw 		= 	1000 + (uint16_t)(0.25f*ADC_ConvertedValue[2]) + offset.yaw;//方向
			if(Remote.yaw>1500)		//把大数转换为小数
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
		else if(Remote.yaw<1500)	//把小数转换为大数
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
	Remote.roll 	=	1000 +(uint16_t)(0.25f*ADC_ConvertedValue[0]) + offset.roll;//副翼
		if(Remote.roll>1500)		//把大数转换为小数
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
		else if(Remote.roll<1500)	//把小数转换为大数
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
		printf("前后: %d   左右: %d   航向: %d   油门: %d \r\n",Remote.pitch,Remote.roll,Remote.yaw,Remote.thr);  //串口1输出2个摇杆数据
		styb=0;
	}
	key();//扫描按键
	NRF_SEND();
}


/******************************************************************************
函数原型：
功    能：	按键扫描
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
			if(SysTick_count - temp >20) //200ms 按键音
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
//					Remote.AUX7 ^= (2000^1000); //1000和2000之间变化	  可以留来一键空翻
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



