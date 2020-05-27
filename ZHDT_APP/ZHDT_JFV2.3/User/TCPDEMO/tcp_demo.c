/**
******************************************************************************
* @file    			tcp_demo.c                                               
* @version 			V1.2										  		
* @date    			2017-11-01								 
* @brief 			  TCP ��ʾ����     					        
*
* @company  		��������Ƽ����޹�˾
* @information  WIZnet W5500�ٷ������̣�ȫ�̼���֧�֣��۸�������ƣ�
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
uint8 buff[256];				                             /*����һ��2KB�Ļ���*/
extern uint8_t work_Acc_success;
uint8  BD_TG_server_ip[4];//{202,106,5,12};        //BD_TG������IP��ַ
uint16 BD_TG_server_port;                   //BD_TG�������˿ں�
extern uint8_t power_status;
extern volatile unsigned int usart_buf3[26];
extern volatile unsigned int usart_buf1[26];
extern u32 id_name_all;
int MQTT_STATE = MQTT_PKT_CONNECT;   //����
const char *topics[] = {"sensor-data"}; //ԭ������������stateUp
const char *topics1[] = {"/hardware-upgrade"}; //ԭ������������stateUp
const char *topics2[] = {"/hardware-upgrade-result"}; //ԭ������������stateUp
uint8 BD_TG_ping_pak[2] = {0xC0,0x00};


extern uint8_t send_version;

