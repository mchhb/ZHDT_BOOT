#include "usart2.h"
#include "Tim2.h"
#include "LOOP.h"
#include "gpio.h"

uint8_t tim3_reset = 0;

void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 USART2 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USART2 使用IO端口配置 */    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA
	  
	/* USART2 工作模式配置 */
	USART_InitStructure.USART_BaudRate = 115200;	//波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //Enable rx enable, 
	USART_Init(USART2, &USART_InitStructure);  //初始化USART2
	USART_Cmd(USART2, ENABLE);// USART2使能
  /* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
	如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART2, USART_FLAG_TC);     /* 清发送外城标志，Transmission Complete flag */
	
}

 /*发送一个字节数据*/
 void UART2SendByte(unsigned char SendData)
{	   
        USART_SendData(USART2,SendData);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	    
}  

void print_msg(unsigned char *msg) {
	int i;
	unsigned char dd;
	for(;;) {
		dd = *(msg + i);
		if(dd == 0) break;
		UART2SendByte(dd);
		i++;
	}
}

/*接收一个字节数据*/
unsigned char UART2GetByte(unsigned char* GetData)
{   	   
        if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
        {  return 0;//没有收到数据 
		}
        *GetData = USART_ReceiveData(USART2); 
        return 1;//收到数据
}
/*接收一个数据，马上返回接收到的这个数据*/
void UART2Test(void)
{
       unsigned char i = 0;

       while(1)
       {    
		 while(UART2GetByte(&i))
        {
         USART_SendData(USART2,i);
        }      
       }     
}

int fputc(int ch, FILE *f)
{
/* 将Printf内容发往串口 */
  USART_SendData(USART2, (unsigned char) ch);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
  return (ch);
}
/*********************************************END OF FILE**********************/
void USART2_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;			// 抢占优先级设为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;						// 子优先级设为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								// IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);																// 根据指定的参数初始化NVIC寄存器
	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);  						// only app, no boot included
}
// 串口2中断服务程序
void USART2_IRQHandler(void)                	
{
	unsigned char c1 = 0;

	if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(USART2); 
		USART_ClearFlag(USART2, USART_FLAG_ORE);		
	
	}
	if(USART_GetFlagStatus(USART2, USART_FLAG_NE) != RESET)
	{
		USART_ReceiveData(USART2); 
		USART_ClearFlag(USART2, USART_FLAG_NE);	

	}
	if(USART_GetFlagStatus(USART2, USART_FLAG_FE) != RESET)
	{
		USART_ReceiveData(USART2); 
		USART_ClearFlag(USART2, USART_FLAG_FE);	
		
	}
	if(USART_GetFlagStatus(USART2, USART_FLAG_PE) != RESET)
	{
		USART_ReceiveData(USART2); 
		USART_ClearFlag(USART2, USART_FLAG_PE);
	}
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		 c1 = USART_ReceiveData(USART2);
		 
		 if(usartRev2.rxd_buffer_locked)
		 {
			 return;
		 }
		 if((usartRev2.ptr_rxd_buffer == 0) &&(usartRev2.start == 0)&& (c1 == 0x24))  //receive 1byte
		 { 	
			 	 usartRev2.buffer[usartRev2.ptr_rxd_buffer++] = c1; 
         usartRev2.start = 1;
				 return;
		 }
		 if (usartRev2.ptr_rxd_buffer<10&&usartRev2.start==1) //receive 2-15byte
     {
		   if(usartRev2.ptr_rxd_buffer<9)
		   {
	        	usartRev2.crc += c1; 
		   }
       usartRev2.buffer[usartRev2.ptr_rxd_buffer++] = c1; 
     }
		 if(usartRev2.ptr_rxd_buffer == 10)
     {
			 usartRev2.crc = usartRev2.buffer[0]+usartRev2.buffer[1]+usartRev2.buffer[2]+usartRev2.buffer[3]   \
			 +usartRev2.buffer[4]+usartRev2.buffer[5]+usartRev2.buffer[6]+usartRev2.buffer[7]+usartRev2.buffer[8];
       if(usartRev2.crc == usartRev2.buffer[9])
       {
				  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*先关闭等待使用*/
				 if(tim3_reset == 0)
				 {
					 tim3_reset++;
					 START_TIME3;				 
				 }
				  WORK_LED_ERROR_OFF;	WORK_LED_NORMAL_ON;
					NETWORK_LED_FAIL_OFF; NETWORK_LED_SUCCESS_ON; 
	        usartRev2.rxd_buffer_locked = 1;          // lock the rxd buffer
				  work_uart_status = 1;
			    usartRev2.crc = 0;
				  usartRev2.ptr_rxd_buffer = 0;
       }
       usartRev2.ptr_rxd_buffer = 0; // reset rxd buffer pointer
       usartRev2.start = 0;
       usartRev2.crc = 0;
      }
		 USART_ClearFlag(USART2,USART_IT_RXNE);	
		}		 
}

void USART_SendData_CBuilder(USART_TypeDef* USARTx,unsigned char ch)
{
	while((USARTx->SR&USART_SR_TXE)==0);
	USARTx->DR=ch;
}

