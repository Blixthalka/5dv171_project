#ifndef INC_5DV171_PROJECT_HTABLE_WRAPPER_H
#define INC_5DV171_PROJECT_HTABLE_WRAPPER_H
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netlink.h>
#include <linux/hashtable.h>
#include "../common/kvs_protocol.h"

#define STORE_FILE "/.kvs"
// size of the hashtable will become 2 ^ HASHTABLE_SIZE
#define HASHTABLE_SIZE 14


struct kvs_htable_entry {
    unsigned int key;
    char *value;
    size_t value_size;
    struct hlist_node hash_list;
};
/**
 * Puts the entry from the input in the hashtable.
 * @param message
 * @return: 1 on success -1 on failure or error.
 */
int htable_put(struct kvs_msg *message);
/**
 * deletes the entry from the input in the hashtable.
 * @param message
 * @return: 1 on success -1 on failure or error.
 */
int htable_del(struct kvs_msg *message);
/**
 * Puts the entry from the input in the hashtable.
 * @param message
 * @return: An entry with the retrieved value.
 */
struct kvs_htable_entry* htable_get(struct kvs_msg *message);

/**
 * Store all the data in the current hashtable to the file defined in STORE_FILE.
 * @return
 */
int htable_store(void);


/**
 * Load data from the file defined in STORE_FILE and inserts it into the hashtable.
 * @return
 */
int htable_load(void);



#endif //INC_5DV171_PROJECT_HTABLE_WRAPPER_H
