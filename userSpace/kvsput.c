#include <stdio.h>
#include <stdlib.h>
#include "netlink_kvs.h"


int main(int argc, char *argv[]) {
    struct kvs_connection connection;
    int ret;
    if(argc != 3) {
        printf("Usage: %s key value\n", argv[0]);
        return 1;
    }

    int key = atoi(argv[1]);
    char *value = argv[2];

    kvs_connection_init(&connection);
    ret = kvs_put(&connection, key, value, strlen(value) + 1);
    if(ret == KVS_COMMAND_SUC) {
        printf("Successfully added value at key %d.\n", key);
    } else {
        printf("Error putting value.\n");
    }
    kvs_connection_close(&connection);

    return 0;
}
