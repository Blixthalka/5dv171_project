#include <stdio.h>
#include <stdlib.h>
#include "netlink_kvs.h"


int main(int argc, char *argv[]) {
    struct kvs_connection connection;
    struct kvs_msg ret;
    int r;

    if(argc != 2) {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }

    int key = atoi(argv[1]);

    kvs_connection_init(&connection);
    r = kvs_get(&connection, key, &ret);
    if(r == KVS_COMMAND_SUC) {
        printf("%s\n", ret.value);
    } else {
        fprintf(stderr, "Error getting value.\n");
    }
    free(ret.value);
    kvs_connection_close(&connection);

    return 0;
}
