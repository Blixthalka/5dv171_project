//
// Created by Emil Vannebäck on 2017-10-02.
//

#ifndef INC_5DV171_PROJECT_NETLINK_KVS_H_H
#define INC_5DV171_PROJECT_NETLINK_KVS_H_H
#include "../common/kvs_protocol.h"
#include <linux/netlink.h>

void put(struct kvs_msg *msg, struct nlmsghdr *nlh);
void get(struct kvs_msg *msg, struct nlmsghdr *nlh);
void del(struct kvs_msg *msg, struct nlmsghdr *nlh);

#endif //INC_5DV171_PROJECT_NETLINK_KVS_H_H
