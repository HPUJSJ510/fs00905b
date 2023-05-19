#ifndef _USART2_h_
#define _USART2_h_

#include "stm32f10x.h"
#include "stdio.h"

#define TRUE  1
#define FALSE 0

extern uint8_t USART2_RX_BUF[50];

void USART2_init(u32 bound);
void USART2_Put_Char(unsigned char DataToSend);
void USART2_Put_String(unsigned char *Str);
void USART2_SendByte(USART_TypeDef * pUSARTx, uint8_t ch);
void USART2_SendString( USART_TypeDef * pUSARTx, char *str);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
void usart2_IRQHandler(void);





#endif





