#include "ALL_DEFINE.h"

int main(void)
{	
	cycleCounterInit();  //�õ�ϵͳÿ��us��ϵͳCLK������Ϊ�Ժ���ʱ�������͵õ���׼�ĵ�ǰִ��ʱ��ʹ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //2��bit����ռ���ȼ���2��bit�������ȼ�
	SysTick_Config(SystemCoreClock / 1000);	//ϵͳ�δ�ʱ��
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

	ALL_Init();//ϵͳ��ʼ��

	
	while(1)
	{
	ANTO_polling(); 	//������λ��
	

	}
}










