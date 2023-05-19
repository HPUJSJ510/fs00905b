#include "systick.h"

__IO u32 TimingDelay;

void SysTick_Init(void){
	/* SystemCoreClock=160M */
	/* SystemCoreClock/1000          	1ms中断一次 */
	/* SystemCoreClock/1000000          1us中断一次 */
	
	if (SysTick_Config(SystemCoreClock/1000)){
		while(1);
	}
}

void Delay_ms(__IO u32 nTime){
	TimingDelay=nTime;
	while(TimingDelay!=0);
}

void SysTick_Delay_Ms( __IO uint32_t ms)
{
	uint32_t i;	
	SysTick_Config(SystemCoreClock/1000);
	
	for(i=0;i<ms;i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1
		// 当置1时，读取该位会清0
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// 关闭SysTick定时器
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
	if(TimingDelay!=0x00)
		TimingDelay--;
}








