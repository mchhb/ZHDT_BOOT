#include "stm32f10x.h"
#include "LOOP.h"
#include "delay.h"
#include "led.h"

void Initialization_configuration(void)
{	
	delay_init(72);      //����ϵͳʱ��Ϊ72M
	LED_GPIO_Config();	   
}

void application(void)
{	
	 LED1_ON;
	 delay_ms(500);
	 LED1_OFF;
	 delay_ms(500);
}


