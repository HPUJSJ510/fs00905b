#ifndef _USART1_h_
#define _USART1_h_



#include "stm32f10x.h"
#include "stdio.h"

void usart1_init(u32 bound);
void usart1_Put_Char(unsigned char DataToSend);
void usart1_Put_String(unsigned char *Str);
void Usart1_SendByte(USART_TypeDef * pUSARTx, uint8_t ch);
void Usart1_SendString( USART_TypeDef * pUSARTx, char *str);
int fputc1(int ch, FILE *f);
int fgetc1(FILE *f);
void USART1_IRQHandler(void);











#endif











