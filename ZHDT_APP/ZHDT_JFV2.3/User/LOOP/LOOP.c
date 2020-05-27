#include "stm32f10x.h"
#include "usart2.h"
#include "LOOP.h"
#include "delay.h"
#include "Tim2.h"
#include "spi2.h"
#include "w5500_config.h"
#include "gpio.h"
#include "dht11.h"
#include "w5500.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "iwdg.h"
#include "dhcp.h"
#include "usart1.h"
#include "usart3.h"
#include "ADXL345.h"
#include "24c16.h"
#include "adc.h"
#include "exti.h"
#include "MqttKit.h"
#include "tcp_demo.h"
#include "socket.h"
#include "BD_TG.h"
//#include "MQTTConnectClient.h"
extern __IO uint16_t ADC_ConvertedValue;// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_Result[1];
extern uint16 local_port;
extern uint8 BD_TG_ping_pak[2];
extern uint8 buff[200];	
extern unsigned char w5500_buf[128];   //之前是128
extern volatile unsigned int  tim3fquence;	//定时中断次数
extern int tim3i;
unsigned char result[20] = {0x66,0x04,0x10,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
														0xFF,0xFF,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x99};
//extern char *DEVID;
void GetLockCode(void);
void USART_SendDatacmd(USART_TypeDef* USARTx,unsigned char ch);
void Power_check(void);
char *rand_str(char *str);

uint16_t publish_rev_head = 20; 														
uint8_t send_version = 1;														
uint8_t id_length = 0;														
uint8_t pubilish_id[10];
u32 pubilish_id_32;
uint16_t publish_rev = 0;	
uint8_t publish_cmd = 0;
uint8_t publish_version = 0;		
uint8_t printf_version = 0;														
														
//------------------------------
u32 Lock_Code;
u32 CpuID[3];
uint8_t reboot_sum = 0;
uint8_t uart_w5500_mac_config = 0;	
uint8_t uart_w5500_port_config = 0;														
External_InitType external1;
External_InitType external3;
External_InitType	interiol;
														
Usart_InitType usartRev1;
Usart_InitType usartRev2;
Usart_InitType usartRev3;	

uint8_t status_code1;
uint8_t status_code2;
uint8_t status_code3;

Number x;
Number y;
Number z;
//------------------------------
unsigned char  flag;
uint8_t adxl345_number = 0;
uint8_t 	read_dht11 = 0;
uint8_t 	send_data = 0;
uint8 		Disconnect_Request[2] = {0xe0,0x00};
uint8_t 	temp_rh[2];
uint8_t   uart2_send_read = 0;
uint8_t   uart2_send_write = 0;
uint8_t 	noise_status = 0;
uint8_t 	ip_config;

int 	adxl_val[3];
uint8_t work_Acc_success = 0;
//------------------------
uint8_t mqtt_send_time = 0;
u32 id_name_all;
unsigned char id_name[4];


uint8_t work_dht11_status;
uint8_t work_noise_status;
uint8_t work_uart_status = 0;
uint8_t work_Acceleration_status;

uint16 disconnet=0;
uint16 overtime=0;
uint8_t mqtt_send_xin = 0;
uint8_t overtime_reset = 0;
uint8_t  work_status;
uint8_t  network_status = 1;
uint8_t  led_set_work;
uint8_t  led_set_network;
#define WORK_NORMAL				0
#define WORK_ERROR				1
#define WORK_USART				2
#define NETWORK_SUCCESS		0
#define NETWORK_FAIL			1
#define NETWORK_USART			2
//------------------------
MQTT_PACKET_STRUCTURE mqttPacket;		
unsigned char sum=0;
uint8_t external_driver = 0;
uint8_t external_led1 = 0;
uint8_t external_led2 = 0;
uint8_t external_led3 = 0;
uint8_t external_led4 = 0;
uint8_t external_led5 = 0;
uint8_t external_led6 = 0;
uint8_t external_led_open = 0;
uint8_t power_status;
uint8_t battery_status = 9;
uint8_t power_time = 0;
uint8_t battery_time = 0;
uint8 txBuffer[40];
uint8_t w5500_connect_success = 0;
uint8_t W5500_REBOOT = 0;
uint8_t w5500_success_crc = 0;

void read_config(void);


