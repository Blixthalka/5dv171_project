#ifndef _KVS_PROTOCOL_H
#define _KVS_PROTOCOL_H

#ifdef __KERNEL__
#include <linux/string.h>
#else

#include <string.h>

#endif

#define KVS_COMMAND_ERR 0xff /* -1 */
#define KVS_COMMAND_SUC 0x00
#define KVS_COMMAND_PUT 0x03
#define KVS_COMMAND_GET 0x01
#define KVS_COMMAND_DEL 0x02



struct kvs_msg {
    char command;
    int key;
    int value_size;
    char *value;
};

#define CREATE_KVS_MSG_PUT(_key, _value, _value_size)  {    \
    .command = KVS_COMMAND_PUT,                             \
    .key = _key,                                            \
    .value_size = _value_size,                              \
    .value = _value,                                        \
}

#define CREATE_KVS_MSG_GET(_key) {                          \
    .command = KVS_COMMAND_GET,                             \
    .key = _key,                                            \
    .value_size = 0,                                        \
    .value = NULL,                                          \
}

#define CREATE_KVS_MSG_DEL(_key)  {                         \
    .command = KVS_COMMAND_DEL,                             \
    .key = _key,                                            \
    .value_size = 0,                                        \
    .value = NULL,                                          \
}

#define CREATE_KVS_MSG_SUC() {                              \
    .command = KVS_COMMAND_SUC,                             \
    .key = 0,                                               \
    .value_size = 0,                                        \
    .value = NULL,                                          \
}


#define CREATE_KVS_MSG_ERR() {                              \
    .command = KVS_COMMAND_ERR,                             \
    .key = 0,                                               \
    .value_size = 0,                                        \
    .value = NULL,                                          \
}

/**
 * Packs the kvs message from msg into buf.
 */
void serialize_kvs_msg(char *buf, struct kvs_msg *msg);

/**
 * Unpacks the kvs message from buf into msg.
 */
void unserialize_kvs_msg(struct kvs_msg *msg, char *buf);

size_t get_value_length(char *buf);


void kvs_msg_print(struct kvs_msg *msg);

#endif /* _KVS_PROTOCOL_H */
