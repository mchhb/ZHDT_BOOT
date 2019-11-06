#include "timer.h"
#include "LOOP.h"
#include "adc.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//PWM  驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
//定时器2通道1输入捕获配置

TIM_ICInitTypeDef  TIM3_ICInitStructure;

void TIM3_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能TIM2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //PA0 下拉
	
	//初始化定时器2 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM2输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC2,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
  TIM_Cmd(TIM3,ENABLE ); 	//使能定时器2
 
}


u8  TIM3CH2_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM3CH2_CAPTURE_VAL;	//输入捕获值
u8 ad=0;
//定时器5中断服务程序	 
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
