//
// Created by Emil Vannebäck on 2017-10-02.
//

#ifndef INC_5DV171_PROJECT_NETLINK_KVS_H_H
#define INC_5DV171_PROJECT_NETLINK_KVS_H_H
#include "../common/kvs_protocol.h"
#include <linux/netlink.h>

/**
 * Put the entry in the hashtable. Uses class htable_wrapper to modify the hashtable, then creates
 * an success message on success or an error message on failure.
 * @param msg
 * @param nlh
 */
void put(struct kvs_msg *msg, struct nlmsghdr *nlh);
/**
 * Gets an entry from the hashtable. Uses class htable_wrapper to access the hashtable. then creates
 * an success message with the retrieved value on success or an error message on failure.
 * @param msg
 * @param nlh
 */
void get(struct kvs_msg *msg, struct nlmsghdr *nlh);
/**
 * deletes an entry in the hashtable. Uses class htable_wrapper to modify the hashtable, then creates
 * an success message on success or an error message on failure.
 * @param msg
 * @param nlh
 */
void del(struct kvs_msg *msg, struct nlmsghdr *nlh);

#endif //INC_5DV171_PROJECT_NETLINK_KVS_H_H
