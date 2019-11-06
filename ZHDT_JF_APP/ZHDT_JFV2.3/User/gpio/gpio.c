#include "gpio.h"

/**
  * @brief  Configure the TIM3 Ouput Channels.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
   
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
 	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;  
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//w5500 int  	

 	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//w5500 reset  	
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 // GPIO_SetBits(GPIOA,GPIO_Pin_8);//DHT11 DATA
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//LED1  
	GPIO_SetBits(GPIOB,GPIO_Pin_5);//
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//LED2  	
	GPIO_SetBits(GPIOB,GPIO_Pin_6);//
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//LED3 
 	GPIO_SetBits(GPIOB,GPIO_Pin_7);//
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//LED4
	GPIO_SetBits(GPIOB,GPIO_Pin_8);//
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//LED5 
 	GPIO_SetBits(GPIOB,GPIO_Pin_9);//
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//LED6
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//LED7
 	GPIO_SetBits(GPIOC,GPIO_Pin_14);//
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//LED8
	GPIO_SetBits(GPIOC,GPIO_Pin_15);//
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//LED9
 	GPIO_SetBits(GPIOC,GPIO_Pin_5);//
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//LED10
	GPIO_SetBits(GPIOB,GPIO_Pin_0);//
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//LED11
	GPIO_SetBits(GPIOB,GPIO_Pin_1);//
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//LED12
	GPIO_SetBits(GPIOC,GPIO_Pin_3);//
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//UART5_CONV
	GPIO_SetBits(GPIOB,GPIO_Pin_4);//
}

