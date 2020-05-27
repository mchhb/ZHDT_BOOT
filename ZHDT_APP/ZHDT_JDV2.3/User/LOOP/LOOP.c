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
#include "ADXL345.h"
#include "24c16.h"
#include "exti.h"
#include "MqttKit.h"
#include "tcp_demo.h"
#include "socket.h"
#include "BD_TG.h"
#include "input.h"


//#include "MQTTConnectClient.h"
extern __IO uint16_t ADC_ConvertedValue;// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_Result[1];
extern uint16 local_port;
extern uint8 BD_TG_ping_pak[2];
extern uint8 buff[256];	
extern unsigned char w5500_buf[128];   //֮ǰ��128
extern volatile unsigned int  tim3fquence;	//��ʱ�жϴ���
extern int tim3i;
unsigned char id_name[4];
unsigned char result[20] = {0x66,0x04,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
														0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x99};
//extern char *DEVID;
void GetLockCode(void);
void USART_SendDatacmd(USART_TypeDef* USARTx,unsigned char ch);
void Power_check(void);
//------------------------------
uint16_t publish_rev_head = 20; 														
uint8_t send_version = 1;														
uint8_t id_length = 0;														
uint8_t pubilish_id[10];
u32 pubilish_id_32;
uint16_t publish_rev = 0;	
uint8_t publish_cmd = 0;
uint8_t publish_version = 0;																
uint8_t 	ip_config;
uint8_t printf_version;
														
u32 id_name_all;
u32 Lock_Code;
u32 CpuID[3];
														
Number x;
Number y;
Number z;
//------------------------------
unsigned char  flag;
uint8_t   power = 0;
uint8_t bed_ready = 0;
uint8_t mqtt_sensor_send = 1;
extern u32 door_close_times_mqtt;
uint8_t   door_close_times;
uint8_t   door_close_times_sum;
uint8_t 	w5500_success_crc = 0;
uint8_t   door_close_times_sum1;
uint8_t   door_close_times_sum2;
uint8_t   ee_test = 0;
uint8_t 	read_dht11 = 0;
uint8_t 	send_data = 0;
uint8 Disconnect_Request[2] = {0xe0,0x00};
uint8_t 	domain_ip[4]={198,41,30,241};
uint8_t 	temp_rh[2];
uint8_t   uart2_send_read = 0;
int floor = 1;
uint8_t door_open_over_time_sum = 0;
uint8_t   uart2_send_write = 0;
uint8_t   uart_config = 0;
uint32_t 	w5500_connect = 0;
uint8_t   uart_w5500_ip_config = 0;
uint8_t   uart_w5500_mac_config = 0;
uint8_t   uart_w5500_subnet_config = 0;
uint8_t   uart_w5500_gateway_config = 0;
uint8_t   uart_w5500_dns_config = 0;
uint8_t   uart_w5500_port_config = 0;
float 	AccelerationX;
float 	AccelerationY;
float 	AccelerationZ;
int 	adxl_val[3];
uint8_t adxl345_number = 0;
uint8_t reboot_sum = 0;
//------------------------
uint8_t work_ACCadd = 0;
uint8_t work_Acc_success = 0;
uint8_t work_dht11_status;
uint8_t work_noise_status;
uint8_t work_uart_status = 0;
uint8_t work_Acceleration_status;
uint16 disconnet=0;
uint16 overtime=0;
uint8_t overtime_reset = 0;
uint8_t  test_while = 0;
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
extern uint8_t up_bed;
extern uint8_t down_bed;
float temperature,humidity;
uint8_t external_driver = 0;
uint8_t external_led1 = 0;
uint8_t external_led2 = 0;
uint8_t external_led3 = 0;
uint8_t external_led4 = 0;
uint8_t publish_crc = 0;
uint8_t external_led_open = 0;
uint8_t input1_printf = 0; 
uint8_t input2_printf = 0; 
uint8_t input3_printf = 0; 
uint8_t input4_printf = 0; 
uint8_t input5_printf = 0; 
uint8_t input6_printf = 0; 
uint8_t input2_up = 0;
uint8_t input1_up = 0;
uint8_t door_not_close = 1;
uint8_t door_mqtt_status = 0;
uint8_t door_not_close_printf = 0;
uint8_t pingceng = 0;
uint8_t driver_start = 0;
uint8_t driver_status = 0;
uint8_t driver1_status = 0;
uint8_t driver2_status = 0;
uint8_t mqtt_send_time = 0;
int rc=0;	
int test_time = 0;
int test_time1 = 0;
uint8_t power_status;
uint8_t battery_status = 9;
uint8_t power_time = 0;
uint8_t power_status = 0;
uint8_t battery_time = 0;
uint8 txBuffer[40];
uint8_t domain_name[]="www.embed-net.com";
uint8_t w5500_connect_success = 0;
uint8_t W5500_REBOOT = 0;
uint8_t door_open_times = 0;
uint8_t door_over_time_alway = 0;
uint8_t door_printf_mqtt = 0;
uint8_t pingceng_printf_mqtt = 0;
uint8_t pingceng_error = 0;
void read_config(void);

