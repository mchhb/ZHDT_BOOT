#ifndef __LOOP_H
#define	__LOOP_H

#include "stm32f10x.h"
#define MQTT_STRING 							
#define PARA_ADDR_FLASH						0x000800F400
#define PARA_ADDR_FLASH_A					0x000800F800
#define PARA_LENGTH								17				// unit is uint32_t

#define PARA_IDX_KEY1							0
#define PARA_IDX_KEY2							1
#define PARA_IDX_PROTECT_EN				2
#define PARA_IDX_ADC_VBAT_START		3
#define PARA_IDX_ADC_SGND_START		4
#define PARA_IDX_ADC_OVERLOAD			5
#define PARA_IDX_DUTY_VBAT_START	6
#define PARA_IDX_DUTY_SGND_START	7
#define PARA_IDX_DUTY_OVERLOAD		8
#define PARA_IDX_DUTY_INCA				9
#define PARA_IDX_DUTY_INCB				10
#define PARA_IDX_DUTY_DECA				11
#define PARA_IDX_DUTY_DECB				12
#define PARA_IDX_TIMES_VBAT_START	13
#define PARA_IDX_TIMES_SGND_START	14
#define PARA_IDX_TIMES_OVERLOAD		15
#define PARA_IDX_TIMES_STANDBY		16

typedef struct
{
	volatile uint8_t rxd_buffer_locked;
	volatile uint8_t ptr_rxd_buffer;
	volatile uint8_t buffer[26];
	uint8_t start;
	uint8_t crc;
}Usart_InitType;

typedef struct
{
	uint8_t status;
	uint8_t temp;
	uint8_t humi;
	uint8_t vibx;
	uint8_t viby;
	uint8_t vibz;
	uint8_t noise;	
}External_InitType;
typedef struct  
{
	uint16_t max;
	uint16_t min;
	uint16_t average;
}Number;
extern Number x;
extern Number y;
extern Number z;
extern External_InitType external1;
extern External_InitType external3;
extern External_InitType	interiol;
extern Usart_InitType usartRev1;
extern Usart_InitType usartRev2;
extern Usart_InitType usartRev3;
extern uint8_t  network_status;
extern uint16_t 	noise;
extern uint8_t work_uart_status;
extern u32 CpuID[3];
extern uint8_t domain_ip[4];
extern uint8_t read_dht11;
extern uint8_t send_data;
void Initialization_configuration(void);
void application(void);
void ENTER_SYS_STANDBY(void);
void SoftReset(void);
#endif



