#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"
#include "w5500_config.h"
#include "gpio.h"
//-----------------DHT11Òý½Å£¬PB5---------------------
#define	DHT11_DATA           GPIO_Pin_8
#define DHT11         (1<<1)
#define DHT11_SET(x)  GPIOA->ODR=(GPIOA->ODR&~DHT11)|(x ? DHT11:0)

#define rDHT11        GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
//----------------------------------------------------
uint8 DHT11_ReadData(void);
uint8 DHT11_GetValue(uint8 *p);

#endif