void Initialization_configuration(void) {

	delay_ms(1000);
	
	IWDG_Init(4,3750);//((prer)*rlr)/40;  6s
	
	TIM2_Configuration();

	TIM2_NVIC_Configuration();
	
	TIM3_Configuration();

	TIM3_NVIC_Configuration();
	
	TIM4_Configuration();

	TIM4_NVIC_Configuration();
	
	GPIO_Configuration();
	
	input_gpio();
	
	USART2_NVIC_Configuration();

	WIZ_SPI_Init();
	
	at24c16_init();

	ADXL345_init();
		
	EXTI_ENABLE();
	
	USART2_Config();	

	Power_check();

	GetLockCode();
	
	Reset_W5500();

  w5500_check();	

	set_w5500_mac();
	
	set_object_ip();
	
	read_config();

	sysinit(txsize, rxsize); 													// ��ʼ��8��socket

  printf("W5500 Init Complete!\r\n");
	
	START_TIME2;
	
	START_TIME4;
}
void application(void)
{
	IWDG_Feed();
	if(rxd_buffer_locked != 1)
	{
		w5500_check();
	}
	if(dhcp_ok != 1)
	{
		auto_dhcp();
	}
	delay_ms(100);
	if(dhcp_ok==1)
	{	
			do_tcp_client();                    /*TCP_Client ���ݻػ����Գ���*/
			delay_ms(100);
	}
	if(disconnet==1)
	{
		disconnet=0;
		dhcp_ok = 0;
		//send(SOCK_TCPC,Restore_factory,10);//��������
		Reset_W5500();												/*Ӳ��λW5500*/
		w5500_check();													/*��������Ƿ����*/   
		set_w5500_mac();											/*����MAC��ַ*/
		set_w5500_ip();
		//dhcp_config_w5500();
		sysinit(txsize, rxsize); 													// ��ʼ��8��socket
		MQTT_STATE =MQTT_PKT_CONNECT;			
	}
	if(driver_start == 1)
	{
		if(pingceng == 1)
		{
				if(t2_count_ms[T2_COUNTER_MS_30000] >= T2_TIMEOUT_MS_30000)  //30s
				{
					t2_count_ms[T2_COUNTER_MS_30000] = 0;	 
					MQTT_STATE = MQTT_PKT_PUBLISH;		
//					printf("30S��ʱ��δƽ�㱨���� \r\n");	
					pingceng_printf_mqtt = 1;
				}
		}
    else
		{
			t2_count_ms[T2_COUNTER_MS_30000] = 0;	 
			pingceng_error = 0;
		}
	}
	if(t2_count_ms[T2_COUNTER_MS_10000] >= T2_TIMEOUT_MS_10000)  //10s
	{
		t2_count_ms[T2_COUNTER_MS_10000] = 0;	 
		test_time++;
		send(SOCK_TCPC,BD_TG_ping_pak,2);//��������      						
		delay_ms(100);
		if(buff[0]==0xd0)
		{		 	 	
			MQTT_STATE = MQTT_PKT_PINGREQ;
			overtime = 0;
			overtime_reset = 0;
			memset(buff,0,sizeof(buff));
			flag =1;	
	//		printf("�յ�����\r\n");
	 		network_status = 0;
			driver_start = 1; 
		}	
		else 
		{
			overtime++;
			overtime_reset++;
			if(overtime > 2)
			{
			MQTT_STATE = MQTT_PKT_DISCONNECT;
			printf("δ�յ����������� %d\r\n",overtime);
			}
			if(overtime>3)
			{
				printf("������ʱ���������ӣ�\r\n");
				overtime=0;
				disconnet=1;
				driver_start = 0;
				send(SOCK_TCPC,Disconnect_Request,2);//���Ͷ���
				flag=0;					
				memset(buff,0,sizeof(buff));
				memset(w5500_buf,0,sizeof(w5500_buf));
			}
		}			
	}
	if(t2_count_ms[21] >= (mqtt_send_time*10000))  //30s
	{
		t2_count_ms[21] = 0;
		test_time1++;
		if((flag == 1)&&(overtime<1))//����
		{	   
		  	printf("������Ϣ\r\n");
				w5500_success_crc++;
			  mqtt_sensor_send = 1;
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
	if(t2_count_ms[T2_COUNTER_MS_0] >= T2_TIMEOUT_MS_0)  //100us
	{
		t2_count_ms[T2_COUNTER_MS_0] = 0;
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
		test_while++;
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
	if((t2_count_ms[T2_COUNTER_MS_60000] >= T2_TIMEOUT_MS_60000)&&(input3_value == 0)&&(door_over_time_alway > 0))  //400s
	{
		t2_count_ms[T2_COUNTER_MS_60000] = 0;
		MQTT_STATE = MQTT_PKT_PUBLISH;		
	//  printf("���������� �� \r\n");	
		door_printf_mqtt = 1;		
	}
	if(t2_count_ms[T2_COUNTER_MS_2000] >= T2_TIMEOUT_MS_2000)  //2s
	{
		t2_count_ms[T2_COUNTER_MS_2000] = 0;

		DHT11_GetValue(temp_rh);
		temperature = temp_rh[1];
		humidity = temp_rh[0];
		if((temperature > 50) || (humidity > 90) ||(temperature == 0)||(humidity == 0))
		{
			//printf("temperature  is %f\r\n",temperature);
			//printf("humidity     is %f\r\n",humidity);
			work_dht11_status = 1;
		}
		else
		{
			work_dht11_status = 0;
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
				power = 3;
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
				power = 2;
				power_status = 0;
				printf("acc is opened !\r\n");
			}
		}
	}
	if((battery_status == 1)&&(battery_time != 0))
	{
		if(t2_count_ms[T2_COUNTER_MS_250] >= T2_TIMEOUT_MS_250)  //250ms
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
		if(t2_count_ms[T2_COUNTER_MS_250] >= T2_TIMEOUT_MS_250)  //250ms
		{
			t2_count_ms[T2_COUNTER_MS_250] = 0;
			if(battery_status == 0)
			{
				battery_time++;
				printf("battery is low !\r\n");
			}
		}
	}
	if(t2_count_ms[T2_COUNTER_MS_5] >= T2_TIMEOUT_MS_5)  //10s
	{
		t2_count_ms[T2_COUNTER_MS_5]=0;
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
		if((publish_buf[0] == '{')&&(publish_buf[9] == 0x31))
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
							printf("�յ�����ָ��.\r\n");
							publish_version = (publish_buf[sub_rev_length-5]-0x30)*10+ (publish_buf[sub_rev_length-3]-0x30);
							printf("���°汾Ϊ:V%d.%d.\r\n",publish_version/10,publish_version%10);
							if(publish_version > at24c16_read(EE_APP_VERSION1))
							{
								printf("���������������汾Ϊ��V%d.%d.\r\n",publish_version/10,publish_version%10);
								at24c16_write(EE_APP_VERSION2,publish_version);
								delay_ms(1000);
								SoftReset();
							}
							else
							{
								send_version = 1;
								printf("�汾Ϊ����.\r\n");
							}
							publish_rev_head = 20;
							pubilish_id_32 = 0;
							break;
						}
						else
						{
							if(publish_buf[publish_rev+1] == ']')
							{
								printf("���Ǳ��豸����\r\n");
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
	
	if(driver_start == 1)
	{
//				input1_status();
//				input2_status();
//				if((input1_value == 1) && (input2_value == 1))
//				{
//					PINGCENG_SUCCESS_ON;
//					PINGCENG_FALL_OFF;					
//				}
//				else
//				{
//					PINGCENG_SUCCESS_OFF;
//					PINGCENG_FALL_OFF;			
//				}
//			if((driver1_status == 1)||(driver2_status == 1)||(driver_status == 0))
//			{
//				if((input1_value == 1) && (input2_value == 0))
//				{
//					printf("���ڵ�¥�������� ��\r\n");
//					driver1_status = 3;
//					driver2_status = 0;
//					driver_status = 1;
//					input1_up = 0;
//					input2_up = 0;
//					pingceng = 1;
//				}
//				if((input1_value == 0) &&(input2_value ==1))
//				{
//					printf("���ڵ�¥�������� ��\r\n");
//					driver1_status = 0;
//					driver2_status = 3;
//					driver_status = 1;
//					input1_up = 0;
//					input2_up = 0;
//					pingceng = 1;
//				}
//			}
//			else
//			{
//				input1_status();
//				if(input1_value == 1)
//				{
//					if(driver2_status ==3)
//					{
//						input2_up = 4;
//						driver2_status = 0;
//					}
//					else if((input2_up != 5)&&(driver1_status != 3))
//					{
//						input1_up = 4;
//					}
//					else if(input2_up == 5)
//					{
//							if(input1_printf == 0)
//							{
//								--floor;
//								if(floor == 0)
//								{
//									--floor;
//								}
//						//		printf("�����Ѿ���λ ��\r\n");
//								printf("���ڵ�¥����  %d  �� ��\r\n",floor);
//								pingceng = 0;
//								input1_printf++;
//								input2_up = 0;
//								input1_up = 0;
//								driver1_status = 1;
//							}
//					}
//					
//				}
//				else
//				{
//					pingceng = 1;
//					input1_printf = 0;
//				}
//				input2_status();
//				if(input2_value == 1)
//				{
//					if(driver1_status ==3)
//					{
//						input2_up = 5;
//						driver1_status = 0;
//					}
//					else if((input1_up != 4)&&(driver2_status != 3))
//					{
//						input2_up = 5;
//					}
//					else if(input1_up == 4)
//					{
//							if(input2_printf == 0)
//							{
//								++floor;
//								if(floor == 0)
//								{
//									++floor;
//								}
//								pingceng = 0;
//						//		printf("�����Ѿ���λ ��\r\n");
//								printf("���ڵ�¥����  %d  ��\r\n",floor);
//								input2_printf++;
//								input1_up = 0;
//								input2_up = 0;
//								driver2_status = 1;
//							}
//					}
//				}
//				else
//				{
//					pingceng = 1;
//					input2_printf = 0;
//				}
//			}
			input3_status();
			if(input3_value == 1)
			{
				if(input3_printf == 0)
				{
				//	printf("���ѹص�λ \r\n");
					input3_printf++;
					door_close_times++;
					if(door_close_times == 0xff)
					{
						door_close_times_sum++;
						if(door_close_times_sum == 0xff)
						{
							door_close_times_sum = 0;
							door_close_times_sum1++;
							if(door_close_times_sum1 == 0xff)
							{
								door_close_times_sum1 = 0;
								door_close_times_sum2++;
								at24c16_write(EE_ADDR_DOOR_CLOSE_TIMES_SUM2,door_close_times_sum2);
							}
							at24c16_write(EE_ADDR_DOOR_CLOSE_TIMES_SUM1,door_close_times_sum1);
						}
						door_close_times = 0;
						at24c16_write(EE_ADDR_DOOR_CLOSE_TIMES_SUM,door_close_times_sum);
					}
					at24c16_write(EE_ADDR_DOOR_CLOSE_TIMES,door_close_times);
				}
				door_not_close_printf = 0;
				door_not_close = 0;
				t2_count_ms[T2_COUNTER_MS_120000] = 0;
				t2_count_ms[T2_COUNTER_MS_60000] = 0;
				DOOR_SUCCESS_ON;
				DOOR_FAIL_OFF;
				door_over_time_alway = 0;
				door_open_over_time_sum = 0;
				door_mqtt_status = 0;
			}
			else
			{
				if((t2_count_ms[T2_COUNTER_MS_120000] >= T2_TIMEOUT_MS_120000)&&(door_over_time_alway == 0))
				{
					if(door_not_close_printf == 0)
					{
						door_open_times = 1;
					//	printf("�ų�ʱ��δ�أ����͹��ϱ��� ! \r\n");
						t2_count_ms[T2_COUNTER_MS_60000] = 0;
						t2_count_ms[T2_COUNTER_MS_120000] = 0;
						door_not_close_printf = 0;
						door_mqtt_status = 1;
					}
				}
				if(door_mqtt_status == 1)
				{
					door_not_close = 2;
				}
				else
				{
					door_not_close = 1;
				}
				input3_printf = 0;
				DOOR_SUCCESS_OFF;
				DOOR_FAIL_ON;
			}
			input4_status();
			if(input4_value == 0)
			{
				if(input4_printf == 0)
				{
					floor = 1;
			//		printf("���ڵ�¥����  %d  ��\r\n",floor);
					driver1_status = 0;
					driver2_status = 0;
					input1_up = 0;
					input2_up = 0;
					driver_status = 0;
					input4_printf++;
				}
				JIZHUNCENG_SUCCESS_ON;
			}
			else
			{
				input4_printf = 0;
				JIZHUNCENG_SUCCESS_OFF;
			}
			input5_status();
			if(input5_value == 1)
			{
				if(input5_printf == 0)
				{
		//			printf("���ع��� ��\r\n");
		//			printf("���ڵ����ѵ����� ! \r\n");
					input5_printf++;
				}
				TOP_YES;
				BUTTON_NO;
			}
			else
			{
				TOP_NO;
				input5_printf = 0;
			}
			input6_status();
			if(input6_value == 1)
			{
				if(input6_printf == 0)
				{
		//			printf("���ع��� ��\r\n");
		//			printf("���ڵ����ѵ��ײ� ! \r\n");
					input6_printf++;
				}
					TOP_NO;
					BUTTON_YES;
			}
			else
			{
				BUTTON_NO;
				input6_printf = 0;
			}
	}
	if(rxd_buffer_locked == 1)
	{
		tim3i = 0;
		tim3fquence=0;
		driver_start = 0;
		rxd_buffer_locked = 0;
		if(usart_buf[1] == 0x01)
		{
			if(usart_buf[3] == 0x01)	//Ŀ��IP
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
			if(usart_buf[3] == 0x02)	//����IP
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
			if(usart_buf[3] == 0x03)	//MAC
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
			if(usart_buf[3] == 0x04)	//��������
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
			if(usart_buf[3] == 0x05)	//����
			{
	
			}
			if(usart_buf[3] == 0x06)	//DNS
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
			if(usart_buf[3] == 0x07)	//�˿�
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
			if(usart_buf[3] == 0x08)	//�¶�ʪ�ȼ��ٶ�
			{
				ADXL345_ReadXYZ(adxl_val);
				AccelerationX = adxl_val[0];
				AccelerationY = adxl_val[1];
				AccelerationZ = adxl_val[2];
				DHT11_GetValue(temp_rh);
				temperature = temp_rh[1];
				humidity = temp_rh[0];
				uart2_send_read = 1;
				txBuffer[0] = 0x24;
				txBuffer[1] = 0x08;
				txBuffer[2] = 0x06;	
				txBuffer[3] = (int)temperature;	
				txBuffer[4] = (int)humidity;	
				txBuffer[5] = AccelerationX;	
				txBuffer[6] = AccelerationY;	
				txBuffer[7] = AccelerationZ;	
				txBuffer[8] = 0x00;		
			}
			if(usart_buf[3] == 0x09)	//����EEprom
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
			if(usart_buf[3] == 0x10)	//CPU��λ
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
			if(usart_buf[3] == 0x13)	//�Ŵ���
			{
				uart2_send_read = 1;
				txBuffer[0] = 0x24;
				txBuffer[1] = 0x13;
				txBuffer[2] = 0x06;	
				txBuffer[3] = at24c16_read(EE_ADDR_DOOR_CLOSE_TIMES_SUM2);	
				txBuffer[4] = at24c16_read(EE_ADDR_DOOR_CLOSE_TIMES_SUM1);	
				txBuffer[5] = at24c16_read(EE_ADDR_DOOR_CLOSE_TIMES_SUM);	;	
				txBuffer[6] = at24c16_read(EE_ADDR_DOOR_CLOSE_TIMES);	;	
				txBuffer[7] = 0x00;	
				txBuffer[8] = 0x00;				
			}
//			if(usart_buf[3] == 0x11)	//��PC��־λ
//			{
//				uart2_send_read = 1;
//				txBuffer[0] = 0x24;
//				txBuffer[1] = 0x11;
//				txBuffer[2] = 0x06;	
//				txBuffer[3] = 0;	
//				txBuffer[4] = 0x00;	
//				txBuffer[5] = 0x00;	
//				txBuffer[6] = 0x00;	
//				txBuffer[7] = 0x00;	
//				txBuffer[8] = 0x00;		
//				at24c16_write(EE_ADDR_W5500_CONFIG, EE_ADDR_W5500_PC_CONFIG_SUCCESS);
//			}
			if(usart_buf[3] == 0x12)	//һ����ȡ
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
				ADXL345_ReadXYZ(adxl_val);
				AccelerationX = adxl_val[0];
				AccelerationY = adxl_val[1];
				AccelerationZ = adxl_val[2];
				DHT11_GetValue(temp_rh);
				temperature = temp_rh[1];
				humidity = temp_rh[0];
				txBuffer[31] = (int)temperature;	
				txBuffer[32] = (int)humidity;	
				txBuffer[33] = AccelerationX;	
				txBuffer[34] = AccelerationY;	
				txBuffer[35] = AccelerationZ;	
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
		if(usart_buf[1] == 0x02)
		{
			if(usart_buf[3] == 0x01)	//Ŀ��IP
			{
				uart2_send_write = 1;
				at24c16_write(EE_ADDR_IP_ONE, usart_buf[4]);
				at24c16_write(EE_ADDR_IP_TWO, usart_buf[5]);
				at24c16_write(EE_ADDR_IP_THREE, usart_buf[6]);
				at24c16_write(EE_ADDR_IP_FOUR, usart_buf[7]);	
				at24c16_write(EE_ADDR_W5500_CONFIG_IP,EE_ADDR_W5500_PC_CONFIG_SUCCESS);	
				
			}
			if(usart_buf[3] == 0x02)	//����IP
			{
				uart2_send_write = 1;
				at24c16_write(EE_ADDR_DHCP_IP_ONE, usart_buf[4]);
				at24c16_write(EE_ADDR_DHCP_IP_TWO, usart_buf[5]);
				at24c16_write(EE_ADDR_DHCP_IP_THREE, usart_buf[6]);
				at24c16_write(EE_ADDR_DHCO_IP_FOUR, usart_buf[7]);
				at24c16_write(EE_ADDR_W5500_CONFIG_DHCP_IP,EE_ADDR_W5500_PC_CONFIG_SUCCESS);
			}
			if(usart_buf[3] == 0x03)	//MAC
			{
				uart2_send_write = 1;
				at24c16_write(EE_ADDR_MAC_ONE, usart_buf[4]);
				at24c16_write(EE_ADDR_MAC_TWO, usart_buf[5]);
				at24c16_write(EE_ADDR_MAC_THREE, usart_buf[6]);
				at24c16_write(EE_ADDR_MAC_FOUR, usart_buf[7]);
				uart_w5500_mac_config = 1;
			}
			if(usart_buf[3] == 0x04)	//MAC
			{
				uart2_send_write = 1;
				at24c16_write(EE_ADDR_MAC_FIVE, usart_buf[4]);
				at24c16_write(EE_ADDR_MAC_SIX, usart_buf[5]);
				if(uart_w5500_mac_config == 1)
				{
					uart_w5500_mac_config = 0;
					at24c16_write(EE_ADDR_W5500_CONFIG_MAC,EE_ADDR_W5500_PC_CONFIG_SUCCESS);
				}
			}
			if(usart_buf[3] == 0x08)	//�˿�
			{
				uart2_send_write = 1;
				at24c16_write(EE_ADDR_PORT_1, usart_buf[4]);
				at24c16_write(EE_ADDR_PORT_2, usart_buf[5]);
				local_port = usart_buf[4]<<8|usart_buf[5];
				uart_w5500_port_config = 1;
			}
			if(usart_buf[3] == 0x09)
			{
				uart2_send_write = 1;
				at24c16_write(EE_ADDR_DOOR_CLOSE_TIMES, usart_buf[4]);
				at24c16_write(EE_ADDR_DOOR_CLOSE_TIMES_SUM, usart_buf[5]);
				at24c16_write(EE_ADDR_DOOR_CLOSE_TIMES_SUM1, usart_buf[6]);
				at24c16_write(EE_ADDR_DOOR_CLOSE_TIMES_SUM2, usart_buf[7]);
			}
			if(usart_buf[3] == 0x10)
			{
				uart2_send_write = 1;
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
				txBuffer[1] = 0x10+usart_buf[2];
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
	printf("Ψһid��%d\r\n",Lock_Code);
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
		WORK_LED_NORMAL_OFF;
		WORK_LED_ERROR_ON;
		power = 3;
	}
	else
	{
			WORK_LED_NORMAL_ON;
			WORK_LED_ERROR_OFF;
			power = 2;
	}
}

void read_config(void)
{
	printf_version = at24c16_read(EE_APP_VERSION1);
	printf("��ǰ����汾:V%d.%d\r\n",printf_version/10,printf_version%10);
	if(at24c16_read(EE_ADDR_REBOOT) == 0x01)
	{
		  at24c16_write(EE_ADDR_REBOOT, 0x00);
			W5500_REBOOT = 1;
	}	
	if((at24c16_read(EE_ADDR_DOOR_CLOSE_TIMES) == 0xFF) && (	at24c16_read(EE_ADDR_DOOR_CLOSE_TIMES_SUM) == 0xFF)&& (	at24c16_read(EE_ADDR_DOOR_CLOSE_TIMES_SUM1) == 0xFF) && ( at24c16_read(EE_ADDR_DOOR_CLOSE_TIMES_SUM2) == 0xFF))
	{
		door_close_times = 0;
		door_close_times_sum = 0;
		door_close_times_sum1 = 0;
		door_close_times_sum2 = 0;
		at24c16_write(EE_ADDR_DOOR_CLOSE_TIMES, 0x00);
		at24c16_write(EE_ADDR_DOOR_CLOSE_TIMES_SUM, 0x00);
		at24c16_write(EE_ADDR_DOOR_CLOSE_TIMES_SUM1, 0x00);
		at24c16_write(EE_ADDR_DOOR_CLOSE_TIMES_SUM2, 0x00);
		printf("�Źش���Ϊ0��\r\n");
	}
	else
	{
		door_close_times = at24c16_read(EE_ADDR_DOOR_CLOSE_TIMES);
		door_close_times_sum = at24c16_read(EE_ADDR_DOOR_CLOSE_TIMES_SUM);
		door_close_times_sum1 = at24c16_read(EE_ADDR_DOOR_CLOSE_TIMES_SUM1);
		door_close_times_sum2 = at24c16_read(EE_ADDR_DOOR_CLOSE_TIMES_SUM2);
		door_close_times_mqtt = door_close_times+(door_close_times_sum*255)+(door_close_times_sum1*255*255)+(door_close_times_sum2*255*255*255);
		printf("�Źش��� ��%d��  ��\r\n",door_close_times_mqtt);
	}
	if(at24c16_read(EE_ADDR_MQTT_SEND_TIME) == 0xFF)
	{
		mqtt_send_time = 10;
	}
	else
	{
		mqtt_send_time = at24c16_read(EE_ADDR_MQTT_SEND_TIME);
	}
	printf("mqtt_send_time is %d.\r\n",mqtt_send_time);
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
	printf("Ψһid��%x,%x,%x,%x\r\n",id_name[0],id_name[1],id_name[2],id_name[3]);
	if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == 0)&&(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0))
	{
		bed_ready = 2;
		pingceng = 0;
		printf("����ƽ��\r\n");
	}
	else
	{
		pingceng = 1;
	}
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
