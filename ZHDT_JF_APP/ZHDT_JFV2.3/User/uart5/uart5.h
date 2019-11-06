#ifndef _UART5_H_
#define _UART5_H_
#include "stm32f10x.h"
#include <stdio.h>
#include "types.h"

#define UART5_REC_LEN  		200  							// ���崮�ڽ��ջ����С
#define EN_USART5_RX 			1									// ʹ�ܴ���3���գ�0Ϊ��ֹ
extern volatile unsigned char rxd_buffer_locked5;
extern volatile unsigned int usart_buf5[26];
extern volatile unsigned char usart_flg5;
extern volatile unsigned char ptr_rxd_buffer5;
extern unsigned char  start5;
extern unsigned char  crc5;  	
extern u8  UART5_RX_BUF[UART5_REC_LEN]; 		// �������ڽ��ջ��� 
extern u16 UART5_RX_STA;         						// �������ڽ���״̬��־λ

void UART5_Config(void);
//int fputc(int ch, FILE *f);
//void USART_NVIC_Configuration(void);
void UART5_Putc(unsigned char c);
void UART5_Puts(char * str);
void UART5_IRQHandler(void);
void UART5_NVIC_Configuration(void);
#endif 