void Initialization_configuration(void) {

	delay_ms(1000);
	
	IWDG_Init(4,3750);//((prer)*rlr)/40; 
	
	TIM2_Configuration();

	TIM2_NVIC_Configuration();
	
	TIM3_Configuration();

	TIM3_NVIC_Configuration();
	
	GPIO_Configuration();

	WIZ_SPI_Init();
	
	at24c16_init();

	ADXL345_init();
	
	ADC1_Init();
	
	EXTI_ENABLE();
	
	USART2_Config();		

	USART1_Config();	
	
	USART3_Config();	
	
	USART1_NVIC_Configuration();
	
	USART3_NVIC_Configuration();
	
	USART2_NVIC_Configuration();
	
	Power_check();

	GetLockCode();
	
	Reset_W5500();

  w5500_check();	

	read_config();
	
	set_w5500_mac();
	
	set_object_ip();

	sysinit(txsize, rxsize); 													// 初始化8个socket

  printf("W5500 Init Complete!\r\n");
	
	START_TIME2;
}
void application(void)
{
	IWDG_Feed();
	//w5500_connect_success = 1;
	if(usartRev2.rxd_buffer_locked != 1)
	{
			w5500_check();
	}
	if(dhcp_ok != 1)
	{
		auto_dhcp();
	}
	if(dhcp_ok==1)
	{	
			do_tcp_client();                    /*TCP_Client 数据回环测试程序*/
			delay_ms(100);
	}
	if(disconnet==1)
	{
		disconnet=0;
		dhcp_ok = 0;
		w5500_connect_success = 0;
		Reset_W5500();												/*硬复位W5500*/
		w5500_check();													/*检查网线是否接入*/   
		set_w5500_mac();											/*配置MAC地址*/
		set_w5500_ip();
		sysinit(txsize, rxsize); 													// 初始化8个socket
		MQTT_STATE =MQTT_PKT_CONNECT;			
	}
	if((t2_count_ms[T2_COUNTER_MS_2500] >= T2_TIMEOUT_MS_2500) && (w5500_connect_success == 1)&&(mqtt_send_xin != 1))  //2.5s
	{
		t2_count_ms[T2_COUNTER_MS_2500] = 0;	 
		if(external_driver == 0)
		{
			for(int k = 0; k < 20; k++)
			{
				USART_SendDatacmd(USART1,result[k]);
			}
			external_led1 = 1;
			external_driver = 1;
		}
		else if(external_driver == 1)
		{
			for(int m = 0; m < 20; m++)
			{
				USART_SendDatacmd(USART3,result[m]);
			}
			external_driver = 0;
			external_led3 = 1;
		}
	}
			if(usartRev1.ptr_rxd_buffer== 1)
			{
				usartRev1.rxd_buffer_locked = 0;
				external_led2 = 1; 
				external1.status = usartRev1.buffer[8]; 
				external1.temp = usartRev1.buffer[14];
				external1.humi = usartRev1.buffer[15];
				external1.vibx = usartRev1.buffer[16];
				external1.noise = usartRev1.buffer[17];
				external1.viby = usartRev1.buffer[20];
				external1.vibz = usartRev1.buffer[21];
		}
			if(usartRev3.rxd_buffer_locked == 1)
			{
				usartRev3.rxd_buffer_locked = 0;
				external_led4 = 1;
				external3.status = usartRev3.buffer[8]; 
				external3.temp = usartRev3.buffer[14];
				external3.humi = usartRev3.buffer[15];
				external3.vibx = usartRev3.buffer[16];
				external3.noise = usartRev3.buffer[17];
				external3.viby = usartRev3.buffer[20];
				external3.vibz = usartRev3.buffer[21];
			}
		/////////////////////////////
	if(t2_count_ms[T2_COUNTER_MS_10000] >= T2_TIMEOUT_MS_10000)  //10s
	{
		t2_count_ms[T2_COUNTER_MS_10000]=0;
		send(SOCK_TCPC,BD_TG_ping_pak,2);//发送心跳      						
		delay_ms(100);
		if(buff[0]==0xd0)
		{		 	 	
			MQTT_STATE = MQTT_PKT_PINGREQ;
			overtime = 0;
			overtime_reset = 0;
			memset(buff,0,sizeof(buff));
			flag =1;	
			//printf("收到心跳\r\n");
			network_status = 0;
		}	
		else 
		{
			overtime++;
			overtime_reset++;
			if(overtime > 2)
			{
			MQTT_STATE = MQTT_PKT_DISCONNECT;
			printf("未收到心跳次数： %d\r\n",overtime);
			}
			if(overtime>3)
			{
				printf("心跳超时，重新连接！\r\n");
				overtime=0;
				disconnet=1;
				send(SOCK_TCPC,Disconnect_Request,2);//发送断网
				flag=0;					
				memset(buff,0,sizeof(buff));
				memset(w5500_buf,0,sizeof(w5500_buf));
				delay_ms(1000);
				SoftReset();
			}
		}			
	}	
	if(t2_count_ms[9] >= (mqtt_send_time*10000))  //30s
	{
		t2_count_ms[9] = 0;
		if((flag == 1)&&(overtime<1))//发布
		{	   
				printf("发布消息\r\n");
		  	w5500_success_crc++;
				mqtt_send_xin = 1;
				MQTT_STATE = MQTT_PKT_PUBLISH;		
		}
	}
	if(w5500_success_crc >= 3) //30s
	{
		reboot_sum++;
		at24c16_write(EE_ADDR_REBOOT_SUM, reboot_sum);
		delay_ms(1000);
		SoftReset();
	}
	if(t2_count_ms[T2_COUNTER_MS_1000] >= T2_TIMEOUT_MS_1000)  //1s
	{
		t2_count_ms[T2_COUNTER_MS_1000] = 0;
		if(work_uart_status == 1)
		{  
			work_status = 2;
			network_status = 2;
		}
		else if((work_dht11_status == 1) || (work_noise_status == 1) || (work_Acceleration_status == 1) || (power_status == 1))
		{
			work_status = 1;
		}
		else
		{
			work_status = 0;
		}
	}		
	if(t2_count_ms[T2_COUNTER_MS_400] >= T2_TIMEOUT_MS_400)  //400s
	{
		t2_count_ms[T2_COUNTER_MS_400] = 0;
		switch(work_status)
		{
			case WORK_NORMAL:
				if(led_set_work == 0) 		{ WORK_LED_ERROR_OFF;	WORK_LED_NORMAL_ON; 	led_set_work = 1; }
				else 											{ WORK_LED_ERROR_OFF;	WORK_LED_NORMAL_OFF;	led_set_work = 0; }
			break;
			case WORK_ERROR:
				if(led_set_work == 1) 		{ WORK_LED_NORMAL_OFF; WORK_LED_ERROR_ON; 	led_set_work = 0; }
				else 											{ WORK_LED_NORMAL_OFF; WORK_LED_ERROR_OFF;	led_set_work = 1; }
		  break;
			case WORK_USART:
					WORK_LED_ERROR_OFF;	WORK_LED_NORMAL_ON;
					NETWORK_LED_FAIL_OFF; NETWORK_LED_SUCCESS_ON; 
			break;
		}
	}
	if((t2_count_ms[T2_COUNTER_MS_4000] >= T2_TIMEOUT_MS_4000)&&(w5500_connect_success == 1))  //2s
	{
		t2_count_ms[T2_COUNTER_MS_4000] = 0;
		external_led_open++;

		DHT11_GetValue(temp_rh);
		interiol.temp = (int)temp_rh[1];
		interiol.humi = (int)temp_rh[0];
		if((interiol.temp > 50) || (interiol.humi > 100) ||((interiol.temp == 0)&&(interiol.humi == 0)))
		{
			work_dht11_status = 1;
		}
		else
		{
			work_dht11_status = 0;
		}		
		interiol.noise = ADC_Result[0]*0.17+43;
		if(interiol.noise > 100)
		{
			interiol.noise = 90;
		}
		//printf("noise is %d\r\n",noise);
		if((interiol.noise == 0)||(interiol.noise == 90))
		{
			noise_status++;
			if(noise_status > 5)
			{
	//			printf("The noise	is bad !\r\n");
				work_noise_status = 1;
			}
		}
		else
		{
			work_noise_status = 0;
			noise_status = 0;
		}
		if((work_dht11_status == 1)||(work_noise_status == 1)||(work_Acceleration_status == 1))
		{
			if(work_dht11_status == 1)
			{
				status_code1 = 1;
			}
			if(work_noise_status == 1)
			{
				status_code1 = 2;
			}
			if(work_Acceleration_status == 1)
			{
				status_code1 = 3;
			}
			if((work_dht11_status == 1)&&(work_noise_status == 1))
			{
				status_code1 = 4;
			}
			if((work_dht11_status == 1)&&(work_Acceleration_status == 1))
			{
				status_code1 = 5;
			}
			if((work_noise_status == 1)&&(work_Acceleration_status == 1))
			{
				status_code1 = 6;
			}
			if((work_noise_status == 1)&&(work_Acceleration_status == 1)&&(work_dht11_status == 1))
			{
				status_code1 = 7;
			}
		}
		else
		{
			status_code1 = 0;
		}
	}
	if(t2_count_ms[T2_COUNTER_MS_200] >= T2_TIMEOUT_MS_200)  //0.2s
	{
		t2_count_ms[T2_COUNTER_MS_200] = 0;
		switch(network_status)
		{
			case NETWORK_SUCCESS:
				if(led_set_network == 0) 	{ NETWORK_LED_FAIL_OFF;	NETWORK_LED_SUCCESS_ON; led_set_network = 1; }
				else 											{ NETWORK_LED_FAIL_OFF;	NETWORK_LED_SUCCESS_OFF;	led_set_network = 0; }
			break;
			case NETWORK_FAIL:
				if(led_set_network == 1) 	{ NETWORK_LED_SUCCESS_OFF; NETWORK_LED_FAIL_ON;  led_set_network = 0; }
				else 											{ NETWORK_LED_SUCCESS_OFF; NETWORK_LED_FAIL_OFF; led_set_network = 1; }
			break;
			case NETWORK_USART:
				WORK_LED_ERROR_OFF;	WORK_LED_NORMAL_ON;
			  NETWORK_LED_FAIL_OFF; NETWORK_LED_SUCCESS_ON; 
			  STOP_TIME2;
			break;	
		}
	}
	if((power_status == 2)&&(power_time == 0))
	{
		if(t2_count_ms[T2_COUNTER_MS_300] >= T2_TIMEOUT_MS_300)  //300ms
		{
			t2_count_ms[T2_COUNTER_MS_300] = 0;
			if(power_status == 2)
			{
				power_time++;
				power_status = 1;
				printf("acc is closed !\r\n");
			}
		}
	}
	if((power_status == 0)&&(power_time != 0))
	{
		if(t2_count_ms[T2_COUNTER_MS_300] >= T2_TIMEOUT_MS_300)  //300ms
		{
			t2_count_ms[T2_COUNTER_MS_300] = 0;
			if(power_status == 0)
			{
				power_time = 0;
				power_status = 0;
				printf("acc is opened !\r\n");
			}
		}
	}
	if((battery_status == 1)&&(battery_time != 0))
	{
		if(t2_count_ms[T2_COUNTER_MS_250] >= T2_TIMEOUT_MS_250)  //300ms
		{
			t2_count_ms[T2_COUNTER_MS_250] = 0;
			if(battery_status == 1)
			{
				battery_time = 0;
				printf("battery is high !\r\n");
			}
		}
	}
	if((battery_status == 0)&&(battery_time == 0))
	{
		if(t2_count_ms[T2_COUNTER_MS_250] >= T2_TIMEOUT_MS_250)  //300ms
		{
			t2_count_ms[T2_COUNTER_MS_250] = 0;
			if(battery_status == 0)
			{
				battery_time++;
				printf("battery is low !\r\n");
			}
		}
	}
	if(t2_count_ms[T2_COUNTER_MS_4] >= T2_TIMEOUT_MS_4)  //10s
	{
		t2_count_ms[T2_COUNTER_MS_4]=0;
		ADXL345_ReadXYZ(adxl_val);
//		if(((int)(adxl_val[0]*10) == 78)||((int)(adxl_val[0]*10) == 156)||((int)(adxl_val[0]*10) == 234)||((int)(adxl_val[0]*10) == 312))
//		{
//			adxl_val[0] = 0;
//		}
		if(adxl345_number >= 9)
		{
			x.max /= 10;
			y.max /= 10;
			z.max /= 10;
			if((x.max-5 < x.average)&&(x.max+5 > x.average))
			{
				x.max = x.average;
			}
			if((y.max-5 < y.average)&&(y.max+5 > y.average))
			{
				y.max = y.average;
			}
			if((z.max-5 < z.average)&&(z.max+5 > z.average))
			{
				z.max = z.average;
			}
			//printf("x=%d,y=%d,z=%d.\r\n",x.max,y.max,z.max);
			x.average = x.max;
			y.average = y.max;
			z.average = z.max;
			adxl345_number = 0;
			x.max = 0;
			y.max = 0;
			z.max = 0;
		}
		else
		{
			x.max += adxl_val[0];
			y.max += adxl_val[1];
			z.max += adxl_val[2];
			adxl345_number++;
		}
		//printf("x=%d,y=%d,z=%d.\r\n",adxl_val[0],adxl_val[1],adxl_val[2]);
	}	
	if(overtime_reset > 5)
	{
		SoftReset();		
	}
	if(sub_flag == 1)
	{
		sub_flag = 0;
		printf("sub_rev_length = %d\r\n",sub_rev_length);
		if((publish_buf[0] == '{')&&(publish_buf[9] == 0x30))
		{
			if(publish_buf[18] == 0x5B)
			{
				publish_rev = publish_rev_head;
				for(;publish_rev < sub_rev_length; publish_rev++)
				{		
					if(publish_buf[publish_rev] == '"')
					{
						id_length = publish_rev-publish_rev_head;
						for(int length = 0; length < id_length; length++)
						{
							pubilish_id[length] = HexToChar(publish_buf[(publish_rev-id_length)+length]);
							pubilish_id_32 = pubilish_id_32*10+pubilish_id[length];
						}
						if(pubilish_id_32 == Lock_Code)
						{
							printf("收到更新指令.\r\n");
							publish_version = (publish_buf[sub_rev_length-5]-0x30)*10+ (publish_buf[sub_rev_length-3]-0x30);
							printf("更新版本为:V%d.%d.\r\n",publish_version/10,publish_version%10);
							if(publish_version > at24c16_read(EE_APP_VERSION1))
							{
								printf("重新启动，升级版本为：V%d.%d.\r\n",publish_version/10,publish_version%10);
								at24c16_write(EE_APP_VERSION2,publish_version);
								delay_ms(1000);
								SoftReset();
							}
							else
							{
								send_version = 1;
								printf("版本为最新.\r\n");
							}
							publish_rev_head = 20;
							pubilish_id_32 = 0;
							break;
						}
						else
						{
							if(publish_buf[publish_rev+1] == ']')
							{
								printf("不是本设备更新\r\n");
								publish_rev_head = 20;
								pubilish_id_32 = 0;
								return;
							}
							pubilish_id_32 = 0;
							publish_rev_head += id_length+3;
							publish_rev += 3;
						}
					}				
				}
			}
		}
	}
//	if(sub_flag == 1)
//	{
//		sub_flag = 0;
//		if(publish_buf[0] == '{')
//		{
//			for(publish_rev = 0;publish_rev < sub_rev_length; publish_rev++)
//			{
//				if((publish_buf[publish_rev] == ',')&&(id_length == 0))
//				{
//					id_length = publish_rev-6;
//					for(int length = 0; length < id_length; length++)
//					{
//						pubilish_id[length] = HexToChar(publish_buf[6+length]);
//						pubilish_id_32 = pubilish_id_32*10+pubilish_id[length];
//					}
//					if(pubilish_id_32 == Lock_Code)
//					{
//						printf("收到更新指令.\r\n");
//					}
//					else
//					{
//						printf("不是本机更新.\r\n");//400163309
//						return;
//					}
//					//printf("id=%d\r\n",pubilish_id_32);
//					publish_rev += 8;
//					break;
//				}
////				publish_buf[a] = HexToChar(publish_buf[a]);
//			}
//			publish_cmd = (publish_buf[publish_rev]-0x30)*10+(publish_buf[publish_rev+1]-0x30);
//			publish_rev += 13;
////			printf("publish_rev = %d\r\n",publish_rev);
////			printf("publish_buf[publish_rev] = %d\r\n",(publish_buf[publish_rev]-0x30)*10);
////			printf("publish_buf[publish_rev++] = %d\r\n",publish_buf[publish_rev+1]-0x30);
//			publish_version = (publish_buf[publish_rev]-0x30)*10+(publish_buf[publish_rev+1]-0x30);
//			if(publish_version != at24c16_read(EE_APP_VERSION1))
//			{
//				printf("重新启动，升级版本为：V%d.%d.\r\n",publish_version/10,publish_version%10);
//				at24c16_write(EE_APP_VERSION2,publish_version);
//				delay_ms(1000);
//				SoftReset();
//			}
//			else
//			{
//				send_version = 1;
//				printf("版本为最新.\r\n");
//	//			MQTT_STATE = MQTT_PKT_PUBLISH;	
//			}
//			pubilish_id_32 = 0;
//			id_length = 0;
//		}
//		
//	}
	if((external_led_open%2 == 0) &&(external_led1 == 1))
	{
			TONGXIN1_LED_T_ON;
			TONGXIN1_LED_R_OFF;
			external_led1 = 0;
	}
	else if((external_led_open%2 == 0) &&(external_led2 == 1))
	{
			TONGXIN1_LED_T_OFF;
			TONGXIN1_LED_R_ON;
			external_led2 = 0;
	}
	else
	{
			TONGXIN1_LED_T_OFF;
			TONGXIN1_LED_R_OFF;
	}
	if((external_led_open%2 == 0) &&(external_led3 == 1))
	{
			TONGXIN2_LED_T_ON;
			TONGXIN2_LED_R_OFF;
			external_led3 = 0;
	}
	else if((external_led_open%2 == 0) &&(external_led4 == 1))
	{
			TONGXIN2_LED_T_OFF;
			TONGXIN2_LED_R_ON;
			external_led4 = 0;
	}
	else
	{
			TONGXIN2_LED_T_OFF;
			TONGXIN2_LED_R_OFF;
	}
	if((external_led_open%2 == 0) &&(external_led5 == 1))
	{
			TONGXIN1_LED_T_OFF;
			TONGXIN1_LED_R_ON;
			TONGXIN2_LED_T_OFF;
			TONGXIN2_LED_R_ON;
			external_led5 = 0;
	}
	else
	{
			TONGXIN1_LED_T_OFF;
			TONGXIN1_LED_R_OFF;
			TONGXIN2_LED_T_OFF;
			TONGXIN2_LED_R_OFF;
	}
	if(usartRev2.rxd_buffer_locked == 1)
	{
		tim3i = 0;
		tim3fquence=0;
		usartRev2.rxd_buffer_locked = 0;
		if(usartRev2.buffer[1] == 0x01)
		{
			if(usartRev2.buffer[3] == 0x01)	//目的IP
			{
				uart2_send_read = 1;
				txBuffer[0] = 0x24;
				txBuffer[1] = 0x01;		
				txBuffer[2] = 0x06;	
				txBuffer[3] = at24c16_read(EE_ADDR_IP_ONE);	
				txBuffer[4] = at24c16_read(EE_ADDR_IP_TWO);	
				txBuffer[5] = at24c16_read(EE_ADDR_IP_THREE);	
				txBuffer[6] = at24c16_read(EE_ADDR_IP_FOUR);	
				txBuffer[7] = 0x00;		
				txBuffer[8] = 0x00;			
			}
			if(usartRev2.buffer[3] == 0x02)	//分配IP
			{
				uart2_send_read = 1;
				txBuffer[0] = 0x24;
				txBuffer[1] = 0x02;
				txBuffer[2] = 0x06;	
				txBuffer[3] = at24c16_read(EE_ADDR_DHCP_IP_ONE);	
				txBuffer[4] = at24c16_read(EE_ADDR_DHCP_IP_TWO);	
				txBuffer[5] = at24c16_read(EE_ADDR_DHCP_IP_THREE);	
				txBuffer[6] = at24c16_read(EE_ADDR_DHCO_IP_FOUR);	
				txBuffer[7] = 0x00;		
				txBuffer[8] = 0x00;	
			}
			if(usartRev2.buffer[3] == 0x03)	//MAC
			{
				uart2_send_read = 1;
				txBuffer[0] = 0x24;
				txBuffer[1] = 0x03;
				txBuffer[2] = 0x06;	
				txBuffer[3] = at24c16_read(EE_ADDR_MAC_ONE);	
				txBuffer[4] = at24c16_read(EE_ADDR_MAC_TWO);	
				txBuffer[5] = at24c16_read(EE_ADDR_MAC_THREE);	
				txBuffer[6] = at24c16_read(EE_ADDR_MAC_FOUR);	
				txBuffer[7] = at24c16_read(EE_ADDR_MAC_FIVE);	
				txBuffer[8] = at24c16_read(EE_ADDR_MAC_SIX);	
				
			}
			if(usartRev2.buffer[3] == 0x04)	//子网掩码
			{
				uart2_send_read = 1;
				txBuffer[0] = 0x24;
				txBuffer[1] = 0x04;
				txBuffer[2] = 0x06;	
				txBuffer[3] = at24c16_read(EE_ADDR_SUBNET_ONE);	
				txBuffer[4] = at24c16_read(EE_ADDR_SUBNET_TWO);	
				txBuffer[5] = at24c16_read(EE_ADDR_SUBNET_THREE);	
				txBuffer[6] = at24c16_read(EE_ADDR_SUBNET_FOUR);	
				txBuffer[7] = 0x00;	
				txBuffer[8] = 0x00;					
			}
			if(usartRev2.buffer[3] == 0x05)	//网关
			{
				uart2_send_read = 1;
				txBuffer[0] = 0x24;
				txBuffer[1] = 0x05;
				txBuffer[2] = 0x06;	
				txBuffer[3] = at24c16_read(EE_ADDR_ID_NAME1);	
				txBuffer[4] = at24c16_read(EE_ADDR_ID_NAME2);	
				txBuffer[5] = at24c16_read(EE_ADDR_ID_NAME3);	
				txBuffer[6] = at24c16_read(EE_ADDR_ID_NAME4);	
				txBuffer[7] = 0x00;
				txBuffer[8] = 0x00;				
			}
			if(usartRev2.buffer[3] == 0x06)	//DNS
			{
				uart2_send_read = 1;
				txBuffer[0] = 0x24;
				txBuffer[1] = 0x06;
				txBuffer[2] = 0x06;	
				txBuffer[3] = at24c16_read(EE_ADDR_DNS_ONE);	
				txBuffer[4] = at24c16_read(EE_ADDR_DNS_TWO);	
				txBuffer[5] = at24c16_read(EE_ADDR_DNS_THREE);	
				txBuffer[6] = at24c16_read(EE_ADDR_DNS_FOUR);	
				txBuffer[7] = 0x00;
				txBuffer[8] = 0x00;				
			}
			if(usartRev2.buffer[3] == 0x07)	//端口
			{
				uart2_send_read = 1;
				txBuffer[0] = 0x24;
				txBuffer[1] = 0x07;
				txBuffer[2] = 0x06;	
				txBuffer[3] = at24c16_read(EE_ADDR_PORT_1);	
				txBuffer[4] = at24c16_read(EE_ADDR_PORT_2);	
				txBuffer[5] = 0x00;	
				txBuffer[6] = 0x00;	
				txBuffer[7] = 0x00;	
				txBuffer[8] = 0x00;		
			}
			if(usartRev2.buffer[3] == 0x08)	//温度湿度加速度
			{
				uart2_send_read = 1;
				txBuffer[0] = 0x24;
				txBuffer[1] = 0x08;
				txBuffer[2] = 0x06;	
				txBuffer[8] = 0x00;		
			}
			if(usartRev2.buffer[3] == 0x09)	//擦除EEprom
			{
				uart2_send_read = 1;
				txBuffer[0] = 0x24;
				txBuffer[1] = 0x09;
				txBuffer[2] = 0x06;	
				txBuffer[3] = 0x00;	
				txBuffer[4] = 0x00;	
				txBuffer[5] = 0x00;	
				txBuffer[6] = 0x00;	
				txBuffer[7] = 0x00;	
				txBuffer[8] = 0x00;
				erase_eeprom(0x00,0x830);				
			}
			if(usartRev2.buffer[3] == 0x10)	//CPU复位
			{
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				SoftReset();				
			}
			if(usartRev2.buffer[3] == 0x12)	//一键读取
			{
				txBuffer[0] = 0x24;
				txBuffer[1] = 0x12;		
				txBuffer[2] = 0x20;	
				txBuffer[3] = at24c16_read(EE_ADDR_IP_ONE);	
				txBuffer[4] = at24c16_read(EE_ADDR_IP_TWO);	
				txBuffer[5] = at24c16_read(EE_ADDR_IP_THREE);	
				txBuffer[6] = at24c16_read(EE_ADDR_IP_FOUR);	
				txBuffer[7] = at24c16_read(EE_ADDR_DHCP_IP_ONE);	
				txBuffer[8] = at24c16_read(EE_ADDR_DHCP_IP_TWO);	
				txBuffer[9] = at24c16_read(EE_ADDR_DHCP_IP_THREE);	
				txBuffer[10] = at24c16_read(EE_ADDR_DHCO_IP_FOUR);	
				txBuffer[11] = at24c16_read(EE_ADDR_MAC_ONE);	
				txBuffer[12] = at24c16_read(EE_ADDR_MAC_TWO);	
				txBuffer[13] = at24c16_read(EE_ADDR_MAC_THREE);	
				txBuffer[14] = at24c16_read(EE_ADDR_MAC_FOUR);	
				txBuffer[15] = at24c16_read(EE_ADDR_MAC_FIVE);	
				txBuffer[16] = at24c16_read(EE_ADDR_MAC_SIX);
				txBuffer[17] = at24c16_read(EE_ADDR_SUBNET_ONE);	
				txBuffer[18] = at24c16_read(EE_ADDR_SUBNET_TWO);	
				txBuffer[19] = at24c16_read(EE_ADDR_SUBNET_THREE);	
				txBuffer[20] = at24c16_read(EE_ADDR_SUBNET_FOUR);				
				txBuffer[21] = at24c16_read(EE_ADDR_GATEWAY_ONE);	
				txBuffer[22] = at24c16_read(EE_ADDR_GATEWAY_TWO);	
				txBuffer[23] = at24c16_read(EE_ADDR_GATEWAY_THREE);	
				txBuffer[24] = at24c16_read(EE_ADDR_GATEWAY_FOUR);	
				txBuffer[25] = at24c16_read(EE_ADDR_DNS_ONE);	
				txBuffer[26] = at24c16_read(EE_ADDR_DNS_TWO);	
				txBuffer[27] = at24c16_read(EE_ADDR_DNS_THREE);	
				txBuffer[28] = at24c16_read(EE_ADDR_DNS_FOUR);	
				txBuffer[29] = at24c16_read(EE_ADDR_PORT_1);
				txBuffer[30] = at24c16_read(EE_ADDR_PORT_2);
				sum = 0;
				for(int k = 0; k < 36; k++)
				{
						sum+=txBuffer[k];	
				}
				txBuffer[36]=sum;
				for(int k=0; k < 37; k++)
				{
						USART_SendData(USART2,txBuffer[k]);
				}
			}
		}
		if(usartRev2.buffer[1] == 0x02)
		{
			if(usartRev2.buffer[3] == 0x01)	//目的IP
			{
				uart2_send_write = 1;
				at24c16_write(EE_ADDR_IP_ONE, usartRev2.buffer[4]);
				at24c16_write(EE_ADDR_IP_TWO, usartRev2.buffer[5]);
				at24c16_write(EE_ADDR_IP_THREE, usartRev2.buffer[6]);
				at24c16_write(EE_ADDR_IP_FOUR, usartRev2.buffer[7]);	
				at24c16_write(EE_ADDR_W5500_CONFIG_IP,EE_ADDR_W5500_PC_CONFIG_SUCCESS);	
				
			}
			if(usartRev2.buffer[3] == 0x02)	//分配IP
			{
				uart2_send_write = 1;
				at24c16_write(EE_ADDR_DHCP_IP_ONE, usartRev2.buffer[4]);
				at24c16_write(EE_ADDR_DHCP_IP_TWO, usartRev2.buffer[5]);
				at24c16_write(EE_ADDR_DHCP_IP_THREE, usartRev2.buffer[6]);
				at24c16_write(EE_ADDR_DHCO_IP_FOUR, usartRev2.buffer[7]);
				at24c16_write(EE_ADDR_W5500_CONFIG_DHCP_IP,EE_ADDR_W5500_PC_CONFIG_SUCCESS);
			}
			if(usartRev2.buffer[3] == 0x03)	//MAC
			{
				uart2_send_write = 1;
				at24c16_write(EE_ADDR_MAC_ONE, usartRev2.buffer[4]);
				at24c16_write(EE_ADDR_MAC_TWO, usartRev2.buffer[5]);
				at24c16_write(EE_ADDR_MAC_THREE, usartRev2.buffer[6]);
				at24c16_write(EE_ADDR_MAC_FOUR, usartRev2.buffer[7]);
				uart_w5500_mac_config = 1;
			}
			if(usartRev2.buffer[3] == 0x04)	//MAC
			{
				uart2_send_write = 1;
				at24c16_write(EE_ADDR_MAC_FIVE, usartRev2.buffer[4]);
				at24c16_write(EE_ADDR_MAC_SIX, usartRev2.buffer[5]);
				if(uart_w5500_mac_config == 1)
				{
					uart_w5500_mac_config = 0;
					at24c16_write(EE_ADDR_W5500_CONFIG_MAC,EE_ADDR_W5500_PC_CONFIG_SUCCESS);
				}
			}
			if(usartRev2.buffer[3] == 0x08)	//端口
			{
				uart2_send_write = 1;
				at24c16_write(EE_ADDR_PORT_1, usartRev2.buffer[4]);
				at24c16_write(EE_ADDR_PORT_2, usartRev2.buffer[5]);
				local_port = usartRev2.buffer[4]<<8|usartRev2.buffer[5];
				uart_w5500_port_config = 1;
			}
			if(usartRev2.buffer[3] == 0x10)
			{
				uart2_send_write = 1;   
				at24c16_write(EE_ADDR_ID_NAME1, usartRev2.buffer[4]);
				at24c16_write(EE_ADDR_ID_NAME2, usartRev2.buffer[5]);
				at24c16_write(EE_ADDR_ID_NAME3, usartRev2.buffer[6]);
				at24c16_write(EE_ADDR_ID_NAME4, usartRev2.buffer[7]);
			}
		}
		if(uart2_send_read == 1)				
		{
			uart2_send_read = 0;
			sum = 0;
			for(int k = 0; k < 9; k++)
			{
					sum+=txBuffer[k];	
			}
			txBuffer[9]=sum;
			for(int k=0; k < 10; k++)
			{
					USART_SendData(USART2,txBuffer[k]);
				  delay_us(100);
			}
		}
		if(uart2_send_write == 1)
		{
			sum = 0;
			uart2_send_write = 0;
				txBuffer[0] = 0x24;
				txBuffer[1] = 0x10+usartRev2.buffer[2];
				txBuffer[2] = 0x06;	
				txBuffer[3] = 0x00;	
				txBuffer[4] = 0x00;	
				txBuffer[5] = 0x00;	
				txBuffer[6] = 0x00;	
				txBuffer[7] = 0x00;		
				txBuffer[8] = 0x00;	
			for(int k = 0; k < 9; k++)
			{
					sum+=txBuffer[k];	
			}
			txBuffer[9] = sum;
			for(int k=0; k < 10; k++)
			{
					USART_SendData(USART2,txBuffer[k]);
					delay_us(100);
			}
		}
	}	
}


