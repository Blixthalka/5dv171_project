cmake_minimum_required(VERSION 2.8)
project(kvs_userspace)

SET(CMAKE_C_FLAGS  "${CMAKE_CXX_FLAGS} --std=c99 -Wall -pedantic" )


set(SOURCE  netlink_kvs.c
        netlink_kvs.h
        ../common/kvs_protocol.c
        ../common/kvs_protocol.h)


# build lib with core code
add_library(kvs-core ${SOURCE})

# build the executables kvsget, kvsput and kvsdel
add_executable(kvs-get kvsget.c)
add_executable(kvs-put kvsput.c)
add_executable(kvs-del kvsdel.c)
add_executable(kvs-tst kvstst.c)

# add the lib to the executables
target_link_libraries(kvs-get kvs-core)
target_link_libraries(kvs-put kvs-core)
target_link_libraries(kvs-del kvs-core)
target_link_libraries(kvs-tst kvs-core)