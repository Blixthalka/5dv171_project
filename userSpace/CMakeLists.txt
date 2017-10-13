cmake_minimum_required(VERSION 2.8)
project(kvs_userspace)


set(SOURCE  netlink_kvs.c
            netlink_kvs.h
            ../common/kvs_protocol.c
            ../common/kvs_protocol.h)


# build the executables kvsget, kvsput and kvsdel
add_executable(kvs-get kvsget.c ${SOURCE})
add_executable(kvs-put kvsput.c ${SOURCE})
add_executable(kvs-del kvsdel.c ${SOURCE})