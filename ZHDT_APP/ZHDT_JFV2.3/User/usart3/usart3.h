#ifndef _USART3_H_
#define _USART3_H_
#include "stm32f10x.h"
#include <stdio.h>
#include "types.h"


void USART3_Config(void);
//int fputc(int ch, FILE *f);
//void USART_NVIC_Configuration(void);
void USART3_Putc(unsigned char c);
void USART3_Puts(char * str);
void USART3_IRQHandler(void);
void USART3_NVIC_Configuration(void);
#endif 

