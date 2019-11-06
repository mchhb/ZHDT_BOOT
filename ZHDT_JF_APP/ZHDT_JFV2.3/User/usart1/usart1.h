#ifndef _USART1_H_
#define _USART1_H_
#include "stm32f10x.h"
#include <stdio.h>
#include "types.h"
#include "LOOP.h"

//extern Usart_InitType usartRev1;
void USART1_Config(void);
void USART1_Putc(unsigned char c);
void USART1_Puts(char * str);
void USART1_IRQHandler(void);
void USART1_NVIC_Configuration(void);
#endif 

