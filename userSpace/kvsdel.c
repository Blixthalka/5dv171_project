#include <stdio.h>
#include <stdlib.h>
#include "netlink_kvs.h"


int main(int argc, char *argv[]) {
    struct kvs_connection connection;
    int r;

    if(argc != 2) {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }

    int key = atoi(argv[1]);


    kvs_connection_init(&connection);
    int r = kvs_del(&connection, key);
    if(ret == KVS_COMMAND_SUC) {
        printf("Deleted value at key %d.", key);
    } else {
        printf("Error fetching value.\n");
    }
    kvs_connection_close(&connection);

    return 0;
}
