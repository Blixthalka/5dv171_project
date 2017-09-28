#!/bin/bash
gcc -Wall hashtable.c hashtable.h main.c -o hashtabletest
gcc -Wall netlink_kvs.c ../common/kvs_protocol.c ../common/kvs_protocol.h -o netlink
