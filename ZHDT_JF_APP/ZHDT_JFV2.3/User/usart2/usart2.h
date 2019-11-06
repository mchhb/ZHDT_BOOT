#ifndef __USART2_H
#define	__USART2_H
#include "stm32f10x.h"
#include <stdio.h>

void USART2_Config(void);
void UART2Test(void);
void print_msg(unsigned char *msg);
int fputc(int ch, FILE *f);
void USART2_NVIC_Configuration(void);
void USART2_IRQHandler(void);
void USART_SendData_CBuilder(USART_TypeDef* USARTx,unsigned char ch);
#endif /* __USART1_H */
