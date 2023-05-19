#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_ms(__IO u32 nTime);
void SysTick_Delay_Ms( __IO uint32_t ms);

#endif  /* _BSP_LED_H */




