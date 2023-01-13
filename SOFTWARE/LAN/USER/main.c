#include "ALL_DEFINE.h"

int main(void)
{	
	cycleCounterInit();  //得到系统每个us的系统CLK个数，为以后延时函数，和得到精准的当前执行时间使用
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //2个bit的抢占优先级，2个bit的子优先级
	SysTick_Config(SystemCoreClock / 1000);	//系统滴答时钟
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

	ALL_Init();//系统初始化

	
	while(1)
	{
	ANTO_polling(); 	//匿名上位机
	

	}
}










