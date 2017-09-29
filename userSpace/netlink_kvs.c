#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <stdlib.h>
#include <string.h>
#include "../common/kvs_protocol.h"
#include "netlink_kvs.h"


int main() {
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

    memset(&d_nladdr, 0, sizeof(d_nladdr));
    connection->dest_addr.nl_family = AF_NETLINK;
    connection->dest_addr.nl_pad    = 0;
    connection->dest_addr.nl_pid    = 0; /* to kernel */
}

void kvs_put(struct kvs_connection *connection, int key, char *value)
{
    struct kvs_msg msg = CREATE_KVS_MSG_PUT(key, value);
    kvs_send_msg(connection, msg);
}

void kvs_get(struct kvs_connection *connection, int key, char *value)
{
    struct kvs_msg msg = CREATE_KVS_MSG_GET(key, value);
    kvs_send_msg(connection, msg);
}

void kvs_del(struct kvs_connection *connection, int key)
{
    struct kvs_msg msg = CREATE_KVS_MSG_DEL(key, value);
    kvs_send_msg(connection, msg);
}

void kvs_send_msg(struct kvs_connection *connection, struct kvs_msg *user_msg)
{
    struct msghdr msg;
    struct nlmsghdr *nlh = NULL;
    struct iovec iov;


    /* Fill the netlink message header */
    nlh = (struct nlmsghdr *) malloc(100);
    memset(nlh, 0, 100);
    strcpy(NLMSG_DATA(nlh), " Mr. Kernel, Are you ready ?");
    nlh->nlmsg_len = 100;
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 1;
    nlh->nlmsg_type = 0;

    iov.iov_base = (void *) nlh;
    iov.iov_len = nlh->nlmsg_len;

    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *) &d_nladdr;
    msg.msg_namelen = sizeof(d_nladdr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    sendmsg(connection->fd, &msg, 0);
}