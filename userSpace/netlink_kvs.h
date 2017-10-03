#ifndef _5DV171_USERSPACE_NETLINK_KVS_H
#define _5DV171_USERSPACE_NETLINK_KVS_H

#include <linux/netlink.h>
#include "../common/kvs_protocol.h"


#define NETLINK_KVS 17
//#define MAX_PAYLOAD 2048

struct kvs_connection {
    int fd;
    struct sockaddr_nl src_addr, dest_addr;
};

/**
 * Initializes the connection to kernelspace.
 * @param connection The connection to be initialized.
 */
void kvs_connection_init (struct kvs_connection *connection);

/**
 * Closes the connection to kernelspace.
 * @param connection The connection to be closed.
 */
void kvs_connection_close(struct kvs_connection *connection);

/**
 * Inserts a value into the kvs. If the key exists in the table, it will update that value.
 * @param connection The connection to kernelspace.
 * @param key The key to insert the value at.
 * @param value The value that should be insertet. It does not have to be nullterminated.
 * @param value_size The size of the value in bytes.
 * @return 0 on success, -1 on error.
 */
int kvs_put(struct kvs_connection *connection, int key, char *value, int value_size);

/**
 * Gets the value that corresopnds to the given key.
 * @param connection The connection to kernelspace.
 * @param key The key to the value.
 * @param ret The struct to fill with the result. kvs_msg.value gets allocated on the heap.
 * @return 0 on success, -1 on error.
 */
int kvs_get(struct kvs_connection *connection, int key, struct kvs_msg *ret);

/**
 * Deletes the value at the given key.
 * @param connection The connection to kernelspace.
 * @return 0 on success, -1 on error.
 */
int kvs_del(struct kvs_connection *connection, int key);


#endif //_5DV171_USERSPACE_NETLINK_KVS_H
