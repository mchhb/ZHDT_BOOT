#ifndef __TIM2__H
#define __TIM2__H

#include "stm32f10x.h"
 
#define T2_MS_ARRAY_NUM  		9


#define T2_COUNTER_MS_200		0
#define T2_COUNTER_MS_1000	1
#define T2_COUNTER_MS_4000	2
#define T2_COUNTER_MS_400		3
#define T2_COUNTER_MS_10000		4
#define T2_COUNTER_MS_2500		5
#define T2_COUNTER_MS_300		6
#define T2_COUNTER_MS_250		7
// unit is 0.1ms

#define T2_TIMEOUT_MS_200			2000	// 6	
#define T2_TIMEOUT_MS_1000		10000	// 8
#define T2_TIMEOUT_MS_4000		40000 // 9
#define T2_TIMEOUT_MS_400			4000	// 11		
#define T2_TIMEOUT_MS_10000	  100000 // 14
#define T2_TIMEOUT_MS_2500	  25000	// 17
#define T2_TIMEOUT_MS_300	    3000	// 18
#define T2_TIMEOUT_MS_250	    2500	// 19
extern uint32_t t2_count_ms[];

#define START_TIME2  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME2   TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)

#define START_TIME3  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);TIM_Cmd(TIM3, ENABLE)
#define STOP_TIME3   TIM_Cmd(TIM3, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE)

void TIM2_NVIC_Configuration(void);
void TIM2_Configuration(void);

void TIM3_NVIC_Configuration(void);
void TIM3_Configuration(void);

#endif
