#include "usart2.h"
#include "Tim2.h"
#include "LOOP.h"
#include "gpio.h"

uint8_t tim3_reset = 0;

void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* ʹ�� USART2 ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USART2 ʹ��IO�˿����� */    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��GPIOA
	  
	/* USART2 ����ģʽ���� */
	USART_InitStructure.USART_BaudRate = 115200;	//���������ã�115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //Enable rx enable, 
	USART_Init(USART2, &USART_InitStructure);  //��ʼ��USART2
	USART_Cmd(USART2, ENABLE);// USART2ʹ��
  /* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
	�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART2, USART_FLAG_TC);     /* �巢����Ǳ�־��Transmission Complete flag */
	
}

 /*����һ���ֽ�����*/
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

/*����һ���ֽ�����*/
unsigned char UART2GetByte(unsigned char* GetData)
{   	   
        if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
        {  return 0;//û���յ����� 
		}
        *GetData = USART_ReceiveData(USART2); 
        return 1;//�յ�����
}
/*����һ�����ݣ����Ϸ��ؽ��յ����������*/
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
/* ��Printf���ݷ������� */
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;			// ��ռ���ȼ���Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;						// �����ȼ���Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								// IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);																// ����ָ���Ĳ�����ʼ��NVIC�Ĵ���
	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);  						// only app, no boot included
}
// ����2�жϷ������
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
				  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*�ȹرյȴ�ʹ��*/
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

