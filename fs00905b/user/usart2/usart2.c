#include "usart2.h"
#include "stdio.h"
#include "led.h"
#include "systick.h"


static unsigned char TxBuffer[256];
static unsigned char TxCounter=0;
static unsigned char count=0; 

extern void CopeSerial2Data(unsigned char ucData);

void USART2_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//GPIOA时钟

    //USART2_TX   GPIO A.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                           //PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	                    //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);                              //初始化GPIOA2

    //USART2_RX	  GPIO A3初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                          //PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;               //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                              //初始化GPIOA3 

    //USART2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;            //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		            //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			            //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	                                    //根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound;                         //串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;         //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;              //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                 //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                //收发模式

    USART_Init(USART2, &USART_InitStructure);                           //初始化串口1
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);                      //开启串口接受中断
    USART_Cmd(USART2, ENABLE);                                          //使能串口1 

}

void USART2_Put_Char(unsigned char DataToSend)
{
	TxBuffer[count++] = DataToSend;  
  USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  
}

void USART2_Put_String(unsigned char *Str)
{
	while(*Str)
	{
		if(*Str=='\r')USART2_Put_Char(0x0d);
			else if(*Str=='\n')USART2_Put_Char(0x0a);
				else USART2_Put_Char(*Str);
		Str++;
	}
}


void USART2_SendByte(USART_TypeDef * pUSARTx, uint8_t ch){
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*****************  发送字符串 **********************/
void USART2_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      USART2_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

////重定向c库函数printf到串口，重定向后可使用printf函数
//int fputc(int ch, FILE *f)
//{
//		/* 发送一个字节数据到串口 */
//		USART_SendData(USART2, (uint8_t) ch);
//		
//		/* 等待发送完毕 */
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		
//	
//		return (ch);
//}

////重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
//int fgetc(FILE *f)
//{
//		/* 等待串口输入数据 */
//		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(USART2);
//}

u16 ucRxBuffer[40];
u16 ucRxCnt=0;
u8 Flag=0;

void USART2_IRQHandler(void)
{
	u16 res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {

		res=USART2->DR;
		ucRxBuffer[ucRxCnt++]=res;
		if(ucRxCnt>40){
			Flag=TRUE;
			ucRxCnt=0;
		}
//		LED1_ON;
//		SysTick_Delay_Ms(10);
//		LED1_OFF;
//		ucRxBuffer[ucRxCnt++]=USART_ReceiveData(USART2);
//		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//		Flag=FALSE;
//		if(Flag!=FALSE)
//			USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//		else{
//			if(ucRxCnt==40){
//				ucRxCnt=0;
//				Flag=TRUE;
//			}
//			ucRxBuffer[ucRxCnt++]=USART_ReceiveData(USART2);
//			LED1_ON;
//			SysTick_Delay_Ms(10);
//			LED1_OFF;
//			if(ucRxBuffer[0]!=0x42){
//				ucRxCnt=0;
//				USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//			}
//			else{
//				if(ucRxBuffer[1]!=0x4d){
//					ucRxCnt=1;
//					USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//				}
//				else{
//					USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//				}
//			}
//		}
  }
//	if(USART_GetFlagStatus(USART2,USART_IT_IDLE)!=RESET){
//		if(ucRxBuffer[0]!=0x42&&ucRxBuffer[1]!=0x4d){
//			Flag=FALSE;
//			ucRxCnt=0;
//			USART_ClearITPendingBit(USART2,USART_IT_IDLE);	
//		}
//		else{
//			if(ucRxCnt==5){
//			Flag=TRUE;
//			ucRxCnt=0;
//			USART_ClearITPendingBit(USART2,USART_IT_IDLE);
//			}
//		}	
//	}
}









