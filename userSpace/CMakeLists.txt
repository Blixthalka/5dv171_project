cmake_minimum_required(VERSION 2.8)
project(kvs_userspace)

set(SOURCE  netlink_kvs.c
        netlink_kvs.h
        ../common/kvs_protocol.c
        ../common/kvs_protocol.h)


# build lib with core code
add_library(kvscore ${SOURCE})

# build the executables kvsget, kvsput and kvsdel
add_executable(kvs-get kvsget.c)
add_executable(kvs-put kvsput.c)
add_executable(kvs-del kvsdel.c)

# add the lib to the executables
target_link_libraries(kvs-get kvscore)
target_link_libraries(kvs-put kvscore)
target_link_libraries(kvs-del kvscore)