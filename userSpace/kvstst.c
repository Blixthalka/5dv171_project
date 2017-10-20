#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "netlink_kvs.h"

#define CONCURRENT_PROCESSES 10
#define TEST_ITERATIONS 100

void test(int offset);

int main(int argc, char *argv[]) {
    pid_t processes[CONCURRENT_PROCESSES];
    printf("Starting tests...\n");

    for (int i = 0; i < CONCURRENT_PROCESSES; i++) {
        processes[i] = fork();

        if (processes[i] == 0) {
            // this is the child, run test and exit
            test(i);
            exit(0);
        } else if (processes[i] > 0) {
        } else {
            printf("fork() failed.\n Exiting...\n");
            exit(-1);
        }
    }

    for (int i = 0; i < CONCURRENT_PROCESSES; i++) {
        waitpid(processes[i], NULL, 0);
    }

    return 0;
}


void test(int offset) {
    struct kvs_connection connection;
    pid_t pid = getpid();

    kvs_connection_init(&connection);

    for (int i = 1; i <= TEST_ITERATIONS; i++) {
        int key = offset * TEST_ITERATIONS + i;
        char value[100];
        sprintf(value, "Test-%d-%d-%d", key, (int)pid, i);

        kvs_put(&connection, key, value, strlen(value));
    }

    for (int i = 0; i < TEST_ITERATIONS; i++) {
        struct kvs_msg ret;
        int key = offset * TEST_ITERATIONS + i;
        char value[100];
        sprintf(value, "Test-%d-%d-%d", key, (int) pid, i);

        kvs_get(&connection, key, &ret);

        if(strcmp(value, ret.value) == 0) {
            printf("%8s: %d", "ERROR", key);
        }  else {
            printf("%8s: %d", "SUCCESS", key);
        }
        fflush(stdout);

        free(ret.value);
    }


    kvs_connection_close(&connection);
}