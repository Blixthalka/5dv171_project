
#ifndef _KVS_PROTOCOL_H
#define _KVS_PROTOCOL_H

#define KVS_COMMAND_PUT 0x00
#define KVS_COMMAND_GET 0x01
#define KVS_COMMAND_DEL 0x02
#define KVS_COMMAND_RET 0x03 // return kanske?

struct kvs_msg {
	char 	command;
	size_t 	key;
	char *  value;
};

#define CREATE_KVS_MSG_PUT(_key, _value) kvs_msg { 	 \
	.command = KVS_COMMAND_PUT			 \
	.key = _key,					 \
	.value = _value					 \
};

#define CREATE_KVS_MSG_GET(_key, _value) kvs_msg { 	 \
	.command = KVS_COMMAND_GET			 \
	.key = _key,					 \
	.value = _value					 \
};

#define CREATE_KVS_MSG_DEL(_key, _value) kvs_msg { 	 \
	.command = KVS_COMMAND_DEL			 \
	.key = _key,					 \
	.value = _value					 \
};


void serialize_kvs_msg(char *buf, struct kvs_msg *msg);

void unserialize_kvs_msg(struct kvs_msg *msg, char *buf);


#endif /* _KVS_PROTOCOL_H */
