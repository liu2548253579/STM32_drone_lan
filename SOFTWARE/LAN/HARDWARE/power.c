#include "ALL_DEFINE.h"
#include "power.h"
#include "ALL_DATA.h"
//����4.17V

void BATT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef   ADC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE); 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//ģ������ģʽѡ��       
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN; //ģ������
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;						//����ģʽ
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;					//�����Ҷ���
	ADC_InitStruct.ADC_NbrOfChannel=1;									//1������ͨ��
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;							//ɨ��ת��ģʽʧ��
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;		//�ⲿ����ʧ��
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;						//����ת��ʧ��
	ADC_Init(ADC1,&ADC_InitStruct);
	
	ADC_Cmd(ADC1, ENABLE); //ʹ��ADC1
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_239Cycles5);	//������ת��ͨ��
}


/******************************************************************************************
* ��  ����uint16_t Get_BatteryAdc(uint8_t ch)
* ��  �ܣ���ȡ��ز������ѹ��ADCֵ
* ��  ����ch  ADC����ͨ��
* ����ֵ������ͨ��ADֵ
* ��  ע����ص�ѹ�������ADCֵ����ص�ѹ������·��ԭ��ͼ
*******************************************************************************************/
uint16_t Get_BatteryAdc(uint8_t ch)
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);	//�������ת��ʹ��
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));	//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);	//����ת�������ֵ
}	

/******************************************************************************************
* ��  ����void BATT_GetVoltage(void)
* ��  �ܣ���ȡ��ص�ѹ
* ��  ������
* ����ֵ����
* ��  ע����ص�ѹ = ADC����ѹ*2 ����ԭ��ɿ�ԭ��ͼ
*******************************************************************************************/
float BATT_GetVoltage(void)
{
	float V,vol;
	float BAT;
	Aver_Filter_BAT((float)Get_BatteryAdc(ADC_Channel_1),&BAT,6); //�����˲���ѹֵ����߾���
	if(BAT)
	V = BAT * 3.38f / 4095.0f;
//		printf("Test Voltage :%0.2f   temp:%0.0f \r\n ",2*V,BAT_ADC);
	vol=V*2;
	return vol; //����ԭ������ѹ����֪ ���ʵ�ʵ�ѹ = ADC������ѹ * 2
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


