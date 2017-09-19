obj-m += KeyStoreModule.o

all:
	make -C /lib/module/$(shell uname -r)/build M=$(PWD)/src modules

clean:
	make -C /lib/module/$(shell uname -r)/build M=$(PWD)/src clean
