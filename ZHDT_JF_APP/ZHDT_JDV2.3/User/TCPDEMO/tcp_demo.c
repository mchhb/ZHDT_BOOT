/**
******************************************************************************
* @file    			tcp_demo.c                                               
* @version 			V1.2										  		
* @date    			2017-11-01								 
* @brief 			  TCP ÑÝÊ¾º¯Êý     					        
*
* @company  		ÉîÛÚì¿ÊÀ¿Æ¼¼ÓÐÏÞ¹«Ë¾
* @information  WIZnet W5500¹Ù·½´úÀíÉÌ£¬È«³Ì¼¼ÊõÖ§³Ö£¬¼Û¸ñ¾ø¶ÔÓÅÊÆ£¡
* @website  		www.wisioe.com																							 	 				
* @forum        www.w5500.cn																								 		
* @qqGroup      383035001																										 
******************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include "tcp_demo.h"
#include "W5500_config.h"
#include "w5500.h"
#include "socket.h"
#include "BD_TG.h"
#include "MqttKit.h"
#include "delay.h"
#include "LOOP.h"
#include "24c16.h"
#include "input.h"
uint8 buff[2048];				                             /*¶¨ÒåÒ»¸ö2KBµÄ»º´æ*/
uint8  BD_TG_server_ip[4] = {111,43,137,206};//{202,106,5,12};        //BD_TG·þÎñÆ÷IPµØÖ·
uint16 BD_TG_server_port  = 1883;                   //BD_TG·þÎñÆ÷¶Ë¿ÚºÅ