void GetLockCode(void)
{
	CpuID[0]=*(vu32*)(0x1ffff7e8);
	CpuID[1]=*(vu32*)(0x1ffff7ec);
	CpuID[2]=*(vu32*)(0x1ffff7f0);

	
	Lock_Code=(CpuID[0]>>1)+(CpuID[1]>>2)+(CpuID[2]>>3);
	printf("唯一id：%d\r\n",Lock_Code);
}

void SoftReset(void)
{
	__set_FAULTMASK(1);      						
	NVIC_SystemReset();									
}

void USART_SendDatacmd(USART_TypeDef* USARTx,unsigned char ch)
{
	while((USARTx->SR&USART_SR_TXE)==0);
	USARTx->DR=ch;
}

void Power_check(void)
{
	if(((GPIOA->IDR & 0x0002) == 2) || ((GPIOA->IDR & 0x0002) == 1))
	{
		  WORK_LED_NORMAL_ON;
			WORK_LED_ERROR_OFF;
	}
	else
	{
		WORK_LED_NORMAL_OFF;
		WORK_LED_ERROR_ON;
	}
}
void read_config(void)
{
	printf_version = at24c16_read(EE_APP_VERSION1);
	printf("当前程序版本:V%d.%d\r\n",printf_version/10,printf_version%10);
	W5500_REBOOT = at24c16_read(EE_ADDR_REBOOT);
	printf("W5500_REBOOT = %d\r\n",W5500_REBOOT);
	if(at24c16_read(EE_ADDR_REBOOT) == 0x01)
	{
		  at24c16_write(EE_ADDR_REBOOT, 0x00);
			W5500_REBOOT = 1;
	}		
	if(at24c16_read(EE_ADDR_MQTT_SEND_TIME) == 0xFF)
	{
		mqtt_send_time = 10;
	}
	else
	{
		mqtt_send_time = at24c16_read(EE_ADDR_MQTT_SEND_TIME);		
	}
	printf("mqtt_send_time = %ds.\r\n",mqtt_send_time);
	if(at24c16_read(EE_ADDR_REBOOT_SUM) == 0xFF)
	{
		reboot_sum = 0;
		at24c16_write(EE_ADDR_REBOOT_SUM, 0x00);
	}
	else
	{
			reboot_sum = at24c16_read(EE_ADDR_REBOOT_SUM);		
	}
	printf("reboot_sum = %ds.\r\n",reboot_sum);
	id_name[0] = (Lock_Code >> 24)&0xff;
	id_name[1] = (Lock_Code >> 16)&0xff;
	id_name[2] = (Lock_Code >> 8)&0xff;
	id_name[3] = Lock_Code&0xff;
	id_name_all = id_name[0] << 24 | id_name[1] << 16 | id_name[2] << 8 | id_name[3];
	//id_name_all = Lock_Code;
	printf("唯一id：%x,%x,%x,%x\r\n",id_name[0],id_name[1],id_name[2],id_name[3]);
	ip_config = at24c16_read(EE_ADDR_IP_SUM);
	if(ip_config == 0x00)
	{
		BD_TG_server_ip[0] = 222;
		BD_TG_server_ip[1] = 32;
		BD_TG_server_ip[2] = 108;
		BD_TG_server_ip[3] = 88;
		BD_TG_server_port = 1883;
	}
	else
	{
		BD_TG_server_ip[0] = 10;
		BD_TG_server_ip[1] = 10;
		BD_TG_server_ip[2] = 10;
		BD_TG_server_ip[3] = 10;
		BD_TG_server_port = 1883;
	}
	printf("connect ip: %d.%d.%d.%d,port:%d\r\n",BD_TG_server_ip[0],BD_TG_server_ip[1],BD_TG_server_ip[2],BD_TG_server_ip[3],BD_TG_server_port);
	
}



