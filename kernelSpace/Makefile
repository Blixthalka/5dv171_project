obj-m += key_value_store.o
key_value_store-objs := ../common/kvs_protocol.o file_handler.o htable_wrapper.o netlink_kvs.o
KVERSION := $(shell uname -r)
PWD := $(shell pwd)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
