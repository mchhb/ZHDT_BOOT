#ifndef __PWM_OUTPUT_H
#define	__PWM_OUTPUT_H

#include "stm32f10x.h"

void TIM1_PWM_Init(void);
void pwm_led_out (void);

void pwm_start (void);
void pwm_stop (void);

#endif /* __PWM_OUTPUT_H */