int MQTT_STATE = MQTT_PKT_CONNECT;   //Á¬½Ó
const char *topics[2] = {"sensor-data","/alarm"}; //Ô­À´µÄÖ÷ÌâÃû³ÆstateUp
const char *topics2[] = {"/alarm"};
const char *topics1[] = {"/v2/conf"}; 
uint8 BD_TG_ping_pak[2] = {0xC0,0x00};
extern uint8_t work_Acc_success;
unsigned char HexToChar(unsigned char bChar);
unsigned char 	GetASCII(unsigned char  x);
uint16 hex1adecimal_to_decimal(u32 hex);
unsigned char *data_ptr = NULL;
extern unsigned char flag;
extern unsigned char TxBuffer2[200]; 
extern unsigned char RxBuffer2[200];
extern unsigned char TxCounter2;
extern unsigned char RxCounter2;
extern uint8_t door_over_time_alway;
extern uint8_t elevator_status;
extern int8_t mqtt_send_time; 
extern uint8_t mqtt_sensor_send;
extern uint8_t w5500_connect_success;
extern uint8_t W5500_REBOOT;
extern uint8_t input5_printf;
extern uint8_t input6_printf;
extern int floor;
extern u32 id_name_all;
u32 door_close_times_mqtt = 0;
extern uint8_t door_open_over_time_sum;
extern uint8_t door_close_times;
extern uint8_t door_close_times_sum;
extern uint8_t door_close_times_sum1;
extern uint8_t door_close_times_sum2;
extern uint8_t door_not_close;
unsigned char ASII_hex=0;
extern uint8_t door_printf_mqtt;
extern uint8_t door_open_times;
extern uint8_t pingceng_error;
char test[300];
extern uint8_t   pingceng;
extern uint8_t   power;
extern uint8_t pingceng_printf_mqtt;
uint8_t tpye = 1;
/**
*@brief		TCP Client»Ø»·ÑÝÊ¾º¯Êý¡£
*@param		ÎÞ
*@return	ÎÞ
*/
void setKPALVTR(SOCKET i,uint8 val)
{
  IINCHIP_WRITE(Sn_KPALVTR(i),val);
}	
void do_tcp_client(void)
{	
	uint16 len=0;	
	switch(getSn_SR(SOCK_TCPC))								  				         /*»ñÈ¡socketµÄ×´Ì¬*/
	{
		case SOCK_CLOSED:											        		         /*socket´¦ÓÚ¹Ø±Õ×´Ì¬*/
			network_status = 1;
			setKPALVTR(SOCK_TCPC,1);//ÐÄÌø±£Ö¤£¬µÍ¶ËÁ¬½Ó
			socket(SOCK_TCPC,Sn_MR_TCP,local_port++,Sn_MR_ND);
		break;

		case SOCK_INIT:													        	         /*socket´¦ÓÚ³õÊ¼»¯×´Ì¬*/
			network_status = 1;
			connect(SOCK_TCPC,BD_TG_server_ip,BD_TG_server_port);                /*socketÁ¬½Ó·þÎñÆ÷*/ 
		break;

		case SOCK_ESTABLISHED: /*socket´¦ÓÚÁ¬½Ó½¨Á¢×´Ì¬*/		
			w5500_connect_success = 1;			
			if(getSn_IR(SOCK_TCPC) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPC, Sn_IR_CON); 							         /*Çå³ý½ÓÊÕÖÐ¶Ï±êÖ¾Î»*/
			}

			len=getSn_RX_RSR(SOCK_TCPC); 								  	         /*¶¨ÒålenÎªÒÑ½ÓÊÕÊý¾ÝµÄ³¤¶È*/
			if(len>0)
			{
				recv(SOCK_TCPC,buff,len); 							   		         /*½ÓÊÕÀ´×ÔServerµÄÊý¾Ý*/			
				data_ptr = buff;
				if(data_ptr != NULL)
				BD_TG_RevPro(data_ptr);
			}
			switch(MQTT_STATE)
			{
				/*MQTTÐ­ÒéÁ¬½ÓBD_TG´úÀíÆ½Ì¨*/
				case MQTT_PKT_CONNECT:
					BD_TG_DevLink();
					MQTT_STATE = MQTT_PKT_SUBSCRIBE;
			   	
				break;
				/*¶©ÔÄÖ÷Ìâ*/
				case MQTT_PKT_SUBSCRIBE:
					BD_TG_Subscribe(topics1,1);
					MQTT_STATE = MQTT_PKT_PINGREQ;
				break;	
				/*Qos2¼¶±ð·¢²¼ÏûÏ¢*/
				case MQTT_PKT_PUBLISH:
					//printf("½øÈë·¢²¼ÏûÏ¢\r\n");
				  memset(test, 0, sizeof(test));
				  
#ifdef MQTT_STRING
			 if(((door_open_times == 1)&&(door_over_time_alway == 0))||(door_printf_mqtt == 1))
				{
					door_over_time_alway++;
					door_printf_mqtt = 0;
					sprintf(test,"{\"id\":%d,\"doorovertime\":%d}",id_name_all,(++door_open_over_time_sum)*2);
					door_open_times = 0;
					BD_TG_Publish(topics[1],test,strlen(test)); //·¢²¼ÏûÏ
				}
				if(pingceng_printf_mqtt == 1)
				{
					pingceng_printf_mqtt = 0;
					sprintf(test,"{\"id\":%d,\"bedovertime\":%d}",id_name_all,(++pingceng_error)*2);
					BD_TG_Publish(topics[1],test,strlen(test)); 
				}
				if(mqtt_sensor_send == 1)
				{
					mqtt_sensor_send = 0;
					door_close_times_mqtt = door_close_times+(door_close_times_sum*255)+(door_close_times_sum1*255*255)+(door_close_times_sum2*255*255*255);
					sprintf(test,"{\"id\":%d,\"type\":%d,\"s1\":%d,\"s2\":%d,\"s3\":%d,\"s4\":%d,\"s5\":%d,\"s6\":%d,\"s7\":%d,\"floor\":%d,\"t\":%d,\"h\":%d,\"vx\":%d,\"vy\":%d,\"vz\":%d,\"acdt\":%d,\"time\":%d}",id_name_all,tpye,door_not_close,input4_value,pingceng,input5_printf,input6_printf,power,elevator_status,floor,(int)temperature,(int)humidity,(int)AccelerationX,(int)AccelerationZ,(int)AccelerationY,door_close_times_mqtt,mqtt_send_time);
					BD_TG_Publish(topics[0],test,strlen(test)); //·¢²¼ÏûÏ
				}
#else
				  for(int i=0; i<200;i++)
					{
						test[i] = i+0x30;
					}
#endif
					//BD_TG_Publish(*topics,test,strlen(test)); //·¢²¼ÏûÏ¢
          work_Acc_success = 0;     
					delay_ms(300);                                 //µÈ´ýÆ½Ì¨ÏìÓ¦    
					/*½ÓÊÕÆ½Ì¨·¢ËÍµÄPubRec²¢»Ø¸´PubRelÏìÓ¦*/		 
					len=getSn_RX_RSR(SOCK_TCPC); 
					recv(SOCK_TCPC,buff,len); 
					data_ptr = buff;
					if(data_ptr != NULL)
						BD_TG_RevPro(data_ptr);
					delay_ms(100);                                 //PubRelÏìÓ¦µÈ´ýÆ½Ì¨ÏìÓ¦
					len=getSn_RX_RSR(SOCK_TCPC); 
					recv(SOCK_TCPC,buff,len); 
					data_ptr = buff;
					if(data_ptr != NULL)
						BD_TG_RevPro(data_ptr);
			    MQTT_STATE = MQTT_PKT_PINGREQ;
				/*120Ãë·¢ËÍÒ»´ÎPingÏìÓ¦±£³Ö³¤Á¬½Ó*/
				break;
				case MQTT_PKT_UNSUBSCRIBE:
					MQTT_UnSubscribe(topics1,1);
					MQTT_STATE = MQTT_PKT_PINGREQ;
				break;
				case MQTT_PKT_PINGREQ:
				//	network_status = 0;
//					if(BD_TG_ping_time > 50)
//					{
//						send(SOCK_TCPC,BD_TG_ping_pak,2);
//						BD_TG_ping_time = 0;
//					}
				break;
				case MQTT_PKT_DISCONNECT:
						network_status = 1;
					break;
			}	
			
					
    break;

	case SOCK_CLOSE_WAIT: 											    	         /*socket´¦ÓÚµÈ´ý¹Ø±Õ×´Ì¬*/
		close(SOCK_TCPC);
		break;
	default:
		close(SOCK_TCPC);
		break;
	}
}

