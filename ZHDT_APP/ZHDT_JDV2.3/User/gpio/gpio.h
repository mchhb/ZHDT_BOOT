#ifndef __GPIO_H_
#define __GPIO_H_

#include "stm32f10x.h"

#define WIZ_RESET		    GPIO_Pin_7	// out
#define WIZ_INT			    GPIO_Pin_6	// in

#define WORK_LED_NORMAL_ON  		GPIO_ResetBits(GPIOB, GPIO_Pin_0);
#define WORK_LED_NORMAL_OFF 		GPIO_SetBits(GPIOB, GPIO_Pin_0);
#define WORK_LED_ERROR_ON  			GPIO_ResetBits(GPIOC, GPIO_Pin_5);
#define WORK_LED_ERROR_OFF 			GPIO_SetBits(GPIOC, GPIO_Pin_5);

#define NETWORK_LED_SUCCESS_ON  GPIO_ResetBits(GPIOB, GPIO_Pin_10);
#define NETWORK_LED_SUCCESS_OFF GPIO_SetBits(GPIOB, GPIO_Pin_10);
#define NETWORK_LED_FAIL_ON  		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
#define NETWORK_LED_FAIL_OFF 		GPIO_SetBits(GPIOB, GPIO_Pin_1);

#define JIZHUNCENG_SUCCESS_ON  	GPIO_ResetBits(GPIOA, GPIO_Pin_12);
#define JIZHUNCENG_SUCCESS_OFF 	GPIO_SetBits(GPIOA, GPIO_Pin_12);
#define JIZHUNCENG_FAIL_ON  		GPIO_ResetBits(GPIOB, GPIO_Pin_11);
#define JIZHUNCENG_FAIL_OFF 		GPIO_SetBits(GPIOB, GPIO_Pin_11);

#define DOOR_SUCCESS_ON 				GPIO_ResetBits(GPIOA, GPIO_Pin_10);
#define DOOR_SUCCESS_OFF 				GPIO_SetBits(GPIOA, GPIO_Pin_10);
#define DOOR_FAIL_ON  					GPIO_ResetBits(GPIOA, GPIO_Pin_11);
#define DOOR_FAIL_OFF 					GPIO_SetBits(GPIOA, GPIO_Pin_11);

#define PINGCENG_SUCCESS_ON  		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
#define PINGCENG_SUCCESS_OFF 		GPIO_SetBits(GPIOA, GPIO_Pin_8);
#define PINGCENG_FAIL_ON  			GPIO_ResetBits(GPIOA, GPIO_Pin_9);
#define PINGCENG_FALL_OFF 			GPIO_SetBits(GPIOA, GPIO_Pin_9);


#define TOP_YES			  				GPIO_ResetBits(GPIOC, GPIO_Pin_12);
#define TOP_NO 								GPIO_SetBits(GPIOC, GPIO_Pin_12);
#define BUTTON_YES  					GPIO_ResetBits(GPIOC, GPIO_Pin_11);
#define BUTTON_NO 						GPIO_SetBits(GPIOC, GPIO_Pin_11);
void GPIO_Configuration(void);


#endif

