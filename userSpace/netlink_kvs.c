#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../common/kvs_protocol.h"
#include "netlink_kvs.h"

/**
 * Sends the kvs message through the connection to the kernel, where it is handled.
 * @param connection The connection to the kernel.
 * @param msg The message to be sent.
 * @param ret The message to be filled with the answer from the kvs.
 */
void kvs_send_msg(struct kvs_connection *connection, struct kvs_msg *user_msg, struct kvs_msg *ret);


/**
 * Sends the kvs message through the connection to the kernel, where it is handled.
 * @param connection The connection to the kernel.
 * @param msg The message to be sent.
 * @param ret The message to be filled with the answer from the kvs.
 */
void kvs_send_msg(struct kvs_connection *connection, struct kvs_msg *user_msg, struct kvs_msg *ret);


int main() {
    struct kvs_connection connection;
    struct kvs_msg ret;
    char *value = "please";
    char *value1 = "bitch";

    kvs_connection_init(&connection);
    kvs_put(&connection, 1337, value, strlen(value) + 1);
    kvs_put(&connection, 1337, value1, strlen(value1) + 1);
    kvs_get(&connection, 1337, &ret);
    print_kvs_msg(&ret);
    kvs_del(&connection, 1337);
    kvs_get(&connection, 1337, &ret);
    print_kvs_msg(&ret);

    kvs_connection_close(&connection);
    return 0;
}


void kvs_connection_init(struct kvs_connection *connection)
{
    connection->fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_KVS);

    memset(&connection->src_addr, 0, sizeof(connection->src_addr));
    connection->src_addr.nl_family = AF_NETLINK;
    connection->src_addr.nl_pad    = 0;
    connection->src_addr.nl_pid    = getpid();

    bind(connection->fd, (struct sockaddr *) &connection->src_addr, sizeof(connection->src_addr));

    memset(&connection->dest_addr, 0, sizeof(connection->dest_addr));
    connection->dest_addr.nl_family = AF_NETLINK;
    connection->dest_addr.nl_pad    = 0;
    connection->dest_addr.nl_pid    = 0; /* to kernel */
}

void kvs_connection_close(struct kvs_connection *connection)
{
    close(connection->fd);
}

int kvs_put(struct kvs_connection *connection, int key, char *value, int value_size)
{
    struct kvs_msg ret;
    struct kvs_msg msg = CREATE_KVS_MSG_PUT(key, value, value_size);
    kvs_send_msg(connection, &msg, &ret);
    free(ret.value);
    return ret.command;
}

int kvs_get(struct kvs_connection *connection, int key, struct kvs_msg *ret)
{
    struct kvs_msg msg = CREATE_KVS_MSG_GET(key);
    kvs_send_msg(connection, &msg, ret);
    return ret->command;
}

int kvs_del(struct kvs_connection *connection, int key)
{
    struct kvs_msg ret;
    struct kvs_msg msg = CREATE_KVS_MSG_DEL(key);
    kvs_send_msg(connection, &msg, &ret);
    free(ret.value);
    return ret.command;
}


void kvs_send_msg(struct kvs_connection *connection, struct kvs_msg *user_msg, struct kvs_msg *ret)
{
    struct msghdr msg;
    struct nlmsghdr *nlh = NULL;
    struct iovec iov;
    size_t kvs_msg_size  = sizeof(struct kvs_msg) + user_msg->value_size;
    size_t full_msg_size = sizeof(struct nlmsghdr) + kvs_msg_size;
    char *serialized_msg = (char *) malloc(kvs_msg_size);

    serialize_kvs_msg(serialized_msg, user_msg);
    
   
    /* create netlink message header */
    nlh = (struct nlmsghdr *) malloc(full_msg_size);
    memset(nlh, 0, full_msg_size);
    memcpy(NLMSG_DATA(nlh), serialized_msg, kvs_msg_size);
    nlh->nlmsg_len = full_msg_size;
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 1;
    nlh->nlmsg_type = 0;

    iov.iov_base = (void *) nlh;
    iov.iov_len = nlh->nlmsg_len;

    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *) &connection->dest_addr;
    msg.msg_namelen = sizeof(connection->dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    sendmsg(connection->fd, &msg, 0);
    recvmsg(connection->fd, &msg, 0);
    
    free(serialized_msg);
    
    ret->value = (char *) malloc(get_value_length((char *) NLMSG_DATA(nlh)));
    unserialize_kvs_msg(ret, (char *) NLMSG_DATA(nlh));
}
