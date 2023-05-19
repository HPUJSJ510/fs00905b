#include "usart2.h"
#include "stdio.h"
#include "led.h"
#include "systick.h"


static unsigned char TxBuffer[256];
static unsigned char TxCounter=0;
static unsigned char count=0; 

extern void CopeSerial2Data(unsigned char ucData);

void USART2_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//GPIOAʱ��

    //USART2_TX   GPIO A.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                           //PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	                    //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                              //��ʼ��GPIOA2

    //USART2_RX	  GPIO A3��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                          //PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;               //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                              //��ʼ��GPIOA3 

    //USART2 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;            //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		            //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			            //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	                                    //����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;                         //���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;         //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;              //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                 //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                //�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure);                           //��ʼ������1
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);                      //�������ڽ����ж�
    USART_Cmd(USART2, ENABLE);                                          //ʹ�ܴ���1 

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
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*****************  �����ַ��� **********************/
void USART2_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      USART2_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

////�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
//int fputc(int ch, FILE *f)
//{
//		/* ����һ���ֽ����ݵ����� */
//		USART_SendData(USART2, (uint8_t) ch);
//		
//		/* �ȴ�������� */
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		
//	
//		return (ch);
//}

////�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
//int fgetc(FILE *f)
//{
//		/* �ȴ������������� */
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