unsigned char HexToChar(unsigned char bChar)
{
if((bChar>=0x30)&&(bChar<=0x39))
{
bChar -= 0x30;
}
else if((bChar>=0x41)&&(bChar<=0x46)) // Capital
{
bChar -= 0x37;
}
else if((bChar>=0x61)&&(bChar<=0x66)) //littlecase
{
bChar -= 0x57;
}
else
{
bChar = 0xff;
}
return bChar;
}

unsigned char 	GetASCII(unsigned char  x)
{
	if(x>0x0f)
	{
		return 0;  
	}
	
	if(x>9)
	{
		return ('A'+x-10);
	}
	return ('0'+x);  
}	
uint16 hex1adecimal_to_decimal(u32 hex)
{
 uint16 ret; //
 unsigned char ret8,ret7,ret6,ret5,ret4,ret3,ret2,ret1; //¾Ö²¿±äÁ¿
 ret8=hex/0x10000000; //È¡¸ßÎ»
 hex=hex<<4; //ret
 ret7=hex/0x10000000; //
 hex=hex<<4; 
 ret6=hex/0x10000000; //
 hex=hex<<4; 
 ret5=hex/0x10000000; // 
 hex=hex<<4;
 ret4=hex/0x10000000; //È¡¸ßÎ»
 hex=hex<<4; //ret
 ret3=hex/0x10000000; //
 hex=hex<<4; 
 ret2=hex/0x10000000; //
 hex=hex<<4; 
 ret1=hex/0x10000000; // 
 ret=ret1+ret2*10+ret3*100+ret4*1000+ret5*10000+ret6*100000+ret7*1000000+ret8*10000000;//Ê®½øÖÆ
 return(ret);
}

void set_object_ip(void)
{
		printf("Ä¿µÄIPÊÇ·ñ±»ÉèÖÃ ?\r\n");
		if(at24c16_read(EE_ADDR_W5500_CONFIG_IP) == EE_ADDR_W5500_PC_CONFIG_SUCCESS)
		{
				printf("Ä¿µÄIP±»ÉèÖÃ£¬¿ªÊ¼¶ÁÈ¡ !\r\n");
				BD_TG_server_ip[0] = at24c16_read(EE_ADDR_IP_ONE);
				BD_TG_server_ip[1] = at24c16_read(EE_ADDR_IP_TWO);
				BD_TG_server_ip[2] = at24c16_read(EE_ADDR_IP_THREE);
			  BD_TG_server_ip[3] = at24c16_read(EE_ADDR_IP_FOUR);
			  printf("Ä¿µÄIP:%d.%d.%d.%d.\r\n",BD_TG_server_ip[0],BD_TG_server_ip[1],BD_TG_server_ip[2],BD_TG_server_ip[3]);
		}
		else
		{
			printf("Ä¿µÄIPÎ´±»ÉèÖÃ !\r\n");
			at24c16_write(EE_ADDR_IP_ONE, BD_TG_server_ip[0]);
			at24c16_write(EE_ADDR_IP_TWO, BD_TG_server_ip[1]);
			at24c16_write(EE_ADDR_IP_THREE, BD_TG_server_ip[2]);
			at24c16_write(EE_ADDR_IP_FOUR, BD_TG_server_ip[3]);	
			printf("Ä¿µÄIP:%d.%d.%d.%d.\r\n",BD_TG_server_ip[0],BD_TG_server_ip[1],BD_TG_server_ip[2],BD_TG_server_ip[3]);
		}
}
