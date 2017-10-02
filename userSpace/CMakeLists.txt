cmake_minimum_required(VERSION 2.8)
project(kvs_userspace)
add_executable(userspace netlink_kvs.c netlink_kvs.h ../common/kvs_protocol.c ../common/kvs_protocol.h)
