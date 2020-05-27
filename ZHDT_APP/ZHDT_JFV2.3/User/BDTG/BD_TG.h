#ifndef _BD_TG_H_
#define _BD_TG_H_
#include "types.h"
#include "stdint.h"

void  BD_TG_DevLink(void);

void BD_TG_Subscribe(const char *topics[], unsigned char topic_cnt);


extern uint8_t sub_flag;
extern uint16_t sub_rev_length;

void BD_TG_Publish(const char *topic,char *msg,unsigned short Length);
//����ԭ��void BD_TG_Publish(const char *topic, const char *msg);

void BD_TG_RevPro(unsigned char *cmd);
void MQTT_UnSubscribe(const char *topics[], uint8 topic_cnt);

extern unsigned char publish_buf[2048];
#endif
