/******************** 鑫盛电子工作室 ********************
 * 文件名  ：pwm_output.c
 * 描述    ：         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 			
 * 库版本  ：ST3.0.0
 * 淘宝店：http://shop66177872.taobao.com
*********************************************************/
#include "pwm_output.h"

/* 配置TIM1复用输出PWM时用到的I/O  */
static void TIM1_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

/* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/*配置TIM1输出的PWM信号的模式，如周期、极性、占空比 */
static void TIM1_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
	//u16 CCR1_Val = 50000;        
	u16 CCR2_Val = 0;//5000;
	u16 CCR3_Val = 0;//5000;
	//u16 CCR4_Val = 125;

/* -----------------------------------------------------------------------
    TIM1 Configuration: generate 2 PWM signals with 2 different duty cycles:
    TIM1CLK = 72 MHz, Prescaler = 0x0, TIM1 counter clock = 36 MHz
    TIM3 ARR Register = 50000 => TIM1 Frequency = TIM1 counter clock/(ARR + 1)
    TIM3 Frequency = 1440Hz.
    TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR)* 100 
    TIM1 Channel2 duty cycle = (TIM1_CCR2/ TIM1_ARR)* 100 
    
  ----------------------------------------------------------------------- */

 /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 5000;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  /*TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//开反向通道
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);*/

  /* PWM1 Mode configuration: Channel2 */
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//开反向通道
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  

  TIM_OC2Init(TIM1, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);	


  /* PWM1 Mode configuration: Channel3 */
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//开反向通道
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  

  TIM_OC3Init(TIM1, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);	

  

  TIM_ARRPreloadConfig(TIM1, ENABLE);

  /* TIM1 enable counter */
  TIM_Cmd(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);	
}

// /*配置和初始化TIM1 */
// void TIM1_PWM_Init(void)
// {
// 	TIM1_GPIO_Config();
// 	TIM1_Mode_Config();	
// }
// void TIM1_NVIC_Init(void)
// {
// 		
// 		NVIC_InitTypeDef NVIC_InitStruct;
// 		
// 		TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_OC3Ref);

// 		TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
// 		NVIC_InitStruct.NVIC_IRQChannel = TIM1_CC_IRQn;
// 		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
// 		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
// 		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
// 		NVIC_Init(&NVIC_InitStruct);
// 		TIM_Cmd(TIM1,ENABLE);
//         
// }
/*配置和初始化TIM1 */
void TIM1_PWM_Init(void)
{
	TIM1_GPIO_Config();
	TIM1_Mode_Config();	
//	TIM1_NVIC_Init();
}

//粗略延时 不精确！
void DelayNuS(unsigned int i)
{
        unsigned char t = 0;
        for(;i>0;i--)
        {
                for(t=0;t<2;t++)
                {
                }
        }
}

void DelayNmS(unsigned int i)
{
        for(;i>0;i--)
        {
                DelayNuS(1000);
        }
}

u16  ZHI = 200;//5000;
u16  A= 0 ;

void pwm_led_out (void){ 
  
   while (1)   { 
		ZHI=5000;
	  for(A = 0; A < 500; A++)	{   //LED灯逐渐由最亮变到最暗
			if(ZHI <= 10) {
				TIM1->CCR2 = 0;
				TIM1->CCR3 = 0;
				break;
			}
			TIM1->CCR2 = ZHI;
			TIM1->CCR3 = ZHI;

			if(ZHI > A)			ZHI -= A;
			else 						ZHI = 0;

	    DelayNmS(500); 
		}
		ZHI=0;
		DelayNmS(10000);
		for(A = 0; A < 500; A++)	{  //LED灯逐渐由最暗变到最亮
			if(ZHI >= 4990)	{ 
				TIM1->CCR2 = 5000;
				TIM1->CCR3 = 5000; 					
				break;
			}
			TIM1->CCR2 = ZHI;
			TIM1->CCR3 = ZHI; 	

	    DelayNmS(500); ZHI += A; //时间稍长     
    }
		DelayNmS(10000); //在最暗处延时1000毫秒
  }
}
void pwm_stop (void){ 
  
//	if(ZHI == 0) return;
	
//	for(A = 0; A < 500; A++)	{
	//	A++;
		if(ZHI <= 10) {
			ZHI = 0;
			A = 0;
			TIM1->CCR2 = 0;
			TIM1->CCR3 = 0;
			return;
		}
		TIM1->CCR2 = ZHI;
		TIM1->CCR3 = ZHI;

		if(ZHI > 3500)
		{
			ZHI -= 20; //

		  DelayNmS(100);
		}
		else 
		{
			ZHI -= 20; //

		  DelayNmS(100);
    }

	//	DelayNmS(100); 
//	}
}

void pwm_start (void){ 
//	ZHI=0;
	//DelayNmS(10000);
//	for(A = 0; A < 500; )	{
	 // A= 10;
	if(ZHI<3500)
	{
		
		TIM1->CCR2 = ZHI;
		TIM1->CCR3 = ZHI; 	
		
		ZHI += 20; //

		DelayNmS(150);
	}
  else
	{
		TIM1->CCR2 = ZHI;
		TIM1->CCR3 = ZHI; 	
		
		ZHI += 10; //

		DelayNmS(300);
			if(ZHI >= 4999)	{ 
			ZHI = 5000;
			A = 0;
			TIM1->CCR2 = 5000;
			TIM1->CCR3 = 5000; 					
			return;
		}	
  }	

//	}
}