unsigned char HexToChar(unsigned char bChar);
unsigned char 	GetASCII(unsigned char  x);
uint16 hex1adecimal_to_decimal(uint16 hex);
unsigned char *data_ptr = NULL;
extern unsigned char flag;
extern uint8_t status_code1;
extern uint8_t status_code2;
extern uint8_t status_code3;
extern uint8_t mqtt_send_time;
uint8_t move,ms_status,floor_int;
extern unsigned char TxCounter2;
extern uint8_t mqtt_send_xin;
extern unsigned char RxCounter2;
extern uint8_t w5500_connect_success;
extern uint8_t W5500_REBOOT;
unsigned char ASII_hex=0;
char test[300];
/**
*@brief		TCP Client�ػ���ʾ������
*@param		��
*@return	��
*/
void setKPALVTR(SOCKET i,uint8 val)
{
  IINCHIP_WRITE(Sn_KPALVTR(i),val);
}	
void do_tcp_client(void)
{	
	uint16 len=0;	
	switch(getSn_SR(SOCK_TCPC))								  				         /*��ȡsocket��״̬*/
	{
		case SOCK_CLOSED:											        		         /*socket���ڹر�״̬*/
			network_status = 1;
			setKPALVTR(SOCK_TCPC,1);//������֤���Ͷ�����
			socket(SOCK_TCPC,Sn_MR_TCP,local_port++,Sn_MR_ND);
		break;

		case SOCK_INIT:													        	         /*socket���ڳ�ʼ��״̬*/
			network_status = 1;
			connect(SOCK_TCPC,BD_TG_server_ip,BD_TG_server_port);                /*socket���ӷ�����*/ 
		break;

		case SOCK_ESTABLISHED: /*socket�������ӽ���״̬*/	
			w5500_connect_success = 1;
		  network_status = 0;
			if(getSn_IR(SOCK_TCPC) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPC, Sn_IR_CON); 							         /*��������жϱ�־λ*/
			}

			len=getSn_RX_RSR(SOCK_TCPC); 								  	         /*����lenΪ�ѽ������ݵĳ���*/
			if(len>0)
			{
				recv(SOCK_TCPC,buff,len); 							   		         /*��������Server������*/			
				data_ptr = buff;
				if(data_ptr != NULL)
				BD_TG_RevPro(data_ptr);
			}
			switch(MQTT_STATE)
			{
				/*MQTTЭ������BD_TG����ƽ̨*/
				case MQTT_PKT_CONNECT:
					BD_TG_DevLink();
					MQTT_STATE = MQTT_PKT_SUBSCRIBE;
			   	
				break;
				/*��������*/
				case MQTT_PKT_SUBSCRIBE:
					BD_TG_Subscribe(topics1,1);
					MQTT_STATE = MQTT_PKT_PINGREQ;
					flag =1;	
					network_status = 0;
				break;	
				/*Qos2���𷢲���Ϣ*/
				case MQTT_PKT_PUBLISH:
//					printf("���뷢����Ϣ\r\n");
				  memset(test, 0, sizeof(test));
				  
#ifdef MQTT_STRING
				
				if(send_version == 1)
				{
					send_version = 0;
					sprintf(test, "{\"id\":%d,\"version\":%d.%d,\"type\":0}",id_name_all,at24c16_read(EE_APP_VERSION1)/10,at24c16_read(EE_APP_VERSION1)%10);
					delay_ms(100);
					BD_TG_Publish(*topics2,test,strlen(test)); //������Ϣ	
				}
				else
				{
					sprintf(test, "{\"id\":%d,\"type\":0,\"s1\":%d,\"t1\":%d,\"h1\":%d,\"n1\":%d,\"vx1\":%d,\"vy1\":%d,\"vz1\":%d,\"acc\":%d,\"s2\":%d,\"t2\":%d,\"h2\":%d,\"n2\":%d,\"vx2\":%d,\"vy2\":%d,\"vz2\":%d,\"s3\":%d,\"t3\":%d,\"h3\":%d,\"n3\":%d,\"vx3\":%d,\"vy3\":%d,\"vz3\":%d,\"time\":%d,\"move\":%d,\"ms\":%d,\"floors\":%d}",id_name_all,status_code1,interiol.temp,interiol.humi,interiol.noise,x.average,y.average,z.average,power_status,status_code2,external1.temp,external1.humi,external1.noise,external1.vibx,external1.viby,external1.vibz,status_code3,external3.temp,external3.humi,external1.noise,external1.vibx,external1.viby,external1.vibz,mqtt_send_time,move,ms_status,floor_int);
					delay_ms(100);
					BD_TG_Publish(*topics,test,strlen(test)); //������Ϣ	
				}
#else
				  for(int i=0; i<200;i++)
					{
						test[i] = i+0x30;
					}
#endif
//					delay_ms(100);
//					BD_TG_Publish(*topics,test,strlen(test)); //������Ϣ	
					delay_ms(300);                                 //�ȴ�ƽ̨��Ӧ    
					/*����ƽ̨���͵�PubRec���ظ�PubRel��Ӧ*/		 
					len=getSn_RX_RSR(SOCK_TCPC); 
					recv(SOCK_TCPC,buff,len); 
					data_ptr = buff;
					if(data_ptr != NULL)
						BD_TG_RevPro(data_ptr);
					delay_ms(100);                                 //PubRel��Ӧ�ȴ�ƽ̨��Ӧ
					len=getSn_RX_RSR(SOCK_TCPC); 
					recv(SOCK_TCPC,buff,len); 
					data_ptr = buff;
					if(data_ptr != NULL)
						BD_TG_RevPro(data_ptr);
			    MQTT_STATE = MQTT_PKT_PINGREQ;
				/*120�뷢��һ��Ping��Ӧ���ֳ�����*/
				break;
				case MQTT_PKT_UNSUBSCRIBE:
					MQTT_UnSubscribe(topics,1);
					MQTT_STATE = MQTT_PKT_PINGREQ;
				break;
				case MQTT_PKT_PINGREQ:
				break;
				case MQTT_PKT_DISCONNECT:
						network_status = 1;
					break;
			}	
			
					
    break;

	case SOCK_CLOSE_WAIT: 											    	         /*socket���ڵȴ��ر�״̬*/
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
uint16 hex1adecimal_to_decimal(uint16 hex)
{
 uint16 ret; //
 unsigned char ret4,ret3,ret2,ret1; //�ֲ�����
 ret4=hex/0x1000; //ȡ��λ
 hex=hex<<4; //ret
 ret3=hex/0x1000; //
 hex=hex<<4; 
 ret2=hex/0x1000; //
 hex=hex<<4; 
 ret1=hex/0x1000; // 
 ret=ret1+ret2*16+ret3*16*16+ret4*16*16*16;//ʮ����
 return(ret);
}
int aaa;
void set_object_ip(void)
{
		printf("Ŀ��IP�Ƿ����� ?\r\n");
		if(at24c16_read(EE_ADDR_W5500_CONFIG_IP) == EE_ADDR_W5500_PC_CONFIG_SUCCESS)
		{
				printf("Ŀ��IP�����ã���ʼ��ȡ !\r\n");
				BD_TG_server_ip[0] = at24c16_read(EE_ADDR_IP_ONE);
				BD_TG_server_ip[1] = at24c16_read(EE_ADDR_IP_TWO);
				BD_TG_server_ip[2] = at24c16_read(EE_ADDR_IP_THREE);
			  BD_TG_server_ip[3] = at24c16_read(EE_ADDR_IP_FOUR);
			  printf("Ŀ��IP:%d.%d.%d.%d.\r\n",BD_TG_server_ip[0],BD_TG_server_ip[1],BD_TG_server_ip[2],BD_TG_server_ip[3]);
		}
		else
		{
			aaa = at24c16_read(EE_ADDR_W5500_CONFIG_IP);
			printf("Ŀ��IPδ������!\r\n");
			at24c16_write(EE_ADDR_IP_ONE, BD_TG_server_ip[0]);
			at24c16_write(EE_ADDR_IP_TWO, BD_TG_server_ip[1]);
			at24c16_write(EE_ADDR_IP_THREE, BD_TG_server_ip[2]);
			at24c16_write(EE_ADDR_IP_FOUR, BD_TG_server_ip[3]);	
			printf("Ŀ��IP:%d.%d.%d.%d.\r\n",BD_TG_server_ip[0],BD_TG_server_ip[1],BD_TG_server_ip[2],BD_TG_server_ip[3]);
		}
}