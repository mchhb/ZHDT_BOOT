#include "uart5.h"
#include "Tim2.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

u8 UART5_RX_BUF[UART5_REC_LEN];     // 定义一个用于串口数据接收的缓存，大小为USART_REC_LEN
u16 UART5_RX_STA=0;       					// 定义并初始化串口接收标志位
volatile unsigned char rxd_buffer_locked5;
volatile unsigned int usart_buf5[26];
volatile unsigned char usart_flg5=0;
volatile unsigned char ptr_rxd_buffer5=0;
unsigned char  start5=0;
unsigned char  crc5=0;
unsigned char usart5_test = 0;
extern uint8_t mqtt_send_xin;
void UART5_Config(void)
{
  USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB1Periph_USART3|RCC_APB2Periph_AFIO , ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  // Configure the USART3_Rx as input floating
  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING ;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_ITConfig(UART5, USART_IT_RXNE, ENABLE); //Enable rx enable, 
  
  /* Configure the USARTx */ 
  USART_Init(UART5, &USART_InitStructure);
  /* Enable the USARTx */
  USART_Cmd(UART5, ENABLE);
	
	USART_ClearFlag(UART5, USART_FLAG_TC);     /* 清发送外城标志，Transmission Complete flag */
}
//int fputc(int ch, FILE *f)
//{
///* 将Printf内容发往串口 */
//  USART_SendData(USART3, (unsigned char) ch);
//  while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); 
//  return (ch);
//}

void UART5_Putc(unsigned char c)
{
    USART_SendData(UART5, c);
    while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET );
}

void UART5_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(UART5, *str++);
        while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
    }
}

// 串口3中断服务程序
void UART5_IRQHandler(void)                	
{
	unsigned char c5 = 0;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{	
	//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*先关闭等待使用*/ 	
	 	 USART_ClearFlag(UART5,USART_IT_RXNE);
		// printf(" UARST3 ！\r\n");
		 c5 = USART_ReceiveData(UART5);
		// USART3_RX_BUF[usart3_test++] = c3;
		 if(rxd_buffer_locked5)
		 {
			 return;
		 }
		 if(mqtt_send_xin == 1)
		 {
			 return;
		 }
		 if((ptr_rxd_buffer5 == 0) &&(start5 == 0)&& (c5 == 0x66))  //receive 1byte
		 {
			 	 usart_buf5[ptr_rxd_buffer5++] = c5; 
         start5 = 1;
				 return;
		 }
		 if(ptr_rxd_buffer5<26&&start5==1) //receive 2-15byte
     {
		   if(ptr_rxd_buffer5<18)
		   {
	        	crc5 += c5; 
		   }
       usart_buf5[ptr_rxd_buffer5++] = c5; 
     }
		 if(ptr_rxd_buffer5 == 26)
     {
       if((usart_buf5[25]== 0x99)&&(usart_buf5[18] == crc5))
       {
	        rxd_buffer_locked5 = 1;          // lock the rxd buffer
			    crc5 = 0;
				  usart_flg5 = 1;
				  ptr_rxd_buffer5 = 0;
       }
       ptr_rxd_buffer5 = 0; // reset rxd buffer pointer
       start5 = 0;
       crc5 = 0;
     }
	}
}
// 中断配置程序
void UART5_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;			// 抢占优先级设为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;						// 子优先级设为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								// IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);																// 根据指定的参数初始化NVIC寄存器
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);  						// only app, no boot included
}


