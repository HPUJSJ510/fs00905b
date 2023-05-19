#include "usart1.h"

static unsigned char TxBuffer[256];
static unsigned char TxCounter=0;
static unsigned char count=0; 

extern void CopeSerial1Data(unsigned char ucData);
  
void usart1_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟

    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                           //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	                    //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);                              //初始化GPIOA.9

    //USART1_RX	  GPIOA.10初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                          //PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;               //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                              //初始化GPIOA.10  

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;            //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		            //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			            //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	                                    //根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound;                         //串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;         //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;              //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                 //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                //收发模式

    USART_Init(USART1, &USART_InitStructure);                           //初始化串口1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                      //开启串口接受中断
    USART_Cmd(USART1, ENABLE);                                          //使能串口1 

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
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*****************  发送字符串 **********************/
void Usart1_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart1_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}


//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

//重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

void USART1_IRQHandler(void)
{
	uint8_t ucTemp;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
		ucTemp=USART_ReceiveData(USART1);
		CopeSerial1Data((unsigned char)ucTemp);//处理数据
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
	
}









