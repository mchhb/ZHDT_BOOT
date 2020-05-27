#ifndef __GPIO_H_
#define __GPIO_H_

#include "stm32f10x.h"

#define WIZ_RESET		    GPIO_Pin_7	// out
#define WIZ_INT			    GPIO_Pin_6	// in

#define WORK_LED_NORMAL_ON  		GPIO_ResetBits(GPIOB, GPIO_Pin_6);	//LED1
#define WORK_LED_NORMAL_OFF 		GPIO_SetBits(GPIOB, GPIO_Pin_6);
#define WORK_LED_ERROR_ON  			GPIO_ResetBits(GPIOB, GPIO_Pin_5);	//LED2
#define WORK_LED_ERROR_OFF 			GPIO_SetBits(GPIOB, GPIO_Pin_5);

#define NETWORK_LED_SUCCESS_ON  GPIO_ResetBits(GPIOB, GPIO_Pin_8);	//LED3
#define NETWORK_LED_SUCCESS_OFF GPIO_SetBits(GPIOB, GPIO_Pin_8);
#define NETWORK_LED_FAIL_ON  		GPIO_ResetBits(GPIOB, GPIO_Pin_7);	//LED4
#define NETWORK_LED_FAIL_OFF 		GPIO_SetBits(GPIOB, GPIO_Pin_7);

#define TONGXIN2_LED_T_ON  		GPIO_ResetBits(GPIOC, GPIO_Pin_13);		//LED5
#define TONGXIN2_LED_T_OFF 		GPIO_SetBits(GPIOC, GPIO_Pin_13);
#define TONGXIN2_LED_R_ON  		GPIO_ResetBits(GPIOB, GPIO_Pin_9);		//LED6
#define TONGXIN2_LED_R_OFF 		GPIO_SetBits(GPIOB, GPIO_Pin_9);

#define TONGXIN1_LED_T_ON  		GPIO_ResetBits(GPIOC, GPIO_Pin_15);		//LED7
#define TONGXIN1_LED_T_OFF 		GPIO_SetBits(GPIOC, GPIO_Pin_15);
#define TONGXIN1_LED_R_ON  		GPIO_ResetBits(GPIOC, GPIO_Pin_14);		//LED8
#define TONGXIN1_LED_R_OFF		GPIO_SetBits(GPIOC, GPIO_Pin_14);

#define TONGXIN3_LED_T_ON  		GPIO_ResetBits(GPIOB, GPIO_Pin_0);		//LED9
#define TONGXIN3_LED_T_OFF 		GPIO_SetBits(GPIOB, GPIO_Pin_0);
#define TONGXIN3_LED_R_ON  		GPIO_ResetBits(GPIOC, GPIO_Pin_5);		//LED10
#define TONGXIN3_LED_R_OFF 		GPIO_SetBits(GPIOC, GPIO_Pin_5);

#define TONGXIN4_LED_T_ON  		GPIO_ResetBits(GPIOC, GPIO_Pin_3);		//LED11
#define TONGXIN4_LED_T_OFF  	GPIO_SetBits(GPIOC, GPIO_Pin_3);
#define TONGXIN4_LED_R_ON  		GPIO_ResetBits(GPIOB, GPIO_Pin_1);		//LED12
#define TONGXIN4_LED_R_OFF 		GPIO_SetBits(GPIOB, GPIO_Pin_1);

#define UART5_COV_RX  				GPIO_ResetBits(GPIOB, GPIO_Pin_4);		//UART5_CONV
#define UART5_COV_TX 					GPIO_SetBits(GPIOB, GPIO_Pin_4);
void GPIO_Configuration(void);


#endif

