#include "w5500_config.h"
#include "Types.h"
#include "w5500.h"
#include "config.h"
#include "usart2.h"
#include <stdio.h> 
#include "w5500_config.h"
#include <string.h>
#include "dhcp.h"
#include "Tim2.h"
#include "24c16.h"
#include "LOOP.h"

CONFIG_MSG  ConfigMsg, RecvMsg;
EEPROM_MSG_STR EEPROM_MSG;	
void dhcp_config_w5500(void);
uint8 mac[6]={0x00,0x08,0xdc,0x22,0x22,0x22};
uint8 local_ip[4]={202,106,5,120};											/*����W5500Ĭ��IP��ַ*/
uint8 subnet[4]={255,255,255,0};												/*����W5500Ĭ����������*/
uint8 gateway[4]={202,106,5,1};													/*����W5500Ĭ������*/
uint8 dns_server[4]={114,114,114,114};									/*����W5500Ĭ��DNS*/
uint16 local_port=8080;	                    						/*���屾�ض˿�*/
/*IP���÷���ѡ��������ѡ��*/
uint8  ip_from=IP_FROM_DHCP;		

uint32 BD_TG_ping_time =0;  
uint8  dhcp_ok=0;																				/*dhcp�ɹ���ȡIP*/
uint32 ms=0;																						/*�������*/
vu8    ntptimer = 0;																		/*NPT�����*/
uint8 txsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};		// ѡ��8��Socketÿ��Socket���ͻ���Ĵ�С����w5500.c��void sysinit()�����ù���
uint8 rxsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};		// ѡ��8��Socketÿ��Socket���ջ���Ĵ�С����w5500.c��void sysinit()�����ù���
/**
*@brief		dhcp�õ��Ķ�ʱ����ʼ��
*@param		��
*@return	��
*/
void dhcp_timer_init(void)
{
  //timer2_init();	
  /*******TIM2 ��ʱ����*********/
	TIM2_Configuration();
	/******* �ж����ȼ� **********/
	TIM2_NVIC_Configuration();
  /* TIM2 ���¿�ʱ�ӣ���ʼ��ʱ */	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
}

/**
*@brief		����W5500��IP��ַ
*@param		��
*@return	��
*/
void set_w5500_ip(void)
{	
	/*ʹ��DHCP��ȡIP�����������DHCP�Ӻ���*/	
	//if(at24c16_read(EE_ADDR_W5500_CONFIG) == EE_ADDR_W5500_PC_CONFIG_SUCCESS)
	if(ip_from==IP_FROM_DEFINE)	
	{
		printf(" ʹ�ö����IP��Ϣ����W5500:\r\n");
		memcpy(ConfigMsg.mac, mac, 6);
		memcpy(ConfigMsg.lip,local_ip,4);
		memcpy(ConfigMsg.sub,subnet,4);
		memcpy(ConfigMsg.gw,gateway,4);
		memcpy(ConfigMsg.dns,dns_server,4);
	}
		/*ʹ��EEPROM�洢��IP����*/	
	if(at24c16_read(EE_ADDR_W5500_CONFIG_DHCP_IP) == EE_ADDR_W5500_PC_CONFIG_SUCCESS)
	{	
		/*�����ȡEEPROM��MAC��Ϣ,��������ã����ʹ��*/		
			printf(" IP from EEPROM\r\n");
			/*����EEPROM������Ϣ�����õĽṹ�����*/
			EEPROM_MSG.lip[0] = at24c16_read(EE_ADDR_DHCP_IP_ONE);
			EEPROM_MSG.lip[1] = at24c16_read(EE_ADDR_DHCP_IP_TWO);
			EEPROM_MSG.lip[2] = at24c16_read(EE_ADDR_DHCP_IP_THREE);
			EEPROM_MSG.lip[3] = at24c16_read(EE_ADDR_DHCO_IP_FOUR);
			memcpy(ConfigMsg.lip,EEPROM_MSG.lip, 4);				
	}

	/*ʹ��DHCP��ȡIP�����������DHCP�Ӻ���*/		
	if(ip_from==IP_FROM_DHCP)								
	{
		/*����DHCP��ȡ��������Ϣ�����ýṹ��*/
		if(dhcp_ok==1)
		{
			printf(" IP from DHCP\r\n");		 
			memcpy(ConfigMsg.sub,DHCP_GET.sub, 4);
			memcpy(ConfigMsg.gw,DHCP_GET.gw, 4);
			memcpy(ConfigMsg.dns,DHCP_GET.dns,4);
			memcpy(ConfigMsg.lip,DHCP_GET.lip, 4);
		}
		else
		{
			printf(" DHCP�ӳ���δ����,���߲��ɹ�\r\n");
			printf(" ʹ�ö����IP��Ϣ����W5500\r\n");
		}
	}
		
	/*����������Ϣ��������Ҫѡ��*/	
	ConfigMsg.sw_ver[0]=FW_VER_HIGH;
	ConfigMsg.sw_ver[1]=FW_VER_LOW;	

	/*��IP������Ϣд��W5500��Ӧ�Ĵ���*/	
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
	
	getSIPR (local_ip);			
	printf(" W5500 IP��ַ   : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
	getSUBR(subnet);
	printf(" W5500 �������� : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
	getGAR(gateway);
	printf(" W5500 ����     : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
}
extern unsigned char id_name[4];
void set_w5500_mac(void)
{
		mac[2] = id_name[0];
		mac[3] = id_name[1];
		mac[4] = id_name[2];
		mac[5] = id_name[3];
	memcpy(ConfigMsg.mac, mac, 6);
	setSHAR(ConfigMsg.mac);	/**/
}

void dhcp_config_w5500(void)
{
//		printf(" First start ram write at24c16 ! \r\n");
//		at24c16_write(EE_ADDR_SUBNET_ONE, subnet[0]);
//		at24c16_write(EE_ADDR_SUBNET_TWO, subnet[1]);
//		at24c16_write(EE_ADDR_SUBNET_THREE, subnet[2]);
//		at24c16_write(EE_ADDR_SUBNET_FOUR, subnet[3]);	
//		at24c16_write(EE_ADDR_GATEWAY_ONE, gateway[0]);
//		at24c16_write(EE_ADDR_GATEWAY_TWO, gateway[1]);
//		at24c16_write(EE_ADDR_GATEWAY_THREE, gateway[2]);
//		at24c16_write(EE_ADDR_GATEWAY_FOUR, gateway[3]);
//		at24c16_write(EE_ADDR_DNS_ONE, dns_server[0]);
//		at24c16_write(EE_ADDR_DNS_TWO, dns_server[1]);
//		at24c16_write(EE_ADDR_DNS_THREE, dns_server[2]);
//		at24c16_write(EE_ADDR_DNS_FOUR, dns_server[3]);
//		printf(" Ram writed w55000 config at24c16 success ! \r\n");
}




