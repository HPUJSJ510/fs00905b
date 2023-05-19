#include "usart1.h"

static unsigned char TxBuffer[256];
static unsigned char TxCounter=0;
static unsigned char count=0; 

extern void CopeSerial1Data(unsigned char ucData);
  
void usart1_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��

    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                           //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	                    //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                              //��ʼ��GPIOA.9

    //USART1_RX	  GPIOA.10��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                          //PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;               //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                              //��ʼ��GPIOA.10  

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;            //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		            //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			            //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	                                    //����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;                         //���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;         //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;              //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                 //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                //�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure);                           //��ʼ������1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                      //�������ڽ����ж�
    USART_Cmd(USART1, ENABLE);                                          //ʹ�ܴ���1 

}

void usart_Put_Char(unsigned char DataToSend)
{
	TxBuffer[count++] = DataToSend;  
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  
}

void usart_Put_String(unsigned char *Str)
{
	while(*Str)
	{
		if(*Str=='\r')usart_Put_Char(0x0d);
			else if(*Str=='\n')usart_Put_Char(0x0a);
				else usart_Put_Char(*Str);
		Str++;
	}
}

void Usart1_SendByte(USART_TypeDef * pUSARTx, uint8_t ch){
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*****************  �����ַ��� **********************/
void Usart1_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart1_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}


//�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

//�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

void USART1_IRQHandler(void)
{
	uint8_t ucTemp;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
		ucTemp=USART_ReceiveData(USART1);
		CopeSerial1Data((unsigned char)ucTemp);//��������
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
	
}









