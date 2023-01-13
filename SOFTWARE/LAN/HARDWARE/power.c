#include "ALL_DEFINE.h"
#include "power.h"
#include "ALL_DATA.h"
//满电4.17V

void BATT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef   ADC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE); 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//模拟输入模式选择       
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN; //模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;						//独立模式
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;					//数据右对齐
	ADC_InitStruct.ADC_NbrOfChannel=1;									//1个数据通道
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;							//扫描转换模式失能
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;		//外部触发失能
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;						//连续转换失能
	ADC_Init(ADC1,&ADC_InitStruct);
	
	ADC_Cmd(ADC1, ENABLE); //使能ADC1
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_239Cycles5);	//规则组转换通道
}


/******************************************************************************************
* 函  数：uint16_t Get_BatteryAdc(uint8_t ch)
* 功  能：获取电池采样点电压的ADC值
* 参  数：ch  ADC采样通道
* 返回值：返回通道AD值
* 备  注：电池电压采样点的ADC值，电池电压采样电路见原理图
*******************************************************************************************/
uint16_t Get_BatteryAdc(uint8_t ch)
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);	//软件触发转换使能
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));	//等待转换结束
	return ADC_GetConversionValue(ADC1);	//返回转换结果的值
}	

/******************************************************************************************
* 函  数：void BATT_GetVoltage(void)
* 功  能：获取电池电压
* 参  数：无
* 返回值：无
* 备  注：电池电压 = ADC检测电压*2 具体原理可看原理图
*******************************************************************************************/
float BATT_GetVoltage(void)
{
	float V,vol;
	float BAT;
	Aver_Filter_BAT((float)Get_BatteryAdc(ADC_Channel_1),&BAT,6); //滑动滤波电压值，提高精度
	if(BAT)
	V = BAT * 3.38f / 4095.0f;
//		printf("Test Voltage :%0.2f   temp:%0.0f \r\n ",2*V,BAT_ADC);
	vol=V*2;
	return vol; //根据原理电阻分压，可知 电池实际电压 = ADC侧量电压 * 2
}


void LOW_VOL_Alarm (float vol)
{
	static uint8_t flag = 1;

if(BATT_GetVoltage()<vol)	
{
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	
	
		if(flag)
	{
		flag = 0;
			RGB_LED_Red();
	}
	else
	{
		flag = 1;
			RGB_LED_Off();
	}	
}
	
}


