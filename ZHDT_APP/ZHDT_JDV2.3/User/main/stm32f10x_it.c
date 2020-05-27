/**
  ******************************************************************************
  * @file    Project/Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    04/06/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "LOOP.h"
#include "Tim2.h"
#include "usart2.h"
#include "dht11.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "iwdg.h"
#include "input.h"
extern uint8_t read_dht11;
extern uint8_t send_data;
extern uint8_t power_status;
extern uint8_t battery_status;
extern uint8_t power_time;
extern uint8_t battery_time;
extern float temperature,humidity;
extern uint8 temp_rh[2];
extern uint8_t bed_ready;
//static char meassage[100];
extern uint8_t timePING;
uint8_t up_bed = 5;
uint8_t bed_status = 5;
uint8_t down_bed = 5;
int ac;
int battery;
int tim3i=0;
volatile unsigned int tim3fquence=0;
extern int floor;
uint8_t elevator_status = 0;
extern uint8_t pingceng;
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}


void TIM2_IRQHandler(void)
{
	int i;
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);    
		for(i = 0; i < T2_MS_ARRAY_NUM; i++)
		{
			t2_count_ms[i]++;
		}
		
	}		 	
}
void TIM4_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM4 , TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);    
//		for(i = 0; i < T4_MS_ARRAY_NUM; i++)
//		{
//			t4_count_ms[i]++;
//		}
		if(input1_time == 1)
		{
			t4_count_ms[1]++;
		}
		if(input2_time == 1)
		{
			t4_count_ms[2]++;
		}
		if(input3_time == 1)
		{
			t4_count_ms[3]++;
		}
		if(input4_time == 1)
		{
			t4_count_ms[4]++;
		}
		if(input5_time == 1)
		{
			t4_count_ms[5]++;
		}
		if(input6_time == 1)
		{
			t4_count_ms[6]++;
		}
		
	}		 	
}
void TIM3_IRQHandler(void)
{
	 if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	  {
			TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
			tim3i++;
//			tim3fquence=0; 
			if(tim3i>=50000)  //
			{
				tim3fquence++;
				tim3i=0;	
			}
			if(tim3fquence > 2)
			{
				SoftReset();
			}
	  }
}
uint8_t acc_on = 0;
void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET) //ȷ���Ƿ������EXTI Line�ж�
  {
    EXTI_ClearITPendingBit(EXTI_Line1);     //����жϱ�־λ
		ac = GPIOA->IDR & 0x0002;
		power_status = ac; 
	}
}
uint8_t door = 0;
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line10) != RESET) //ȷ���Ƿ������EXTI Line�ж�
  {
    EXTI_ClearITPendingBit(EXTI_Line10);     //����жϱ�־λ
		door = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10);
	//	printf("door = %d\r\n",door); 
	}
}
void EXTI0_IRQHandler(void)
{ 		    		    				     		    
  if(EXTI_GetITStatus(EXTI_Line0) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		EXTI_ClearITPendingBit(EXTI_Line0); // ����ϵ��жϱ�־λ	
		battery = GPIOA->IDR & 0x0001;
		battery_status = battery;
	}
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line9) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		EXTI_ClearITPendingBit(EXTI_Line9); // ����ϵ��жϱ�־λ	
		up_bed = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9); 
    if((bed_ready == 2)&&(down_bed == 0)&&(up_bed == 1))
		{
//			printf("��������\r\n");
			bed_status = 1;
			elevator_status = 1;
		}
		else if((down_bed != 0)&&(up_bed == 0))
		{
	//		printf("��������\r\n");
			bed_status = 1;
			elevator_status = 1;
		}
		
		if((bed_status == 2)&&(up_bed == 0))
		{
			--floor;
			if(floor == 0)
			{
				--floor;
			}
			bed_ready = 2;
			printf("���ݵ�λ��%d¥\r\n",floor);
			pingceng = 0;
		}
		else
		{
			pingceng = 1;
		}
	}
	
	if(EXTI_GetITStatus(EXTI_Line6) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		EXTI_ClearITPendingBit(EXTI_Line6); // ����ϵ��жϱ�־λ	
		down_bed = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6);
    if((bed_ready == 2)&&(up_bed == 0)&&(down_bed == 1))
		{
		//	printf("��������\r\n");
			bed_status = 2;
			elevator_status = 2;
		}
		else if((down_bed == 0)&&(up_bed != 0))
		{
		//	printf("��������\r\n");
			bed_status = 2;
			elevator_status = 2;
		}
		if((bed_status == 1)&&(down_bed == 0))
		{
			bed_ready = 2;
			++floor;
			if(floor == 0)
			{
				++floor;
			}
			printf("���ݵ�λ��%d¥\r\n",floor);
			elevator_status = 0;
			pingceng = 0;
		}
		else
		{
			pingceng = 1;
		}
	}
}
/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
/*void PPP_IRQHandler(void)
{
}*/


/**
  * @brief  This function handles TIM2 interrupt request.
  * @param  None
  * @retval : None
  */



/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/