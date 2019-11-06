#ifndef _UART5_H_
#define _UART5_H_
#include "stm32f10x.h"
#include <stdio.h>
#include "types.h"

#define UART5_REC_LEN  		200  							// 定义串口接收缓存大小
#define EN_USART5_RX 			1									// 使能串口3接收，0为禁止
extern volatile unsigned char rxd_buffer_locked5;
extern volatile unsigned int usart_buf5[26];
extern volatile unsigned char usart_flg5;
extern volatile unsigned char ptr_rxd_buffer5;
extern unsigned char  start5;
extern unsigned char  crc5;  	
extern u8  UART5_RX_BUF[UART5_REC_LEN]; 		// 声明串口接收缓存 
extern u16 UART5_RX_STA;         						// 声明串口接收状态标志位

void UART5_Config(void);
//int fputc(int ch, FILE *f);
//void USART_NVIC_Configuration(void);
void UART5_Putc(unsigned char c);
void UART5_Puts(char * str);
void UART5_IRQHandler(void);
void UART5_NVIC_Configuration(void);
#endif 

