#ifndef _KVS_PROTOCOL_H
#define _KVS_PROTOCOL_H

#define KVS_COMMAND_PUT 0x00
#define KVS_COMMAND_GET 0x01
#define KVS_COMMAND_DEL 0x02

struct kvs_msg {
	char 	command;
	int 	key;
	char *  value;
};

#define CREATE_KVS_MSG_PUT(_key, _value)  { 	 	\
	.command = KVS_COMMAND_PUT,			\
	.key = _key,					\
	.value = _value,				\
}

#define CREATE_KVS_MSG_GET(_key, _value) { 	 	\
	.command = KVS_COMMAND_GET,			\
	.key = _key,					\
	.value = _value,				\
}

#define CREATE_KVS_MSG_DEL(_key, _value)  { 	 	\
	.command = KVS_COMMAND_DEL,			\
	.key = _key,					\
	.value = _value,				\
}


/**
 * Packs the kvs message from msg into buf.
 */
void serialize_kvs_msg(char *buf, struct kvs_msg *msg);

/**
 * Unpacks the kvs message from buf into msg.
 */ 
void unserialize_kvs_msg(struct kvs_msg *msg, char *buf);


#endif /* _KVS_PROTOCOL_H */