#ifndef INC_5DV171_PROJECT_NETLINK_KVS_H
#define INC_5DV171_PROJECT_NETLINK_KVS_H

#include <linux/netlink.h>
#include "../common/kvs_protocol.h"


#define NETLINK_KVS 17
#define MAX_PAYLOAD 2048

struct kvs_connection {
    int fd;
    struct sockaddr_nl src_addr, dest_addr;
};


void kvs_connection_init (struct kvs_connection *connection);
void kvs_connection_close(struct kvs_connection *connection);


void kvs_put(struct kvs_connection *connection, int key, char *value, int value_size);
void kvs_get(struct kvs_connection *connection, int key, char *value, int value_size);
void kvs_del(struct kvs_connection *connection, int key);

/**
 * Sends the kvs message through the connection to the kernel, where it is handled.
 * @param connection The connection to the kernel.
 * @param msg The message to be sent.
 */
void kvs_send_msg(struct kvs_connection *connection, struct kvs_msg *msg);

#endif //INC_5DV171_PROJECT_NETLINK_KVS_H
