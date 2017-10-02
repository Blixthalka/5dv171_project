#include "kvs_protocol.h"
    
#ifdef __KERNEL__
#include <linux/kernel.h>
#else
#include <stdio.h>
#endif

void serialize_kvs_msg(char *buf, struct kvs_msg *msg) 
{
	buf[0] = msg->command;
	buf[1] = msg->key >> 24;
	buf[2] = msg->key >> 16;
	buf[3] = msg->key >>  8;
	buf[4] = msg->key;
	buf[5] = msg->value_size >> 24;
	buf[6] = msg->value_size >> 16;
	buf[7] = msg->value_size >>  8;
	buf[8] = msg->value_size;

	memcpy(&buf[9], msg->value, msg->value_size);
}
  
void unserialize_kvs_msg(struct kvs_msg *msg, char *buf) 
{
	msg->command    = buf[0];
	msg->key        = buf[1] << 24 | buf[2] << 16 | buf[3] <<  8 | buf[4];
	msg->value_size = buf[5] << 24 | buf[6] << 16 | buf[7] <<  8 | buf[8];
	memcpy(msg->value, &buf[9], msg->value_size);
}

size_t get_value_length(char* buf)
{
	return buf[5] << 24 | buf[6] << 16 | buf[7] <<  8 | buf[8];
}

void print_kvs_msg(struct kvs_msg *msg) 
{
#ifdef __KERNEL__
   printk(KERN_INFO "Command: %02X, Key: %d, Value length %d.\n", msg->command, msg->key, msg->value_size);
#else
   int i;
   printf("Command: %02X, Key: %d, Value length: %d.\n", msg->command, msg->key, msg->value_size);
   printf("Buffer: ");
   for(i = 0; i < msg->value_size; i++) {
	printf("%02X", msg->value[i]);
   }
   printf("\n");
#endif
}
