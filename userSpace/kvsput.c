#include <stdio.h>
#include <stdlib.h>
#include "netlink_kvs.h"


int main(int argc, char *argv[]) {
    struct kvs_connection connection;
    struct kvs_msg ret;

    if(argc != 3) {
        printf("Usage: %s key value\n", argv[0]);
        return 1;
    }

    int key = atoi(argv[1]);
    char *value = argv[2];

    kvs_connection_init(&connection);
    kvs_put(&connection, key, value, strlen(value) + 1);
    if(ret.command == KVS_COMMAND_SUC) {
        printf("Value: %s\n", ret.value);
    } else {
        printf("Error fetching value.\n");
    }
    free(ret.value);
    kvs_connection_close(&connection);

    return 0;
}
