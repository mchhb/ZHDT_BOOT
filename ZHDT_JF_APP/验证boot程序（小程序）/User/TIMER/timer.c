#include "timer.h"
#include "LOOP.h"
#include "adc.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//PWM  ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//��ʱ��2ͨ��1���벶������

TIM_ICInitTypeDef  TIM3_ICInitStructure;

void TIM3_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ��TIM2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //PA0 ����
	
	//��ʼ����ʱ��2 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM2���벶�����
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC2,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
  TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��2
 
}


u8  TIM3CH2_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM3CH2_CAPTURE_VAL;	//���벶��ֵ
u8 ad=0;
//��ʱ��5�жϷ������	 
void TIM3_IRQHandler(void)
{ 

//  	if((TIM3CH2_CAPTURE_STA&0X80)==0)
// 	{	  
// 		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
// 		 
// 		{	    
// 			if(TIM3CH2_CAPTURE_STA&0X40)
// 			{
// 				if((TIM3CH2_CAPTURE_STA&0X3F)==0X3F)
// 				{
// 					TIM3CH2_CAPTURE_STA|=0X80;
// 					TIM3CH2_CAPTURE_VAL=0XFFFF;
// 				}else TIM3CH2_CAPTURE_STA++;
// 			}	 
// 		}
// 	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
// 		{	
// 			if(TIM3CH2_CAPTURE_STA&0X40)				
// 			{	  			
// 				TIM3CH2_CAPTURE_STA|=0X80;		
// 				TIM3CH2_CAPTURE_VAL=TIM_GetCapture1(TIM2);
// 		   
// 			}else  							
// 			{
// 				TIM3CH2_CAPTURE_STA=0;			
// 				TIM3CH2_CAPTURE_VAL=0;
// 	 			TIM_SetCounter(TIM3,0);
// 				TIM3CH2_CAPTURE_STA|=0X40;		//?????????
// 		   		//TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 ????????
// 			}		    
// 		}			     	    					   
//  	}
		if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)//??1??????
		{	
// 			if(TIM3CH2_CAPTURE_STA&0X40)		//???????? 		
// 			{	  			
				TIM3CH2_CAPTURE_STA|=0X80;		//????????????
			  //ad = 1;
				//TIM3CH2_CAPTURE_VAL=TIM_GetCapture1(TIM3);
		//	}
			
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2|TIM_IT_Update); 
 
}
