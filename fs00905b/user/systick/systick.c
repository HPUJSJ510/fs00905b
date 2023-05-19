#include "systick.h"

__IO u32 TimingDelay;

void SysTick_Init(void){
	/* SystemCoreClock=160M */
	/* SystemCoreClock/1000          	1ms�ж�һ�� */
	/* SystemCoreClock/1000000          1us�ж�һ�� */
	
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
		// ����������ֵ��С��0��ʱ��CRTL�Ĵ�����λ16����1
		// ����1ʱ����ȡ��λ����0
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// �ر�SysTick��ʱ��
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
	if(TimingDelay!=0x00)
		TimingDelay--;
}








