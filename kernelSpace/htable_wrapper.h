//
// Created by Emil Vannebäck on 2017-10-02.
//

#ifndef INC_5DV171_PROJECT_HTABLE_WRAPPER_H
#define INC_5DV171_PROJECT_HTABLE_WRAPPER_H
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netlink.h>
#include <linux/hashtable.h>
#include "../common/kvs_protocol.h"

// size of the hashtable will become 2 ^ HASHTABLE_SIZE
#define HASHTABLE_SIZE 3

//DEFINE_HASHTABLE(kvs_htable, HASHTABLE_SIZE);

struct kvs_htable_entry {
    unsigned int key;
    char *value;
    size_t value_size;
    struct hlist_node hash_list;
};

int table_put(struct kvs_msg *message);
int table_del(struct kvs_msg *message);
struct kvs_htable_entry* table_get(struct kvs_msg *message);



#endif //INC_5DV171_PROJECT_HTABLE_WRAPPER_H
