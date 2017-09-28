#include "kvs_protocol.h"
#include <string.h>
     
void serialize_kvs_msg(char *buf, struct kvs_msg *msg) 
{
	buf[0] = msg->command;
	buf[1] = msg->key >> 24;
	buf[2] = msg->key >> 16;
	buf[3] = msg->key >>  8;
	buf[4] = msg->key;

	strcpy(buf[5], msg->value;
}
  
void unserialize_kvs_msg(struct kvs_msg *msg, char *buf) 
{
	msg->command = buf[0]
	msg->key = buf[1] << 24 | buf[2] << 16 | buf[3] <<  8 | buf[4];
	strcpy(msg->value, buf[5]);
}
