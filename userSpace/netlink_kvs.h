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


int kvs_put(struct kvs_connection *connection, int key, char *value, int value_size);
int kvs_get(struct kvs_connection *connection, int key, struct kvs_msg *ret);
int kvs_del(struct kvs_connection *connection, int key);

/**
 * Sends the kvs message through the connection to the kernel, where it is handled.
 * @param connection The connection to the kernel.
 * @param msg The message to be sent.
 * @param ret The message to be filled with the answer from the kvs. 
 */
void kvs_send_msg(struct kvs_connection *connection, struct kvs_msg *msg, struct kvs_msg *ret);

#endif //INC_5DV171_PROJECT_NETLINK_KVS_H
