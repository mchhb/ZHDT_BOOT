/******************** 鑫盛电子工作室 ********************
 * 文件名  ：Time_test.c
 * 描述    ：TIM2 1ms 定时应用函数库
 *          
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 硬件连接：无
 * 库版本  ：ST3.0.0  
 * 淘宝店：http://shop66177872.taobao.com
*********************************************************/	
#include "Tim2.h"
//#include "misc.h"

TIM_ICInitTypeDef  TIM2_ICInitStructure;

void TIM2_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//??TIM2??
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //??GPIOA??
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 ??????  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ??  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //PA0 ??
	
	//??????2 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = arr; //?????????? 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//????   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //??????:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //??TIM_TimeBaseInitStruct?????????TIMx???????
  
	//???TIM2??????
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	????? IC1???TI1?
	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//?????
	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //???TI1?
	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //??????,??? 
	TIM2_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ??????? ???
	TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	//???????
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //?????2?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //????0?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ?????
	NVIC_Init(&NVIC_InitStructure);  //??NVIC_InitStruct???????????NVIC??? 
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);//?????? ,??CC1IE????	
	
  TIM_Cmd(TIM2,ENABLE ); 	//?????2
 
}


u8  TIM2CH1_CAPTURE_STA=0;	//??????		    				
u16	TIM2CH1_CAPTURE_VAL;	//?????
 
//???5??????	 
void TIM2_IRQHandler(void)
{ 

 	if((TIM2CH1_CAPTURE_STA&0X80)==0)//??????	
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM2CH1_CAPTURE_STA&0X40)//?????????
			{
				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)//??????
				{
					TIM2CH1_CAPTURE_STA|=0X80;//?????????
					TIM2CH1_CAPTURE_VAL=0XFFFF;
				}else TIM2CH1_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)//??1??????
		{	
			if(TIM2CH1_CAPTURE_STA&0X40)		//???????? 		
			{	  			
				TIM2CH1_CAPTURE_STA|=0X80;		//????????????
				TIM2CH1_CAPTURE_VAL=TIM_GetCapture1(TIM2);
		   		TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 ????????
			}else  								//????,????????
			{
				TIM2CH1_CAPTURE_STA=0;			//??
				TIM2CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM2,0);
				TIM2CH1_CAPTURE_STA|=0X40;		//?????????
		   		TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 ????????
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //???????
 
}
