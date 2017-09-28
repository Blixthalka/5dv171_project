#!/bin/bash
gcc -Wall hashtable.c hashtable.h main.c -o hashtabletest
gcc -Wall kvs_netlink.c -o netlink