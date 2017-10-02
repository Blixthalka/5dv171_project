#obj-m += KeyValueStoreModule.o
#MODULE_NAME = key_value_store
#
#SRC := netlink_kvs.c ../common/kvs_protocol.c ../common/kvs_protocol.h
#
#KDIR := $(shell pwd)
#
#$(MODULE_NAME)-objs = $(SRC:.c=.o)
#
#obj-m := $(MODULE_NAME).o
#PWD := $(shell pwd)
#
#all:
#	$(MAKE) -C $(KDIR) m=$(PWD) modules

obj-m += netlink_kvsko.o
netlink_kvsko-objs := ../common/kvs_protocol.o netlink_kvs.o
KVERSION := $(shell uname -r)
PWD := $(shell pwd)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
